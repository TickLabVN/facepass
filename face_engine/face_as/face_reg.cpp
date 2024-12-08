#include "face_reg.h"

torch::Tensor to_tensor(const cv::Mat& img) {
	torch::Tensor img_tensor = torch::from_blob(img.data, {img.rows, img.cols, 3}, torch::kUInt8);
	img_tensor = img_tensor / 255.0;
	img_tensor = img_tensor.permute({2, 0, 1}).to(torch::kFloat32);
	return img_tensor;
};

torch::Tensor norm1lize(const torch::Tensor& input, const std::vector<float>& mean, const std::vector<float>& std) {
    TORCH_CHECK(input.size(0) == mean.size(), "Mean size must match number of channels");
    TORCH_CHECK(input.size(0) == std.size(), "Std size must match number of channels");

    torch::Tensor norm1lized = input.clone();
    for (size_t i = 0; i < mean.size(); ++i) {
        norm1lized[i] = (norm1lized[i] - mean[i]) / std[i];
    }
    return norm1lized.unsqueeze(0);
}

FaceRecognition::FaceRecognition(
    const std::string &ckpt,
    const cv::Size &imgsz,
    const bool &cuda,
    const float threshold)
{
    this->ckpt  = ckpt;
    this->imgsz = imgsz;
    this->cuda  = cuda;
    this->threshold = threshold;
    if (this->cuda) this->device = torch::Device(torch::kCUDA);
    else this->device = torch::Device(torch::kCPU);
    this->load_model(ckpt);
}

void FaceRecognition::load_model(const std::string& ckpt)
{
    this->ckpt = ckpt;
    this->model = torch::jit::load(this->ckpt);
    this->model.eval();
    this->model.to(this->device, torch::kFloat32);
}

torch::Tensor FaceRecognition::preprocess(cv::Mat& input_image) {
    cv::Mat resize_img;
	cv::resize(input_image, resize_img, cv::Size(112, 112));

    std::vector<float> mean({0.5, 0.5, 0.5});
    std::vector<float> std({0.5, 0.5, 0.5});
	
    torch::Tensor img_tensor = to_tensor(resize_img);
    torch::Tensor norm_img_tensor = norm1lize(img_tensor, mean, std);

    return norm_img_tensor;
}

torch::Tensor FaceRecognition::inference(cv::Mat& image) {
    torch::Tensor input_image = this->preprocess(image);
    torch::jit::IValue input(input_image);

	std::vector<torch::jit::IValue> inputs;
	inputs.push_back(input);

	torch::Tensor output = this->model.forward(inputs).toTensor().squeeze();
    return output;
}

float FaceRecognition::cosine(const torch::Tensor& tensor1, const torch::Tensor& tensor2) {
    torch::Tensor norm1 = torch::linalg_norm(tensor1, 2);
    torch::Tensor norm2 = torch::linalg_norm(tensor2, 2);

    if (norm1.item<float>() == 0 || norm2.item<float>() == 0) {
        throw std::runtime_error("One of the tensors has zero magnitude.");
    }

    float dot_product = torch::dot(tensor1, tensor2).item<float>();
    float magnitude1 = norm1.item<float>();
    float magnitude2 = norm2.item<float>();
    float sim = dot_product / (magnitude1 * magnitude2);

    return sim;
}

MatchResult FaceRecognition::match(cv::Mat& image1, cv::Mat& image2) {
    torch::Tensor feature1 = this->inference(image1);
    torch::Tensor feature2 = this->inference(image2);
    float distance = this->cosine(feature1, feature2);
    bool  similar = false;
    if (distance > this->threshold) similar = true;
    return MatchResult(distance, similar);
}