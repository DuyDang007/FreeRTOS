/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef __SCIF_H__
#define __SCIF_H__

#include <stdint.h>

/// Initialize scif .
/// \param[in]     port          port number
/// \return 1 on success, 0 on error.

#ifndef UART_BAUDRATE
#if (BOARD == x5h_vdk)
#define UART_BAUDRATE 115200
#else
#define UART_BAUDRATE 3000000
#endif
#endif

uint32_t console_init(uint32_t port);

void console_putc(char c);
int console_getc(unsigned char *p_char);
#endif	/* __SCIF_H__ */
