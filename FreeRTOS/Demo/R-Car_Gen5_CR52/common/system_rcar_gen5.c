/*
 * Copyright (C) 2019-2020 Renesas Electronics Europe Ltd. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cmsis_rcar_gen3.h"
#include "register.h"

extern const unsigned int __bss_start__;
extern const unsigned int __bss_end__;
extern const unsigned int _STACK_SIZE;	

extern uint32_t _Reset;
uint32_t resource_table;
#if ETHER_ENABLE
extern uint32_t eth_non_cache_start;
#endif

extern int main(void);

static void Init_MPU(void)
{
	int region = 0;
	uint32_t attributes;
	uint32_t nr_regions = ARM_MPU_GetNrRegions();

	ARM_MPU_Disable();

	/* ARM_MPU_ATTRIB = XN, AP, TEX, S, C, B. See DRACR register description for
	 * the meanings */

	/* Default */
	attributes = ARM_MPU_ATTRIB(1, ARM_MPU_AP_NONE, 1, 0, 1, 1);
	ARM_MPU_SetRegionCR(region++, 0x00000000U, attributes, ARM_MPU_REGION_SIZE_4GB);

	/* Peripherals */
	attributes = ARM_MPU_ATTRIB(1, ARM_MPU_AP_FULL, 2, 0, 0, 0);
	ARM_MPU_SetRegionCR(region++, 0xC0000000U, attributes, ARM_MPU_REGION_SIZE_1GB);

	/* Data (non-Cacheable, non-Bufferable) */
	attributes = ARM_MPU_ATTRIB(1, ARM_MPU_AP_FULL, 1, 0, 0, 0);
	ARM_MPU_SetRegionCR(region++, 0x40000000U, attributes, ARM_MPU_REGION_SIZE_1GB);

	/* Data (Cacheable, Bufferable) */
	attributes = ARM_MPU_ATTRIB(1, ARM_MPU_AP_FULL, 1, 0, 1, 1);
	ARM_MPU_SetRegionCR(region++, 0x70000000U, attributes, ARM_MPU_REGION_SIZE_256MB);

	/* Code */
	attributes = ARM_MPU_ATTRIB(0, ARM_MPU_AP_FULL, 1, 0, 1, 1);
	ARM_MPU_SetRegionCR(region++, (uint32_t)&_Reset, attributes, ARM_MPU_REGION_SIZE_8MB);

	/*
	 * The Cortex R7 can have the vectors at address 0x0 or 0xffff0000 (see the
	 * SCTLR V bit). On R-Car they are at 0x0, but the access is offset by
	 * the address programmed into the CR7BAR register. However, as far as
	 * the MPU is concerned, the access is not offset.
	 * Therefore, set the MPU to enable access to address 0x0 for the vectors.
	 */
	ARM_MPU_SetRegionCR(region++, 0x00000000U, attributes, ARM_MPU_REGION_SIZE_256B);

	/* Resource table (non-Cacheable, non-Bufferable) */
	attributes = ARM_MPU_ATTRIB(1, ARM_MPU_AP_FULL, 1, 0, 0, 0);
	ARM_MPU_SetRegionCR(region++, (uint32_t)&resource_table, attributes, ARM_MPU_REGION_SIZE_256B);

#if ETHER_ENABLE
	/* 128KB non-cached area for URAM (descriptor) area of ETH */
	attributes = ARM_MPU_ATTRIB(1, ARM_MPU_AP_FULL, 1, 0, 0, 0);
	ARM_MPU_SetRegionCR(region++, (uint32_t)&eth_non_cache_start, attributes, ARM_MPU_REGION_SIZE_128KB);
#endif
	while (region < nr_regions)
		ARM_MPU_ClrRegion(region++);

	ARM_MPU_Enable();
}

__STATIC_INLINE void bss_init(unsigned int* section_begin, unsigned int* section_end)
{
  // Iterate and clear word by word.
  // It is assumed that the pointers are word aligned.
  unsigned int *p = section_begin;
  while (p < section_end)
    *p++ = 0;
}

void SystemInit(void)
{
//	uint32_t tmp;
//
//	tmp = __get_SCTLR();
//	tmp &= ~SCTLR_M_Msk;			/* Disable MPU (M bit) */
//	tmp &= ~SCTLR_C_Msk;			/* Disable data cache (C bit) */
//	tmp &= ~SCTLR_Z_Msk;			/* Disable branch prediction (Z bit) */
//	tmp &= ~SCTLR_I_Msk;			/* Disable instruction cache (I bit) */
//	__DSB();
//	__set_SCTLR(tmp);
//	__ISB();
//
//	/* Enable Floating point hardware */
//#if (defined(__FPU_USED) && (__FPU_USED == 1U))
//	__FPU_Enable();
//#endif
//
//	/*
//	 * Do not use global variables because this function is called before
//	 * reaching pre-main. RW section may be overwritten afterwards.
//	 */
//
//	/* Invalidate instruction cache and flush branch target cache */
//	__set_ICIALLU(0);
//	__DSB();
//	__ISB();
//
//	L1C_InvalidateDCacheAll();
//
//	/*
//	 * R-Car specific
//	 * Set the address of the vector table using RBAR. Note that although
//	 * we can change the address of the vector table using RBAR, as far as
//	 * the MPU is concerned, the vector table is still at address 0x0.
//	 */
//#define CR7BAR	0xE6160070U
//	writel((uint32_t)&_Reset, CR7BAR);
//	__ISB();
//	/* Enable BAREN */
//	writel((uint32_t)&_Reset | BIT(4), CR7BAR);
//
//	Init_MPU();
//
//	L1C_EnableCaches();
//	L1C_EnableBTAC();
//	bss_init((void *)&__bss_start__, (void *)&__bss_end__);
}
