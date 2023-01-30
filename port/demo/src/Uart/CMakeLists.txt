set(MODULE_NAME PortDemo_Uart)

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/uart.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    charon_interface_debug-Interface
    charon_ISocket-Interface
    charon_Uds-Interface
    ${CHARON_CONFIG}
  )