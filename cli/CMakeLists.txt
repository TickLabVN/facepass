set(CLI ${PROJECT_NAME})
add_executable(${CLI} main.cc
    add_face.cc
    remove_face.cc
)
target_include_directories(${CLI} PRIVATE 
    ${OpenCV_INCLUDE_DIRS}
    ${TORCH_INCLUDE_DIRS}
    ${FaceDetection_INCLUDE_DIRS}
    ${FaceConfig_INCLUDE_DIRS}
)
target_link_libraries(${CLI} PRIVATE
    ${OpenCV_LIBS} ${TORCH_LIBRARIES}
    face_detection
    face_config
    CLI11::CLI11
)