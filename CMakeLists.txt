cmake_minimum_required(VERSION 3.22)
project(facepass)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_executable(${PROJECT_NAME} cli/main.cc)
