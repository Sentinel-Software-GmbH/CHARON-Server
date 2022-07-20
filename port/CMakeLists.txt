cmake_minimum_required(VERSION 3.19)


if(PORT STREQUAL Windows)
  add_subdirectory(windows)
elseif(PORT STREQUAL STM32F4)
  add_subdirectory(stm32f4-discovery)
elseif(PROT STREQUAL standaloneLib)
  add_subdirectory(standalone)
endif()


message(INFO " ${CHARON_CONFIG}")