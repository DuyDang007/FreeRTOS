#*************************************************************************************************************
# Copyright (c) 2025 Renesas Electronics Corporation
#
# SPDX-License-Identifier: MIT
# *************************************************************************************************************/

set(app_name "main_blinky")

add_executable(${app_name}
    main_blinky.c
)

target_include_directories(${app_name}
    PRIVATE
        ${FREERTOS_DIR}/FreeRTOS/Demo/Common/include
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
