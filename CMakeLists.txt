cmake_minimum_required(VERSION 3.22)
project(facepass)
set(PROJECT_VERSION 0.1.0)
if(DEFINED ENV{PROJECT_VERSION})
    message(STATUS "Project version: $ENV{PROJECT_VERSION}")
    set(PROJECT_VERSION $ENV{PROJECT_VERSION})
else()
    message(STATUS "Project version: latest")
    set(PROJECT_VERSION latest)
endif()

# Require C++17
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} ${OpenCV_CFLAGS_OTHER}")

# Find dependencies
SET(Torch_DIR external/libtorch/share/cmake/Torch)
find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)
find_package(CLI11 CONFIG REQUIRED)

add_subdirectory(lib/config)
add_subdirectory(lib/face_detection)
add_subdirectory(lib/face_recognition)

set(FaceDetection_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib/face_detection)
set(FaceRecognition_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib/face_recognition)
set(FaceConfig_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib/config)

add_subdirectory(cli)
add_subdirectory(auth)
add_subdirectory(test/face_engine)
add_subdirectory(test/pam)

install(
    TARGETS ${PROJECT_NAME} facepass_pam facepass_cfg facepass_det facepass_reg 
    COMPONENT applications
    RUNTIME DESTINATION "/usr/bin"
    LIBRARY DESTINATION "/usr/lib"
)

# Install libtorch libraries
install(
    DIRECTORY external/libtorch/lib/
    DESTINATION "/usr/lib"
    FILES_MATCHING PATTERN "*.so"
)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_PACKAGE_NAME ${PROJECT_NAME})
SET(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Facepass is a face recognition authentication system.")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libopencv-dev, libpam0g-dev, libcli11-dev")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Nguyen Phuc Vinh & Tran Trung Thai") #required

INCLUDE(CPack)
