cmake_minimum_required(VERSION 3.19)

#set name for port root directory
set(PORT_STM32_SOURCE ${CMAKE_CURRENT_LIST_DIR})


#set name for charonServer stm32f4 port executable
set(MODULE_NAME charonServerF4)
#create elf for f4
add_executable(${MODULE_NAME}.elf
    ${PORT_STM32_SOURCE}/Src/main/main.c
    $<TARGET_OBJECTS:PortF4_NvmEmulator>
    $<TARGET_OBJECTS:PortF4_stm32f4_hal_msp>
    $<TARGET_OBJECTS:PortF4_stm32f4_it>
    $<TARGET_OBJECTS:PortF4_system_stm32f4>
    $<TARGET_OBJECTS:PortF4_Timer>
    $<TARGET_OBJECTS:PortF4_Uart>
    ${PORT_STM32_SOURCE}/startup_stm32f407xx.s
  )
target_link_libraries(${MODULE_NAME}.elf
  PRIVATE
    CMSIS-Interface
    STM32F4_HAL-Interface
    STM32F4_HAL
    PortF4_stm32f4_hal_conf-Interface
    PortF4_Main-Interface
    PortF4_stm32f4_it-Interface
    PortF4_Timer-Interface
    PortF4_Uart-Interface
    charon_Uds-Interface
    charon_Uds
    charon_interface_NvmDriver-Interface
    charon_interface_debug-Interface
    charon_interface_clock-Interface
    charon_ISocket-Interface
    charon_types-Interface
    ${CHARON_CONFIG}
  )

#set linker file and linker options also create map file
set(LINKER_SCRIPT ${PORT_STM32_SOURCE}/STM32F407VGTx_FLASH.ld)
target_link_options(${MODULE_NAME}.elf 
  PRIVATE
    -T${LINKER_SCRIPT} --specs=nosys.specs
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}.map
)

set(HEX_FILE ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}.hex)
set(BIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}.bin)

add_custom_command(TARGET ${MODULE_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${MODULE_NAME}.elf> ${HEX_FILE}
        COMMENT "Building ${HEX_FILE}"
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${MODULE_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE} Building ${BIN_FILE}"
        )


add_subdirectory(Drivers)
add_subdirectory(Src)