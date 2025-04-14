#ifndef FACE_AS_H
#define FACE_AS_H

#include <string>

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

// Torch
#include <torch/torch.h>
#include <torch/script.h>

class SpoofResult {
public:
    float score;
    bool  spoof;
    SpoofResult(float score, bool spoof) { this->score = score; this->spoof = spoof; }
};

class FaceAntiSpoofing {
public:
    FaceAntiSpoofing(
        const std::string &ckpt,
        const cv::Size &imgsz={128, 128},
        const bool &cuda=false,
        const float threshold=0.8
    );

    void load_model(const std::string& ckpt);
    SpoofResult inference(cv::Mat& image);
    torch::Tensor preprocess(cv::Mat& image);

private:
    std::string ckpt;
    bool  cuda;
    float threshold;
    cv::Size imgsz;
    torch::jit::script::Module model;
    torch::Device device = torch::Device(torch::kCPU);
};


#endif // FACE_AS_H
