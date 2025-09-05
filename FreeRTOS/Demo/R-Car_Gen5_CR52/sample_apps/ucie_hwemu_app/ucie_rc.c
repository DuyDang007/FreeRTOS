/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
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
#include "smmu/smmu.h"
#include "ucie_teg/uciedrv.h"

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

#define DRAM_DBSC01_ADDR_PA     (0xB0000000)

#define DBSC01_HDMA_PA(n)       (DRAM_DBSC01_ADDR_PA + (n) * DMA_SIZE_PER_CHAN)

#define MASK 0x00000FFF
// For RC write EP read test data
const struct hdma_info_ex hdma_tbl_wrtest_dt[] = {	// UCIE1 WRCHx1
	/*	ucieCh	dmaCh	sar					dar					llp			rw	size		        tc	weight	*/
	{	1,		0,		DBSC01_HDMA_PA(0), 	DBSC01_HDMA_PA(1),	0x7FF00000,	0,	DMA_SIZE_PER_CHAN,	0,	0x00 },
	{	0,		0,		0x0,				0x0,				0x0,		0,	0,			        0,	0    },	// End Of Table
};

const uint32_t TEST_DATA0[8U] = {0x12345678, 0xFEDCBA98, 0xA5A5A5A5, 0x5A5A5A5A,
						   0x11223344, 0x55667788, 0xAABBCCDD, 0xEEEEFFFF};

/*-----------------------------------------------------------*/

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void ucie_comm_task( void *pvParameters );
extern int console_getc(unsigned char *p_char);
/*-----------------------------------------------------------*/

void create_test_data(uint64_t src, const uint32_t *pattern, uint32_t size) {
    for (uint32_t i = 0; i < size/32; i++) {
        *((volatile uint32_t *)(uintptr_t)src + 0) = pattern[0];
        *((volatile uint32_t *)(uintptr_t)src + 1) = pattern[1];
        *((volatile uint32_t *)(uintptr_t)src + 2) = pattern[2];
        *((volatile uint32_t *)(uintptr_t)src + 3) = pattern[3];
        *((volatile uint32_t *)(uintptr_t)src + 4) = pattern[4];
        *((volatile uint32_t *)(uintptr_t)src + 5) = pattern[5];
        *((volatile uint32_t *)(uintptr_t)src + 6) = pattern[6];
        *((volatile uint32_t *)(uintptr_t)src + 7) = pattern[7];

        src = src + 32;
    }
}

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

/*******************************************************
 * NAME		: ucie_xfer
 * FUNCTION	: Root Complex UCIE HDMA transfer test
 * INPUT	: hdma_info_ex *hdma_tbl
 * OUTPUT	: none
 * RETURN	: none
 ******************************************************/
void ucie_xfer(const struct hdma_info_ex *hdma_tbl)
{
    uint32_t index;

    /* Create Linked List and Set HDMA Registers */
    printf("UCIE setting HDMA Registers\n");
    index = 0;
    while (hdma_tbl[index].llp != 0x0)
    {
	    Ucie_hdma_SetRegs(hdma_tbl[index].ucieCh, hdma_tbl[index].dmaCh, hdma_tbl[index].rw, hdma_tbl[index].sar, hdma_tbl[index].dar, hdma_tbl[index].size); 
        index++;
    }

    /* Start All CH Transfer */
    printf("UCIE HDMA STARTS\n");
    index = 0;
    while (hdma_tbl[index].llp != 0x0)
    {
	    Ucie_hdma_start(hdma_tbl[index].ucieCh, hdma_tbl[index].dmaCh, hdma_tbl[index].rw);
	    index++;
    }

    /* Wait All CH STOP */
    printf("UCIE wait for HDMA transfer\n");
    index = 0;
    while (hdma_tbl[index].llp != 0x0)
    {
        if (NORMAL_END != Ucie_hdma_waitStop(hdma_tbl[index].ucieCh, hdma_tbl[index].dmaCh, hdma_tbl[index].rw))
        {
            printf("Transfer timeout on ch%d, dma%d\n", hdma_tbl[index].ucieCh, hdma_tbl[index].dmaCh);
        }
	    index++;
    }

    /* Stop All CH */
    printf("UCIE HDMA transfer STOPS\n");
    index = 0;
    while (hdma_tbl[index].llp != 0x0)
    {
	    Ucie_hdma_stop(hdma_tbl[index].ucieCh, hdma_tbl[index].dmaCh, hdma_tbl[index].rw);
        index++;
    }
}

/*-----------------------------------------------------------*/

