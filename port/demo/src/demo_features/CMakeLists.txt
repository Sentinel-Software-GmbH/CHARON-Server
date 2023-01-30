set(MODULE_NAME PortDemo_showcaseDemoData)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/port/demo/inc/demo_features
  )

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/demo.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    charon_StoredDataTransmissionFunctionalUnit-Interface
    charon_interface_NvmDriver-Interface
    charon_DTC_LookupTable-Interface
    charon_types-Interface
    ${CHARON_CONFIG}
  )