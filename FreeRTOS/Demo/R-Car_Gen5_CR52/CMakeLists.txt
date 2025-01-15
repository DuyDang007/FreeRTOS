cmake_minimum_required(VERSION 3.21.0)

project(rcar-freertos-bsp VERSION 1.0.0)

enable_language(C ASM)

file(REAL_PATH ${CMAKE_SOURCE_DIR}/../../.. FREERTOS_DIR)
set(BSP_DIR ${CMAKE_SOURCE_DIR})
set(POSIX_DIR ${FREERTOS_DIR}/FreeRTOS-Plus/Source/FreeRTOS-Plus-POSIX)
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
    -DconfigUSE_POSIX_ERRNO=1
    -DconfigSUPPORT_POSIX=1
)

### Set common build flags
set(CMAKE_ASM_FLAGS "--specs=nosys.specs -mfloat-abi=hard")
set(CMAKE_C_FLAGS "-std=gnu99")
add_compile_options(
    -ffreestanding -mfpu=neon-fp-armv8 -mcpu=cortex-r52 -mtune=cortex-r52 -mfloat-abi=softfp
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
    ${BSP_DIR}/drivers/gpio/r_gpio_api.c
    ${BSP_DIR}/drivers/gpio/r_ioport.c
    ${BSP_DIR}/drivers/mfis/mfis.c
    ${BSP_DIR}/drivers/i2c/src/r_i2c.c
    ${BSP_DIR}/drivers/i2c/src/r_i2c_regs.c
    ${BSP_DIR}/drivers/i2c/r_sci_i2c.c
    ${BSP_DIR}/drivers/watchdog/r_wwdt.c
    ${BSP_DIR}/drivers/watchdog/r_wwdt_reg.c
    ${BSP_DIR}/common/ARM_CR52/port.c
    ${BSP_DIR}/drivers/ucie/ucie_controller.c
    ${BSP_DIR}/drivers/ucie/ucie_host.c
    ${BSP_DIR}/drivers/ucie/ucie_ep.c
)

set(posix_source
    ${POSIX_DIR}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_clock.c
    ${POSIX_DIR}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_mqueue.c 
    ${POSIX_DIR}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_pthread.c 
    ${POSIX_DIR}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_pthread_cond.c 
    ${POSIX_DIR}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_pthread_mutex.c 
    ${POSIX_DIR}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_sched.c 
    ${POSIX_DIR}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_semaphore.c 
    ${POSIX_DIR}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_timer.c 
    ${POSIX_DIR}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_unistd.c 
    ${POSIX_DIR}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_utils.c
    ${POSIX_DIR}/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_pthread_barrier.c
)

add_library(dummy OBJECT
    ${BSP_DIR}/common/dummy.c
)
link_libraries(dummy)

add_library(freertos_bsp STATIC
    ${kernel_source}
    ${bsp_source}
    ${posix_source}
)

target_include_directories(freertos_bsp
    PUBLIC
        $<BUILD_INTERFACE:${KERNEL_DIR}/include>
        $<BUILD_INTERFACE:${BSP_DIR}/include>
        $<BUILD_INTERFACE:${BSP_DIR}/drivers/mpu>
        $<BUILD_INTERFACE:${BSP_DIR}/drivers/serial>
        $<BUILD_INTERFACE:${BSP_DIR}/drivers/timer>
        $<BUILD_INTERFACE:${BSP_DIR}/drivers/gic>
        $<BUILD_INTERFACE:${BSP_DIR}/drivers/mfis>
        $<BUILD_INTERFACE:${BSP_DIR}/drivers/gpio>
        $<BUILD_INTERFACE:${BSP_DIR}/drivers/i2c/lib>
        $<BUILD_INTERFACE:${BSP_DIR}/drivers/i2c>
	$<BUILD_INTERFACE:${BSP_DIR}/drivers/watchdog>
        $<BUILD_INTERFACE:${BSP_DIR}/drivers/ucie>
        # POSIX
        $<BUILD_INTERFACE:${BSP_DIR}/common/POSIX_portable>
        $<BUILD_INTERFACE:${POSIX_DIR}/include>
        $<BUILD_INTERFACE:${POSIX_DIR}/include/private>
        $<BUILD_INTERFACE:${POSIX_DIR}/FreeRTOS-Plus-POSIX/include>
        $<BUILD_INTERFACE:${POSIX_DIR}/FreeRTOS-Plus-POSIX/include/portable>

        $<INSTALL_INTERFACE:include/kernel/include>
        $<INSTALL_INTERFACE:include>
        # POSIX
        $<INSTALL_INTERFACE:include/POSIX/include>
        $<INSTALL_INTERFACE:include/POSIX/include/private>
        $<INSTALL_INTERFACE:include/POSIX/include/portable>

    PRIVATE
        $<BUILD_INTERFACE:${BSP_DIR}>
        $<BUILD_INTERFACE:${BSP_DIR}/CMSIS_5>
        $<BUILD_INTERFACE:${BSP_DIR}/CMSIS_5/CMSIS/Core_R/Include>
)

### Installation config
# Kernel
install(
    DIRECTORY ${KERNEL_DIR}/include
    DESTINATION include/kernel
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
    DIRECTORY ${BSP_DIR}/include
    DESTINATION .
)
# POSIX
install(
    DIRECTORY
        ${POSIX_DIR}/include
        ${POSIX_DIR}/FreeRTOS-Plus-POSIX/include
    DESTINATION include/POSIX
    FILES_MATCHING PATTERN "*.h"
)
install(
    FILES ${BSP_DIR}/common/POSIX_portable/FreeRTOS_POSIX_portable.h
    DESTINATION include
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
add_subdirectory(sample_apps/posix_app)
add_subdirectory(sample_apps/logging_app)
add_subdirectory(sample_apps/mpu_app)
add_subdirectory(sample_apps/gpio_app)
add_subdirectory(sample_apps/cpp_app)
add_subdirectory(sample_apps/i2c_app)
add_subdirectory(sample_apps/wwdt_app)
add_subdirectory(sample_apps/cxl_app)
if(ENABLE_OPENAMP)
    add_subdirectory(sample_apps/rpmsg_sample)
endif()
# TODO: Delete application makefile
# TODO: Remove porting code in Kernel
# TODO: Remove rcar code in FreeRTOS_POSIX repo
