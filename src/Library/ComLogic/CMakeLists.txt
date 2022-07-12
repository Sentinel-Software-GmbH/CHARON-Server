cmake_minimum_required(VERSION 3.19)

set(MODULE_NAME ComLogic)

add_library(${MODULE_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/charon_ServiceLookupTable.c
  ${CMAKE_CURRENT_LIST_DIR}/ charon_SessionAndSerivceControl.c
)
target_include_directories(${MODULE_NAME} 
  PRIVATE
  ${CHARON_SOURCE_DIR}/src/Library
  PUBLIC 
  ${CHARON_SOURCE_DIR}/src/Library/ComLogic
)
target_link_libraries(${MODULE_NAME}
  PUBLIC
  BusinessLogic
  Common
  HSDI
  Socket
)