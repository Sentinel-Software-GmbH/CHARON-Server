cmake_minimum_required(VERSION 3.19)

add_subdirectory(config)
#set name for charonServer Windows port executable
set(MODULE_NAME charonShowCase)
#create windows executable
add_executable(${MODULE_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/src/main.c
  ${CMAKE_CURRENT_LIST_DIR}/src/NvmEmulator.c
  ${CMAKE_CURRENT_LIST_DIR}/src/pipe.c
  ${CMAKE_CURRENT_LIST_DIR}/src/timer.c
  ${CMAKE_CURRENT_LIST_DIR}/src/uart.c
  ${CMAKE_CURRENT_LIST_DIR}/src/charon_interface_debug.c
  ${CMAKE_CURRENT_LIST_DIR}/src/terminal.c
  ${CMAKE_CURRENT_LIST_DIR}/src/demo.c
)

target_include_directories(${MODULE_NAME}
  INTERFACE PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/inc

)

target_link_libraries(${MODULE_NAME}
  PUBLIC
  HSDI
  Interface
  Socket
  charon_StoredDataTransmissionFunctionalUnit
  ${CHARON_CONFIG}
)
