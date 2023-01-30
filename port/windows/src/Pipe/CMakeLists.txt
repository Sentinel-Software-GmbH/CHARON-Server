set(MODULE_NAME PortWindows_Pipe)

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/pipe.c
  ) 
target_link_libraries(${MODULE_NAME}
  PRIVATE
    charon_ISocket-Interface
  )