set(MODULE_NAME PortWindows_Terminal)

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/terminal.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    charon_Uds-Interface
    charon_ISocket-Interface
    charon_interface_debug-Interface
    ${CHARON_CONFIG}
  ) 