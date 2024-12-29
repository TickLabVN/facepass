add_library(face_detection SHARED
    face_detection.cc
    utils.cc
)
set(FaceDetection_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(face_detection PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${TORCH_INCLUDE_DIRS}
)
target_include_directories(face_detection PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
