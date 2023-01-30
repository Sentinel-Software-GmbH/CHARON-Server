set(MODULE_NAME charon_types-Interface)

add_library(${MODULE_NAME}
  INTERFACE
  )
target_include_directories(${MODULE_NAME}
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include/Common/Types
  )