set(MODULE_NAME charon_interface_clock-Interface)

add_library(${MODULE_NAME}
  INTERFACE
  )
target_include_directories(${MODULE_NAME}
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include/HSDI/interface_clock
  )