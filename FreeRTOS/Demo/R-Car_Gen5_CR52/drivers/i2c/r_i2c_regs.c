/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "r_i2c_regs.h"
//#include "r_print_api.h"
#include "stdio.h"
#include "devicetree-binding.h"

void R_I2C_PRV_RegWrite32(uintptr_t Addr, uint32_t Data)
{
    *((volatile uint32_t *)Addr) = Data;
    return;
}

uint32_t R_I2C_PRV_RegRead32(uintptr_t Addr)
{
    return *((volatile uint32_t *)Addr);
}

uintptr_t R_I2C_PRV_GetRegbase(r_i2c_Unit_t I2cUnit)
{
    if((uint32_t)I2cUnit > dt_count_node((void *)i2c_list) - 1 || 
       i2c_list[(uint32_t)I2cUnit]->status != OKAY)
    {
        printf("[R_I2C_PRV_GetRegbase] : Wrong I2C Unit %d\r\n", I2cUnit);
        return 0;
    }

    return i2c_list[(uint32_t)I2cUnit]->base_address;
}
