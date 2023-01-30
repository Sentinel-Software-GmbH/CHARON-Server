cmake_minimum_required(VERSION 3.19)
#set up module and library name
set(MODULE_NAME Common)
#creat library and link all needed librariers
add_library(${MODULE_NAME} 
  $<TARGET_OBJECTS:charon_negativeResponse>
  $<TARGET_OBJECTS:charon_Crc16>
)
target_link_libraries(${MODULE_NAME} 
  PRIVATE
  charon_ServiceLookupTable-Interface
  charon_SessionAndServiceControl-Interface
  charon_Crc16-Interface
  charon_negativeResponse-Interface
  charon_types-Interface
  charon_ISocket-Interface
  charon_Uds-Interface
  ${CHARON_CONFIG}
)


add_subdirectory(Crc16)
add_subdirectory(NegativeResponse)
add_subdirectory(Types)