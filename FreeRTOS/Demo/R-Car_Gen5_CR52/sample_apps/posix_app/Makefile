OUTPUT_DIR = ./bin

board := x5h_vdk

FREERTOS_LIB = freertos_lib

CC = ${CROSS_COMPILE}gcc
LD = ${CROSS_COMPILE}ld
AR = ${CROSS_COMPILE}ar
ARFLAGS = rcs
OBJCOPY = ${CROSS_COMPILE}objcopy

TARGET_NAME = freertos
FREERTOS_DIR ?= ../..
FREERTOS_BSP_DIR = ${FREERTOS_DIR}/FreeRTOS/Demo/R-Car_Gen5_CR52
LOGGING_INC = ${FREERTOS_DIR}/FreeRTOS-Plus/Source/Utilities/logging
POSIX_DIR = ${FREERTOS_DIR}/FreeRTOS-Plus/Source/FreeRTOS-Plus-POSIX
POSIX_SRC = ${POSIX_DIR}/FreeRTOS-Plus-POSIX/source
POSIX_PORT_INC = ${POSIX_DIR}/FreeRTOS-Plus-POSIX/include

SOURCE_C_DUMMY_LIBC = ${FREERTOS_BSP_DIR}/common/dummy.c

SOURCE_C_MAIN = main.c posix_demo.c


OBJ_LIST = $(SOURCE_C_LIST:.c=.o) $(SOURCE_ASM_LIST:.S=.o)

MAIN_OBJ_LIST = $(SOURCE_C_MAIN:.c=.o)

LDFILE = ${FREERTOS_DIR}/FreeRTOS/Demo/R-Car_Gen5_CR52/common/linker/lscript_vram.ld -L${FREERTOS_DIR}/FreeRTOS/Demo/R-Car_Gen5_CR52/common/linker/

CFLAGS = -ffreestanding -std=gnu99  -mfpu=neon-fp-armv8 -mcpu=cortex-r52 -mtune=cortex-r52 -lnosys --specs=nosys.specs \
		-Wl,-Map=${OUTPUT_DIR}/${TARGET_NAME}.map -T ${LDFILE} -Wl,--no-warn-rwx-segments \
        -mfloat-abi=hard \
		-DconfigSUPPORT_POSIX \
		-L${FREERTOS_BSP_DIR}/common/linker		     \
        -I${FREERTOS_BSP_DIR}				     \
        -I${FREERTOS_BSP_DIR}/CMSIS_5/CMSIS/Core_R/Include/ \
		-I${FREERTOS_BSP_DIR}/drivers/mpu \
        -I${FREERTOS_DIR}/FreeRTOS/Source/include \
        -I${FREERTOS_DIR}/FreeRTOS/Source/portable/GCC/ARM_CR52 \
        -I${FREERTOS_DIR}FreeRTOS-Plus/Demo/Common/FreeRTOS_Plus_CLI_Demos \
        -I${FREERTOS_DIR}FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI \
        -I${FREERTOS_DIR}/FreeRTOS-Plus/Source/FreeRTOS-Plus-Trace/Include \
        -I${FREERTOS_DIR}/FreeRTOS-Plus/Source/FreeRTOS-Plus-Trace/config \
        -I${FREERTOS_DIR}/FreeRTOS/Demo/Common/include \
        -I${FREERTOS_BSP_DIR}/common \
        -I${POSIX_DIR}/include \
        -I${POSIX_DIR}/include/private \
        -I${POSIX_PORT_INC} \
        -I${POSIX_PORT_INC}/portable \
        -I${POSIX_PORT_INC}/portable/renesas/rcar_gen5 \
		-I${LOGGING_INC} \
		-I./

AFLAGS = -mfpu=neon-fp-armv8 --specs=nosys.specs -mcpu=cortex-r52 -mtune=cortex-r52 -mfloat-abi=hard  \
         -I${FREERTOS_BSP_DIR} \
         -I${FREERTOS_BSP_DIR}/common

all: target

target: ${MAIN_OBJ_LIST}
	@mkdir -p $(OUTPUT_DIR)
	@echo make freertos.elf
	@make -f ${FREERTOS_BSP_DIR}/Makefile ${FREERTOS_LIB}
	${CC} ${CFLAGS} ${SOURCE_C_DUMMY_LIBC} ${MAIN_OBJ_LIST} ${FREERTOS_BSP_DIR}/bin/${FREERTOS_LIB}.a  -lm -o $(OUTPUT_DIR)/$(TARGET_NAME).elf
	$(OBJCOPY) -O binary $(OUTPUT_DIR)/$(TARGET_NAME).elf $(OUTPUT_DIR)/$(TARGET_NAME).bin

%.o: %.c
	@echo CC $<
	@${CC} -c -o $@ $< ${CFLAGS}

%.o: %.S 
	@echo CC $<
	@${CC} -c -o $@ $< ${AFLAGS}

.PHONY: clean
clean:
	@make -f ${FREERTOS_BSP_DIR}/Makefile clean
	@rm -rf $(OBJ_LIST) $(MAIN_OBJ_LIST) $(OUTPUT_DIR)
