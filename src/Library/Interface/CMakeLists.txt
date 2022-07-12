cmake_minimum_required(VERSION 3.19)

set(MODULE_NAME Interface)


add_library(${MODULE_NAME} 
  ${CMAKE_CURRENT_LIST_DIR}/charon_uds.c
)
target_include_directories(${MODULE_NAME} 
  INTERFACE PUBLIC
  ${CHARON_SOURCE_DIR}/src/Library
  ${CHARON_SOURCE_DIR}/src/Library/Interface
)
target_link_libraries(${MODULE_NAME} 
  PRIVATE
  BusinessLogic
  ComLogic
  Socket
  WindowsPortHSDI
)

add_subdirectory(Socket)