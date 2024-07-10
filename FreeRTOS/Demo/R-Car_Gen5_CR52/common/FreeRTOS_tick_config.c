/*
 * Copyright (c) 2019-2020 Renesas Electronics Europe Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"

#include "interrupts.h"
#include "drivers/timer/arm_generic_timer.h"

#include "cmsis_cp15.h"

#define COUNT_PER_TICK  (GENERIC_TIMER_CLK / configTICK_RATE_HZ)
void TickInterruptHandler(void) {
    CNTP_CVAL_WRITE(CNTPCT_READ() + COUNT_PER_TICK);
    FreeRTOS_Tick_Handler();
}

void vConfigureTickInterrupt(void)
{
    uint64_t tempCNTPCT = CNTPCT_READ();

    Irq_SetupEntry(R_OS_BSP_GENERIC_ARM_TIMER_IRQNUM, (IrqHandlerFn)TickInterruptHandler, NULL);

    //uint64_t g_timer_delay = GENERIC_TIMER_CLK / configTICK_RATE_HZ;
    //g_timer_delay = CNTPCT_READ() + 0x100000; //hardcode`

    /* set timer expiration from current counter value */
    CNTP_CVAL_WRITE(CNTPCT_READ() + COUNT_PER_TICK);

    /* configure CNTP_CTL to enable timer interrupts */
    CNTP_CTL_WRITE(1);

    Irq_Enable(R_OS_BSP_GENERIC_ARM_TIMER_IRQNUM);

}
