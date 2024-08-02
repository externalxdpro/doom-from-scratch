cmake_minimum_required(VERSION 3.16)
project(game VERSION 0.1.0 DESCRIPTION "Doom from Scratch")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(SOURCE_FILES src/main.cpp)

add_executable(game ${SOURCE_FILES})
target_include_directories(game PRIVATE include)
target_link_directories(game PRIVATE lib)
target_link_libraries(game glfw glut GLU GL X11 pthread dl m)
