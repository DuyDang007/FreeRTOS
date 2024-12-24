set(app_name "cpp_app")

add_executable(${app_name}
    main.cc
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
