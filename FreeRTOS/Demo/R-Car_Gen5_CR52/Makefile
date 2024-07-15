OUTPUT_DIR = ./bin
PROJ_NAME = freertos

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

SOURCE_ASM_LIST = ./common/boot.S \
                ./common/asm_vectors.S \
                ./drivers/gic/gicv3_cpuif.S \
                ./drivers/serial/scif.S \

OBJ_LIST = $(SOURCE_C_LIST:.c=.o) $(SOURCE_ASM_LIST:.S=.o)

#MAIN_SOURCE = ./common/main.c
	      

#MAIN_OBJ = $(SOURCE_C_LIST:.c=.o) 

LDFILE = ./common/lscript.ld

CFLAGS = -ffreestanding -std=gnu99  -mfpu=neon-fp-armv8 -mcpu=cortex-r52 -mtune=cortex-r52  -lnosys --specs=nosys.specs \
                -Wl,-Map=${OUTPUT_DIR}/${PROJ_NAME}.map -T ${LDFILE} -Wl,--no-warn-rwx-segments \
                -mfloat-abi=hard \
                -DmainCREATE_SIMPLE_BLINKY_DEMO_ONLY \
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
	@${CC} ${CFLAGS} ${OBJ_LIST}  ${LIBC} ${LIBGCC}  -o ${OUTPUT_DIR}/${PROJ_NAME}.elf 
	$(OBJCOPY) -O binary "$(OUTPUT_DIR)/$(PROJ_NAME).elf" "$(OUTPUT_DIR)/$(PROJ_NAME).bin"

%.o: %.c
	@echo CC $<
	@${CC} -c -o $@ $< ${CFLAGS}

%.o: %.S 
	@echo CC $<
	@${CC} -c -o $@ $< ${AFLAGS}

.PHONY: clean
clean:
	@rm -rf $(OBJ_LIST) $(OUTPUT_DIR)

