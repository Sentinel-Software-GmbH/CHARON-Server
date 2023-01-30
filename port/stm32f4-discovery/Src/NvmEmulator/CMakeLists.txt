set(MODULE_NAME PortF4_NvmEmulator)

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/NvmEmulator.c
  ) 
target_link_libraries(${MODULE_NAME}
  PRIVATE
    charon_interface_NvmDriver-Interface
    charon_types-Interface
  )