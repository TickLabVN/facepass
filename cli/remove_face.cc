#include "remove_face.h"

int remove_face() {
    string faceImagePath = get_config_path();
    faceImagePath.append("/faces/face.jpg");
    return remove(faceImagePath.c_str()) != 0;
}