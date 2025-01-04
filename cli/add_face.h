#ifndef __ADD_FACE_H__
#define __ADD_FACE_H__

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <unistd.h>
#include <string>
#include "face_detection.h"
#include "config.h"

int capture_face(cv::Mat &);
cv::Mat detect_face(cv::Mat &);
int add_face();

#endif // __ADD_FACE_H__