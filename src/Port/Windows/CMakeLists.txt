cmake_minimum_required(VERSION 3.19)

add_subdirectory(config)

set(MODULE_NAME charonServerWindows)

add_executable(${MODULE_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/src/main.c
  ${CMAKE_CURRENT_LIST_DIR}/src/NvmEmulator.c
  ${CMAKE_CURRENT_LIST_DIR}/src/pipe.c
  ${CMAKE_CURRENT_LIST_DIR}/src/timer.c
  ${CMAKE_CURRENT_LIST_DIR}/src/uart.c
  ${CMAKE_CURRENT_LIST_DIR}/src/charon_interface_debug.c
)
target_include_directories(${MODULE_NAME} 
  PRIVATE
  ${CHARON_SOURCE_DIR}/src/Port/Windows/config
)
target_link_libraries(${MODULE_NAME}
  PUBLIC
  HSDI
  Interface
  Socket
  ${CHARON_CONFIG}
)
