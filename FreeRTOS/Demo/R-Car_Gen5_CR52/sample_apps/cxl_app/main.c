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
#include "stdio.h"
#include <stdlib.h>
#include <stdbool.h>
#include "pcie/r_pcie_ctrl.h"
#include "pcie/r_pcie_host.h"
#include "pcie/r_pcie_ep.h"

#define main_CXL_TASK_PRIORITY        ( tskIDLE_PRIORITY + 2 )

/* Test Data */
const uint32_t TESTDATA1[] = { 0x12345678, 0xFEDCBA98, 0xA5A5A5A5, 0x5A5A5A5A,
				0x11223344, 0x55667788, 0xAABBCCDD, 0xEEFF0011 };
#define TESTDATA1_SIZE		(8U)

/*-----------------------------------------------------------*/

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void prvCXLTask( void *pvParameters );

void mem_write32(volatile uintptr_t addr, uint32_t data)
{
    *((volatile uint32_t*)(addr)) = data;
}

uint32_t mem_read32(const volatile uintptr_t addr)
{
    return *((volatile uint32_t*)(addr));
}

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
    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    uint8_t ret = 0;
    struct st_pcie_ep ep;
    struct st_pcie_host host;
    uint32_t timeout = 10000;
    uint32_t EP_CH = 1; // ch1 is set to EP
    uint32_t HOST_CH = 0; // ch0 is set to Host
    uint32_t dma_rd_dar, dma_wr_sar;
    uint32_t offset;
    int i;

    dma_wr_sar = 0x62000000;
    dma_rd_dar = 0x62080000;

    printf("** Test verifying DMA transfer using UCIe/CXL **\r\n");
    for (offset = 0; offset < TESTDATA1_SIZE; offset++) {
	mem_write32((dma_wr_sar + (offset * 0x4)), TESTDATA1[offset]);

	mem_write32((dma_rd_dar + (offset * 0x4)), 0x0);
    }

    printf("[Test Data:SAR]\n");
    printf("0x%x | 0x%08x 0x%08x 0x%08x 0x%08x\n", dma_wr_sar,
		mem_read32(dma_wr_sar), mem_read32(dma_wr_sar + 0x4),
		mem_read32(dma_wr_sar + 0x8), mem_read32(dma_wr_sar + 0xC));
    printf("0x%x | 0x%08x 0x%08x 0x%08x 0x%08x\n", (dma_wr_sar + 0x10),
		mem_read32(dma_wr_sar + 0x10), mem_read32(dma_wr_sar + 0x14),
		mem_read32(dma_wr_sar + 0x18), mem_read32(dma_wr_sar + 0x1C));

    printf("TC1: Initialize for UCIe RC ch0\n");
    ret = R_PCIE_InitHost(&host, HOST_CH);
    if (ret)
        printf("Result: Failed\r\n");
    else
        printf("Result: Passed\r\n");
    
    printf("**************************************************\r\n");

    printf("TC2: Initialize for UCIe EP ch1\n");
    ret = R_PCIE_EP_Init(&ep, EP_CH);
    if (ret)
        printf("Result: Failed\r\n");
    else
        printf("Result: Passed\r\n");

    printf("**************************************************\r\n");

    printf("TC3: UCIe RC ch0 inbound configuration\n");
    ret = R_PCIE_Host_Inbound_ATU(HOST_CH);
    if (ret)
        printf("Result: Failed\r\n");
    else
        printf("Result: Passed\r\n");

    printf("**************************************************\r\n");

    printf("TC4: UCIe EP ch1 outbound configuration\n");
    ret = R_PCIE_EP_Outbound_ATU(&ep, EP_CH);
    if (ret)
        printf("Result: Failed\r\n");
    else
        printf("Result: Passed\r\n");

    printf("**************************************************\r\n");

    printf("TC5: Start DMA transfer\n....\n");
    ret = R_PCIE_DMAtransfer(EP_CH, dma_wr_sar, dma_rd_dar);
    if (ret) { 
        printf("Result: Failed\r\n");
    }
    else {
        printf("[Dump:DAR]\n");
        printf("0x%x | 0x%08x 0x%08x 0x%08x 0x%08x\n", dma_rd_dar,
            mem_read32(dma_rd_dar), mem_read32(dma_rd_dar + 0x4),
            mem_read32(dma_rd_dar + 0x8), mem_read32(dma_rd_dar + 0xC));
        printf("0x%x | 0x%08x 0x%08x 0x%08x 0x%08x\n", (dma_rd_dar + 0x10),
            mem_read32(dma_rd_dar + 0x10), mem_read32(dma_rd_dar + 0x14),
            mem_read32(dma_rd_dar + 0x18), mem_read32(dma_rd_dar + 0x1C));

        /* Verify: DMA Write Channel SAR -> Read Channel DAR */
        for (offset = 0; offset < 8; offset++) {
            if ((mem_read32(dma_wr_sar + (offset * 0x4)))
                != (mem_read32(dma_rd_dar + (offset * 0x4)))) {
                ret = -1;
                break;
            }
        }

        if (ret)
            printf("Result: Failed\r\n");
        else
            printf("Result: Passed\r\n");
    }
    printf("**************************************************\r\n");

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
