set(MODULE_NAME PortDemo_Timer)

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/timer.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    charon_interface_clock-Interface
)