#set modulename and create stanalone library without dependency to other ports

set(MODULE_NAME charonStandAlone)

add_library(${MODULE_NAME}
    $<TARGET_OBJECTS:charon_Uds>
    $<TARGET_OBJECTS:charon_DataLookupTable>
    $<TARGET_OBJECTS:charon_DataTransmissionFunctionalUnit>
    $<TARGET_OBJECTS:charon_DCMFunctionalUnit>
    $<TARGET_OBJECTS:charon_DTC_LookupTable>
    $<TARGET_OBJECTS:charon_InputOutputControlFunctionalUnit>
    $<TARGET_OBJECTS:charon_RoutineFunctionalUnit>
    $<TARGET_OBJECTS:charon_StoredDataTransmissionFunctionalUnit>
    $<TARGET_OBJECTS:charon_UploadDownloadFunctionalUnit>
    $<TARGET_OBJECTS:charon_SessionAndServiceControl>
    $<TARGET_OBJECTS:charon_ServiceLookupTable>
    $<TARGET_OBJECTS:charon_Crc16>
    $<TARGET_OBJECTS:charon_negativeResponse>
    $<TARGET_OBJECTS:charon_DevNullSocket>
  )
  
target_link_libraries(${MODULE_NAME}
  PUBLIC
    ${CHARON_CONFIG}
    charon_DataLookupTable-Interface
    charon_Data_TransmissionFunctionalUnit-Interface
    charon_DCMFunctionalUnit-Interface
    charon_DTC_LookupTable-Interface
    charon_InputOutputControlFunctionalUnit-Interface
    charon_RoutineFunctionalUnit-Interface
    charon_StoredDataTransmissionFunctionalUnit-Interface
    charon_UploadDownloadFunctionalUnit-Interface
    charon_SessionAndServiceControl-Interface
    charon_ServiceLookupTable-Interface
    charon_negativeResponse-Interface
    charon_types-Interface
    charon_interface_debug-Interface
    charon_interface_NvmDriver-Interface
    charon_interface_clock-Interface
    charon_Crc16-Interface
    charon_Uds-Interface
    charon_Logo-Interface
    charon_ISocket-Interface
    charon_DevNullSocket-Interface
    charon_ISocket-Interface
  )

add_subdirectory(src)
    