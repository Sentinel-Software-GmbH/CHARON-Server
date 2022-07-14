cmake_minimum_required(VERSION 3.19)

set(MODULE_NAME CMSIS)

add_library(${MODULE_NAME}
  INTERFACE
)
target_include_directories(${MODULE_NAME}
  INTERFACE
  ${STM32_SOURCE}/Drivers/CMSIS/Core/Include
  ${STM32_SOURCE}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
)