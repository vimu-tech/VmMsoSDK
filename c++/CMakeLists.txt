cmake_minimum_required(VERSION 3.14)

project(cpp-total)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
add_definitions(-D_DEBUG)
endif ()

get_filename_component(COMPILER_NAME ${CMAKE_CXX_COMPILER} NAME)
message("Compiler name: ${COMPILER_NAME}")
message("C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

    execute_process(
        COMMAND ${CMAKE_C_COMPILER} -dumpmachine
        OUTPUT_VARIABLE GCC_TARGET_TRIPLE
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "GCC target triple: ${GCC_TARGET_TRIPLE}")

    string(FIND ${GCC_TARGET_TRIPLE} "x86_64-linux-gnu" IS_LINUX_X64)
    string(FIND ${GCC_TARGET_TRIPLE} "arm-linux-gnueabi" IS_LINUX_ARM_EABI)
    string(FIND ${GCC_TARGET_TRIPLE} "arm-linux-gnueabihf" IS_LINUX_ARM_EABIHF)
    string(FIND ${GCC_TARGET_TRIPLE} "aarch64" IS_LINUX_AARCH64)

    if(EXISTS "/sys/firmware/devicetree/base/model")  # 更精确的判断，检查是否存在特定文件     # 在树莓派上执行的代码
        set (HOST "RaspberryPi")

        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            if(IS_LINUX_ARM_EABI GREATER -1)
                set (PLAT_DIR "arm-linux-gnueabi")
            elseif(IS_LINUX_ARM_EABIHF GREATER -1)
                set (PLAT_DIR "arm-linux-gnueabihf")
            elseif(IS_LINUX_AARCH64 GREATER -1)
                set (PLAT_DIR "aarch64-linux")   
            else()
                set (PLAT_DIR "X64")
            endif()
        endif()

    else()
        # X64编译器
        if(IS_LINUX_X64 GREATER -1)
            # 检测是否为Ubuntu
            execute_process(
                COMMAND lsb_release -is
                OUTPUT_VARIABLE DISTRIB_ID
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            if(DISTRIB_ID STREQUAL "Ubuntu")
                # 获取版本号
                execute_process(
                    COMMAND lsb_release -rs
                    OUTPUT_VARIABLE UBUNTU_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                )
                message(STATUS "Run in the Ubuntu ${UBUNTU_VERSION}")
                set (HOST Ubuntu${UBUNTU_VERSION})
            else()
                set (HOST "Linux")
            endif()
            set (PLAT_DIR "X64")
        else()
            set (HOST "Linux")
            message(STATUS "COMPILER_NAME ${COMPILER_NAME}")
            set (PLAT_DIR ${COMPILER_NAME})
        endif()
    endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")

    set (HOST ${CMAKE_SYSTEM_NAME})

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        string(FIND ${CMAKE_CXX_COMPILER} "mingw64" IS_WINDOWS_MINGW64)
        string(FIND ${CMAKE_CXX_COMPILER} "mingw32" IS_WINDOWS_MINGW32)
        if(IS_WINDOWS_MINGW64 GREATER -1) 
            set (PLAT_DIR "mingw64")
        elseif(IS_WINDOWS_MINGW32 GREATER -1) 
            set (PLAT_DIR "mingw32")
        endif()
    elseif(MSVC)
        # MSVC版本（对应Visual Studio版本）
        message(STATUS "MSVC_VERSION: ${MSVC_VERSION}")
        # 工具集版本
        message(STATUS "MSVC_TOOLSET_VERSION: ${MSVC_TOOLSET_VERSION}")
        # 编译器工具版本
        message(STATUS "CMAKE_VS_PLATFORM_TOOLSET: ${CMAKE_VS_PLATFORM_TOOLSET}")
        
        # Visual Studio版本
        message(STATUS "CMAKE_GENERATOR: ${CMAKE_GENERATOR}")
        message(STATUS "CMAKE_GENERATOR_TOOLSET: ${CMAKE_GENERATOR_TOOLSET}")

        # if(CMAKE_EXE_LINKER_FLAGS MATCHES "/machine:ARM64")
        #     set (PLAT_DIR "${CMAKE_GENERATOR}/AMR64")
        # elseif(CMAKE_EXE_LINKER_FLAGS MATCHES "/machine:X86")
        #     set (PLAT_DIR "${CMAKE_GENERATOR}/X86")
        # else()
        #     set (PLAT_DIR "${CMAKE_GENERATOR}/X64")
        # endif()

        if(CMAKE_EXE_LINKER_FLAGS MATCHES "/machine:ARM64")
            set (PLAT_DIR "AMR64")
        elseif(CMAKE_EXE_LINKER_FLAGS MATCHES "/machine:X86")
            set (PLAT_DIR "X86")
        else()
            set (PLAT_DIR "X64")
        endif()
    endif()

endif()

message(">>> HOST=${HOST} PLAT_DIR=${PLAT_DIR} ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER}")

set (SHARED_LIB_SUBDIR SharedLibrary/${HOST}/${PLAT_DIR})
set (SHARED_LIB_DIR ${PROJECT_SOURCE_DIR}/${SHARED_LIB_SUBDIR})

# 包含子项目。

add_subdirectory( "./DllTest-Polling") 
add_subdirectory( "./DllTest-Callback")
add_subdirectory( "./DllTestForMso41-Callback") 
add_subdirectory( "./DLLMultDev-Callback")
add_subdirectory( "./DLLMultDev-Stream")


