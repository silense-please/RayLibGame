cmake_minimum_required(VERSION 3.24)
project(RayLibGame)

set(CMAKE_CXX_STANDARD 14)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/raylib) #link "sub-project" with its own CmakeLists e.t.c
#link_directories(LIBS_FOR_RELEASE_BUILD) # Links DLLs. Not needed when libs are statically linked.
#target_link_libraries(${PROJECT_NAME} libstdc++.a libgcc_eh.a) # Links static CRT(libc) libs MANUALLY.

add_executable(${PROJECT_NAME} src/main.cpp)


# That's just for testing. 
#add_executable(RayLibGame raylib/examples/core/core_window_letterbox.c)
#add_executable(RayLibGame raylib/examples/shapes/shapes_collision_area.c)
#add_executable(RayLibGame src/bunny_mark.cpp)
#add_executable(RayLibGame src/shadows_example.cpp)



if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "relapsed_debug-build")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    # Link static libc if build is release. In debug (dev) build libc is hooked by Windows' PATH variable.
    #https://stackoverflow.com/questions/18138635/mingw-exe-requires-a-few-gcc-dlls-regardless-of-the-code
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++  ${CMAKE_CXX_STANDARD_LIBRARIES}")

    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "relapsed_release-build")
    #target_link_options(${PROJECT_NAME} PRIVATE -mwindows)   # USE THIS TO HIDE CONSOLE
endif()


target_link_libraries(${PROJECT_NAME} raylib) # This is static raylib lib.

set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time") # Compile times.
#set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CMAKE_COMMAND} -E time") # Link times; ERROR - No such command.
