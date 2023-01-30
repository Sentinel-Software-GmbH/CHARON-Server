cmake_minimum_required(VERSION 3.19)
#set up name for whole library
set(MODULE_NAME BusinessLogic)

#create whole businesslogic library out of sub librariers
add_library(${MODULE_NAME}
  $<TARGET_OBJECTS:charon_DataLookupTable>
  $<TARGET_OBJECTS:charon_DataTransmissionFunctionalUnit>
  $<TARGET_OBJECTS:charon_DCMFunctionalUnit>
  $<TARGET_OBJECTS:charon_DTC_LookupTable>
  $<TARGET_OBJECTS:charon_InputOutputControlFunctionalUnit>
  $<TARGET_OBJECTS:charon_RoutineFunctionalUnit>
  $<TARGET_OBJECTS:charon_StoredDataTransmissionFunctionalUnit>
  $<TARGET_OBJECTS:charon_UploadDownloadFunctionalUnit>
)
#link all sublibrariers and needed librariers
target_link_libraries(${MODULE_NAME} 
  PRIVATE
  charon_DataLookupTable-Interface
  charon_DataTransmissionFunctionalUnit-Interface
  charon_DCMFunctionalUnit-Interface
  charon_DTC_LookupTable-Interface
  charon_InputOutputControlFunctionalUnit-Interface
  charon_RoutineFunctionalUnit-Interface
  charon_StoredDataTransmissionFunctionalUnit-Interface
  charon_UploadDownloadFunctionalUnit-Interface
  charon_types-Interface
  charon_interface_clock-Interface
  charon_interface_NvmDriver-Interface
  charon_interface_debug-Interface
  charon_Uds-Interface
  charon_ISocket-Interface
  ${CHARON_CONFIG}
)

add_subdirectory(DataLookupTable)
add_subdirectory(DataTransmissionFunctionalUnit)
add_subdirectory(DiagnosticAndCommunicationManagementFunctionalUnit)
add_subdirectory(DTCLookupTable)
add_subdirectory(InputOutputControlFunctionalUnit)
add_subdirectory(RoutineFunctionalUnit)
add_subdirectory(StoredDataTransmissionFunctionalUnit)
add_subdirectory(UploadDownloadFunctionalUnit)