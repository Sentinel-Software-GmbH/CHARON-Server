set(MODULE_NAME charon_DataTransmissionFunctionalUnit)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include/BusinessLogic/DataTransmissionFunctionalUnit
  )

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/charon_DataTransmissionFunctionalUnit.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    charon_SessionAndServiceControl-Interface
    charon_DataLookupTable-Interface
    charon_DataLookupTable
    charon_Uds-Interface
    charon_types-Interface
    charon_ISocket-Interface
    charon_interface_debug-Interface
    charon_interface_clock-Interface
    charon_negativeResponse-Interface
    charon_interface_NvmDriver-Interface
    ${CHARON_CONFIG}  
  )