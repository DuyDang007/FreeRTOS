/*
 * Copyright (c) 2019-2020 Renesas Electronics Europe Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"

#include "interrupts.h"
#include "core_cr52.h"
#include "CMSIS_5/irq_ctrl.h"
#include "drivers/timer/arm_generic_timer.h"

#include "cmsis_rcar_gen5.h"

#define COUNT_PER_TICK  (GENERIC_TIMER_CLK / configTICK_RATE_HZ)

/* This tick value is initialized from timer start, not equal to FreeRTOS tick */
static uint32_t tick = 0;

void TickInterruptHandler(void) {
    tick++;
    CNTP_CVAL_WRITE(COUNT_PER_TICK * tick + COUNT_PER_TICK);
    FreeRTOS_Tick_Handler();
}

void vConfigureTickInterrupt(void)
{
    uint64_t tempCNTPCT = CNTPCT_READ();

    Irq_SetupEntry(R_OS_BSP_GENERIC_ARM_TIMER_IRQNUM, (IrqHandlerFn)TickInterruptHandler, NULL);

    Irq_SetPriority(R_OS_BSP_GENERIC_ARM_TIMER_IRQNUM, IPRIORITY(24));

    /* Set timer expiration to the next 2 ticks from current counter value to avoid missing
    an interrupt when time to next tick is too short */
    tick = CNTPCT_READ() / COUNT_PER_TICK;
    CNTP_CVAL_WRITE((tick + 2) * COUNT_PER_TICK);

    /* configure CNTP_CTL to enable timer interrupts */
    CNTP_CTL_WRITE(1);

    Irq_Enable(R_OS_BSP_GENERIC_ARM_TIMER_IRQNUM);

}
