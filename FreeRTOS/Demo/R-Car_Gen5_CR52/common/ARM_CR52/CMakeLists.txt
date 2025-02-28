#*************************************************************************************************************
# Copyright (c) 2025 Renesas Electronics Corporation
#
# SPDX-License-Identifier: MIT
# *************************************************************************************************************/

add_library(freertos_kernel_port OBJECT)
target_sources(freertos_kernel_port
  PRIVATE
    port.c
    portmacro.h)
add_library(freertos_kernel_port_headers INTERFACE)
target_include_directories(freertos_kernel_port_headers INTERFACE
  .
  ${private_include_dir}
)
target_link_libraries(freertos_kernel_port
  PRIVATE
    freertos_kernel_port_headers
    freertos_kernel_include)
