cmake_minimum_required(VERSION 3.19)
#set up module and library name
set(MODULE_NAME Interface)
#create whole Interface library 
add_library(${MODULE_NAME} 
  ${CMAKE_CURRENT_LIST_DIR}/charon_uds.c
)
target_include_directories(${MODULE_NAME} 
  INTERFACE PUBLIC
  ${PROJECT_SOURCE_DIR}/include/Interface
)
target_link_libraries(${MODULE_NAME} 
  PRIVATE
  charon_DiagnosticAndCommunicationManagementFunctionalUnit
  charon_UploadDownloadFunctionalUnit
  charon_SessionAndServiceControl
  Socket
  Common
  HSDI
  ${CHARON_CONFIG}
)

add_subdirectory(Socket)