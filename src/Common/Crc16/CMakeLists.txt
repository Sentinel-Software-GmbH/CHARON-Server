set(MODULE_NAME charon_Crc16)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include/Common/Crc16
  )

add_library(${MODULE_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/charon_Crc16.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    
)