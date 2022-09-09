cmake_minimum_required(VERSION 3.19)
#set up module and library name
set(MODULE_NAME HSDI)
# create interface or headeronly library
add_library(${MODULE_NAME}
  INTERFACE 
)
target_include_directories(${MODULE_NAME} 
  INTERFACE 
  ${CHARON_SOURCE_DIR}/include/HSDI
)
target_link_libraries(${MODULE_NAME}
  INTERFACE
  Common
)