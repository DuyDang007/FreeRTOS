/*
 * FreeRTOS Kernel V11.1.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
#include "stdio.h"
#include <stdlib.h>
#include "pcie/r_pcie_ctrl.h"
#include "pcie/r_pcie_host.h"
#include "pcie/r_pcie_ep.h"

#define main_CXL_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

extern int printf_delay(const char *format, ...);

/*-----------------------------------------------------------*/

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void prvCXLTask( void *pvParameters );

/*-----------------------------------------------------------*/

int main( void )
{
	/* Configure the hardware ready to run the demo. */
	prvSetupHardware();

    xTaskCreate( prvCXLTask, "CXL task", configMINIMAL_STACK_SIZE, NULL, main_CXL_TASK_PRIORITY, NULL );
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
}

static void prvCXLTask( void *pvParameters )
{

    uint8_t ret;
    struct st_pcie_ep ep;
    struct st_pcie_host host;
    uintptr_t *dma_local_addr;
    enum pcie_ob_mem_type ob_mem_type = PCIE_OB_ANYMEM;
    uint32_t UCIE_D2D_CH0_LOWER = 0x00000000;
    uint64_t UCIE_D2D_CH0_UPPER = 0x00000200;
    uint32_t SIZE_IN_BYTE = 1024;
    uint32_t EP_CH = 1;
    uint32_t HOST_CH = 0;

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    printf_delay("****** TEST: UCIe/CXL driver ******\n");

    printf_delay("Initialize for UCIe EP channel 1\n");
    R_PCIE_EP_Init(&ep, EP_CH);
    printf_delay("Initialize for UCIe RC channel 0\n");
    R_PCIE_InitHost(&host, HOST_CH);
    printf_delay("Inbound ATU Setting\n");
    R_PCIE_EP_Inbound_ATU(EP_CH);
    printf_delay("Outbound ATU Setting\n");
    R_PCIE_Host_Outbound_ATU(HOST_CH);

    dma_local_addr = malloc(SIZE_IN_BYTE);
    if (!dma_local_addr)
        printf_delay("\n UCIe/CXL DMA alloc fail\n");

    printf_delay("\n Performing DMA Read\n");
    R_PCIE_EP_TransferDataDMA(&ep, (UCIE_D2D_CH0_UPPER << 32) | UCIE_D2D_CH0_LOWER,
					dma_local_addr, SIZE_IN_BYTE, ob_mem_type,
					HOST_TO_DEVICE);
    printf_delay("\n Performing DMA Write\n");
    R_PCIE_EP_TransferDataDMA(&ep, (UCIE_D2D_CH0_UPPER << 32) | UCIE_D2D_CH0_LOWER,
                                        dma_local_addr, SIZE_IN_BYTE, ob_mem_type,
                                        DEVICE_TO_HOST);

    vTaskDelay(1000);
    for( ;; )
    {
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
