#ifndef __ANTI_SPOOF_H
#define __ANTI_SPOOF_H

#include <stdio.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include "face_as.h"
#include "face_config.h"

int scan_face(const string &, int8_t, const int);
#endif