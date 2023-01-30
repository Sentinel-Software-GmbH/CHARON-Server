#add port directorie depending on preset
if(PORT_lower STREQUAL windows)
  add_subdirectory(windows)
elseif(PORT_lower STREQUAL stm32f4)
  add_subdirectory(stm32f4-discovery)
elseif(PORT_lower STREQUAL standalone)
  add_subdirectory(standalone)
elseif(PORT_lower STREQUAL demo)
  add_subdirectory(demo)
endif()

message(" PORT is ${PORT_lower} ")