set(MODULE_NAME charon_DevNullSocket)

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )
target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include/Sockets/DevNullSocket
  )

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/DevNullSocket.c
  )
target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}-Interface
    charon_ISocket-Interface
  )