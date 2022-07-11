cmake_minimum_required(VERSION 3.19)

set(MODULE_NAME Common)

add_library(${MODULE_NAME} 
  ${CMAKE_CURRENT_LIST_DIR}/charon_negativeResponse.c
)

target_include_directories(${MODULE_NAME} 
  PRIVATE
  ${CHARON_SOURCE_DIR}/src/Library
  PUBLIC 
  ${CHARON_SOURCE_DIR}/src/Library/Common
)

target_link_libraries(${MODULE_NAME} 
  PUBLIC
  ComLogic  
  HSDI
)