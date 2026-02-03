#
# Copyright (c) 2025 Renesas Electronics Corporation
#
# SPDX-License-Identifier: MIT
#


set(rc_main "ucie_rc")

add_executable(${rc_main}
    ucie_rc.c
)

convert_bin_to_srec(${rc_main})

set(ep_main "ucie_ep")

add_executable(${ep_main}
    ucie_ep.c
)

convert_bin_to_srec(${ep_main})

target_compile_definitions(${rc_main}
    PUBLIC
)

target_compile_definitions(${ep_main}
    PUBLIC
)

# Essential libraries to link with
target_link_libraries(${rc_main}
    freertos_bsp m c
)

target_link_libraries(${ep_main}
    freertos_bsp m c
)

# Add output program address map file
target_link_options(${rc_main}
    PUBLIC
        -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${rc_main}.map
)

target_link_options(${ep_main}
    PUBLIC
        -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${ep_main}.map
)

set(app_name "ucie_mdp_rc")

add_executable(${app_name}
    ucie_mdp_rc.c
)

target_compile_definitions(${app_name}
    PUBLIC
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

set(app_name "ucie_mdp_ep")

add_executable(${app_name}
    ucie_mdp_ep.c
)

target_compile_definitions(${app_name}
    PUBLIC
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

