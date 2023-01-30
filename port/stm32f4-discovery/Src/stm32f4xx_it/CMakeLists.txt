set(MODULE_NAME PortF4_stm32f4_it)

add_library(${MODULE_NAME}-Interface 
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PORT_STM32_SOURCE}/Inc/stm32f4xx_it
  )

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/stm32f4xx_it.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    PortF4_Main-Interface  
    STM32F4_HAL-Interface
    STM32F4_HAL
    PortF4_stm32f4_hal_conf-Interface
    CMSIS-Interface
  )
