if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()


if(PORT MATCHES "^Windows")
  set(ARCHITECTURES "-fno-set-stack-executable -mwin32 -mconsole -mnop-fun-dllimport") 
elseif(PORT MATCHES "^STM32F4")
  set(ARCHITECTURES " -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfloat-abi=hard -fdata-sections -ffunction-sections")  
  add_definitions("-DSTM32F407xx")
#elseif(PORT MATCHES "^standalone")
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

set(SHARED_LIBS OFF)
set(STATIC_LIBS ON)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(COMMON_FLAGS "${ARCHITECTURES} -O0 -Wall")
set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -fno-rtti -fno-exceptions")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -x assembler-with-cpp")

# Debug Flags
set(COMMON_DEBUG_FLAGS "-g3 -gdwarf-2 --debug")
set(CMAKE_C_FLAGS_DEBUG "${COMMON_DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_DEBUG_FLAGS}")
set(CMAKE_ASM_FLAGS_DEBUG "${COMMON_DEBUG_FLAGS}")
# Release Flags
set(COMMON_RELEASE_FLAGS "-DNDEBUG=1 -DRELEASE=1")
set(CMAKE_C_FLAGS_RELEASE "${COMMON_RELEASE_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_RELEASE_FLAGS}")
set(CMAKE_ASM_FLAGS_RELEASE "${COMMON_RELEASE_FLAGS}")

set(CMAKE_C_COMPILER_TARGET gcc)
set(CMAKE_CXX_COMPILER_TARGET arm-none-eabi)