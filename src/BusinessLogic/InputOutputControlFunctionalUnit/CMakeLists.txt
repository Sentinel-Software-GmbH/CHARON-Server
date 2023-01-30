set(MODULE_NAME charon_InputOutputControlFunctionalUnit)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include/BusinessLogic/InputOutputControlFunctionalUnit
  )

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/charon_InputOutputControlFunctionalUnit.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    charon_interface_debug-Interface
    charon_SessionAndServiceControl-Interface
    charon_Uds-Interface
    charon_negativeResponse-Interface
    charon_types-Interface
    charon_ISocket-Interface
    ${CHARON_CONFIG}
  )