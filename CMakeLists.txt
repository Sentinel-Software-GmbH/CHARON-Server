cmake_minimum_required(VERSION 3.19)

project(Charon_Server LANGUAGES C ASM)

set(CHARON_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/toolchain/cmake)


if(PORT STREQUAL Windows)
  set(CHARON_CONFIG "windowsConfig")
elseif(PORT STREQUAL STM32F4)
  set(CHARON_CONFIG "F4Config")
endif()


add_subdirectory(src)
add_subdirectory(port)