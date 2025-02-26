#
# Copyright (c) 2025 Renesas Electronics Corporation
#
# SPDX-License-Identifier: MIT
#

include(ExternalProject)

set (C_FLAGS
        "-mfloat-abi=soft -mcpu=cortex-r52 -Os -flto \
        -I${BSP_DIR}/include \
        -I${BSP_DIR}/common/ARM_CR52 \
        -I${KERNEL_DIR}/include"
)

get_target_property(libmetal_include_dir libmetal INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(libmetal_lib libmetal IMPORTED_LOCATION)

ExternalProject_Add(openamp_build
    GIT_REPOSITORY    https://github.com/OpenAMP/open-amp.git
    GIT_TAG           v2024.10.0
    GIT_CONFIG http.sslverify=false

    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
    SOURCE_DIR open-amp
    BINARY_DIR build
    INSTALL_DIR install

    DEPENDS libmetal

    CONFIGURE_COMMAND ${CMAKE_COMMAND}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_SOURCE_DIR}/toolchain-openamp.cmake 
        -DCMAKE_C_FLAGS=${C_FLAGS}
        -DWITH_DOC=OFF
        -DWITH_PROXY=OFF
        -DLIBMETAL_LIB=${libmetal_lib}
        -DLIBMETAL_INCLUDE_DIR=${libmetal_include_dir}
        ${CMAKE_CURRENT_BINARY_DIR}/open-amp
            
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --target all --verbose
    INSTALL_COMMAND make DESTDIR=${CMAKE_CURRENT_BINARY_DIR}/install install
)

add_library(openamp STATIC IMPORTED GLOBAL)
add_dependencies(openamp openamp_build)
set_target_properties(openamp PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/install/usr/local/lib/libopen_amp.a
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR}/install/usr/local/include
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
        ${CMAKE_CURRENT_LIST_DIR}/openampTargets.cmake
        ${CMAKE_CURRENT_LIST_DIR}/openampTargets-noconfig.cmake
    DESTINATION usr/local/lib/
)
