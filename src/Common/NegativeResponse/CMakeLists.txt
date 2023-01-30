set(MODULE_NAME charon_negativeResponse)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include/Common/NegativeResponse
  )

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/charon_negativeResponse.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    charon_SessionAndServiceControl-Interface
    charon_ServiceLookupTable-Interface
    charon_types-Interface
    charon_interface_debug-Interface
    charon_ISocket-Interface
    charon_Uds-Interface
    ${CHARON_CONFIG}
  )