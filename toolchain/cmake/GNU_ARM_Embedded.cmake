if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR armv7-m)

#select arm toolchain and name it for later perpuse
set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(ARM_GCC_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(ARM_GCC_CPP_COMPILER ${TOOCHAIN_PREFIX}g++)
set(ARM_GCC_OBJECT_DUMP ${TOOCHAIN_PREFIX}objdump)
set(ARM_GCC_OBJECT_COPY ${TOOCHAIN_PREFIX}objcopy)

# set compiler using costum names
set(CMAKE_ASM_COMPILER ${ARM_GCC_C_COMPILER})
set(CMAKE_C_COMPILER ${ARM_GCC_C_COMPILER})
set(CMAKE_OBJCOPY ${ARM_GCC_OBJECT_COPY})
set(CMAKE_OBJDUMP ${ARM_GCC_OBJECT_DUMP})
set(CMAKE_CXX_COMPILER ${ARM_GCC_CPP_COMPILER})
set(CMAKE_C_COMPILE_FEATURES "c_std_99")

#set target library type
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
#set library linker flags

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_ASM_FLAGS "")


# insert switch so wop library types from shared to static
set(SHARED_LIBS OFF)
set(STATIC_LIBS ON)

# Generate compile_commands.json can be used for cppCheck 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default C compiler flags
if(PORT MATCHES "^stm32f4" OR PORT MATCHES "^STM32F4")
  set(ARCHITECTURES "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mlittle-endian -fdata-sections -ffunction-sections")  
  add_definitions("-DSTM32F407xx")
endif()
set(COMMON_C_FLAGS "-Wall -pedantic")
set(CMAKE_C_FLAGS_DEBUG_INIT "-g3 -O0 -gdwarf-2 --debug ${COMMON_C_FLAGS} ${ARCHITECTURES}")
set(CMAKE_C_FLAGS_RELEASE_INIT "-O3 -g0 ${COMMON_C_FLAGS} ${ARCHITECTURES}")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -fno-rtti -fno-exceptions")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -x assembler-with-cpp")

set(CMAKE_C_COMPILER_TARGET arm-none-eabi)
set(CMAKE_CXX_COMPILER_TARGET arm-none-eabi)
