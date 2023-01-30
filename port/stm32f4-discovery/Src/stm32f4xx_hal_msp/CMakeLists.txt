set(MODULE_NAME PortF4_stm32f4_hal_msp)

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/stm32f4xx_hal_msp.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
      PortF4_Main-Interface
      STM32F4_HAL-Interface
      STM32F4_HAL
      PortF4_stm32f4_hal_conf-Interface
      CMSIS-Interface
  )