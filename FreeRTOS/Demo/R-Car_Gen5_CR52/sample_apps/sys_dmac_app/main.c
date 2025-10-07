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
#include "semphr.h"
#include "dmac/dmac_common.h"
#include "dmac/rtdmac_ctrl.h"
#include "dmac/sysdmac_ctrl.h"
#include "pfc/r_pfc_api.h"
#include "device_tree_x5h.h"
#include "rcar_utils.h"

#include "stdio.h"
#include "stdbool.h"
#include <stdlib.h>
#define main_DMAC_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )
#define DESTINATION_OFFSET 			   0x01000000

void dmacUserCallback(void *data);
void dmacUserCallback1(void *data);
void dmacUserCallback2(void *data);

/*-----------------------------------------------------------*/

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void prvDMACTask( void *pvParameters );

SemaphoreHandle_t xSemaphore = NULL;

/*------------------------- Configure mem-to-mem with Normal mode ----------------------------------*/
/* Define configure DMA Controller */
rDmacCfg_t cfg0 =
{
    //Fill in the configuration details
    .mSrcAddr = 0,
    .mDestAddr = 0,
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
    .mSrcAddr = 0,
    .mDestAddr = 0,
    .mTransferCount = 3,
    .mDMAMode = DRV_DMAC_DMA_DESC_NORMAL, // Assuming DRV_DMAC_DMA_DESC_NORMAL is defined
    .mSrcAddrMode = DRV_RTDMAC_ADDR_FIXED, // Assuming ADDR_MODE_FIXED is defined
    .mDestAddrMode = DRV_RTDMAC_ADDR_INCREMENTED, // Assuming ADDR_MODE_INCREMENTED is defined
    .mTransferUnit = DRV_RTDMAC_TRANS_UNIT_4BYTE, // Assuming DRV_RTDMAC_TRANS_UNIT_4BYTE is defined
    .mResource = DRV_RTDMAC_MEMORY, // Assuming DRV_RTDMAC_MEMORY is defined
    .mLowSpeed = DRV_RTDMAC_SPEED_NORMAL, // Assuming DRV_RTDMAC_SPEED_NORMAL is defined
    .mPrioLevel = 0
};

rDmacDescMemCfg_t  desc_mem[] = {
    {.SAR = 0x64000000, .DAR =  0x74000000, .TCR =  4, .CHCR =  0},
    {       0x64100000,         0x74100000,         4,          0},
    {       0x64200000,         0x74200000,         4,          0},
    {       0x64300000,         0x74300000,         4,          0},
};

/* Define configure the DMA descriptor */
rDmacDescCfg_t descCfg1 =
{
    .mDescBaseAddr = (uintptr_t)desc_mem,
    .mDescUpdate = {
        .mCHCRUpdate = false,
        .mDestAddrUpdate = true,
        .mSrcAddrUpdate = true,
        .mTransCountUpdate = true,
    },
    .mDescRead1st = true,
    .mStateEndEnable = false,
    .mDescCount = 4,
    .mDescIndex = 0
};

/*------------------------- Configure mem-to-mem with Descriptor Repeat mode ----------------------------------*/

rDmacDescMemCfg_t  desc_mem2[] = {
    {.SAR = 0x65000000, .DAR =  0x75000000, .TCR =  4, .CHCR =  0},
    {       0x65000000,         0x75100000,         4,          0},
    {       0x65000000,         0x75200000,         4,          0},
    {       0x65000000,         0x75300000,         4,          0},
};

/* Define configure DMA Controller */
rDmacCfg_t cfg2 =
{
    .mSrcAddr = 0,                     // Source address
    .mDestAddr = 0,                    // Destination address
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
    .mDescBaseAddr = (uintptr_t)desc_mem2,                // Descriptor base address
    .mDescUpdate = {                            // No descriptor update
        .mCHCRUpdate = false,
        .mDestAddrUpdate = true,
        .mSrcAddrUpdate = true,
        .mTransCountUpdate = true,
    },
    .mDescRead1st = true,                       // Read descriptor first
    .mStateEndEnable = false,                    // Trigger state end
    .mDescCount = 4,                            // Descriptor count
    .mDescIndex = 0                             // Descriptor index
};

#define REPEAT_NUMBER           4

/*-----------------------------------------------------------*/

rDmacIrqCfg_t rDmacIrqHandler_t_irq =
{
    .Unit = SYS_DMAC3,
    .SubCh = DMAC_CH1,
    .irq_channel = INTID_SYSDMA3_CH1,
};

rDmacIrqCfg_t rDmacIrqHandler_t_irq1 =
{
    .Unit = SYS_DMAC2,
    .SubCh = DMAC_CH2,
    .irq_channel = INTID_SYSDMA2_CH2,
};

