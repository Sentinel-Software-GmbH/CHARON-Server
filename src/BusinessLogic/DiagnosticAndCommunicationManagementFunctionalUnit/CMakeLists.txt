set(MODULE_NAME charon_DCMFunctionalUnit)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include/BusinessLogic/DiagnosticAndCommunicationManagementFunctionalUnit
  )

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/charon_DiagnosticAndCommunicationManagementFunctionalUnit.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    charon_SessionAndServiceControl-Interface
    charon_interface_debug-Interface
    charon_negativeResponse-Interface
    charon_negativeResponse
    charon_types-Interface
    charon_ServiceLookupTable-Interface
    charon_StoredDataTransmissionFunctionalUnit-Interface
    charon_StoredDataTransmissionFunctionalUnit
    charon_ISocket-Interface
    charon_Uds-Interface
    ${CHARON_CONFIG}
  )