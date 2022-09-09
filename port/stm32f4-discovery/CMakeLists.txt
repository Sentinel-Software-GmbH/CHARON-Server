cmake_minimum_required(VERSION 3.19)

add_subdirectory(Drivers)
add_subdirectory(config)

#set name for port root directory
set(STM32_SOURCE ${CMAKE_CURRENT_LIST_DIR})
#set name for charonServer stm32f4 port executable
set(MODULE_NAME charonServerF4.elf)
#create elf for f4
add_executable(${MODULE_NAME}
  ${STM32_SOURCE}/Src/main.c
  ${STM32_SOURCE}/Src/NvmEmulator.c
  ${STM32_SOURCE}/Src/stm32f4xx_hal_msp.c
  ${STM32_SOURCE}/Src/stm32f4xx_it.c
  ${STM32_SOURCE}/Src/system_stm32f4xx.c
  ${STM32_SOURCE}/Src/timer.c
  ${STM32_SOURCE}/Src/uart.c
  ${STM32_SOURCE}/startup_stm32f407xx.s
)
target_include_directories(${MODULE_NAME}
  INTERFACE PUBLIC
  ${STM32_SOURCE}/Inc
)
target_link_libraries(${MODULE_NAME}
  PRIVATE
  CMSIS
  STM32F4_HAL
  Interface
  HSDI
  Socket
  ${CHARON_CONFIG}
)

#set linker file and linker options also create map file
set(LINKER_SCRIPT ${STM32_SOURCE}/STM32F407VGTx_FLASH.ld)
target_link_options(${MODULE_NAME} 
  PRIVATE
    -T${LINKER_SCRIPT} --specs=nosys.specs
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}.map
)
  