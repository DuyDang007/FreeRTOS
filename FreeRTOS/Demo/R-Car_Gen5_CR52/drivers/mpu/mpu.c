/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "mpu.h"

static uint8_t last_mpu_index = 0;

void MPU_Enable(void) 
{
    ARM_MPU_Enable();
}

void MPU_Disable(void) 
{
    ARM_MPU_Disable();
}

void MPU_Init(void) 
{
    __set_CP(15, 0, (uint32_t)MPU_MAIR_DEFAULT_ATTRS, 10, 2, 0);
}

uint8_t MPU_SetRegion_ByIndex(e_region_index_t rnr, ARM_MPU_Region_t region_attr)
{
    if (rnr >= MAX_MPU_REGION_SUPPORTED) {
        return 2;
    }

    uint32_t PRLAR_Value;
    __set_CP(15, 0, rnr, 6, 2, 1);
    __get_CP(15, 0, PRLAR_Value, 6, 3, 1);
    if ((PRLAR_Value & (uint32_t)0x1) == 1 ) {
        return 1;
    }

    ARM_MPU_SetRegion(rnr, region_attr);
    
    return 0;
}

uint8_t MPU_SetRegion(ARM_MPU_Region_t region_attr)
{
    while (last_mpu_index < MAX_MPU_REGION_SUPPORTED) {
        if (MPU_SetRegion_ByIndex(last_mpu_index, region_attr) == 1) {
            last_mpu_index++;
        }
        else {
            last_mpu_index++;
            return 0;
        }
    }
    return 1;
}

uint8_t MPU_ClrRegion_ByIndex(e_region_index_t rnr)
{
    if (rnr >= MAX_MPU_REGION_SUPPORTED) {
        return 1;
    }

    ARM_MPU_ClrRegion(rnr);
    if ((uint8_t)rnr < last_mpu_index) {
        last_mpu_index = (uint8_t)rnr;
    }
}
