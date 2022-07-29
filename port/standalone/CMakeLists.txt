cmake_minimum_required(VERSION 3.19)

add_subdirectory(config)

set(MODULE_NAME charonStandaloneLib)

add_library(${MODULE_NAME}
  $<TARGET_OBJECTS:BusinessLogic>
  $<TARGET_OBJECTS:ComLogic>
  $<TARGET_OBJECTS:Common>
  $<TARGET_OBJECTS:Socket>
  $<TARGET_OBJECTS:Interface>
)
target_include_directories(${MODULE_NAME}
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/inc
  ${CMAKE_CURRENT_LIST_DIR}/config
)
target_link_libraries(${MODULE_NAME}
  PUBLIC
  ${CHARON_CONFIG}
  HSDI
  Interface
  Socket
)
