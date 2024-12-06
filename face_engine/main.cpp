#include <iostream>
#include <vector>
#include <getopt.h>

#include <opencv2/opencv.hpp>

// Torch
#include <torch/script.h> 
#include <torch/torch.h>
#include <memory>

#include "inference.h"

using namespace std;
using namespace cv;


float cosine_dist(const at::Tensor& A, const at::Tensor& B) {
    torch::Tensor normA = torch::linalg_norm(A, 2);
    torch::Tensor normB = torch::linalg_norm(B, 2);

    if (normA.item<float>() == 0 || normB.item<float>() == 0) {
        throw std::runtime_error("One of the tensors has zero magnitude.");
    }

    float dotProduct = torch::dot(A, B).item<float>();
    float magnitudeA = normA.item<float>();
    float magnitudeB = normB.item<float>();
    float sim = dotProduct / (magnitudeA * magnitudeB);

    return sim;
}

at::Tensor to_tensor(const cv::Mat& img) {
	at::Tensor img_tensor = torch::from_blob(img.data, {img.rows, img.cols, 3}, torch::kUInt8);
	img_tensor = img_tensor / 255.0;
	img_tensor = img_tensor.permute({2, 0, 1}).to(torch::kFloat32);
	return img_tensor;
};

at::Tensor normalize(const at::Tensor& input, const std::vector<float>& mean, const std::vector<float>& std) {
    // Ensure the input tensor has 3 channels (C, H, W)
    TORCH_CHECK(input.size(0) == mean.size(), "Mean size must match number of channels");
    TORCH_CHECK(input.size(0) == std.size(), "Std size must match number of channels");

    // Normalize each channel
    torch::Tensor normalized = input.clone();
    for (size_t i = 0; i < mean.size(); ++i) {
        normalized[i] = (normalized[i] - mean[i]) / std[i];
    }
    return normalized.unsqueeze(0);
}

cv::Mat preprocess_anti_spoof(
    const cv::Mat& img, const cv::Size& shape = cv::Size(224, 224), 
    const std::vector<float>& mean = {0.485, 0.456, 0.406}, 
    const std::vector<float>& std = {0.229, 0.224, 0.225}) {
    
    cv::Mat resized_img;
    cv::resize(img, resized_img, shape);

    // Convert BGR (OpenCV default) to RGB
    cv::cvtColor(resized_img, resized_img, cv::COLOR_BGR2RGB);

    // Convert image to float32 and normalize to range [0, 1]
    resized_img.convertTo(resized_img, CV_32F, 1.0 / 255.0);

    // Normalize each channel
    std::vector<cv::Mat> channels(3);
    cv::split(resized_img, channels); // Split into channels
    for (size_t i = 0; i < channels.size(); ++i) {
        channels[i] = (channels[i] - mean[i]) / std[i];
    }
    cv::merge(channels, resized_img); // Merge channels back

    return resized_img;
}

cv::Mat softmax(const cv::Mat& input) {
    // Ensure input is of type CV_64F
    cv::Mat input64f;
    input.convertTo(input64f, CV_64F);

    // Subtract the maximum value from each row for numerical stability
    cv::Mat maxVal;
    cv::reduce(input64f, maxVal, 1, cv::REDUCE_MAX);  // Reduce across rows (axis=1)
    
    // Normalize the input by subtracting the max value
    cv::Mat inputNormalized = input64f - maxVal;

    // Compute the exponential of each element
    cv::Mat expInput;
    cv::exp(inputNormalized, expInput);

    // Sum of exponentials across rows
    cv::Mat sumExp;
    cv::reduce(expInput, sumExp, 1, cv::REDUCE_SUM);

    // Reshape sumExp to make sure it's a column vector with the correct dimensions for broadcasting
    sumExp = sumExp.reshape(1, expInput.rows); // Ensure sumExp is a column vector

    // Divide each element by the sum of its row
    cv::Mat softmaxOutput;
    cv::divide(expInput, sumExp, softmaxOutput);

    return softmaxOutput;
}

int argmax(const cv::Mat& input) {
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(input, &minVal, &maxVal, &minLoc, &maxLoc);
    return maxLoc.y;
};

std::vector<cv::Mat> inference_face_det(Inference& inf, const Mat& image) {
    std::vector<Detection> output = inf.runInference(image);
    int num_detections = output.size();
    
    std::vector<cv::Mat> det_images;

    for (int i = 0; i < num_detections; ++i) {
        Detection detection = output[i];
        cv::Rect box = detection.box;
        det_images.push_back(image(box));
    }
    
    return det_images;
};

