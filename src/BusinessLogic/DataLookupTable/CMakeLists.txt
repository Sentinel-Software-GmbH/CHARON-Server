set(MODULE_NAME charon_DataLookupTable)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include/BusinessLogic/DataLookupTable
  )

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/charon_DataLookupTable.c  
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    charon_types-Interface
    charon_ServiceLookupTable-Interface
    charon_SessionAndServiceControl-Interface
    charon_ISocket-Interface
  )