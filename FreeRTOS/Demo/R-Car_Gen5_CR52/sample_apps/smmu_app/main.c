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

#include "interrupts.h"
#include "stdio.h"

#include "smmu/smmu.h"
#include "dmac/dmac_common.h"
#include "dmac/rtdmac_ctrl.h"
#include "dmac/sysdmac_ctrl.h"

#include "cmsis_rcar_gen5.h"

#define main_SMMU_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

/*-----------------------------------------------------------*/

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void prvSMMUTask( void *pvParameters );
extern int printf_delay(const char *format, ...);
void dmacUserCallback(void *data);
/* Define configure DMA Controller */
rDmacCfg_t cfg =
{
	//Fill in the configuration details
	// .mSrcAddr = 0x189E7000,
	// .mDestAddr = 0x189E7100,
    .mSrcAddr = 0x60000000,
	.mDestAddr = 0x60001000,
	.mTransferCount = 1,
	.mDMAMode = DRV_DMAC_DMA_NO_DESCRIPTOR, // Assuming DRV_DMAC_DMA_NO_DESCRIPTOR is defined
	.mSrcAddrMode = DRV_RTDMAC_ADDR_FIXED, // Assuming ADDR_MODE_FIXED is defined
	.mDestAddrMode = DRV_RTDMAC_ADDR_FIXED, // Assuming ADDR_MODE_FIXED is defined
	.mTransferUnit = DRV_RTDMAC_TRANS_UNIT_4BYTE, // Assuming DRV_RTDMAC_TRANS_UNIT_4BYTE is defined
	.mResource = DRV_RTDMAC_MEMORY, // Assuming DRV_RTDMAC_MEMORY is defined
	.mLowSpeed = DRV_RTDMAC_SPEED_NORMAL, // Assuming DRV_RTDMAC_SPEED_NORMAL is defined
	.mPrioLevel = 0
};

rDmacIrqCfg_t rDmacIrqHandler_t_irq =
{
	.Unit = SYS_DMAC3,
	.SubCh = DMAC_CH1,
	.irq_channel = INTID_SYSDMA3_CH1
};
/*-----------------------------------------------------------*/

int main( void )
{
	/* Configure the hardware ready to run the demo. */
    
    prvSetupHardware();

    xTaskCreate(prvSMMUTask, "SMMUTask", configMINIMAL_STACK_SIZE, NULL, main_SMMU_TASK_PRIORITY, NULL );
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

static void prvSMMUTask( void *pvParameters )
{
    /* Remove compiler warning about unused parameter. */
    (void)pvParameters;
    int ret;

    st_smmu_instance_ctrl_t smmu_crtl = {
        .smmu_domain = SMMU_PERW,
    };
    st_smmu_cmd_t cmd_ste_cfg = {0};

    ret = R_SMMU_Init(&smmu_crtl);

    printf("*Test case 1: Enable command queue and event queue*\r\n");
    if (ret == 0) {
        printf("Result: Passed\r\n");
    } else {
        printf("Result: Failed\r\n");
    }
    printf("**********************************************\r\n");

    printf("*Test case 2: Issue command*\r\n");
    uint32_t stream_id = 0x40000;
    R_SMMU_Attach(&smmu_crtl, stream_id);
    
    vTaskDelay(100);

    int ret_cmd1, ret_cmd2 = -1;
    memset(&cmd_ste_cfg, 0, sizeof(cmd_ste_cfg));
    cmd_ste_cfg.opcode = CMDQ_OP_CFGI_STE;
    cmd_ste_cfg.st_cmd_cfgi_ste_t.sid = 0x40000;
    cmd_ste_cfg.st_cmd_cfgi_ste_t.leaf = 1;
    ret_cmd1 = R_SMMU_IssueCommand(&smmu_crtl, &cmd_ste_cfg, true);

    vTaskDelay(100);

    // Issue commands to invalidate all cached configuration and TLB entrie
    ret_cmd2 = R_SMMU_InvalidateTLB(&smmu_crtl);
    if ((ret_cmd1 == 0) && (ret_cmd2 == 0)) {
       printf("Result: Passed\r\n");
    } else {
        printf("Result: Failed\r\n");
    }
    printf("**********************************************\r\n");

    vTaskDelay(1000);
    printf("*Test case 3: Enable SMMU*\r\n");
    ret = R_SMMU_Enable(&smmu_crtl);
    
    L1C_InvalidateDCacheAll();
    __DSB();
    if (ret == 0 ) {
        printf("Result: Passed\n");
    } else {
        printf("Result: Failed\n");
    }
    printf("**********************************************\r\n");
    vTaskDelay(1000);

    /* Device Driver Part */
    R_SYSDMAC_RcarDmacCtrlInit(SYS_DMAC3, DRV_RTDMAC_PRIO_FIX);

    *(volatile uint32_t *)cfg.mSrcAddr = 0x479;

    Context_t usr_context = 
    {
        .ctx = &rDmacIrqHandler_t_irq,
    };

    ret = R_SYSDMAC_RcarCallBackSet(&rDmacIrqHandler_t_irq, dmacUserCallback, &usr_context);

    int dmaStatus = R_SYSDMAC_RcarDmacExec(SYS_DMAC3, DMAC_CH1, &cfg, 0);

    // Verify destination data
    uint32_t destData = *(volatile uint32_t *)cfg.mDestAddr;

    printf("*Test case 4: Test transaction data*\r\n");
    if (destData == (*(volatile uint32_t *)cfg.mSrcAddr)) {
        printf("Result: Passed\n");
    } else {
        printf("Result: Failed\n");
    }
    printf("**********************************************\r\n");

    for (;;)
    {
    }
}

/*-----------------------------------------------------------*/
void dmacUserCallback(void *data)
{
    rDmacIrqCfg_t *instance_ctrl = (rDmacIrqCfg_t *)data;
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
