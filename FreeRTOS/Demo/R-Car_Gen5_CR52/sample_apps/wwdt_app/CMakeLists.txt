set(app_name "wwdt_app")

add_executable(${app_name}
    main.c
)

target_compile_definitions(${app_name}
    PUBLIC

)

# Essential libraries to link with
target_link_libraries(${app_name}
    freertos_bsp m c
)

# Add output program address map file
target_link_options(${app_name}
    PUBLIC
        -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${app_name}.map
)
