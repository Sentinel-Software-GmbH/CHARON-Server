set(MODULE_NAME windowsPort_showcaseData)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${WINDOWS_PORT_SOURCE}/include/showcaseData
  )


add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/showcaseData.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    charon_StoredDataTransmissionFunctionalUnit
  )
