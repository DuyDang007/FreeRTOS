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
#include "string.h"
#include "ucie/r_ucie.h"
#include "pfc/r_pfc_api.h"
#include "device_tree_x5h.h"

#define main_ucie_TASK_PRIORITY        (tskIDLE_PRIORITY + 1)

/* Memory size macros */
#define SIZE_64MB           (0x4000000)
#define SIZE_32MB           (0x2000000)
#define SIZE_16MB           (0x1000000)
#define SIZE_8MB            (0x800000)
#define SIZE_1MB            (0x100000)
#define SIZE_256KB          (0x40000)
#define SIZE_1KB            (0x400)
#define DMA_SIZE_PER_CHAN	(SIZE_64MB)
/*-----------------------------------------------------------*/

#define DRAM_DBSC01_ADDR_PA     (0xB0000000)

#define DBSC01_HDMA_PA(n)       (DRAM_DBSC01_ADDR_PA + (n) * DMA_SIZE_PER_CHAN)

// For RC write EP read test data
st_ucie_hdma_cfg_t hdma_tbl_wrtest_dt[] = {    // UCIE1 WRCHx1
    /*  ucie_ch     hdma_ch     mSrcAddr            mDestAddr           size                rw */
    {   UCIE_CH1,   HDMA_CH0,   DBSC01_HDMA_PA(0),  DBSC01_HDMA_PA(1),  DMA_SIZE_PER_CHAN,  0,  },
    {   0,          0,          0x0,                0x0,                0x0,                0,  },  // End Of Table
};

const uint32_t TEST_DATA0[8U] = {0x12345678, 0xFEDCBA98, 0xA5A5A5A5, 0x5A5A5A5A,
                           0x11223344, 0x55667788, 0xAABBCCDD, 0xEEEEFFFF};

#define PIO_RC_WRITE_DATA   0xFFCCFFCC
#define PIO_EP_WRITE_DATA   0xCCFFCCFF

/*-----------------------------------------------------------*/

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void ucie_comm_task( void *pvParameters );

extern int console_getc(unsigned char *p_char);
extern uint32_t Ucie_Setup_ep(uint8_t ch);

void print_test_data(uint64_t src, uint32_t size) {
    printf("Start data at addr 0x%llX\n\t", src);
    for (uint8_t i = 0; i < 8; i++) {
        printf("0x%X ", *((volatile uint32_t *)(uintptr_t)src + i));
    }
    printf("\n");

    if (size > 32) {
        printf("End data at addr 0x%llX\n\t", src + size - 32);
        for (uint8_t i = 0; i < 8; i++) {
            printf("0x%X ", *((volatile uint32_t *)(uintptr_t)(src + size - 32) + i));
        }
        printf("\n");
    }
}

uint32_t verify_test_data(uint64_t src, const uint32_t *pattern, uint32_t size) {
    for(uint32_t i = 0; i < size/4; i++) {
        if (*((volatile uint32_t*)(uintptr_t)src + i) != pattern[i%8]) {
            return 1;
        }
    }
    return 0;
}

int main( void )
{
    /* Configure the hardware ready to run the demo. */
    prvSetupHardware();

    xTaskCreate(ucie_comm_task, "UCIe", configMINIMAL_STACK_SIZE, NULL, main_ucie_TASK_PRIORITY, NULL );
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

extern void mem_write32(uintptr_t addr, uint32_t data);
extern uint32_t mem_read32(uintptr_t addr);

static void ucie_comm_task(void *pvParameters)
{
    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    uint32_t ret = 0;
    uint32_t val;
    uint32_t timeout;

    /* ucie_chan = 1 - Endpoint */
    printf("<----- [EP] TC1: UCIE LINKUP ----->\n");
    
    ret = R_UCIE_Setup(UCIE_CH1, UCIE_MODE_EP, LINKSPEED_16GTPS);
    if (ret == LINKUP_TIMEOUT) {
        printf("The first time linkup timeout. Retry 30 times\n");
        ret = R_UCIE_Retry_Linkup(UCIE_CH1, UCIE_MODE_EP, LINKSPEED_16GTPS, 30);
    }

    if (ret) {
        printf("Result: FAILED\n");
    }
    else {
        printf("Result: PASSED\n");
    }

    printf("<----- [EP] TC 2: VERIFY HDMA TRANSFER DATA ----->\n");
    
    timeout = 0x1000000;
    while (verify_test_data(hdma_tbl_wrtest_dt->mDestAddr, TEST_DATA0, DMA_SIZE_PER_CHAN)) {
        timeout--;
    }
    printf("Data after transfer:\n");
    print_test_data(hdma_tbl_wrtest_dt->mDestAddr, DMA_SIZE_PER_CHAN);
    
    if (timeout) {
        printf("Result: PASSED\n");
    }
    else {
        printf("Result: FAILED\n");
    }

    printf("<----- [EP] END TEST ----->\n");

    for(;;) {
        __asm__ volatile("nop");
    }
}

/*-----------------------------------------------------------*/

/* Raw printf to avoid using FreeRTOS heap/locking in asserts */
int printf_raw(const char *format, ...);

/* Assertion failure handler */
void vMainAssertCalled( const char *pcFileName, uint32_t ulLineNumber )
{
    /* Don't use printf as it uses FreeRTOS resources */
    printf_raw("ASSERT!  Line %d of file %s\n", ulLineNumber, pcFileName);
    taskENTER_CRITICAL();
    for( ;; );
}

/* Utility to delete the calling task */
void vDeleteCallingTask( void )
{
     vTaskDelete( NULL );
}
