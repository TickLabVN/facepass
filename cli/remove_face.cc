#include "remove_face.h"

int remove_face(const string &user) {
    return remove(user_face_path(user).c_str()) != 0;
}