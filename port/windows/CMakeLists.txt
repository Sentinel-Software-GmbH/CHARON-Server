cmake_minimum_required(VERSION 3.19)

set(PORT_WINDOWS_SOURCE ${CMAKE_CURRENT_LIST_DIR})

#set name for charonServer Windows port executable
set(MODULE_NAME charonServerWindows)
#create windows executable
add_executable(${MODULE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/src/main/main.c
    $<TARGET_OBJECTS:PortWindows_Pipe>
    $<TARGET_OBJECTS:PortWindows_Timer>
    $<TARGET_OBJECTS:PortWindows_NvmEmulator>
    $<TARGET_OBJECTS:PortWindows_interface_debug>
    $<TARGET_OBJECTS:PortWindows_Terminal>
    $<TARGET_OBJECTS:PortWindows_Uart>
    $<TARGET_OBJECTS:PortWindows_showcaseData>
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
    PortWindows_showcaseData-Interface
    ${CHARON_CONFIG}
  )

add_definitions("-DShowCaseForDummys")


add_subdirectory(src)