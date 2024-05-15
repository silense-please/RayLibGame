cmake_minimum_required(VERSION 3.24)
project(RayLibGame)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++")



add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/raylib) #link "sub-project" with its own CmakeLists e.t.c
#link_directories(LIBS_FOR_RELEASE_BUILD)

add_executable(RayLibGame src/main.cpp)


#add_executable(RayLibGame raylib/examples/audio/audio_module_playing.c)
#add_executable(RayLibGame raylib/examples/shapes/shapes_collision_area.c)
#add_executable(RayLibGame src/bunny_mark.cpp)
#add_executable(RayLibGame src/shadows_example.cpp)



if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(RayLibGame libstdc++.a) # Link static library if build is release
endif()
#target_link_options(RayLibGame PRIVATE -mwindows)   # USE THIS TO HIDE CONSOLE
target_link_libraries(RayLibGame raylib)

#target_link_libraries(RayLibGame raylib libatomic-1.dll libgcc_s_dw2-1.dll libgfortran-4.dll libgomp-1.dll libquadmath-0.dll libssp-0.dll libstdc++-6.dll libwinpthread-1.dll)
