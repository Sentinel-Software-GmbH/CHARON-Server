cmake_minimum_required(VERSION 3.19)

project(Charon_Server LANGUAGES C)

set(CHARON_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/toolchain/cmake)

add_subdirectory(src)