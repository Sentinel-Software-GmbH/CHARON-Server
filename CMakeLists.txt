cmake_minimum_required(VERSION 3.19)
#set name of Project and specify which languages ar allowed
project(Charon_Server LANGUAGES C ASM)
#set up extra name for Project Top level directory
set(CHARON_SOURCE_DIR ${PROJECT_SOURCE_DIR})
#set up path where to  find .cmake files for toolchain oder cmake specifig files 
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/toolchain/cmake)

#set up name for charon config file to  specify which portis used
string(TOLOWER ${PORT} PORT_lower)
if(PORT_lower STREQUAL windows )
  set(CHARON_CONFIG "windowsConfig")
elseif(PORT_lower STREQUAL stm32f4)
  set(CHARON_CONFIG "F4Config")
elseif(PORT_lower STREQUAL standalone)
  set(CHARON_CONFIG "standaloneConfig")
endif()

add_subdirectory(src)
add_subdirectory(port)