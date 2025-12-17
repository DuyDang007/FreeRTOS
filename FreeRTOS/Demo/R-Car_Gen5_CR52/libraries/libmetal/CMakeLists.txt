#
# Copyright (c) 2025 Renesas Electronics Corporation
#
# SPDX-License-Identifier: MIT
#

include(ExternalProject)

set (C_FLAGS
    "-I${KERNEL_DIR}/include -I${BSP_DIR}/common/ARM_CR52 -I${BSP_DIR}/include \
    -mfloat-abi=soft -mcpu=cortex-r52 -Wall -Werror -Wextra -flto -Os"
)

ExternalProject_Add(libmetal_build
    GIT_REPOSITORY    https://github.com/OpenAMP/libmetal.git
    GIT_TAG           v2024.10.0
    GIT_CONFIG http.sslverify=false

    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
    SOURCE_DIR libmetal
    BINARY_DIR build
    INSTALL_DIR install

    # DEPENDS freertos_bsp

    CONFIGURE_COMMAND ${CMAKE_COMMAND}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_SOURCE_DIR}/toolchain-libmetal.cmake 
        -DCMAKE_C_FLAGS=${C_FLAGS}
        -DWITH_DOC=OFF
        ${CMAKE_CURRENT_BINARY_DIR}/libmetal
            
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --target all --verbose
    INSTALL_COMMAND make DESTDIR=${CMAKE_CURRENT_BINARY_DIR}/install install
)

add_library(libmetal STATIC IMPORTED GLOBAL)
add_dependencies(libmetal libmetal_build)

set_target_properties(libmetal PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/install/usr/local/lib/libmetal.a
    INTERFACE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_BINARY_DIR}/install/usr/local/include
)
# Workaround to avoid error: Imported target "libmetal" includes non-existent path
# The path does not exist at configuration time, but after build
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/install/usr/local/include)

install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/install/usr
    DESTINATION .
)
# Custom import cmake file, since IMPORTED library cannot be exported, we write it manually
install(
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/libmetalTargets.cmake
        ${CMAKE_CURRENT_LIST_DIR}/libmetalTargets-noconfig.cmake
    DESTINATION usr/local/lib/
)
