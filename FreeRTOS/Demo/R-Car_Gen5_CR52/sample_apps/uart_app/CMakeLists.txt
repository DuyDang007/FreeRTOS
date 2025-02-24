#
# Copyright (c) 2025 Renesas Electronics Corporation
#
# SPDX-License-Identifier: MIT
#

set(app_name "uart_app")

set(UART_ID 5 CACHE STRING "0:SCIF0, 1:SCIF1, 2:No support, 3:SCIF3, 4:SCIF4, 5:HSCIF0, 6:HSCIF1, 7:HSCIF2, 8:HSCIF3")
set(VALID_IDS "0;1;2;3;4;5;6;7;8")

list(FIND VALID_IDS "${UART_ID}" IDX)
if(IDX EQUAL -1)
    message(FATAL_ERROR "ID value must be: 0 - 8. You insert: ${UART_ID}")
endif()

if(UART_ID EQUAL 2)
    message(FATAL_ERROR "You insert: ${UART_ID} is not supported. Please insert another IDs 0 - 8")
endif()

add_executable(${app_name}
    main.c
)

target_compile_definitions(${app_name}
    PUBLIC UART_ID=${UART_ID}
)

# target_include_directories(${app_name}
#     PRIVATE
#         ${BSP_DIR}
# )

# Essential libraries to link with
target_link_libraries(${app_name}
    freertos_bsp m
)

# Add output program address map file
target_link_options(${app_name}
    PUBLIC
        -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${app_name}.map
)
