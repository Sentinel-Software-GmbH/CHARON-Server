set(MODULE_NAME PortWindows_showcaseData)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/port/windows/include/showcaseData
  )


add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/showcaseData.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    charon_StoredDataTransmissionFunctionalUnit-Interface
    charon_DTC_LookupTable-Interface
    charon_types-Interface
    charon_interface_NvmDriver-Interface
    ${CHARON_CONFIG}
  )
