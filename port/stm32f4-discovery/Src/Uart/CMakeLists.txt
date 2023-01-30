set(MODULE_NAME PortF4_Uart)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PORT_STM32_SOURCE}/Inc/Uart
  )

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/uart.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    ISocket-Interface
    STM32F4_HAL-Interface
    CMSIS-Interface
    charon_ISocket-Interface
  ) 