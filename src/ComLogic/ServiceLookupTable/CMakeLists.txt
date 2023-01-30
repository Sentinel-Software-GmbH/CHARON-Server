
set(MODULE_NAME charon_ServiceLookupTable)
#set up name for sub library and create sub library from comlogic
add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include/ComLogic/ServiceLookupTable
  )

add_library(${MODULE_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/charon_ServiceLookupTable.c
)
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    charon_SessionAndServiceControl-Interface
    charon_DCMFunctionalUnit-Interface
    charon_RoutineFunctionalUnit-Interface
    charon_RoutineFunctionalUnit
    charon_UploadDownloadFunctionalUnit-Interface
    charon_InputOutputControlFunctionalUnit-Interface
    charon_InputOutputControlFunctionalUnit
    charon_StoredDataTransmissionFunctionalUnit-Interface
    charon_DataTransmissionFunctionalUnit-Interface
    ${CHARON_CONFIG}
    charon_ISocket-Interface
    charon_types-Interface
  )
