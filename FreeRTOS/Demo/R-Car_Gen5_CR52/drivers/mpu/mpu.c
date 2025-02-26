/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "mpu.h"

void MPU_Enable(void) {
    ARM_MPU_Enable();
}

void MPU_Disable(void) {
    ARM_MPU_Disable();
}

void MPU_Init(void) {
    __set_CP(15, 0, MPU_MAIR_DEFAULT_ATTRS, 10, 2, 0);
}

void MPU_SetRegion(Region_num rnr, ARM_MPU_Region_t region_attr) {
    ARM_MPU_SetRegion(rnr, region_attr);
}

void MPU_ClrRegion(Region_num rnr) {
    ARM_MPU_ClrRegion(rnr);
}
