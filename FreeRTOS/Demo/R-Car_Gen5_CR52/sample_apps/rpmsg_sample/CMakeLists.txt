set(app_name "rpmsg_sample")

add_executable(${app_name}
    rpmsg-echo.c
    platform_rcar.c
    remoteproc_rcar.c
    rsc_table.c
)

target_compile_options(${app_name}
    PRIVATE
        -DENABLE_OPENAMP_
)

target_include_directories(${app_name}
    PRIVATE
    ${BSP_DIR}
    ${BSP_DIR}/CMSIS_5
    ${BSP_DIR}/CMSIS_5/CMSIS/Core_R/Include
)

# Essential libraries to link with
target_link_libraries(${app_name}
    freertos_bsp
    openamp
    libmetal
    m
)

target_link_options(${app_name} BEFORE
    PRIVATE
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${app_name}.map
)
