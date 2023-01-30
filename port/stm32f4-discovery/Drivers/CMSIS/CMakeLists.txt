cmake_minimum_required(VERSION 3.19)
#set module name and create cmsis monolith library for hal
set(MODULE_NAME CMSIS-Interface)

add_library(${MODULE_NAME}
  INTERFACE
)
target_include_directories(${MODULE_NAME}
  INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/Core/Include
  ${CMAKE_CURRENT_LIST_DIR}/Device/ST/STM32F4xx/Include
)