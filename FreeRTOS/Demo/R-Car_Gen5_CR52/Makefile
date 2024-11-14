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

LOGGING_INC = ../../../FreeRTOS-Plus/Source/Utilities/logging

POSIX_DIR = ../../../FreeRTOS-Plus/Source/FreeRTOS-Plus-POSIX
POSIX_SRC = ${POSIX_DIR}/FreeRTOS-Plus-POSIX/source
POSIX_PORT_INC = ${POSIX_DIR}/FreeRTOS-Plus-POSIX/include

SOURCE_C_LIST = ./common/FreeRTOS_tick_config.c \
		./common/system_rcar_gen5.c \
		./common/interrupts.c \
		./common/heap_useNewlib.c \
		./common/dummy.c \
		./CMSIS_5/irq_ctrl_gic.c \
		./drivers/gic/gicv3_basic.c \
		./drivers/serial/serial.c \
		./drivers/serial/scif.c \
		./drivers/pcie/pcie.c \
		./drivers/pcie/printer.c \
		./drivers/pcie/mem_io.c  \
        ./drivers/mpu/mpu.c \
		../../Source/portable/GCC/ARM_CR52/port.c \
	    ../../Source/queue.c \
	    ../../Source/tasks.c \
	    ../../Source/list.c \
	    ../../Source/timers.c \
        ../../Source/event_groups.c \
		./common/main.c

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
				 ../../Source/stream_buffer.c 
				 
ifdef SUPPORT_POSIX
SOURCE_C_LIST += ./Full_Demo/posix_demo/posix_demo.c \
				 ${POSIX_SRC}/FreeRTOS_POSIX_clock.c \
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
endif # SUPPORT_POSIX
else 
SOURCE_C_LIST += ./Blinky/main_blinky.c 
endif

SOURCE_ASM_LIST = ./common/boot.S \
                ./common/asm_vectors.S \
                ./drivers/gic/gicv3_cpuif.S \

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
				-I./drivers/mpu \
                -I../../Source/include \
                -I../../Source/portable/GCC/ARM_CR52 \
                -I../../../FreeRTOS-Plus/Demo/Common/FreeRTOS_Plus_CLI_Demos \
                -I../../../FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI \
                -I../../../FreeRTOS-Plus/Source/FreeRTOS-Plus-Trace/Include \
                -I../../../FreeRTOS-Plus/Source/FreeRTOS-Plus-Trace/config \
                -I../Common/include \
                -I./${POSIX_DIR}/include \
                -I./${POSIX_DIR}/include/private \
                -I./${POSIX_PORT_INC} \
                -I./${POSIX_PORT_INC}/portable \
                -I./${POSIX_PORT_INC}/portable/renesas/rcar_gen5 \
                -I./Full_Demo/posix_demo \
		-I${LOGGING_INC} \


ifdef TEST_FULL
CFLAGS += -DmainCREATE_SIMPLE_BLINKY_DEMO_ONLY=0 

ifdef SUPPORT_POSIX
CFLAGS += -DconfigSUPPORT_POSIX 
endif # SUPPORT_POSIX

ifdef UART_TEST
CFLAGS += -DUART_TEST
endif # UART_TEST

ifdef LOGGING_ENABLE
CFLAGS += -DLOGGING_ENABLE
endif # SUPPORT_LOGGING
else # TEST_FULL
CFLAGS += -DmainCREATE_SIMPLE_BLINKY_DEMO_ONLY=1 
endif

ifdef BAUDRATE
CFLAGS += -DUART_BAUDRATE=${BAUDRATE}
endif

AFLAGS = -mfpu=neon-fp-armv8 --specs=nosys.specs -mcpu=cortex-r52 -mtune=cortex-r52 -mfloat-abi=hard  \
                -I./        \
                -I./common

all: create target

create:
	@echo CREATE ${OUTPUT_DIR} folder
	@mkdir -p ${OUTPUT_DIR}

check_file:
ifdef TEST_FULL
	@if [ -e "./Blinky/main_blinky.o" ]; then \
		rm -rf ./common/main.o ./Blinky/main_blinky.o; \
		if [ $$SUPPORT_POSIX ]; then \
			rm -rf ../../Source/tasks.o; \
		fi; \
	fi

ifdef UART_TEST
	@rm -rf Full_Demo/main_full.o
else # UART_TEST
	@rm -rf Full_Demo/main_full.o
endif 

ifdef SUPPORT_POSIX
	@if [ ! -e "Full_Demo/posix_demo/posix_demo.o" ]; then \
		rm -rf Full_Demo/main_full.o; \
		rm -rf ../../Source/tasks.o; \
    	rm -rf ../Common/Minimal/StaticAllocation.o; \
	fi
else 
	@if [ -e "Full_Demo/posix_demo/posix_demo.o" ]; then \
		rm -rf Full_Demo/main_full.o; \
		rm -rf Full_Demo/posix_demo/posix_demo.o; \
		rm -rf ../../Source/tasks.o; \
    	rm -rf ../Common/Minimal/StaticAllocation.o; \
	fi
endif # SUPPORT_POSIX
else 
	@if [ ! -e "./Blinky/main_blinky.o" ]; then \
		rm -rf ./common/main.o ; \
	fi
endif
ifdef BAUDRATE
	@rm -rf ./drivers/serial/serial.o ./drivers/serial/scif.o
endif


#lib: ${OBJ_LIST}
#	@echo LD ${PROJ_NAME}.a
#	@${AR} ${ARFLAGS} ${OUTPUT_DIR}/${PROJ_NAME}.a ${OBJ_LIST}

target: check_file ${OBJ_LIST}
	@echo LD ${PROJ_NAME}.elf
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
