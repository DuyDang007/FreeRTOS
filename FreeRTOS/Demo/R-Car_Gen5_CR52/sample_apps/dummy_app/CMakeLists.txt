#
# Copyright (c) 2025 Renesas Electronics Corporation
#
# SPDX-License-Identifier: MIT
#

set(CORES 0 1)

foreach(CORE IN LISTS CORES)
    if(CORE EQUAL 0)
        set(VRAM_REGION 1)
    elseif(CORE EQUAL 1)
        set(VRAM_REGION 2)
    else()
        message(FATAL_ERROR "INVALID CORE: ${CORE}")
    endif()

    set(app_name "dummy_app_core${CORE}")
    
    set(TEMP_LDFILE ${BSP_DIR}/common/linker/lscript_vram${VRAM_REGION}.ld)

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

    set_target_properties(${app_name} PROPERTIES
        LINK_OPTIONS ""  # Clear all link options
        LINK_FLAGS "-lnosys --specs=nosys.specs -L${BSP_DIR}/common/linker/ -T ${TEMP_LDFILE} -Wl,--no-warn-rwx-segments"
    )

    target_link_options(${app_name}
        PRIVATE
        -T${BSP_DIR}/common/linker/lscript_rsc_table_vram${VRAM_REGION}.ld
    )
    
    target_link_options(${app_name} BEFORE
        PRIVATE
        -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${app_name}.map
    )

    convert_bin_to_srec(${app_name})
endforeach()
