set(MODULE_NAME PortF4_stm32f4_hal_conf)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PORT_STM32_SOURCE}/Inc/stm32f4xx_hal_conf
)