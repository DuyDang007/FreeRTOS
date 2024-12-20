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
