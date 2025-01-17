/*************************************************************************************************************
* Copyright (c) [2025] Renesas Electronics Corporation
* This software is released under the MIT License
* http://opensource.org/licenses/mit-license.php
* SPDX-License-Identifier: MIT
*************************************************************************************************************/
#ifndef R_WWDT_REGS_H_
#define R_WWDT_REGS_H_

#include <stdint.h>
#include "watchdog/r_wwdt_api.h"

#define R_WWDT0_BASE	0x1C100000
#define R_WWDT1_BASE	0x1C110000
#define R_WWDT2_BASE	0x1C120000
#define R_WWDT3_BASE	0x1C130000
#define R_WWDT4_BASE	0x1C140000
#define R_WWDT5_BASE	0x1C150000
#define R_WWDT6_BASE	0x1C160000
#define R_WWDT7_BASE	0x1C170000
#define R_WWDT8_BASE	0x1C180000
#define R_WWDT9_BASE	0x1C190000
#define R_WWDT10_BASE	0x1C1A0000
#define R_WWDT11_BASE	0x1C1B0000
#define R_WWDT12_BASE	0x1C1C0000
#define R_WWDT13_BASE	0x1C1D0000
#define R_WWDT14_BASE	0x1C1E0000
#define R_WWDT15_BASE	0x1C1F0000
#define R_WWDT16_BASE	0x1C200000
#define R_WWDT17_BASE	0x1C210000
#define R_WWDT18_BASE	0x1C220000
#define R_WWDT19_BASE	0x1C230000
#define R_WWDT20_BASE	0xC1380000

#define WDTA0WDTE       0
#define WDTA0RUN        0xAC
#define WDTA0MD		0xC
#define WSIZE(x)	(x)
#define WDTA0ERM	(1 << 2)
#define WDTA0WIE	(1 << 3)
#define WDTA0OVF(x)	(((x) << 4) & 0x70)

uintptr_t R_WWDT_PRV_GetRegbase(wwdt_unit_t unit);
void r_wwdt_write(uintptr_t Addr, uint32_t val);
uint32_t r_wwdt_read(uintptr_t Addr);

#endif /* R_WWDT_REGS_H_ */
