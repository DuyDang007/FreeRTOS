/*
 * Copyright (c) 2019-2020 Renesas Electronics Europe Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */
 
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"

#include "../Common/include/serial.h"
#include "CMSIS_5/cmsis_rcar_gen5.h"
#include "scif.h"


/* PFC (Pin Function Controller) */
#define RCAR_PFC_GPSR1  0xC0800840u       /* R/W 32 GPIO/Peripheral_Function Select register PortGroup 1 */
#define RCAR_PFC_GPSR1_SCIF_ENABLE  0x0001F000u  /* bit16:HRX0, bit15:HSCK0, bit14:HRTS0#, bit13:HCTS0#, bit12:HTX0 */
#define RCAR_PFC_IP1SR1 0xC0800864u       /* R/W 32 Peripheral Function Select register 1 PortGroup 1 */
#define RCAR_PFC_IP1SR1_SCIF_ENABLE  0x11110000u /* [31:16]: 0x1111 (select SCK0, RTS0#, CTS0#, TX0) */
#define RCAR_PFC_IP1SR1_SCIF_CLEAR_MASK  0x0000FFFFu /* [31:16]:clear */
#define RCAR_PFC_IP2SR1 0xC0800868u       /* R/W 32 Peripheral Function Select register 2 PortGroup 1 */
#define RCAR_PFC_IP2SR1_SCIF_ENABLE  0x00000001u /* [3:0]: 0x1 (select RX0) */
#define RCAR_PFC_IP2SR1_SCIF_CLEAR_MASK  0xFFFFFFF0u /* [3:0]: clear */
#define RCAR_PFC_PMMR(addr)  ((addr) & 0xFFFFF800u) /* R/W 32 LSI Multiplexed Pin Setting Mask Register */

static void uart_rcar_pfc_init(void)
{
	uint32_t drv_data;

	/* GPSR1:Set 0xf to [16:12] */
	drv_data = sys_read32(RCAR_PFC_GPSR1);
	drv_data = drv_data | RCAR_PFC_GPSR1_SCIF_ENABLE;
	sys_write32(~drv_data, RCAR_PFC_PMMR(RCAR_PFC_GPSR1));
	sys_write32(drv_data, RCAR_PFC_GPSR1);

	/* IP1SR1:Set 0x1111 to [31:16] */
	drv_data = sys_read32(RCAR_PFC_IP1SR1);
	drv_data = (drv_data & RCAR_PFC_IP1SR1_SCIF_CLEAR_MASK) | RCAR_PFC_IP1SR1_SCIF_ENABLE;
	sys_write32(~drv_data, RCAR_PFC_PMMR(RCAR_PFC_IP1SR1));
	sys_write32(drv_data, RCAR_PFC_IP1SR1);

	/* IP2SR1:Set 0x1 to [3:0] */
	drv_data = sys_read32(RCAR_PFC_IP2SR1);
	drv_data = (drv_data & RCAR_PFC_IP2SR1_SCIF_CLEAR_MASK) | RCAR_PFC_IP2SR1_SCIF_ENABLE;
	sys_write32(~drv_data, RCAR_PFC_PMMR(RCAR_PFC_IP2SR1));
	sys_write32(drv_data, RCAR_PFC_IP2SR1);
}


static int xSerialPortInitialized;

xComPortHandle xSerialPortInitMinimal(unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength)
{
	(void) ulWantedBaud;
	(void) uxQueueLength;

	if (xSerialPortInitialized)
		return (xComPortHandle) 0;

	uart_rcar_pfc_init();
	console_init(0);
	xSerialPortInitialized = 1;

	return (xComPortHandle) 0;
}

void vSerialPutString(xComPortHandle pxPort, const signed char *pcString, unsigned short usStringLength)
{
	(void) usStringLength;

	if (!xSerialPortInitialized)
		return;

	/* Send each character in the string, one at a time. */
	while (*pcString) {
		xSerialPutChar(pxPort, *pcString, portMAX_DELAY);
		pcString++;
	}
}

signed portBASE_TYPE xSerialGetChar(xComPortHandle pxPort, signed char *pcRxedChar, TickType_t xBlockTime)
{
	/* Not supported */
	return pdFALSE;
}

signed portBASE_TYPE xSerialPutChar(xComPortHandle pxPort, signed char cOutChar, TickType_t xBlockTime)
{
	(void) pxPort;
	(void) xBlockTime;

	console_putc(cOutChar);
	return pdTRUE;
}

void vSerialClose(xComPortHandle xPort)
{
	/* Not supported */
	(void) xPort;
}

void outbyte(char c)
{
	if (!xSerialPortInitialized)
		xSerialPortInitMinimal(UART_BAUDRATE, 200);

	/* Standard practice to convert \n to \r\n */
	if (c == '\n')
		console_putc('\r');

	console_putc(c);
}

/* Override std C lib output for printf, fprintf */
int _write(int file, char *ptr, int len)
{
	int i;

	for (i = 0; i < len; i++) {
		outbyte(*ptr++);
	}

	return len;
}

int printf_raw(const char *format, ...)
{
	va_list args;
	int ret;

	va_start(args, format);
	ret = vfprintf(stderr, format, args);
	va_end(args);

	return ret;
}
