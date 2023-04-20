cmake_minimum_required(VERSION 3.24)
project(RayLibGame)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/raylib)

add_executable(RayLibGame src/main.cpp)
#add_executable(RayLibGame raylib/examples/core/core_custom_frame_control.c)
#add_executable(RayLibGame src/bunny_mark.cpp)
#add_executable(RayLibGame src/shadows_example.cpp)

target_link_libraries(RayLibGame raylib)
