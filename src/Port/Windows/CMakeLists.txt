cmake_minimum_required(VERSION 3.19)

set(MODULE_NAME charonServerWindows)

add_library(NvmEmulator 
${CMAKE_CURRENT_LIST_DIR}/src/NvmEmulator.c
)






add_executable(${MODULE_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/src/main.c
  ${CMAKE_CURRENT_LIST_DIR}/src/NvmEmulator.c
  ${CMAKE_CURRENT_LIST_DIR}/src/pipe.c
  ${CMAKE_CURRENT_LIST_DIR}/src/timer.c
  ${CMAKE_CURRENT_LIST_DIR}/src/uart.c
)

target_include_directories(${MODULE_NAME} 
  PRIVATE
  ${CHARON_SOURCE_DIR}/src/Library
 
)

target_link_libraries(${MODULE_NAME}
  PUBLIC
  # HSDI
  Interface
  Socket
  # Common
  WindowsPortHSDI
)

add_subdirectory(src)