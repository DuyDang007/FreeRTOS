/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <stdint.h>
#include <stdio.h>
#include "r_wwdt_reg.h"

void r_wwdt_write(uintptr_t Addr, uint32_t val)
{
	*((volatile uint32_t *)Addr) = val;

	return;
}

uint32_t r_wwdt_read(uintptr_t Addr)
{
    return *((volatile uint32_t *)Addr);
}
