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
#include "interrupts.h"
#include "FreeRTOS.h"
#include "task.h"
#include "dmac/dmac_common.h"
#include "dmac/rtdmac_ctrl.h"
#include "dmac/sysdmac_ctrl.h"

#include "stdio.h"
#include "stdbool.h"
#define main_DMAC_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

extern int printf_delay(const char *format, ...);
void dmacUserCallback(void *data);

/*-----------------------------------------------------------*/

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void prvDMACTask( void *pvParameters );

/*------------------------- Configure mem-to-mem with Normal mode ----------------------------------*/
/* Define configure DMA Controller */
rDmacCfg_t cfg0 =
{
	//Fill in the configuration details
	.mSrcAddr = 0x189E7000,
	.mDestAddr = 0x189E7100,
	.mTransferCount = 1,
	.mDMAMode = DRV_DMAC_DMA_NO_DESCRIPTOR, // Assuming DRV_DMAC_DMA_NO_DESCRIPTOR is defined
	.mSrcAddrMode = DRV_RTDMAC_ADDR_FIXED, // Assuming ADDR_MODE_FIXED is defined
	.mDestAddrMode = DRV_RTDMAC_ADDR_FIXED, // Assuming ADDR_MODE_FIXED is defined
	.mTransferUnit = DRV_RTDMAC_TRANS_UNIT_4BYTE, // Assuming DRV_RTDMAC_TRANS_UNIT_4BYTE is defined
	.mResource = DRV_RTDMAC_MEMORY, // Assuming DRV_RTDMAC_MEMORY is defined
	.mLowSpeed = DRV_RTDMAC_SPEED_NORMAL, // Assuming DRV_RTDMAC_SPEED_NORMAL is defined
	.mPrioLevel = 0
};

/*------------------------- Configure mem-to-mem with Descriptor Normal mode ----------------------------------*/
rDmacCfg_t cfg1 =
{
	//Fill in the configuration details
	.mSrcAddr = 0x189E7000,
	.mDestAddr = 0x189E7100,
	.mTransferCount = 3,
	.mDMAMode = DRV_DMAC_DMA_DESC_NORMAL, // Assuming DRV_DMAC_DMA_DESC_NORMAL is defined
	.mSrcAddrMode = DRV_RTDMAC_ADDR_FIXED, // Assuming ADDR_MODE_FIXED is defined
	.mDestAddrMode = DRV_RTDMAC_ADDR_FIXED, // Assuming ADDR_MODE_FIXED is defined
	.mTransferUnit = DRV_RTDMAC_TRANS_UNIT_4BYTE, // Assuming DRV_RTDMAC_TRANS_UNIT_4BYTE is defined
	.mResource = DRV_RTDMAC_MEMORY, // Assuming DRV_RTDMAC_MEMORY is defined
	.mLowSpeed = DRV_RTDMAC_SPEED_NORMAL, // Assuming DRV_RTDMAC_SPEED_NORMAL is defined
	.mPrioLevel = 0
};

/* Define configure the DMA descriptor */
rDmacDescCfg_t descCfg1 =
{
	.mDescBaseAddr = 0x189E7000,
	.mDescUpdate = {0},
	.mDescRead1st = true,
	.mStateEndEnable = false,
	.mDescCount = 3,
	.mDescIndex = 0
};

/*------------------------- Configure mem-to-mem with Descriptor Repeat mode ----------------------------------*/

/* Define configure DMA Controller */
rDmacCfg_t cfg2 =
{
    .mSrcAddr = 0x189E7000,                     // Source address
    .mDestAddr = 0x189E7100,                    // Destination address
    .mTransferCount = 4,                        // Transfer count
    .mDMAMode = DRV_DMAC_DMA_DESC_REPEAT,       // Reapeat descriptor mode
    .mSrcAddrMode = DRV_RTDMAC_ADDR_FIXED,      // Source address fixed
    .mDestAddrMode = DRV_RTDMAC_ADDR_INCREMENTED, // Destination address increment
    .mTransferUnit = DRV_RTDMAC_TRANS_UNIT_4BYTE, // Transfer unit 4 bytes
    .mResource = DRV_RTDMAC_MEMORY,            // DMA resource
    .mLowSpeed = DRV_RTDMAC_SPEED_NORMAL,
    .mPrioLevel = 1
};

/* Define configure the DMA descriptor */
rDmacDescCfg_t descCfg2 =
{
    .mDescBaseAddr = 0x189E7200,                // Descriptor base address
    .mDescUpdate = {0},                         // No descriptor update
    .mDescRead1st = true,                       // Read descriptor first
    .mStateEndEnable = true,                    // Trigger state end
    .mDescCount = 2,                            // Descriptor count
    .mDescIndex = 0                             // Descriptor index
};

/*-----------------------------------------------------------*/

rDmacIrqCfg_t rDmacIrqHandler_t_irq =
{
	.Unit = RT_DMAC3,
	.SubCh = DMAC_CH1,
	.irq_channel = INTID_RTDMA3_CH1,
};

/*-----------------------------------------------------------*/

int main( void )
{
        /* Configure the hardware ready to run the demo. */
        prvSetupHardware();


        xTaskCreate( prvDMACTask, "DMACTask", configMINIMAL_STACK_SIZE, NULL, main_DMAC_TASK_PRIORITY, NULL );
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

static void prvDMACTask( void *pvParameters )
{
	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;
	int ret;

	Context_t usr_context =
	{
		.ctx = &rDmacIrqHandler_t_irq,
	};

	/* Device Driver Part */
	R_RTDMAC_RcarDmacCtrlInit(RT_DMAC3, DRV_RTDMAC_PRIO_FIX);

	*(volatile uint32_t*)cfg0.mSrcAddr = 0x3;
	printf_delay("Value at SrcAddr: 0x%x \n",*(volatile uint32_t*)cfg0.mSrcAddr);

	ret = R_RTDMAC_RcarCallBackSet(&rDmacIrqHandler_t_irq, dmacUserCallback, &usr_context);
	if (ret)
		printf_delay("CallbackSet Failed: ret = %d\n", ret);

	int dmaStatus =R_RTDMAC_RcarDmacExec(RT_DMAC3, DMAC_CH1, &cfg0, 0);

	// Check DMA execution status
	if (dmaStatus != 0)
		printf_delay("DMA execution failed with status: %d\n", dmaStatus);

	// Verify destination data
	uint32_t destData = *(volatile uint32_t*)cfg0.mDestAddr;
	printf_delay("Value at DestAddr after DMA: 0x%x \n", destData);

	for( ;; )
	{
	}
}

/*-----------------------------------------------------------*/

void dmacUserCallback(void *data) {
	rDmacIrqCfg_t * instance_ctrl = (rDmacIrqCfg_t *) data;
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
