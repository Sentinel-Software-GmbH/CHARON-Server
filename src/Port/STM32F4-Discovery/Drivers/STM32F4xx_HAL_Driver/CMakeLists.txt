cmake_minimum_required(VERSION 3.19)

set(MODULE_NAME STM32F4_HAL)

add_library(${MODULE_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/Src/stm32f4xx_hal_cortex.c
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
)
target_include_directories(${MODULE_NAME} 
  INTERFACE PUBLIC
  ${STM32_SOURCE}/Drivers/STM32F4xx_HAL_Driver/Inc
)
