cmake_minimum_required(VERSION 3.19)

set(MODULE_NAME Socket)

add_library(${MODULE_NAME} 
  ${CMAKE_CURRENT_LIST_DIR}/DevNullSocket.c
)

target_include_directories(${MODULE_NAME} 
  PRIVATE
  ${CHARON_SOURCE_DIR}/src/Library
  PUBLIC 
  ${CHARON_SOURCE_DIR}/src/Library/Interface/Socket
)
