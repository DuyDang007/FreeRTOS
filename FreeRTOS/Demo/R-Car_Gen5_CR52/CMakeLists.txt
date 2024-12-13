cmake_minimum_required(VERSION 3.21.0)

project(rcar-freertos-bsp VERSION 1.0.0)

enable_language(C ASM)

file(REAL_PATH ${CMAKE_SOURCE_DIR}/../../.. FREERTOS_DIR)
set(BSP_DIR ${CMAKE_SOURCE_DIR})
set(KERNEL_DIR ${FREERTOS_DIR}/FreeRTOS/Source)
set(LOGGING_DIR ${FREERTOS_DIR}/FreeRTOS-Plus/Source/Utilities/logging)

message(STATUS ${BSP_DIR})
message(STATUS ${POSIX_DIR})
message(STATUS ${KERNEL_DIR})
message(STATUS ${LOGGING_DIR})

### Check the argumment
set(BOARD "x5h_vdk")
set(LDFILE ${BSP_DIR}/common/linker/lscript_sram.ld)
if(BOARD STREQUAL "x5h_vdk")
    set(LDFILE ${BSP_DIR}/common/linker/lscript_vram.ld)
endif()

### Global compile definitions
set(global_definition
)

### Set common build flags
set(CMAKE_ASM_FLAGS "-mfpu=neon-fp-armv8 --specs=nosys.specs -mcpu=cortex-r52 -mtune=cortex-r52 -mfloat-abi=hard")
add_compile_options(
    -ffreestanding -std=gnu99  -mfpu=neon-fp-armv8 -mcpu=cortex-r52 -mtune=cortex-r52 -mfloat-abi=softfp
    ${global_definition}
)
add_link_options(
    -lnosys --specs=nosys.specs
    -L${BSP_DIR}/common/linker/ -T ${LDFILE} -Wl,--no-warn-rwx-segments
)

#######################################################################################
### Set the common BSP and upstream projects source files
set(kernel_source
    ${KERNEL_DIR}/croutine.c
    ${KERNEL_DIR}/event_groups.c
    ${KERNEL_DIR}/list.c
    ${KERNEL_DIR}/queue.c
    ${KERNEL_DIR}/stream_buffer.c
    ${KERNEL_DIR}/tasks.c
    ${KERNEL_DIR}/timers.c
)

set(bsp_source
    ${BSP_DIR}/common/boot.S
    ${BSP_DIR}/common/asm_vectors.S
    ${BSP_DIR}/common/FreeRTOS_tick_config.c
    ${BSP_DIR}/common/system_rcar_gen5.c
    ${BSP_DIR}/common/interrupts.c
    ${BSP_DIR}/common/heap_useNewlib.c
    ${BSP_DIR}/CMSIS_5/irq_ctrl_gic.c
    ${BSP_DIR}/drivers/gic/gicv3_cpuif.S
    ${BSP_DIR}/drivers/gic/gicv3_basic.c
    ${BSP_DIR}/drivers/serial/serial.c
    ${BSP_DIR}/drivers/serial/scif.c
    ${BSP_DIR}/drivers/mpu/mpu.c
    ${BSP_DIR}/common/ARM_CR52/port.c
)

set(bsp_include_dir
    ${BSP_DIR}
    ${BSP_DIR}/common/
    ${BSP_DIR}/CMSIS_5
    ${BSP_DIR}/CMSIS_5/CMSIS/Core_R/Include
    ${BSP_DIR}/drivers
    ${BSP_DIR}/drivers/mpu
    ${BSP_DIR}/drivers/serial
    ${BSP_DIR}/drivers/timer
    ${BSP_DIR}/drivers/gic
)

add_library(dummy OBJECT
    ${BSP_DIR}/common/dummy.c
)
link_libraries(dummy)

add_library(freertos_bsp STATIC
    ${kernel_source}
    ${bsp_source}
)

target_include_directories(freertos_bsp
    PUBLIC
        $<BUILD_INTERFACE:${KERNEL_DIR}/include>
        $<BUILD_INTERFACE:${BSP_DIR}>
        $<BUILD_INTERFACE:${BSP_DIR}/common/>
        $<BUILD_INTERFACE:${BSP_DIR}/common/ARM_CR52>
        $<BUILD_INTERFACE:${BSP_DIR}/CMSIS_5>
        $<BUILD_INTERFACE:${BSP_DIR}/CMSIS_5/CMSIS/Core_R/Include>
        $<BUILD_INTERFACE:${BSP_DIR}/drivers>
        $<BUILD_INTERFACE:${BSP_DIR}/drivers/mpu>
        $<BUILD_INTERFACE:${BSP_DIR}/drivers/serial>
        $<BUILD_INTERFACE:${BSP_DIR}/drivers/timer>
        $<BUILD_INTERFACE:${BSP_DIR}/drivers/gic>

        # Kernel
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:include/kernel/include>
        # BSP
        $<INSTALL_INTERFACE:include>
        # CR52
        $<INSTALL_INTERFACE:include/ARM_CR52>
)

### Installation config
# Kernel
install(
    DIRECTORY ${KERNEL_DIR}/include
    DESTINATION include/kernel
    COMPONENT Kernel
    FILES_MATCHING PATTERN "*.h"
)
# BSP
install(
    TARGETS freertos_bsp dummy
    EXPORT freertos_bspTargets
    ARCHIVE DESTINATION lib
    OBJECTS DESTINATION .
)
install(
    EXPORT freertos_bspTargets
    NAMESPACE freertos_bsp::
    DESTINATION lib
)
install(
    FILES ${CMAKE_SOURCE_DIR}/freertos_bspConfig.cmake
    DESTINATION .
)
install(
    FILES 
        ${BSP_DIR}/common/interrupts.h
        ${BSP_DIR}/common/FreeRTOSConfig.h
    DESTINATION include
)
install(
    DIRECTORY 
        ${BSP_DIR}/common/ARM_CR52
        ${BSP_DIR}/driver
        ${BSP_DIR}/CMSIS_5
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# Logging
install(
    FILES
        ${LOGGING_DIR}/logging.h
        ${LOGGING_DIR}/logging_levels.h
        ${LOGGING_DIR}/logging_stack.h
    DESTINATION include
)

# Linker scripts
install(
    DIRECTORY ${BSP_DIR}/common/linker
    DESTINATION .
    FILES_MATCHING PATTERN "*.ld"
)

#######################################################################################
# Add subdirectory for applications below
if(ENABLE_OPENAMP)
    add_subdirectory(libraries/libmetal)
    add_subdirectory(libraries/openamp)
endif()

add_subdirectory(Blinky)
add_subdirectory(Full_Demo)
add_subdirectory(sample_apps/hello_world)
add_subdirectory(sample_apps/uart_app)
add_subdirectory(sample_apps/logging_app)
add_subdirectory(sample_apps/mpu_app)

# TODO: Delete application makefile
# TODO: Remove porting code in Kernel
