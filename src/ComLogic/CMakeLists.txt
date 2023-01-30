cmake_minimum_required(VERSION 3.19)
#set up name for whole comlogic library
set(MODULE_NAME ComLogic)
#create whole comlogic library out of sub librariers
add_library(${MODULE_NAME}
  $<TARGET_OBJECTS:charon_ServiceLookupTable>
  $<TARGET_OBJECTS:charon_SessionAndServiceControl>
)
target_link_libraries(${MODULE_NAME}
  PRIVATE
  ${CHARON_CONFIG}
  charon_ServiceLookupTable-Interface
  charon_SessionAndServiceControl-Interface
  BusinessLogic
  charon_types-Interface
  charon_interface_clock-Interface
  charon_ISocket-Interface
  charon_Uds-Interface
  ${CHARON_CONFIG}
)


add_subdirectory(ServiceLookupTable)
add_subdirectory(SessionAndServiceControl)