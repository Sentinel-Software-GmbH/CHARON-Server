cmake_minimum_required(VERSION 3.19)

set(MODULE_NAME ${CHARON_CONFIG})

add_library(${MODULE_NAME} 
  INTERFACE
)
target_include_directories(${MODULE_NAME}
  INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}
)