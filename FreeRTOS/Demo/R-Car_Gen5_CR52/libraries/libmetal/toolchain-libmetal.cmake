#
# Copyright (c) 2025 Renesas Electronics Corporation
#
# SPDX-License-Identifier: MIT
#

set(CMAKE_SYSTEM_NAME       FreeRTOS)
set(CMAKE_SYSTEM_PROCESSOR  arm)
set(MACHINE                 "metal" CACHE STRING "")


# Define TOOLCHAIN: COMPILER, ASSEMBLER, ACHIVER, LINKER
find_program(CMAKE_C_COMPILER       NAMES "arm-none-eabi-gcc")
find_program(CMAKE_CXX_COMPILER     NAMES "arm-none-eabi-g++")
find_program(CMAKE_ASM_COMPILER     NAMES "arm-none-eabi-gcc")
find_program(CMAKE_AR               NAMES "arm-none-eabi-gcc-ar")
find_program(CMAKE_LINKER           NAMES "arm-none-eabi-gcc")
find_program(CMAKE_OBJCOPY          NAMES "arm-none-eabi-objcopy")
find_program(CMAKE_BIN_CONVERTER    NAMES "arm-none-eabi-objcopy")

if(CMAKE_C_COMPILER)
    string(REGEX REPLACE "gcc$" "" CROSS_PREFIX "${CMAKE_C_COMPILER}")
else()
    message(FATAL_ERROR "Compiler not found!")
endif()

# Set build rule
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_USER_MAKE_RULES_OVERRIDE      ${CMAKE_CURRENT_LIST_FILE})
set(CMAKE_ASM_CREATE_STATIC_LIBRARY     "<CMAKE_AR> -r <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_C_CREATE_STATIC_LIBRARY       "<CMAKE_AR> -r <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_CXX_CREATE_STATIC_LIBRARY     "<CMAKE_AR> -r <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_C_ARCHIVE_CREATE              "<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_C_LINK_EXECUTABLE             "<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES> -o <TARGET>")
set(CMAKE_CXX_LINK_EXECUTABLE           "<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> <LINK_LIBRARIES> -o <TARGET>")

include (cross-freertos-gcc)
