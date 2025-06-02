#
# Copyright (c) 2025 Renesas Electronics Corporation
#
# SPDX-License-Identifier: MIT
#

set(app_name "power-management_app")

add_executable(${app_name}
    main.c
)

target_compile_definitions(${app_name}
    PUBLIC

)

target_include_directories(${app_name}
    PRIVATE
    ${BSP_DIR}
    ${BSP_DIR}/CMSIS_5
    ${BSP_DIR}/CMSIS_5/CMSIS/Core_R/Include
)

# Essential libraries to link with
target_link_libraries(${app_name}
    freertos_bsp m
)

# Add output program address map file
target_link_options(${app_name}
    PUBLIC
        -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${app_name}.map
)


convert_bin_to_srec(${app_name})
