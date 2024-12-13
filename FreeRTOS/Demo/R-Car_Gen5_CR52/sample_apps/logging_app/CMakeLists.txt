set(app_name "logging_app")

add_executable(${app_name}
    main.c
)

### Add option to set log level
option(LIBRARY_LOG_LEVEL "Log level (0: NONE, 1: ERROR, 2: INFO, 3: DEBUG)" 0)

### Include header
include_directories(
    ${LOGGING_DIR}
)

target_compile_definitions(${app_name}
    PUBLIC
        LIBRARY_LOG_NAME="Logging"
        $<$<BOOL:${LIBRARY_LOG_LEVEL}>:LIBRARY_LOG_LEVEL=${LIBRARY_LOG_LEVEL}>

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
