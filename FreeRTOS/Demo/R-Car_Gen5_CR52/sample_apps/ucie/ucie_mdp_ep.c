/*
 * FreeRTOS Kernel V11.1.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://github.com/FreeRTOS
 *
 */

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "interrupts.h"

#include <stdlib.h>
#include "stdio.h"
#include "string.h"

#include "pfc/r_pfc_api.h"
#include "device_tree_x5h.h"
#include "ucie/r_ucie.h"
#include "rcar_utils.h"
#define main_ucie_TASK_PRIORITY        (tskIDLE_PRIORITY + 1)
#define UCIE_EP_SIZE (configMINIMAL_STACK_SIZE * 20)

#define PIO_RC_WRITE_DATA   0xFFCCFFCC
#define PIO_EP_WRITE_DATA   0x68686868

/*-----------------------------------------------------------*/

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void ucie_comm_task( void *pvParameters );

int main( void )
{
    /* Configure the hardware ready to run the demo. */
    prvSetupHardware();

    xTaskCreate(ucie_comm_task, "UCIe", UCIE_EP_SIZE, NULL, main_ucie_TASK_PRIORITY, NULL );

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
    for( ;; )
    {
    }
    /* Don't expect to reach here. */
    return 0;
}

/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
    /* Ensure no interrupts execute while the scheduler is in an inconsistent
    state.  Interrupts are automatically enabled when the scheduler is
    started. */
    portDISABLE_INTERRUPTS();

    Irq_Setup();

    (void)pfcInitModules(getModuleConfigs());
}

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

static void ucie_comm_task(void *pvParameters)
{
    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    uint32_t ret = 0;
    uint32_t timeout;

    printf("<----- [EP] TC1: UCIE LINKUP ----->\n");
    ret = R_UCIE_Setup(UCIE_CH0, UCIE_MODE_EP, LINKSPEED_16GTPS);
    if (ret == LINKUP_TIMEOUT) {
        printf("The first time linkup timeout. Retry 30 times\n");
        ret = R_UCIE_Retry_Linkup(UCIE_CH0, UCIE_MODE_EP, LINKSPEED_16GTPS, 30);
    }
    
    if (ret) {
        printf("Result: FAILED\n");
    }
    else {
        printf("Result: PASSED\n");
    }

    printf("<----- [EP] TC2: Verify data written by RC ----->\n");
    uint64_t rc_addr = 0x90000000;
    uint64_t ep_addr = 0x90000000;

    timeout = 1000000;
    while (*(volatile uint32_t*)(uintptr_t)ep_addr != PIO_RC_WRITE_DATA) {
        timeout--;
    }

    printf("Value at 0x%llX: 0x%X\n", ep_addr, *(volatile uint32_t*)(uintptr_t)ep_addr);
    if (timeout) {
        printf("Result: PASSED\n");
    }
    else {
        printf("Result: FAILED\n");
    }

    printf("EP write 0x%X to share mem region\n", PIO_EP_WRITE_DATA);
    *(volatile uint32_t*)(uintptr_t)ep_addr = PIO_EP_WRITE_DATA;

    printf("<----- [EP] END TEST ----->\n");
    for(;;) {
        __asm__ volatile("nop");
    }
}

/*-----------------------------------------------------------*/

int printf_raw(const char *format, ...);

void vMainAssertCalled( const char *pcFileName, uint32_t ulLineNumber )
{
    /* Don't use printf as it uses FreeRTOS resources */
    printf_raw("ASSERT!  Line %d of file %s\n", ulLineNumber, pcFileName);
    taskENTER_CRITICAL();
    for( ;; );
}

void vDeleteCallingTask( void )
{
     vTaskDelete( NULL );
}
