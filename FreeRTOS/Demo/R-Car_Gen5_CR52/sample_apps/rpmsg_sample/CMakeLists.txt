set(app_name "rpmsg_sample")

add_executable(${app_name}
    rpmsg-echo.c platform_rcar.c remoteproc_rcar.c
)

target_compile_options(${app_name}
    PRIVATE 
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
)
