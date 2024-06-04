/*
 * Copyright (c) 2019-2020 Renesas Electronics Europe Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */
 
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"

#include "serial.h"
#include "scif.h"
#include "register.h"

extern uint32_t console_init(void);
extern void console_putc(char c);

static int xSerialPortInitialized;

xComPortHandle xSerialPortInitMinimal(unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength)
{
	(void) ulWantedBaud;
	(void) uxQueueLength;

	if (xSerialPortInitialized)
		return (xComPortHandle) 0;

	console_init();
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

signed portBASE_TYPE xSerialGetChar(xComPortHandle pxPort, signed char *pcRxedChar, portTickType xBlockTime)
{
	/* Not supported */
	return pdFALSE;
}

signed portBASE_TYPE xSerialPutChar(xComPortHandle pxPort, signed char cOutChar, portTickType xBlockTime)
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
		xSerialPortInitMinimal(115200, 200);

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
