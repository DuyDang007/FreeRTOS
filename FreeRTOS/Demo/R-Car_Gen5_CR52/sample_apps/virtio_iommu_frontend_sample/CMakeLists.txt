#
# Copyright (c) 2025 Renesas Electronics Corporation
#
# SPDX-License-Identifier: MIT
#


set(CORE 1)
set(VRAM_REGION 2)
set(MFIS_CHAN 1)


if(NOT MFIS_CHAN EQUAL 0 AND NOT MFIS_CHAN EQUAL 1 AND NOT MFIS_CHAN EQUAL 2)
    message(FATAL_ERROR "MFIS CHANNEL ${MFIS_CHAN}: RPMSG NOT supported!")
endif()

set(app_name "virtio_iommu_frontend_app_mfis${MFIS_CHAN}_cluster0_core${CORE}")

set(TEMP_LDFILE ${BSP_DIR}/common/linker/lscript_vram${VRAM_REGION}.ld)

add_executable(${app_name}
    virtio-iommu-frontend-app.c
)

target_compile_definitions(${app_name}
    PUBLIC MFIS_CHAN=${MFIS_CHAN}
)

# Essential libraries to link with
target_link_libraries(${app_name}
    freertos_bsp
    openamp
    libmetal
    m
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