at::Tensor inference_face_reg(torch::jit::script::Module& module, const cv::Mat& img) {
    cv::Mat resize_img;
	cv::resize(img, resize_img, cv::Size(112, 112));

    std::vector<float> mean({0.5, 0.5, 0.5});
    std::vector<float> std({0.5, 0.5, 0.5});
	
    at::Tensor img_tensor = to_tensor(resize_img);
    at::Tensor norm_img_tensor = normalize(img_tensor, mean, std);
    
    torch::jit::IValue input(norm_img_tensor);

	std::vector<torch::jit::IValue> inputs;
	inputs.push_back(input);

	at::Tensor output = module.forward(inputs).toTensor().squeeze();
	return output;
};


int inference_anti_spoof(cv::dnn::Net& net, const cv::Mat& image) {
    cv::Mat preprocess_image = preprocess_anti_spoof(image);
    cv::Mat blob;
    cv::dnn::blobFromImage(preprocess_image, blob, 1.0, cv::Size(224, 224), cv::Scalar(), true, false);
    net.setInput(blob);

    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());
    cv::Mat result;
    for (size_t i = 0; i < outputs.size(); ++i) {
        if (i == 0) {
            result = outputs[i];
        } else {
            cv::vconcat(result, outputs[i], result);  // Stack vertically
        }
    }

    cv::Mat softmax_result = softmax(result);
    int cls = argmax(softmax_result);
    return cls;
};


int main(int argc, char **argv) {
    // Face det model
    Inference model_face_det("./weights/yolov8n-face.onnx", cv::Size(640, 640), "classes.txt", false);

    // Face reg model
    torch::jit::script::Module model_face_reg;
    model_face_reg = torch::jit::load("./weights/edgeface_s_gamma_05_ts.pt");

    // Anti spoof model
    cv::dnn::Net model_anti_spoof;
    model_anti_spoof = cv::dnn::readNetFromONNX("./weights/mobilenet_v3_small.onnx");

    // Face1 Inference
    std::vector<cv::Mat> det_images;
    cv::Mat image = cv::imread(argv[1]);
    det_images = inference_face_det(model_face_det, image);

    for (int i = 0; i < det_images.size(); ++i) {
        cv::Mat frame = det_images[i];
        std::string index = std::to_string(i);
        std::string file_name = std::string("result") + index + std::string(".jpg");
        cv::imwrite(file_name, frame);

    }
    cv::Mat face1 = det_images[0];
    at::Tensor face_feature1 = inference_face_reg(model_face_reg, face1);
    int spoof = inference_anti_spoof(model_anti_spoof, face1);
    string spoof_result;
    if (spoof == 0) {
        spoof_result = "good";
    }
    else {
        spoof_result = "spoof";
    }

    std::cout << "Face1 saved at results0.jpg\n";
    std::cout << "Face1 embedding size: " << face_feature1.sizes() << std::endl;
    std::cout << "Face1 spoofing result: " << spoof << " - mean " << spoof_result;
    std::cout << "\n\n";

    // Face2 Inference
    image = cv::imread(argv[2]);
    det_images = inference_face_det(model_face_det, image);

    for (int i = 0; i < det_images.size(); ++i) {
        cv::Mat frame = det_images[i];
        std::string index = std::to_string((i + 1));
        std::string file_name = std::string("result") + index + std::string(".jpg");
        cv::imwrite(file_name, frame);
    }

    cv::Mat face2 = det_images[0];
    at::Tensor face_feature2 = inference_face_reg(model_face_reg, face2);
    spoof = inference_anti_spoof(model_anti_spoof, face1);
    if (spoof == 0) {
        spoof_result = "good";
    }
    else {
        spoof_result = "spoof";
    }
    std::cout << "Face2 saved at results0.jpg\n";
    std::cout << "Face2 embedding size: " << face_feature2.sizes() << std::endl;
    std::cout << "Face2 spoofing result: " << spoof << " - mean " << spoof_result;
    std::cout << "\n\n";

    float dist = cosine_dist(face_feature1, face_feature2);
    std::cout << "Face matching result (close to 1 mean look similar, over than 0.5 mean they might be the same person): " << dist << std::endl;
}