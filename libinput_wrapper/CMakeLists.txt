cmake_minimum_required(VERSION 3.5)

project(libinput_wrapper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} libinput_wrapper.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE -linput)
