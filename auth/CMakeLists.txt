set(PAM pam_facepass)
add_library(${PAM} SHARED
    pam.cc
    identify.cc
)
target_include_directories(${PAM} PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${TORCH_INCLUDE_DIRS}
    ${FaceDetection_INCLUDE_DIRS}
    ${FaceRecognition_INCLUDE_DIRS}
    ${FaceConfig_INCLUDE_DIRS}
)
target_link_libraries(${PAM}
    ${OpenCV_LIBS} ${TORCH_LIBRARIES}
    face_detection face_recognition face_config
)