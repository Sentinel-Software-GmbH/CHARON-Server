cmake_minimum_required(VERSION 3.19)

set(WINDOWS_PORT_SOURCE ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(config)
#set name for charonServer Windows port executable
set(MODULE_NAME charonServerWindows)
#create windows executable
add_executable(${MODULE_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/src/main.c
  ${CMAKE_CURRENT_LIST_DIR}/src/NvmEmulator.c
  ${CMAKE_CURRENT_LIST_DIR}/src/pipe.c
  ${CMAKE_CURRENT_LIST_DIR}/src/timer.c
  ${CMAKE_CURRENT_LIST_DIR}/src/uart.c
  ${CMAKE_CURRENT_LIST_DIR}/src/charon_interface_debug.c
  ${CMAKE_CURRENT_LIST_DIR}/src/terminal.c
)

target_link_libraries(${MODULE_NAME}
  PUBLIC
  HSDI
  Interface
  Socket
  BusinessLogic
  windowsPort_showcaseData-Interface
  windowsPort_showcaseData
  ${CHARON_CONFIG}
)
add_subdirectory(src)