cmake_minimum_required(VERSION 3.19)
#set up name for whole library
set(MODULE_NAME BusinessLogic)

#set up name for sub library and create sub library from businesslogic
add_library(charon_DataLookupTable 
  ${CMAKE_CURRENT_LIST_DIR}/charon_DataLookupTable.c
)
target_include_directories(charon_DataLookupTable
  INTERFACE PUBLIC
  ${PROJECT_SOURCE_DIR}/include/BusinessLogic/DataLookupTable
)
target_link_libraries(charon_DataLookupTable
PRIVATE
  Common
  charon_ServiceLookupTable
  charon_SessionAndServiceControl
  Socket
)


add_library(charon_DataTransmissionFunctionalUnit 
  ${CMAKE_CURRENT_LIST_DIR}/charon_DataTransmissionFunctionalUnit.c
)
target_include_directories(charon_DataTransmissionFunctionalUnit
  INTERFACE PUBLIC
  ${PROJECT_SOURCE_DIR}/include/BusinessLogic/DataTransmissionFunctionalUnit
)
target_link_libraries(charon_DataTransmissionFunctionalUnit
PRIVATE
  charon_SessionAndServiceControl
  charon_DataLookupTable
  Interface
  Common
  Socket
  ${CHARON_CONFIG}
)


add_library(charon_DiagnosticAndCommunicationManagementFunctionalUnit
${CMAKE_CURRENT_LIST_DIR}/charon_DiagnosticAndCommunicationManagementFunctionalUnit.c
)
target_include_directories(charon_DiagnosticAndCommunicationManagementFunctionalUnit
  INTERFACE PUBLIC
  ${PROJECT_SOURCE_DIR}/include/BusinessLogic/DiagnosticAndCommunicationManagementFunctionalUnit
)
target_link_libraries(charon_DiagnosticAndCommunicationManagementFunctionalUnit
  PRIVATE
  charon_SessionAndServiceControl
  HSDI
  Common
  charon_ServiceLookupTable
  charon_StoredDataTransmissionFunctionalUnit
  Socket
  Interface
  ${CHARON_CONFIG}
)


add_library(charon_InputOutputControlFunctionalUnit
  ${CMAKE_CURRENT_LIST_DIR}/charon_InputOutputControlFunctionalUnit.c
)
target_include_directories(charon_InputOutputControlFunctionalUnit
INTERFACE PUBLIC
  ${PROJECT_SOURCE_DIR}/include/BusinessLogic/InputOutputControlFunctionalUnit
)
target_link_libraries(charon_InputOutputControlFunctionalUnit
PRIVATE
  HSDI
  charon_SessionAndServiceControl
  Interface
  Common
  Socket
  ${CHARON_CONFIG}
)


add_library(charon_RoutineFunctionalUnit
  ${CMAKE_CURRENT_LIST_DIR}/charon_RoutineFunctionalUnit.c
)
target_include_directories(charon_RoutineFunctionalUnit
INTERFACE PUBLIC
  ${PROJECT_SOURCE_DIR}/include/BusinessLogic/RoutineFunctionalUnit
)
target_link_libraries(charon_RoutineFunctionalUnit
PRIVATE
  HSDI
  Common
  charon_SessionAndServiceControl
  Socket
)


add_library(charon_StoredDataTransmissionFunctionalUnit
  ${CMAKE_CURRENT_LIST_DIR}/charon_StoredDataTransmissionFunctionalUnit.c
)
target_include_directories(charon_StoredDataTransmissionFunctionalUnit
INTERFACE PUBLIC
  ${PROJECT_SOURCE_DIR}/include/BusinessLogic/StoredDataTransmissionFunctionalUnit
)
 target_link_libraries(charon_StoredDataTransmissionFunctionalUnit
   PUBLIC
   Common
   ComLogic
   charon_DTC_LookupTable
 ) 


add_library(charon_UploadDownloadFunctionalUnit
  ${CMAKE_CURRENT_LIST_DIR}/charon_UploadDownloadFunctionalUnit.c
)
target_include_directories(charon_UploadDownloadFunctionalUnit
  INTERFACE PUBLIC
  ${PROJECT_SOURCE_DIR}/include/BusinessLogic/UploadDownloadFunctionalUnit
)
target_link_libraries(charon_UploadDownloadFunctionalUnit
  PRIVATE
  HSDI
  Interface
  charon_SessionAndServiceControl
  Common
  Socket
  ${CHARON_CONFIG}
)


add_library(charon_DTC_LookupTable 
  ${CMAKE_CURRENT_LIST_DIR}/charon_DTC_LookupTable.c
)
target_include_directories(charon_DTC_LookupTable
  INTERFACE PUBLIC
  ${PROJECT_SOURCE_DIR}/include/BusinessLogic/DTCLookupTable 
)
target_link_libraries(charon_DTC_LookupTable
  PRIVATE
  Common
  charon_StoredDataTransmissionFunctionalUnit
  HSDI
)

#create whole businesslogic library out of sub librariers
add_library(${MODULE_NAME}
  $<TARGET_OBJECTS:charon_DataLookupTable>
  $<TARGET_OBJECTS:charon_DataTransmissionFunctionalUnit>
  $<TARGET_OBJECTS:charon_DiagnosticAndCommunicationManagementFunctionalUnit>
  $<TARGET_OBJECTS:charon_DTC_LookupTable>
  $<TARGET_OBJECTS:charon_InputOutputControlFunctionalUnit>
  $<TARGET_OBJECTS:charon_RoutineFunctionalUnit>
  $<TARGET_OBJECTS:charon_StoredDataTransmissionFunctionalUnit>
  $<TARGET_OBJECTS:charon_UploadDownloadFunctionalUnit>
)
#link all sublibrariers and needed librariers
target_link_libraries(${MODULE_NAME} 
  PUBLIC
  charon_DataLookupTable
  charon_DataTransmissionFunctionalUnit
  charon_DiagnosticAndCommunicationManagementFunctionalUnit
  charon_DTC_LookupTable
  charon_InputOutputControlFunctionalUnit
  charon_RoutineFunctionalUnit
  charon_StoredDataTransmissionFunctionalUnit
  charon_UploadDownloadFunctionalUnit
  Common
  HSDI
  Interface
  Socket
  ComLogic
  ${CHARON_CONFIG}
)