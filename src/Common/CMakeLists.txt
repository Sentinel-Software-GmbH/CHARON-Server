cmake_minimum_required(VERSION 3.19)
#set up module and library name
set(MODULE_NAME Common)
#creat library and link all needed librariers
add_library(${MODULE_NAME} 
  ${CMAKE_CURRENT_LIST_DIR}/charon_negativeResponse.c
)
target_include_directories(${MODULE_NAME} 
  INTERFACE PUBLIC
  ${CHARON_SOURCE_DIR}/include/Common
)
target_link_libraries(${MODULE_NAME} 
  PRIVATE
  charon_ServiceLookupTable
  charon_SessionAndServiceControl
  HSDI
  Socket
  Interface
  ${CHARON_CONFIG}
)


