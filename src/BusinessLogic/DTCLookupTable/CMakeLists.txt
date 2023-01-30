set(MODULE_NAME charon_DTC_LookupTable)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )

target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include/BusinessLogic/DTCLookupTable
  )

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/Charon_DTC_LookupTable.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    charon_negativeResponse-Interface
    charon_types-Interface
    charon_StoredDataTransmissionFunctionalUnit-Interface
    charon_SessionAndServiceControl-Interface
    charon_interface_debug-Interface
    charon_interface_NvmDriver-Interface
    charon_ISocket-Interface
    ${CHARON_CONFIG}
    charon_Uds-Interface
  )