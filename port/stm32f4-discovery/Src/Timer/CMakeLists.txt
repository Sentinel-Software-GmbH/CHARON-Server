set(MODULE_NAME PortF4_Timer)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PORT_STM32_SOURCE}/Inc/Timer
  )

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/timer.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    charon_interface_clock-Interface
    STM32F4_HAL-Interface
    CMSIS-Interface
  )