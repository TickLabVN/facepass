cmake_minimum_required(VERSION 3.22)
project(facepass)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CLI_SOURCES 
    cli/add_face.h
    cli/add_face.cc
    cli/main.cc
)
add_executable(facepass ${CLI_SOURCES})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(facepass ${OpenCV_LIBS} )
