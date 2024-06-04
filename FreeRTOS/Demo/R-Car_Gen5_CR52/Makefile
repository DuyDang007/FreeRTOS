OUTPUT_DIR = ./bin
PROJ_NAME = freertos

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
AR = arm-none-eabi-ar
ARFLAGS = rcs
OBJCOPY = arm-none-eabi-objcopy

SOURCE_C_LIST = ./Blinky/main_blinky.c \
		./common/main.c \
	        ./common/FreeRTOS_tick_config.c \
	        ./common/system_rcar_gen5.c \
		./common/interrupts.c \
		./common/heap_useNewlib.c \
		./CMSIS/irq_ctrl_gic.c \
		../../Source/portable/GCC/ARM_CR52/port.c \
	        ../../Source/queue.c \
	        ../../Source/tasks.c \
	        ../../Source/list.c \
	        ../../Source/timers.c \

SOURCE_ASM_LIST = ./common/boot.S \
		  ./common/asm_vectors.S \
		  ../../Source/portable/GCC/ARM_CR52/portASM.S \

OBJ_LIST = $(SOURCE_C_LIST:.c=.o) $(SOURCE_ASM_LIST:.S=.o)

MAIN_SOURCE = ./common/main.c
	      

MAIN_OBJ = $(SOURCE_C_LIST:.c=.o) 

LDFILE = ./common/lscript.ld

CFLAGS = -ffreestanding -std=gnu99  -mfpu=vfpv3-d16 -mcpu=cortex-r52 -mtune=cortex-r52 \
                -mfloat-abi=hard \
                -DmainCREATE_SIMPLE_BLINKY_DEMO_ONLY \
                -I./CMSIS \
                -I./common \
                -I../../Source/include \
                -I../../Source/portable/GCC/ARM_CR52 \
                -I../../../FreeRTOS-Plus/Demo/Common/FreeRTOS_Plus_CLI_Demos \
                -I../../../FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI \
                -I../../../FreeRTOS-Plus/Source/FreeRTOS-Plus-Trace/Include \
                -I../../../FreeRTOS-Plus/Source/FreeRTOS-Plus-Trace/config \

AFLAGS = -mfpu=vfpv3-d16 -mcpu=cortex-r52 -mtune=cortex-r52 -mfloat-abi=hard  \
                -I./common

LDFLAGS = -no-warn-rwx-segments   -T ${LDFILE} \
                -Map ${OUTPUT_DIR}/${PROJ_NAME}.map \

LIBGCC=${shell ${CC} -mcpu=cortex-r52 -mtune=cortex-r52 -mfloat-abi=hard -print-libgcc-file-name}
LIBC=${shell ${CC} -mcpu=cortex-r52 -mtune=cortex-r52 -mfloat-abi=hard -print-file-name=libc.a}

all: create lib target

create:
	@echo CREATE ${OUTPUT_DIR} folder
	@mkdir -p ${OUTPUT_DIR}

lib: ${OBJ_LIST}
	@echo LD ${PROJ_NAME}.a
	@${AR} ${ARFLAGS} ${OUTPUT_DIR}/${PROJ_NAME}.a ${OBJ_LIST}

target: ${MAIN_OBJ}
	@echo CC $<
	@${LD} ${LDFLAGS} ${MAIN_OBJ} ${OUTPUT_DIR}/${PROJ_NAME}.a ${LIBC} ${LIBGCC}  -o ${OUTPUT_DIR}/${PROJ_NAME}.elf 
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

