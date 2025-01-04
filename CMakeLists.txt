cmake_minimum_required(VERSION 3.22)
project(facepass)

# Require C++17
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} ${OpenCV_CFLAGS_OTHER}")

# Find dependencies
SET(Torch_DIR external/libtorch/share/cmake/Torch)
find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)
find_package(CLI11 CONFIG REQUIRED)

add_subdirectory(lib/face_detection)
add_subdirectory(lib/face_recognition)
add_subdirectory(lib/pam)
# set(FaceDetection_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib/face_detection)
# set(FaceRecognition_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib/face_recognition)
add_subdirectory(cli)

