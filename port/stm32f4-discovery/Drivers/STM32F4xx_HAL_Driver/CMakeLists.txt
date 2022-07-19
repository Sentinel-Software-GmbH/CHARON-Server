cmake_minimum_required(VERSION 3.19)

set(MODULE_NAME STM32F4_HAL)

add_library(${MODULE_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_cortex.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_dma_ex.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_dma.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_exti.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_flash_ex.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_flash_ramfunc.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_flash.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_gpio.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_pwr_ex.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_pwr.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_rcc_ex.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_rcc.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_tim_ex.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_tim.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_uart.c
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal.c
)
target_include_directories(${MODULE_NAME}
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/Inc
  ${CMAKE_CURRENT_LIST_DIR}/Inc/Legacy
  ${PROJECT_SOURCE_DIR}/port/stm32f4-discovery/Inc
)
target_link_libraries(${MODULE_NAME}
  PRIVATE
  CMSIS
)