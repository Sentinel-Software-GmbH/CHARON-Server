cmake_minimum_required(VERSION 3.19)

add_subdirectory(config)

set(MODULE_NAME charonStandaloneLib)

add_library(${MODULE_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/src/charon_Dummy.c
)
target_include_directories(${MODULE_NAME}
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/inc
  ${CMAKE_CURRENT_LIST_DIR}/config
)
target_link_libraries(${MODULE_NAME}
  PUBLIC
  Interface
  HSDI
  Socket
  $<TARGET_OBJECTS:BusinessLogic>
  $<TARGET_OBJECTS:ComLogic>
  ${CHARON_CONFIG}
)
