add_library(face_recognition SHARED
    face_recognition.cc
)
set(FaceRecognition_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(face_recognition PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${TORCH_INCLUDE_DIRS}
)
target_include_directories(face_recognition PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
