/*************************************************************************************************************
* i2c_regs_h
* Copyright (c) [2021-2022] Renesas Electronics Corporation
* Copyright (c) [2019-2020] Renesas Electronics (Europe) GmbH.
* This software is released under the MIT License
* http://opensource.org/licenses/mit-license.php
*************************************************************************************************************/

#ifndef R_I2C_REGS_H_
#define R_I2C_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "r_i2c_api.h"

#define R_I2C_IF0_BASE    0xc11d0000
#define R_I2C_IF1_BASE    0xc06c0000
#define R_I2C_IF2_BASE    0xc06c8000
#define R_I2C_IF3_BASE    0xc06d0000
#define R_I2C_IF4_BASE    0xc06d8000
#define R_I2C_IF5_BASE    0xc06e0000
#define R_I2C_IF6_BASE    0xc06e8000
#define R_I2C_IF7_BASE    0xc06f0000
#define R_I2C_IF8_BASE    0xc06f8000

#define R_I2C_ICSCR       0x00
#define R_I2C_ICMCR       0x04
#define R_I2C_ICSSR       0x08
#define R_I2C_ICMSR       0x0c
#define R_I2C_ICSIER      0x10
#define R_I2C_ICMIER      0x14
#define R_I2C_ICCCR       0x18
#define R_I2C_ICSAR       0x1c
#define R_I2C_ICMAR       0x20
#define R_I2C_ICTXD       0x24
#define R_I2C_ICRXD       0x24
#define R_I2C_ICCCR2      0x28
#define R_I2C_ICMPR       0x2c
#define R_I2C_ICHPR       0x30
#define R_I2C_ICLPR       0x34
#define R_I2C_ICFBSCR     0x38

#define R_I2C_MNR_BIT   (1 << 6)
#define R_I2C_MAL_BIT   (1 << 5)
#define R_I2C_MST_BIT   (1 << 4)
#define R_I2C_MDE_BIT   (1 << 3)
#define R_I2C_MDT_BIT   (1 << 2)
#define R_I2C_MDR_BIT   (1 << 1)
#define R_I2C_MAT_BIT   (1 << 0)

#define R_I2C_GCAR      (1 << 6)
#define R_I2C_STM       (1 << 5)
#define R_I2C_SSR       (1 << 4)
#define R_I2C_SDE       (1 << 3)
#define R_I2C_SDT       (1 << 2)
#define R_I2C_SDR       (1 << 1)
#define R_I2C_SAR       (1 << 0)

#define R_I2C_SDBS      (1 << 3)
#define R_I2C_SIE       (1 << 2)
#define R_I2C_GCAE      (1 << 1)
#define R_I2C_FNA       (1 << 0)


#define R_I2C_FSDA_BIT (1 << 5)

void        R_I2C_PRV_RegWrite32(uintptr_t Addr, uint32_t Data);
uint32_t    R_I2C_PRV_RegRead32(uintptr_t Addr);
uintptr_t   R_I2C_PRV_GetRegbase(r_i2c_Unit_t I2cUnit);

#ifdef __cplusplus
}
#endif

#endif /* R_I2C_REGS_H_ */
