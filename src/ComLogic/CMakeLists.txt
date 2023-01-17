cmake_minimum_required(VERSION 3.19)
#set up name for whole comlogic library
set(MODULE_NAME ComLogic)

#set up name for sub library and create sub library from comlogic
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
  charon_DataTransmissionFunctionalUnit
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
  charon_DataTransmissionFunctionalUnit
  Common
  Interface
  HSDI
  Socket
  ${CHARON_CONFIG}
)

#create whole comlogic library out of sub librariers
add_library(${MODULE_NAME}
  $<TARGET_OBJECTS:charon_ServiceLookupTable>
  $<TARGET_OBJECTS:charon_SessionAndServiceControl>
)
target_link_libraries(${MODULE_NAME}
  PUBLIC
  charon_ServiceLookupTable
  charon_SessionAndServiceControl
  BusinessLogic
  Common
  HSDI
  Socket
  Interface
  ${CHARON_CONFIG}
)
