/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "interrupts.h"
#include "stdio.h"
#include "string.h"
#include "ucie_teg/uciedrv.h"
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
const struct hdma_info_ex hdma_tbl_wrtest_dt[] = {  // UCIE1 WRCHx1
    /*  ucieCh  dmaCh   sar                 dar                 llp         rw  size                tc  weight  */
    {   1,      0,      DBSC01_HDMA_PA(0),  DBSC01_HDMA_PA(1),  0x7FF00000, 0,  DMA_SIZE_PER_CHAN,  0,  0x00 },
    {   0,      0,      0x0,                0x0,                0x0,        0,  0,                  0,  0    }, // End Of Table
};

const uint32_t TEST_DATA0[8U] = {0x12345678, 0xFEDCBA98, 0xA5A5A5A5, 0x5A5A5A5A,
                           0x11223344, 0x55667788, 0xAABBCCDD, 0xEEEEFFFF};

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void ucie_comm_task( void *pvParameters );

extern int console_getc(unsigned char *p_char);

void print_test_data(uint64_t src, uint32_t size) {
    printf("\nStart data at addr 0x%llX\n\t", src);
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

void verify_test_data(uint64_t src, const uint32_t *pattern, uint32_t size) {
    uint8_t ret = 0;
    for(uint32_t i = 0; i < size/4; i++) {
        if (*((volatile uint32_t*)(uintptr_t)src + i) != pattern[i%8]) {
            printf("Addr %llp. Expect data: 0x%X, Real data: 0x%X\n", (volatile uint32_t*)(uintptr_t)src + i, pattern[i%8], *((volatile uint32_t*)(uintptr_t)src + i));
            ret = 1;
            break;
        }
    }

    if (ret) {
        printf("Verify Fail\n");
    }
    else {
        printf("Verify Pass\n");
    }
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

static void ucie_comm_task(void *pvParameters)
{
    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    uint32_t ret = NORMAL_END;
    uint32_t val;
    uint32_t timeout;

    uint8_t uice_chn = UCIE_CH1;

    printf("[X5H TEG] EP SETUP START\n");
    /* ucie_chan = 1 - Endpoint */
    ret = Ucie_Setup_ep(uice_chn);
    if (ret == ERROR_END)
    {
       printf("[X5H TEG] EP FAILED\n");
    }
    else {
        printf("[X5H TEG] EP SETUP DONE \n");
    }

    mem_write32(0xD9000000 + 0x000080, 0x00000000); // ???
	mem_write32(0xD9000000 + 0x0008BC, 0x040BFF4A); // ???
	mem_write32(0xD9000000 + 0x0008BC, 0x040BFF4E); // ???

    printf("Waiting for RC tranfer data. Press any key if RC tranfer done to continue!\n");
    uint8_t p_char;
    while (console_getc(&p_char)) {
        __asm__ volatile("nop");
    }

    verify_test_data(hdma_tbl_wrtest_dt->dar, TEST_DATA0, DMA_SIZE_PER_CHAN);
    printf("EP Data after transfer:\n");
    print_test_data(hdma_tbl_wrtest_dt->dar, DMA_SIZE_PER_CHAN);
    
    printf("[X5H TEG] UCIE EP:Setup PIO mode\n");
    printf("[X5H TEG] UCIE EP:Setup inbound\n");
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x000004, 0x00110007); // ???
    mem_read32(UCIE_AXI_BASE(uice_chn) + 0x000004);
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x000080, 0x00000000); // ???
    mem_read32(UCIE_AXI_BASE(uice_chn) + 0x000080);
	mem_write32(UCIE_AXI_BASE(uice_chn) + 0x0008BC, 0x040BFF4A); // ???
    mem_read32(UCIE_AXI_BASE(uice_chn) + 0x0008BC);
	mem_write32(UCIE_AXI_BASE(uice_chn) + 0x0008BC, 0x040BFF4E); // ???
    mem_read32(UCIE_AXI_BASE(uice_chn) + 0x0008BC);

	/*====================================*/
	/* UCIE memory area                   */
	/*====================================*/
    uint64_t  ucie1_mem = 0x24000000000;  // D2D (UCIe ch1/no coherent (CXL))
    uint32_t  ucie1_tgt = 0x90000000;     // UCIE0 INBOUND: translate from 0x20000000000 to 0x4E000000
 
	/*====================================*/
	/* Setup inbound region               */
	/*====================================*/
   	/*** ch0 ***/
    val = mem_read32(UCIE_AXI_BASE(uice_chn) + 0x300510) | 0x00FFFFF0;		    //; IATU LIMIT_ADDR_OFF_INBOUND_2
    printf("val %x \v", val);
    // val = mem_read32(UCIE_AXI_BASE(uice_chn) + 0x300510) | 0x000FFFF0;		    //; IATU LIMIT_ADDR_OFF_INBOUND_2
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300510, val);			            //; LIMIT_ADD_RW -> 16MB
    mem_read32(UCIE_AXI_BASE(uice_chn) + 0x300510);

    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300508, ucie1_mem & 0xFFFFFFFF);	//; IATU LWR_BASE_ADDR_OFF_INBOUND_2
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x30050C, ucie1_mem >> 32);		    //; IATU UPPER_BASE_ADDR_OFF_INBOUND_2
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300514, ucie1_tgt);                //; IATU LWR_TARGET_ADDR_OFF_INBOUND_2
	mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300518, 0x00000000);               //; IATU UPPER_TARGET_ADDR_OFF_INBOUND_2
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300500, 0x00000000);			    //; IATU REGION_CTRL_1_OFF_INBOUND_2
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300504, 0x80000000);			    //; IATU REGION_CTRL_2_OFF_INBOUND_2: [31] REDION_EN=1, [30] MATCH_MODE=0 (Address Match Mode)
    printf("[X5H TEG] UCIE EP:Setup inbound done\n");

    printf("Waiting for RC tranfer data. Press any key if RC tranfer done to continue!\n");
    while (console_getc(&p_char)) {
        __asm__ volatile("nop");
    }

    printf("ep waiting rx transfer\n");
    vTaskDelay(2000);
    printf("read: address: 0x%x",(uint32_t *)ucie1_tgt);
    printf(" value: 0x%08x\n", *(uint32_t *)ucie1_tgt);

    printf("[X5H TEG] EP: TEST DONE\n");


    for(;;) {
        printf("\nInfinity loop");
        vTaskDelay(5000);
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
