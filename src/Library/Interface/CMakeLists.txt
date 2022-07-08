cmake_minimum_required(VERSION 3.19)

set(MODULE_NAME Interface)

add_library(${MODULE_NAME} 
${CMAKE_CURRENT_LIST_DIR}/charon_uds.c
)

target_include_directories(${MODULE_NAME} 
PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${MODULE_NAME} 
PUBLIC
BusinessLogic
ComLogic
Socket
)

add_subdirectory(Socket)