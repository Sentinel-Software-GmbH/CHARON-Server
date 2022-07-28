cmake_minimum_required(VERSION 3.19)


if(PORT MATCHES "^Windows")
  add_subdirectory(windows)
elseif(PORT MATCHES "^STM32F4")
  add_subdirectory(stm32f4-discovery)
elseif(PORT MATCHES "^standalone")
  add_subdirectory(standalone)
endif()


