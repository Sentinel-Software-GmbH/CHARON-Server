cmake_minimum_required(VERSION 3.19)

set(MODULE_NAME BusinessLogic)

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
   PRIVATE
   Common
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


add_library(${MODULE_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/charon_DataLookupTable.c 
  ${CMAKE_CURRENT_LIST_DIR}/charon_DataTransmissionFunctionalUnit.c
  ${CMAKE_CURRENT_LIST_DIR}/charon_DiagnosticAndCommunicationManagementFunctionalUnit.c
  ${CMAKE_CURRENT_LIST_DIR}/charon_InputOutputControlFunctionalUnit.c
  ${CMAKE_CURRENT_LIST_DIR}/charon_RoutineFunctionalUnit.c
  ${CMAKE_CURRENT_LIST_DIR}/charon_StoredDataTransmissionFunctionalUnit.c
  ${CMAKE_CURRENT_LIST_DIR}/charon_UploadDownloadFunctionalUnit.c
)
target_include_directories(${MODULE_NAME} 
  INTERFACE PUBLIC
  ${CHARON_SOURCE_DIR}/include/BusinessLogic/DataLookupTable
  ${CHARON_SOURCE_DIR}/include/BusinessLogic/DataTransmissionFunctionalUnit
  ${CHARON_SOURCE_DIR}/include/BusinessLogic/DiagnosticAndCommunicationManagementFunctionalUnit
  ${CHARON_SOURCE_DIR}/include/BusinessLogic/InputOutputControlFunctionalUnit
  ${CHARON_SOURCE_DIR}/include/BusinessLogic/RoutineFunctionalUnit
  ${CHARON_SOURCE_DIR}/include/BusinessLogic/StoredDataTransmissionFunctionalUnit
  ${CHARON_SOURCE_DIR}/include/BusinessLogic/UploadDownloadFunctionalUnit
)
target_link_libraries(${MODULE_NAME} 
  PUBLIC
  Common
  HSDI
  Interface
  Socket
  ComLogic
  ${CHARON_CONFIG}
)