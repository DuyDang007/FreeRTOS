ifdef TEST_FULL
	PROJ_NAME = full_demo
else
	PROJ_NAME = blinky_demo
endif

board ?= x5h_vdk

FREERTOS_BSP_DIR=${FREERTOS_DIR}/FreeRTOS/Demo/R-Car_Gen5_CR52
FREERTOS_LIB = freertos_lib
OUTPUT_DIR = $(FREERTOS_BSP_DIR)/bin

CC = ${CROSS_COMPILE}gcc
LD = ${CROSS_COMPILE}ld
AR = ${CROSS_COMPILE}ar
ARFLAGS = rcs
OBJCOPY = ${CROSS_COMPILE}objcopy

LOGGING_INC = $(FREERTOS_BSP_DIR)/FreeRTOS-Plus/Source/Utilities/logging

POSIX_DIR = ${FREERTOS_DIR}/FreeRTOS-Plus/Source/FreeRTOS-Plus-POSIX
POSIX_SRC = ${POSIX_DIR}/FreeRTOS-Plus-POSIX/source
POSIX_PORT_INC = ${POSIX_DIR}/FreeRTOS-Plus-POSIX/include

SOURCE_C_DUMMY_LIBC = $(FREERTOS_BSP_DIR)/common/dummy.c

SOURCE_C_MAIN = $(FREERTOS_BSP_DIR)/common/main.c
ifdef TEST_FULL
	SOURCE_C_MAIN += $(FREERTOS_BSP_DIR)/Full_Demo/main_full.c
else
    SOURCE_C_MAIN += $(FREERTOS_BSP_DIR)/Blinky/main_blinky.c
endif


SOURCE_C_LIST = $(FREERTOS_BSP_DIR)/common/FreeRTOS_tick_config.c \
				$(FREERTOS_BSP_DIR)/common/system_rcar_gen5.c \
				$(FREERTOS_BSP_DIR)/common/interrupts.c \
				$(FREERTOS_BSP_DIR)/common/heap_useNewlib.c \
				$(FREERTOS_BSP_DIR)/CMSIS_5/irq_ctrl_gic.c \
				$(FREERTOS_BSP_DIR)/drivers/gic/gicv3_basic.c \
				$(FREERTOS_BSP_DIR)/drivers/serial/serial.c \
				$(FREERTOS_BSP_DIR)/drivers/serial/scif.c \
				$(FREERTOS_BSP_DIR)/drivers/mpu/mpu.c \
				${FREERTOS_DIR}/FreeRTOS/Source/portable/GCC/ARM_CR52/port.c \
				${FREERTOS_DIR}/FreeRTOS/Source/queue.c \
				${FREERTOS_DIR}/FreeRTOS//Source/tasks.c \
				${FREERTOS_DIR}/FreeRTOS//Source/list.c \
				${FREERTOS_DIR}/FreeRTOS//Source/timers.c \
				${FREERTOS_DIR}/FreeRTOS//Source/event_groups.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/EventGroupsDemo.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/blocktim.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/recmutex.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/semtest.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/dynamic.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/countsem.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/GenQTest.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/flop.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/IntSemTest.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/TaskNotify.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/StaticAllocation.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/AbortDelay.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/QueueOverwrite.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/TimerDemo.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/QPeek.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/QueueSet.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/StreamBufferDemo.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/death.c \
				${FREERTOS_DIR}/FreeRTOS/Demo/Common/Minimal/MessageBufferDemo.c \
				${FREERTOS_DIR}/FreeRTOS/Source/stream_buffer.c

SOURCE_C_LIST += ${POSIX_SRC}/FreeRTOS_POSIX_clock.c \
				${POSIX_SRC}/FreeRTOS_POSIX_mqueue.c  \
				${POSIX_SRC}/FreeRTOS_POSIX_pthread.c  \
				${POSIX_SRC}/FreeRTOS_POSIX_pthread_cond.c  \
				${POSIX_SRC}/FreeRTOS_POSIX_pthread_mutex.c  \
				${POSIX_SRC}/FreeRTOS_POSIX_sched.c  \
				${POSIX_SRC}/FreeRTOS_POSIX_semaphore.c  \
				${POSIX_SRC}/FreeRTOS_POSIX_timer.c  \
				${POSIX_SRC}/FreeRTOS_POSIX_unistd.c  \
				${POSIX_SRC}/FreeRTOS_POSIX_utils.c \
				${POSIX_SRC}/FreeRTOS_POSIX_pthread_barrier.c

SOURCE_ASM_LIST = $(FREERTOS_BSP_DIR)/common/boot.S \
                $(FREERTOS_BSP_DIR)/common/asm_vectors.S \
                $(FREERTOS_BSP_DIR)/drivers/gic/gicv3_cpuif.S \

OBJ_LIST = $(SOURCE_C_LIST:.c=.o) $(SOURCE_ASM_LIST:.S=.o)
MAIN_OBJ_LIST = $(SOURCE_C_MAIN:.c=.o)
DUMMY_OBJ = $(SOURCE_C_DUMMY_LIBC:.c=.o)

