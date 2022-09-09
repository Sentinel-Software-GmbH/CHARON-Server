cmake_minimum_required(VERSION 3.19)

#add port directorie depending on preset
if(PORT MATCHES "^windows")
  add_subdirectory(windows)
elseif(PORT MATCHES "^stm32f4")
  add_subdirectory(stm32f4-discovery)
elseif(PORT MATCHES "^standalone")
  add_subdirectory(standalone)
endif()


