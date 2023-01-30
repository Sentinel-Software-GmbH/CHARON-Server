cmake_minimum_required(VERSION 3.19)

set(MODULE_NAME ${CHARON_CONFIG})

add_library(${MODULE_NAME} 
  INTERFACE
  )
target_include_directories(${MODULE_NAME}
  INTERFACE
    ${PROJECT_SOURCE_DIR}/port/demo/inc/config
  )