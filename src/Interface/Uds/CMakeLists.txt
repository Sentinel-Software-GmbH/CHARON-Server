set(MODULE_NAME charon_Uds)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include/Interface/Uds
)

#create whole Interface library 
add_library(${MODULE_NAME} 
    ${CMAKE_CURRENT_LIST_DIR}/charon_uds.c
)
target_link_libraries(${MODULE_NAME} 
  PRIVATE
    ${MODULE_NAME}-Interface
    charon_DCMFunctionalUnit-Interface
    charon_DCMFunctionalUnit
    charon_UploadDownloadFunctionalUnit-Interface
    charon_UploadDownloadFunctionalUnit
    charon_SessionAndServiceControl-Interface
    charon_SessionAndServiceControl
    charon_Logo-Interface
    charon_ISocket-Interface
    charon_types-Interface
    charon_interface_debug-Interface
    ${CHARON_CONFIG}
  )