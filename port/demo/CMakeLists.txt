cmake_minimum_required(VERSION 3.19)

set(DEMO_PORT_SOURCE ${CMAKE_CURRENT_LIST_DIR})
#set name for charonServer Windows port executable
set(MODULE_NAME charonDemoShowCase)
#create windows executable
add_executable(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/src/main/main.c
    $<TARGET_OBJECTS:PortDemo_Pipe>
    $<TARGET_OBJECTS:PortDemo_Timer>
    $<TARGET_OBJECTS:PortDemo_NvmEmulator>
    $<TARGET_OBJECTS:PortDemo_interface_debug>
    $<TARGET_OBJECTS:PortDemo_Terminal>
    $<TARGET_OBJECTS:PortDemo_Uart>
    $<TARGET_OBJECTS:PortDemo_showcaseDemoData>
  )

target_link_libraries(${MODULE_NAME}
  PUBLIC
    charon_Uds-Interface
    charon_Uds
    charon_ISocket-Interface
    charon_StoredDataTransmissionFunctionalUnit-Interface
    charon_DTC_LookupTable-Interface
    charon_StoredDataTransmissionFunctionalUnit
    charon_types-Interface
    charon_interface_clock-Interface
    charon_DTC_LookupTable
    charon_interface_debug-Interface
    charon_interface_NvmDriver-Interface
    PortDemo_showcaseDemoData-Interface
    ${CHARON_CONFIG}
  )


add_subdirectory(src)