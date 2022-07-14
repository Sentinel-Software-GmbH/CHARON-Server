cmake_minimum_required(VERSION 3.19)


if(PORT STREQUAL Windows)
  add_subdirectory(Windows)
elseif(PORT STREQUAL STM32)
  add_subdirectory(STM32F4-Discovery)
endif()