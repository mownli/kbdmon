cmake_minimum_required(VERSION 3.5)

project(kbdmon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(sdl)
add_subdirectory(libinput_wrapper)
add_subdirectory(config_reader)
add_subdirectory(test)

add_library(config_parser config_parser.cpp)
target_link_libraries(config_parser config_reader)

add_library(app app.cpp sdl_adapter.cpp)
target_link_libraries(app sdl libinput_wrapper config_parser)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} app)
