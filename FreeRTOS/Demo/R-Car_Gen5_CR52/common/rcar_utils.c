/*************************************************************************************************************
* Copyright (c) 2025 Renesas Electronics Corporation
*
* SPDX-License-Identifier: MIT
 *************************************************************************************************************/

#include "memory_map.h"
#include "cmsis_rcar_gen5.h"
#include "drivers/timer/arm_generic_timer.h"
#include <stdlib.h>
#include <stddef.h>

st_memory_t R_UTILS_GetMemoryRegionInfo(e_memory_type_t type, uint8_t region_idx)
{   
    st_memory_t region = {.base_address = 0, .size = 0};

    uint8_t idx = 0;

    for (uint8_t i = 0; i < sizeof(RCAR_MEMMORY_ARR)/sizeof(st_memory_region_t); i++) {
        if (RCAR_MEMMORY_ARR[i].type == type) {
            if (idx == region_idx) {
                region.base_address = RCAR_MEMMORY_ARR[i].mem_addr.base_address;
                region.size = RCAR_MEMMORY_ARR[i].mem_addr.size;
                break;
            }
            idx ++;
        }
    }
   
    return region;
}

uint8_t R_UTILS_GetTotalRegionOfMemory(e_memory_type_t type)
{    
    uint8_t total_region = 0;
    
    for (uint8_t i = 0; i < sizeof(RCAR_MEMMORY_ARR)/sizeof(st_memory_region_t); i++) {
        if (RCAR_MEMMORY_ARR[i].type == type) {
            total_region++;
        }
    }

    return total_region;
}

uint64_t R_UTILS_GetTimerCounter(void)
{
    return CNTPCT_READ();
}

uint32_t R_UTILS_GetTimerFrequency(void)
{
    return CNTFRQ_READ();

}
