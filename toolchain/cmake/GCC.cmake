if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_SYSTEM_NAME Windows)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_OBJCOPY objcopy)
set(CMAKE_OBJDUMP objdump) 

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

set(SHARED_LIBS OFF)
set(STATIC_LIBS ON)


# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(COMMON_FLAGS "-O0 -Wall -fno-set-stack-executable -mwin32 -mconsole -mnop-fun-dllimport")
set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
# set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -fno-rtti -fno-exceptions")

# Debug Flags
set(COMMON_DEBUG_FLAGS "-g3 -gdwarf-2 --debug")
set(CMAKE_C_FLAGS_DEBUG "${COMMON_DEBUG_FLAGS}")

# Release Flags
set(COMMON_RELEASE_FLAGS "-DNDEBUG=1 -DRELEASE=1")
set(CMAKE_C_FLAGS_RELEASE "${COMMON_RELEASE_FLAGS}")

set(CMAKE_C_COMPILER_TARGET gcc)
