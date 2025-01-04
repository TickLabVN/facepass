set(CLI ${PROJECT_NAME})
add_executable(${CLI} main.cc
    add_face.h
    add_face.cc
)

target_include_directories(${CLI} PRIVATE 
    ${OpenCV_INCLUDE_DIRS}
    ${TORCH_INCLUDE_DIRS}
    ${FaceDetection_INCLUDE_DIRS}
)
target_link_libraries(${CLI} PRIVATE
    ${OpenCV_LIBS} ${TORCH_LIBRARIES}
    face_detection
    CLI11::CLI11
)