rDmacIrqCfg_t rDmacIrqHandler_t_irq2 =
{
    .Unit = SYS_DMAC1,
    .SubCh = DMAC_CH1,
    .irq_channel = INTID_SYSDMA1_CH1,
};

/*-----------------------------------------------------------*/

int main( void )
{
        /* Configure the hardware ready to run the demo. */
        prvSetupHardware();

        xSemaphore = xSemaphoreCreateBinary();
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
    (void)pfcInitModules(getModuleConfigs());
}

static void prvDMACTask( void *pvParameters )
{
    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;
    int ret,i;

    Context_t usr_context =
    {
        .ctx = &rDmacIrqHandler_t_irq,
    };

    Context_t usr_context1 =
    {
        .ctx = &rDmacIrqHandler_t_irq1,
    };

    Context_t usr_context2 =
    {
        .ctx = &rDmacIrqHandler_t_irq2,
    };

    /* Get memory region first */
    st_memory_t region = R_UTILS_GetMemoryRegionInfo(OSAL, 0);
    cfg0.mSrcAddr = region.base_address;
    cfg0.mDestAddr = region.base_address + DESTINATION_OFFSET;
    if (cfg0.mDestAddr > region.base_address + region.size) 
    {
        printf("Failed: Destination address 0x%08X exceeds memory region (end at 0x%08X)\n", cfg0.mDestAddr, region.base_address + region.size);
        for( ;; )
        {
            vTaskDelay(3000);
        }
    }

    printf("**********************************************************\r\n");
    printf("*  TC1: SYS-DMAC mem-to-mem transfer in Normal mode       *\r\n");
    /* Device Driver Part */
    R_SYSDMAC_RcarDmacCtrlInit( rDmacIrqHandler_t_irq.Unit, DRV_RTDMAC_PRIO_FIX);

    *(volatile uint32_t*)cfg0.mDestAddr = 0x9;
    printf("Value at DestAddr before DMA: 0x%x \n", *(volatile uint32_t*)cfg0.mDestAddr);

    *(volatile uint32_t*)cfg0.mSrcAddr = 0x3;
    printf("Value at SrcAddr: 0x%x \n",*(volatile uint32_t*)cfg0.mSrcAddr);

    ret = R_SYSDMAC_RcarCallBackSet(&rDmacIrqHandler_t_irq, dmacUserCallback, &usr_context);
    if (ret)
        printf("CallbackSet Failed: ret = %d\n", ret);

    int dmaStatus =R_SYSDMAC_RcarDmacExec(rDmacIrqHandler_t_irq.Unit, rDmacIrqHandler_t_irq.SubCh, &cfg0, NULL);

    for (i=0; i<10000; i++)
	{}

    // Check DMA execution status
    if (dmaStatus != 0)
        printf("DMA execution failed with status: %d\n", dmaStatus);

    // Verify destination data
    uint32_t total_transfer_size = 4;
    uint32_t destData = R_UTILS_ReadMemForDMA((void*)cfg0.mDestAddr, total_transfer_size);
    printf("Value at DestAddr after DMA: 0x%x \n", destData);

    if( destData == (*(volatile uint32_t*)cfg0.mSrcAddr) )
    {
        printf("TC1 Result: Passed\n");
    }
    else
    {
        printf("TC1 Result: Failed\n");
    }

    R_SYSDMAC_RcarDmacStop(rDmacIrqHandler_t_irq.Unit, rDmacIrqHandler_t_irq.SubCh);


    printf("*************************************************************\r\n");
    printf("*TC2: SYS-DMAC mem-to-mem transfer in Descriptor Normal mode*\r\n");
    /* Device Driver Part */
    R_SYSDMAC_RcarDmacCtrlInit(rDmacIrqHandler_t_irq1.Unit, DRV_RTDMAC_PRIO_FIX);

    for(int i = 0; i < descCfg1.mDescCount ; i++)
    {
        *(volatile uint32_t*)desc_mem[i].DAR = 0x33333333;
        *(volatile uint32_t*)desc_mem[i].SAR = 0x11111111;

        printf("Before dma: Value src desc %d at %x : 0x%x \n", i, desc_mem[i].SAR, *(volatile uint32_t*)desc_mem[i].SAR);
        printf("Before dma: Value dst desc %d at %x : 0x%x \n", i, desc_mem[i].DAR, *(volatile uint32_t*)desc_mem[i].DAR);
    }

    
    ret = R_SYSDMAC_RcarCallBackSet(&rDmacIrqHandler_t_irq1, dmacUserCallback1, &usr_context1);
    if (ret)
        printf("CallbackSet Failed: ret = %d\n", ret);

    dmaStatus =R_SYSDMAC_RcarDmacExec(rDmacIrqHandler_t_irq1.Unit, rDmacIrqHandler_t_irq1.SubCh, &cfg1, &descCfg1);


    // Check DMA execution status
    if (dmaStatus != 0)
        printf("DMA execution failed with status: %d\n", dmaStatus);

    if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
    {

    }
    // Verify destination data
    total_transfer_size = 4;
    printf("Verify destination data\n");
    ret = 0;
    for(int i = 0; i < descCfg1.mDescCount ; i++)
    {
        for(int j = 0; j < desc_mem[i].TCR ; j++)
        {
            destData = R_UTILS_ReadMemForDMA((void*)(desc_mem[i].DAR + 4*j), total_transfer_size);
            printf("After dma: Value dst desc %d at %x : 0x%x \n", i, (desc_mem[i].DAR + 4*j), destData);
            if(destData != (*(volatile uint32_t*)(desc_mem[i].SAR)))
            {
                ret = -1;
            }
        }
        
    }

    if(ret == 0)
    {
        printf("TC2 Result: Passed\n");
    }
    else
    {
        printf("TC2 Result: Failed\n");
    }

    R_SYSDMAC_RcarDmacStop(rDmacIrqHandler_t_irq1.Unit, rDmacIrqHandler_t_irq1.SubCh);
    printf("*************************************************************\r\n");

    printf("*TC3: SYS-DMAC mem-to-mem transfer in Descriptor Repeat mode*\r\n");
    /* Device Driver Part */
    R_SYSDMAC_RcarDmacCtrlInit(rDmacIrqHandler_t_irq2.Unit, DRV_RTDMAC_PRIO_FIX);

    *(volatile uint32_t*)desc_mem2[0].SAR = 0x11111111;
    printf("Before dma: Value src desc at %x : 0x%x \n", desc_mem2[0].SAR, *(volatile uint32_t*)desc_mem2[0].SAR);

    ret = R_SYSDMAC_RcarCallBackSet(&rDmacIrqHandler_t_irq2, dmacUserCallback2, &usr_context2);
    if (ret)
        printf("CallbackSet Failed: ret = %d\n", ret);

    dmaStatus =R_SYSDMAC_RcarDmacExec(rDmacIrqHandler_t_irq2.Unit, rDmacIrqHandler_t_irq2.SubCh, &cfg2, &descCfg2);


    // Check DMA execution status
    if (dmaStatus != 0)
        printf("DMA execution failed with status: %d\n", dmaStatus);

    if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
    {

    }

    total_transfer_size = 4;
    uint32_t descadd;
    printf("Verify destination data\n");
    ret = 0;
    for (int  repeat = 0; repeat < REPEAT_NUMBER; repeat++)
    {
        for(int i = 0; i < descCfg1.mDescCount ; i++)
        {
            for(int j = 0; j < desc_mem2[0].TCR ; j++)
            {
                descadd = (desc_mem2[0].SAR + 0x10000000 + 0x01000000*repeat + 0x00100000*i + 4*j);
                destData = R_UTILS_ReadMemForDMA((void*)descadd, total_transfer_size);
                printf("After dma: Value dst desc %d at %x : 0x%x \n", i, descadd, destData);
                if(destData != (*(volatile uint32_t*)(desc_mem2[0].SAR)))
                {
                    ret = -1;
                }
            }
            
        }
    }
    
    

    if(ret == 0)
    {
        printf("TC3 Result: Passed\n");
    }
    else
    {
        printf("TC3 Result: Failed\n");
    }

    R_SYSDMAC_RcarDmacStop(rDmacIrqHandler_t_irq2.Unit, rDmacIrqHandler_t_irq2.SubCh);

    printf("*************************************************************\r\n");

    for( ;; )
    {
    }
}

/*-----------------------------------------------------------*/

void dmacUserCallback(void *data) {
	rDmacIrqCfg_t * instance_ctrl = (rDmacIrqCfg_t *) data;
}

void dmacUserCallback1(void *data) {
    rDmacIrqCfg_t * instance_ctrl = (rDmacIrqCfg_t *) data;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void dmacUserCallback2(void *data) {
    rDmacIrqCfg_t * instance_ctrl = (rDmacIrqCfg_t *) data;
    static uint32_t repeat = REPEAT_NUMBER;

    if(repeat == 0)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    for(int i = 0; i < descCfg2.mDescCount; i++)
    {
        desc_mem2[i].DAR += 0x00100000;
    }
    repeat -= 1;
    
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
