set(MODULE_NAME charon_SessionAndServiceControl)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include/ComLogic/SessionAndServiceControl
  )

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/charon_SessionAndServiceControl.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    charon_ServiceLookupTable-Interface
    charon_ServiceLookupTable
    charon_negativeResponse-Interface
    charon_types-Interface
    charon_Uds-Interface
    charon_interface_clock-Interface
    charon_ISocket-Interface
    charon_interface_debug-Interface
    charon_DataTransmissionFunctionalUnit-Interface
    charon_DataTransmissionFunctionalUnit
    ${CHARON_CONFIG}
  )
