set(MODULE_NAME PortF4_system_stm32f4)

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/system_stm32f4xx.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    CMSIS-Interface  
  )