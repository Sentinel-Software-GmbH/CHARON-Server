set(MODULE_NAME PortF4_Main)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PORT_STM32_SOURCE}/Inc/main
  )

