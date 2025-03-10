#ifndef __IDENTIFY_H
#define __IDENTIFY_H

#include <stdio.h>
#include <string.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include "face_recognition.h"
#include "face_detection.h"
#include "face_config.h"

int scan_face(const string &, int8_t);
int face_identify(const char *);
#endif