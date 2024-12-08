cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(face-pass)

find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} ${OpenCV_CFLAGS_OTHER}")

set(PROJECT_SOURCES
    face_det/utils.h
    face_det/utils.cpp
    face_det/face_det.h
    face_det/face_det.cpp
    face_reg/face_reg.h
    face_reg/face_reg.cpp
    main.cpp
)

add_executable(face-pass ${PROJECT_SOURCES})
target_link_libraries(face-pass ${TORCH_LIBRARIES})
target_link_libraries(face-pass ${OpenCV_LIBS})
set_property(TARGET face-pass PROPERTY CXX_STANDARD 17)

# The following code block is suggested to be used on Windows.
# According to https://github.com/pytorch/pytorch/issues/25457,
# the DLLs need to be copied to avoid memory errors.
if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET face-pass
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:face-pass>)
endif (MSVC)