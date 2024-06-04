/*
 * Copyright (c) 2019-2020 Renesas Electronics Europe Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"

#include "register.h"
#include "interrupts.h"

#define R_OS_BSP_GENERIC_ARM_TIMER_IRQNUM                29

#define R_OS_BSP_GENERIC_ARM_CR7_PRIV_TIMER_BASEADDR     0xf0000600

#define LOC_PTIMER_BASE R_OS_BSP_GENERIC_ARM_CR7_PRIV_TIMER_BASEADDR

#define LOC_PTLR        (LOC_PTIMER_BASE + 0x00)
#define LOC_PTCR        (LOC_PTIMER_BASE + 0x04)
#define LOC_PTCONR      (LOC_PTIMER_BASE + 0x08)
#define LOC_PTISR       (LOC_PTIMER_BASE + 0x0C)

#define CR7_PRIV_TIMER_CLOCK    200000000   /* Hz */

#define LOC_PT_WRITE32(ADDR, VAL) *((volatile unsigned int*)((uintptr_t)ADDR)) = (VAL)
#define LOC_PT_READ32(ADDR) *((volatile unsigned int*)((uintptr_t)ADDR))

void vConfigureTickInterrupt(void)
{
    uint32_t tmr_load_val = CR7_PRIV_TIMER_CLOCK / configTICK_RATE_HZ;

	Irq_SetupEntry(R_OS_BSP_GENERIC_ARM_TIMER_IRQNUM, (IrqHandlerFn)FreeRTOS_Tick_Handler, NULL);
	Irq_Enable(R_OS_BSP_GENERIC_ARM_TIMER_IRQNUM);

    LOC_PT_WRITE32(LOC_PTLR, tmr_load_val);
    LOC_PT_WRITE32(LOC_PTCONR, 0x7);
}

void vClearTickInterrupt(void)
{
    LOC_PT_WRITE32(LOC_PTISR, 0x1);
}
