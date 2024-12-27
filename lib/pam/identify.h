#ifndef __IDENTIFY_H
#define __IDENTIFY_H

#include <stdio.h>
#include <string.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <opencv2/opencv.hpp>
#include <iostream>

int capture_face();
int face_identify(pam_handle_t *pamh, int flags, int argc,
                  const char **argv);
#endif