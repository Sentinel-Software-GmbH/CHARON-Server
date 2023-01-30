set(MODULE_NAME PortWindows_interface_debug)

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/charon_interface_debug.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    charon_interface_debug-Interface
    charon_Uds-Interface
    charon_ISocket-Interface
    ${CHARON_CONFIG}
  )