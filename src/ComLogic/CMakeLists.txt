cmake_minimum_required(VERSION 3.19)

set(MODULE_NAME ComLogic)


add_library(charon_ServiceLookupTable
  ${CMAKE_CURRENT_LIST_DIR}/charon_ServiceLookupTable.c
)
target_include_directories(charon_ServiceLookupTable
  INTERFACE PUBLIC
  ${PROJECT_SOURCE_DIR}/include/ComLogic/ServiceLookupTable
)
target_link_libraries(charon_ServiceLookupTable
  PRIVATE
  charon_SessionAndServiceControl
  charon_DiagnosticAndCommunicationManagementFunctionalUnit
  charon_RoutineFunctionalUnit
  charon_UploadDownloadFunctionalUnit
  charon_InputOutputControlFunctionalUnit
  charon_StoredDataTransmissionFunctionalUnit
  Socket
  Common
)


add_library(charon_SessionAndServiceControl
  ${CMAKE_CURRENT_LIST_DIR}/charon_SessionAndServiceControl.c
)
target_include_directories(charon_SessionAndServiceControl
  INTERFACE PUBLIC
  ${PROJECT_SOURCE_DIR}/include/ComLogic/SessionAndServiceControl
)
target_link_libraries(charon_SessionAndServiceControl
  PRIVATE
  charon_ServiceLookupTable
  Common
  Interface
  HSDI
  Socket
  ${CHARON_CONFIG}
)



add_library(${MODULE_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/charon_ServiceLookupTable.c
  ${CMAKE_CURRENT_LIST_DIR}/charon_SessionAndServiceControl.c
)
target_include_directories(${MODULE_NAME} 
  INTERFACE PUBLIC
  ${CHARON_SOURCE_DIR}/include/ComLogic/ServiceLookupTable
  ${CHARON_SOURCE_DIR}/include/ComLogic/SessionAndServiceControl
)
target_link_libraries(${MODULE_NAME}
  PUBLIC
  BusinessLogic
  Common
  HSDI
  Socket
  Interface
  ${CHARON_CONFIG}
)
