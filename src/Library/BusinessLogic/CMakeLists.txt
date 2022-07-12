cmake_minimum_required(VERSION 3.19)

set(MODULE_NAME BusinessLogic)

add_library(${MODULE_NAME} 
  ${CMAKE_CURRENT_LIST_DIR}/charon_DataTransmissionFunctionalUnit.c
  ${CMAKE_CURRENT_LIST_DIR}/charon_DiagnosticAndCommunicationManagementFunctionalUnit.c
  ${CMAKE_CURRENT_LIST_DIR}/charon_InputOutputControlFunctionalUnit.c
  ${CMAKE_CURRENT_LIST_DIR}/charon_RoutineFunctionalUnit.c
  ${CMAKE_CURRENT_LIST_DIR}/charon_StoredDataTransmissionFunctionalUnit.c
  ${CMAKE_CURRENT_LIST_DIR}/charon_UploadDownloadFunctionalUnit.c
)

target_include_directories(${MODULE_NAME} 
  PRIVATE
  ${CHARON_SOURCE_DIR}/src/Library
  PUBLIC 
  ${CHARON_SOURCE_DIR}/src/Library/BusinessLogic
)

target_link_libraries(${MODULE_NAME} 
  PUBLIC
  Common
  WindowsPortHSDI
  HSDI
  # Socket
  # ComLogic
  # Interface
)