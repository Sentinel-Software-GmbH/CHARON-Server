set(MODULE_NAME ${CHARON_CONFIG})

add_library(${MODULE_NAME} 
  INTERFACE
)
target_include_directories(${MODULE_NAME}
  INTERFACE
    ${PORT_WINDOWS_SOURCE}/include/config
)