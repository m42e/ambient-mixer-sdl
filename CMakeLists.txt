cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#include(CMakeToolchainClang.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(LibXml2 REQUIRED)
find_package(SDL2 REQUIRED )
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)

message (STATUS "SDL2_LIBRARY ${SDL2_LIBRARY}")
message (STATUS "SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIR}")

message (STATUS "SDL2_MIXER_LIBRARY ${SDL2_MIXER_LIBRARY}")
message (STATUS "SDL2_MIXER_INCLUDE_DIR ${SDL2_MIXER_INCLUDE_DIR}")

message (STATUS "SDL2_IMAGE_LIBRARY ${SDL2_IMAGE_LIBRARY}")
message (STATUS "SDL2_IMAGE_INCLUDE_DIR ${SDL2_INCLUDE_DIR}")
find_package(Threads REQUIRED)

add_subdirectory(googletest)
add_subdirectory(spdlog)
add_subdirectory(src)
add_subdirectory(test)

execute_process(COMMAND patch_compile_flags)

