cmake_minimum_required(VERSION 3.19)

#add port directorie depending on preset
if(PORT MATCHES "^windows" OR PORT MATCHES "^Windows" OR PORT MATCHES "^WINDWOS")
  add_subdirectory(windows)
elseif(PORT MATCHES "^stm32f4" OR PORT MATCHES "^STM32F4")
  add_subdirectory(stm32f4-discovery)
elseif(PORT MATCHES "^standalone" OR PORT MATCHES "^Standalone" OR PORT MATCHES "^StandAlone")
  add_subdirectory(standalone)
endif()


