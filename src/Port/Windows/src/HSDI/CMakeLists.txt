cmake_minimum_required(VERSION 3.19)

set(MODULE_NAME WindowsPortHSDI)

add_library(${MODULE_NAME} 
INTERFACE
)

target_include_directories(${MODULE_NAME} 
INTERFACE
${CMAKE_CURRENT_SOURCE_DIR}
)