static void ucie_comm_task(void *pvParameters)
{
    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    uint32_t ret = NORMAL_END;
    uint32_t val;

    uint8_t uice_chn = UCIE_CH1;

    vTaskDelay(pdMS_TO_TICKS(1));
    printf("[X5H TEG] RC SETUP START\n");
    /* UCIe_chan = 1 - Root-Complex */
    ret = Ucie_Setup_rc(uice_chn);
    if (ret != NORMAL_END)
    {
        printf("[X5H TEG] RC FAILED\n");
	    goto LABEL_ERROR;
    }
    printf("[X5H TEG] RC SETUP DONE \n");

    printf("\nUCIE generate RC sar data - START \n");
    create_test_data(hdma_tbl_wrtest_dt->sar, TEST_DATA0, hdma_tbl_wrtest_dt->size); 
    print_test_data(hdma_tbl_wrtest_dt->sar, hdma_tbl_wrtest_dt->size); 

    printf("\nProgram pending. Press any key to start tranfer data\n");
    uint8_t p_char;
    while (console_getc(&p_char)) {
        __asm__ volatile("nop");
    }

    printf("Starting UCIe RC HDMA WRCHx1\n");
    ucie_xfer(hdma_tbl_wrtest_dt);
    printf("UCIE RC HDMA transfer done\n");

    printf("[X5H TEG] UCIE CR:Setup PIO mode\n");
    printf("[X5H TEG] UCIE CR:Setup SMMU\n");

    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x000004, 0x00110007); // ???
    mem_read32(UCIE_AXI_BASE(uice_chn) + 0x000004);

    bool is_secure = true;

    uint32_t streamId[] = {
		0x00000,
		0x00C00,
    };

    st_smmu_streamid_instance_ctrl_t smmu_ctrl = {
        .smmu_domain = SMMU_RT,
	.is_secure = is_secure,
    };
    uint64_t  ucie1_mem = 0x24000000000;  // D2D (UCIe ch1/no coherent (CXL))
    uint32_t  ucie1_tgt = 0x90000000;
    printf("**********************************************\r\n");

    printf("* SMMU-RT Cortex-R52 Cluster0 core0 *\r\n");

    R_SMMU_Init(SMMU_RT, is_secure);
    R_SMMU_InvalidateTLB(SMMU_RT, is_secure);

    for (uint8_t i = 0; i < sizeof(streamId)/sizeof(uint32_t); i ++) {
        smmu_ctrl.stream_id = streamId[i];

        ret = R_SMMU_Attach(&smmu_ctrl);
        if (ret == 0) {
            printf("Attach stream id 0x%x result: Passed\r\n", streamId[i]);
        } else {
            printf("Attach stream id 0x%x result: Failed\r\n", streamId[i]);
        }

        R_SMMU_Map(&smmu_ctrl, 0x00, 0x00, 0x60000000);
        R_SMMU_Map(&smmu_ctrl, 0xC0000000, 0xC0000000, 0x40000000);
        R_SMMU_Map(&smmu_ctrl, 0x90000000, ucie1_mem, 0x1000000);
        R_SMMU_Map(&smmu_ctrl, 0x70000000, ucie1_mem, 0x1000000);
        // R_SMMU_Map(&smmu_ctrl, 0x90000000, 0x0000000, 0x1000000);
    }

    printf("**********************************************\r\n");

    printf("* Test case 5: Disable SMMU bypass mode Cluster0 core0 *\r\n");

    volatile uint32_t *RCTBUBYPSEN = (volatile uint32_t *)0x18B47800;
    uint32_t smmu_bypass = 0xFFE;
    uint32_t old = *RCTBUBYPSEN;
    uint32_t new = (old & ~MASK) | (smmu_bypass & MASK);
    *RCTBUBYPSEN = new;

    if ((*RCTBUBYPSEN & MASK) == (smmu_bypass & MASK))
        printf("Disable Successfully\r\n");
    else
        printf("Disable Failed\r\n");


    R_SMMU_Enable(SMMU_RT, is_secure);

    printf("**********************************************\r\n");
    printf("[X5H TEG] UCIE CR:Setup SMMU done\n");

    printf("\nProgram pending. Press any key to start tranfer data for pio mode\n");
    while (console_getc(&p_char)) {
        __asm__ volatile("nop");
    }

    printf("Starting UCIe RC PIO\n");
    *(uint32_t *)ucie1_tgt = 0x1234567;
    vTaskDelay(10);
    printf("vaulue at VA 0x90000000: 0x%x \n", *(uint32_t *)0x90000000 );
    printf("vaulue at VA 0x70000000: 0x%x \n", *(uint32_t *)0x70000000 );

    printf("write: 0x%08x\n", *(uint32_t *)ucie1_tgt);
    vTaskDelay(5000);

    printf("[X5H TEG] RC: TEST DONE\n");



LABEL_ERROR:

    for(;;) {
        printf("\nLoop infinity");
        vTaskDelay(5000);
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
