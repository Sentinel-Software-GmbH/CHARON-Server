set(MODULE_NAME charon_StoredDataTransmissionFunctionalUnit)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include/BusinessLogic/StoredDataTransmissionFunctionalUnit
  )

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/charon_StoredDataTransmissionFunctionalUnit.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    charon_negativeResponse-Interface
    charon_negativeResponse
    charon_Crc16-Interface
    charon_Crc16
    charon_types-Interface
    charon_DTC_LookupTable-Interface
    charon_DTC_LookupTable
    charon_Crc16-Interface
    charon_interface_debug-Interface
    charon_interface_NvmDriver-Interface
    charon_Uds-Interface
    charon_ISocket-Interface
    charon_SessionAndServiceControl-Interface
    ${CHARON_CONFIG}
  )