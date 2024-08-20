OUTPUT_DIR = ./bin

ifdef TEST_FULL
PROJ_NAME = full_demo
else
PROJ_NAME = blinky_demo
endif

board := x5h_vdk

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
AR = arm-none-eabi-ar
ARFLAGS = rcs
OBJCOPY = arm-none-eabi-objcopy

SOURCE_C_LIST = ./Blinky/main_blinky.c \
		./common/FreeRTOS_tick_config.c \
		./common/system_rcar_gen5.c \
		./common/interrupts.c \
		./common/heap_useNewlib.c \
		./common/dummy.c \
		./CMSIS_5/irq_ctrl_gic.c \
		./drivers/gic/gicv3_basic.c \
		./drivers/serial/serial.c \
		../../Source/portable/GCC/ARM_CR52/port.c \
	        ../../Source/queue.c \
	        ../../Source/tasks.c \
	        ../../Source/list.c \
	        ../../Source/timers.c \
		./common/main.c \

# Full Demo Source
ifdef TEST_FULL
SOURCE_C_LIST += ./Full_Demo/main_full.c \
                 ../Common/Minimal/EventGroupsDemo.c \
                 ../Common/Minimal/blocktim.c \
                 ../Common/Minimal/recmutex.c \
                 ../Common/Minimal/semtest.c \
                 ../Common/Minimal/dynamic.c \
                 ../Common/Minimal/countsem.c \
                 ../Common/Minimal/GenQTest.c \
                 ../Common/Minimal/flop.c \
                 ../Common/Minimal/IntSemTest.c \
                 ../Common/Minimal/TaskNotify.c \
                 ../Common/Minimal/StaticAllocation.c \
                 ../Common/Minimal/AbortDelay.c \
                 ../Common/Minimal/QueueOverwrite.c \
                 ../Common/Minimal/TimerDemo.c \
                 ../Common/Minimal/QPeek.c \
                 ../Common/Minimal/QueueSet.c \
                 ../Common/Minimal/StreamBufferDemo.c \
                 ../Common/Minimal/death.c \
                 ../Common/Minimal/MessageBufferDemo.c \
                 ../../Source/event_groups.c \
                 ../../Source/stream_buffer.c
endif

SOURCE_ASM_LIST = ./common/boot.S \
                ./common/asm_vectors.S \
                ./drivers/gic/gicv3_cpuif.S \
                ./drivers/serial/scif.S \

OBJ_LIST = $(SOURCE_C_LIST:.c=.o) $(SOURCE_ASM_LIST:.S=.o)

LDFILE = ./common/linker/lscript_sram.ld
ifeq ($(board),x5h_vdk)
	LDFILE = ./common/linker/lscript_vram.ld
endif

CFLAGS = -ffreestanding -std=gnu99  -mfpu=neon-fp-armv8 -mcpu=cortex-r52 -mtune=cortex-r52  -lnosys --specs=nosys.specs \
                -Wl,-Map=${OUTPUT_DIR}/${PROJ_NAME}.map -T ${LDFILE} -Wl,--no-warn-rwx-segments \
                -mfloat-abi=hard \
		-L./common/linker		     \
                -I./				     \
                -I./CMSIS_5/CMSIS/Core_R/Include/ \
                -I./CMSIS_5 \
                -I./common \
                -I../../Source/include \
                -I../../Source/portable/GCC/ARM_CR52 \
                -I../../../FreeRTOS-Plus/Demo/Common/FreeRTOS_Plus_CLI_Demos \
                -I../../../FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI \
                -I../../../FreeRTOS-Plus/Source/FreeRTOS-Plus-Trace/Include \
                -I../../../FreeRTOS-Plus/Source/FreeRTOS-Plus-Trace/config \
                -I../Common/include

ifdef TEST_FULL
CFLAGS += -DmainCREATE_SIMPLE_BLINKY_DEMO_ONLY=0 \
	  -DINCLUDE_eTaskGetState=1 \
	  -DINCLUDE_xTimerPendFunctionCall=1 \
	  -DconfigUSE_RECURSIVE_MUTEXES=1 \
          -DconfigUSE_MUTEXES=1 \
          -DINCLUDE_xTaskAbortDelay=1 \
	  -DconfigUSE_QUEUE_SETS=1 \
	  -DINCLUDE_xTaskGetHandle=1 \
	  -DINCLUDE_xSemaphoreGetMutexHolder=1
else
CFLAGS += -DmainCREATE_SIMPLE_BLINKY_DEMO_ONLY=1
endif

AFLAGS = -mfpu=neon-fp-armv8 --specs=nosys.specs -mcpu=cortex-r52 -mtune=cortex-r52 -mfloat-abi=hard  \
                -I./        \
                -I./common

all: create target

create:
	@echo CREATE ${OUTPUT_DIR} folder
	@mkdir -p ${OUTPUT_DIR}

#lib: ${OBJ_LIST}
#	@echo LD ${PROJ_NAME}.a
#	@${AR} ${ARFLAGS} ${OUTPUT_DIR}/${PROJ_NAME}.a ${OBJ_LIST}

target: ${OBJ_LIST}
	@echo CC $<
	@${CC} ${CFLAGS} ${OBJ_LIST} -lm -o  ${OUTPUT_DIR}/${PROJ_NAME}.elf 
	$(OBJCOPY) -O binary "$(OUTPUT_DIR)/$(PROJ_NAME).elf" "$(OUTPUT_DIR)/$(PROJ_NAME).bin"
	ln -sf ${PROJ_NAME}.elf ${OUTPUT_DIR}/freertos.elf

%.o: %.c
	@echo CC $<
	@${CC} -c -o $@ $< ${CFLAGS}

%.o: %.S 
	@echo CC $<
	@${CC} -c -o $@ $< ${AFLAGS}

.PHONY: clean
clean:
	@rm -rf $(OBJ_LIST) $(OUTPUT_DIR)