LDFILE = ${FREERTOS_BSP_DIR}/common/linker/lscript_sram.ld
ifeq ($(board),x5h_vdk)
	LDFILE = $(FREERTOS_BSP_DIR)/common/linker/lscript_vram.ld
endif
LDFILE += -L${FREERTOS_BSP_DIR}/common/linker/

CFLAGS = -ffreestanding -std=gnu99  -mfpu=neon-fp-armv8 -mcpu=cortex-r52 -mtune=cortex-r52 -lnosys --specs=nosys.specs \
		-Wl,-Map=${OUTPUT_DIR}/${PROJ_NAME}.map  -T ${LDFILE} -Wl,--no-warn-rwx-segments \
        -mfloat-abi=hard \
		-DconfigSUPPORT_POSIX \
        -I$(FREERTOS_BSP_DIR)				     \
        -I$(FREERTOS_BSP_DIR)/CMSIS_5/CMSIS/Core_R/Include/ \
        -I$(FREERTOS_BSP_DIR)/CMSIS_5 \
        -I$(FREERTOS_BSP_DIR)/common \
		-I$(FREERTOS_BSP_DIR)/drivers/mpu \
        -I${FREERTOS_DIR}/FreeRTOS/Source/include \
        -I${FREERTOS_DIR}/FreeRTOS/Source/portable/GCC/ARM_CR52 \
        -I${FREERTOS_DIR}/FreeRTOS-Plus/Demo/Common/FreeRTOS_Plus_CLI_Demos \
        -I${FREERTOS_DIR}/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI \
        -I${FREERTOS_DIR}/FreeRTOS-Plus/Source/FreeRTOS-Plus-Trace/Include \
        -I${FREERTOS_DIR}/FreeRTOS-Plus/Source/FreeRTOS-Plus-Trace/config \
        -I$(FREERTOS_DIR)/FreeRTOS/Demo/Common/include \
        -I${POSIX_DIR}/include \
        -I${POSIX_DIR}/include/private \
        -I${POSIX_PORT_INC} \
        -I${POSIX_PORT_INC}/portable \
        -I${POSIX_PORT_INC}/portable/renesas/rcar_gen5 \
        -I$(FREERTOS_BSP_DIR)/Full_Demo/posix_demo \
		-I${LOGGING_INC} \


ifdef TEST_FULL
	CFLAGS += -DmainCREATE_SIMPLE_BLINKY_DEMO_ONLY=0 \
            -DconfigUSE_TICK_HOOK=1              
else
    CFLAGS += -DmainCREATE_SIMPLE_BLINKY_DEMO_ONLY=1
endif
	
ifdef UART_TEST
	CFLAGS += -DUART_TEST
endif # UART_TEST

ifdef LOGGING_ENABLE
	CFLAGS += -DLOGGING_ENABLE
endif # SUPPORT_LOGGING

ifdef BAUDRATE
	CFLAGS += -DUART_BAUDRATE=${BAUDRATE}
endif

AFLAGS = -mfpu=neon-fp-armv8 --specs=nosys.specs -mcpu=cortex-r52 -mtune=cortex-r52 -mfloat-abi=hard  \
         -I$(FREERTOS_BSP_DIR) \
         -I$(FREERTOS_BSP_DIR)

all: create target

env_check:
ifeq ($(strip $(FREERTOS_DIR)),)
	$(error Please export FREERTOS_DIR=<FreeRTOS_ROOT_PATH>)
endif

create: env_check
	@echo CREATE ${OUTPUT_DIR} folder
	@mkdir -p ${OUTPUT_DIR}
	@rm -f $(MAIN_OBJ_LIST)

${FREERTOS_LIB}: ${OBJ_LIST} create
	@echo build ${FREERTOS_LIB}
	@${AR} ${ARFLAGS} ${OUTPUT_DIR}/${FREERTOS_LIB}.a ${OBJ_LIST}
	@echo created ${OUTPUT_DIR}/${FREERTOS_LIB}.a

target: ${FREERTOS_LIB} ${MAIN_OBJ_LIST}
	@echo LD ${PROJ_NAME}.elf
	${CC} ${CFLAGS} ${MAIN_OBJ_LIST} ${OUTPUT_DIR}/${FREERTOS_LIB}.a -lm ${SOURCE_C_DUMMY_LIBC} -o ${OUTPUT_DIR}/${PROJ_NAME}.elf
	$(OBJCOPY) -O binary "$(OUTPUT_DIR)/$(PROJ_NAME).elf" "$(OUTPUT_DIR)/$(PROJ_NAME).bin"
	ln -sf ${PROJ_NAME}.elf ${OUTPUT_DIR}/freertos.elf

%.o: %.c
	@echo CC $<
	@${CC} -c -o $@ $< ${CFLAGS}

%.o: %.S 
	@echo CC $<
	@${CC} -c -o $@ $< ${AFLAGS}

.PHONY: clean target ${FREERTOS_LIB} env_check create

clean: env_check
	@rm -rf $(OBJ_LIST) $(MAIN_OBJ_LIST) $(OUTPUT_DIR) ${DUMMY_OBJ}
