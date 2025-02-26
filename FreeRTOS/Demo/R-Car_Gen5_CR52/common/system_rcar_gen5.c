/*
 * Copyright (C) 2019-2020 Renesas Electronics Europe Ltd. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cmsis_rcar_gen5.h"
#include "mpu.h"

#define SYSTEM_CLOCK_COUNTER_DEFAULT 25000000

// Define all peripheral address regions
#define PERIPHERAL_START_0          0x18800000
#define PERIPHERAL_SIZE_0           0x00080000  // to 0x1888_0000

#define PERIPHERAL_START_1          0x188C0000
#define PERIPHERAL_SIZE_1           0x07740000  // to 0x2000_0000

#define CA_CMA_ADDRESS              0x50000000
#define CA_CMA_SIZE                 0x0FF00000  // to 0x5FF0_0000

#define OSAL_MEMORY_ADDRESS         0x60000000
#define OSAL_MEMORY_SIZE            0x20000000  // to 0x8000_0000

#define SHARED_DRAM_ADDRESS         0x80000000
#define SHARED_DRAM_SIZE            0x20000000  // to 0xA000_0000

#define PERIPHERAL_START_2          0xC0000000
#define PERIPHERAL_SIZE_2           0x40000000  // to 0x1_0000_0000

extern const unsigned int __bss_start__;
extern const unsigned int __bss_end__;
extern const unsigned int _STACK_SIZE;

extern uint32_t _RAM_START;
extern uint32_t _RAM_SIZE;

extern uint32_t _Reset;
uint32_t resource_table;
#if ETHER_ENABLE
extern uint32_t eth_non_cache_start;
#endif

extern int main(void);

extern void __libc_init_array(void) ;

static void Init_MPU(void)
{
    /* Disable MPU */
    MPU_Disable();

    MPU_Init();

    MPU_SetRegion(REGION_0, REGION_SRAM_ATTR((uint32_t) &_RAM_START, (uint32_t) &_RAM_SIZE));
    MPU_SetRegion(REGION_1, REGION_DEVICE_ATTR((uint32_t) CA_CMA_ADDRESS, (uint32_t) CA_CMA_SIZE));
    MPU_SetRegion(REGION_2, REGION_DEVICE_ATTR((uint32_t) OSAL_MEMORY_ADDRESS, (uint32_t) OSAL_MEMORY_SIZE));
    MPU_SetRegion(REGION_3, REGION_DEVICE_ATTR((uint32_t) SHARED_DRAM_ADDRESS, (uint32_t) SHARED_DRAM_SIZE));
    MPU_SetRegion(REGION_4, REGION_DEVICE_ATTR((uint32_t) PERIPHERAL_START_0, (uint32_t) PERIPHERAL_SIZE_0));
    MPU_SetRegion(REGION_5, REGION_DEVICE_ATTR((uint32_t) PERIPHERAL_START_1, (uint32_t) PERIPHERAL_SIZE_1));
    MPU_SetRegion(REGION_6, REGION_DEVICE_ATTR((uint32_t) PERIPHERAL_START_2, (uint32_t) PERIPHERAL_SIZE_2));

    /* Enable MPU */
    MPU_Enable();
}

__STATIC_INLINE void bss_init(unsigned int* section_begin, unsigned int* section_end)
{
  // Iterate and clear word by word.
  // It is assumed that the pointers are word aligned.
  unsigned int *p = section_begin;
  while (p < section_end)
    *p++ = 0;
}

static void FPU_Enable()
{
#define BSP_CPCAR_CP_ENABLE             (0x00F00000)
#define BSP_FPEXC_EN_ENABLE             (0x40000000)
    uint32_t apacr;
    uint32_t fpexc;

    /* Enables cp10 and cp11 accessing */
    apacr  = __get_CPACR();
    apacr |= BSP_CPCAR_CP_ENABLE;
    __set_CPACR(apacr);
    __ISB();

    /* Enables the FPU */
    fpexc  = __get_FPEXC();
    fpexc |= BSP_FPEXC_EN_ENABLE;
    __set_FPEXC(fpexc);
    __ISB();

}

static void system_counter_init(uint32_t clock_rate) {
#if 0 // Fix me later
    __set_CNTFRQ(clock_rate);
#endif
}

void SystemInit(void)
{
//    uint32_t tmp;
//
//    tmp = __get_SCTLR();
//    tmp &= ~SCTLR_M_Msk;            /* Disable MPU (M bit) */
//    tmp &= ~SCTLR_C_Msk;            /* Disable data cache (C bit) */
//    tmp &= ~SCTLR_Z_Msk;            /* Disable branch prediction (Z bit) */
//    tmp &= ~SCTLR_I_Msk;            /* Disable instruction cache (I bit) */
//    __DSB();
//    __set_SCTLR(tmp);
//    __ISB();
//
//    /* Enable Floating point hardware */
#if (defined(__FPU_USED) && (__FPU_USED == 1U))
    FPU_Enable();
#endif
    // Init system counter.
    system_counter_init(SYSTEM_CLOCK_COUNTER_DEFAULT);
//
//    /*
//     * Do not use global variables because this function is called before
//     * reaching pre-main. RW section may be overwritten afterwards.
//     */
//
//    /* Invalidate instruction cache and flush branch target cache */
//    __set_ICIALLU(0);
//    __DSB();
//    __ISB();
//
//    L1C_InvalidateDCacheAll();
//
//    /*
//     * R-Car specific
//     * Set the address of the vector table using RBAR. Note that although
//     * we can change the address of the vector table using RBAR, as far as
//     * the MPU is concerned, the vector table is still at address 0x0.
//     */
//#define CR7BAR    0xE6160070U
//    writel((uint32_t)&_Reset, CR7BAR);
//    __ISB();
//    /* Enable BAREN */
//    writel((uint32_t)&_Reset | BIT(4), CR7BAR);
//
    Init_MPU();
//
//    L1C_EnableCaches();
//    L1C_EnableBTAC();
//    bss_init((void *)&__bss_start__, (void *)&__bss_end__);
    __libc_init_array();
}

void assert_func(const char *file, int line, const char *func)
{
    printf("ASSERT! File \"%s\", Line \"%d\", Function \"%s\" \n", file, line, func);
    for (;;)
    {
        __BKPT(0);
    }
}
