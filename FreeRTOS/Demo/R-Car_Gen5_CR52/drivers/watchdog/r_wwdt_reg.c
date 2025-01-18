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

extern uintptr_t R_WWDT_PRV_GetRegbase(wwdt_unit_t unit)
{
    uintptr_t ret = 0;

    switch (unit) {
    case R_WWDT0:
        ret = R_WWDT0_BASE;
        break;
    case R_WWDT1:
        ret = R_WWDT1_BASE;
        break;
    case R_WWDT2:
        ret = R_WWDT2_BASE;
        break;
    case R_WWDT3:
        ret = R_WWDT3_BASE;
        break;
    case R_WWDT4:
        ret = R_WWDT4_BASE;
        break;
    case R_WWDT5:
        ret = R_WWDT5_BASE;
        break;
    case R_WWDT6:
        ret = R_WWDT6_BASE;
        break;
    case R_WWDT7:
        ret = R_WWDT7_BASE;
        break;
    case R_WWDT8:
        ret = R_WWDT8_BASE;
        break;
    case R_WWDT9:
        ret = R_WWDT9_BASE;
        break;
    case R_WWDT10:
        ret = R_WWDT10_BASE;
        break;
    case R_WWDT11:
        ret = R_WWDT11_BASE;
        break;
    case R_WWDT12:
        ret = R_WWDT12_BASE;
        break;
    case R_WWDT13:
        ret = R_WWDT13_BASE;
        break;
    case R_WWDT14:
        ret = R_WWDT14_BASE;
        break;
    case R_WWDT15:
        ret = R_WWDT15_BASE;
        break;
    case R_WWDT16:
        ret = R_WWDT16_BASE;
        break;
    case R_WWDT17:
        ret = R_WWDT17_BASE;
        break;
    case R_WWDT18:
        ret = R_WWDT18_BASE;
        break;
    case R_WWDT19:
        ret = R_WWDT19_BASE;
        break;
    case R_WWDT20:
	ret = R_WWDT20_BASE;
	break;
    default:
        printf("[R_WWDT_PRV_GetRegbase] : Wrong WWDT Unit %d\r\n", unit);
        break;
    }

    return ret;
}
