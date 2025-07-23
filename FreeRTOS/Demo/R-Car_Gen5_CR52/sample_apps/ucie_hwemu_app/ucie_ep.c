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

#if 0
#include "ucie_reg.h"
#include "ucie_drv.h"
#include "ucie_drv_port.h"
#endif

/*-----------------------------------------------------------*/
// UCIe HDMA test cases
/*-----------------------------------------------------------*/
// #define RCAR_UCIE_4HDMA_8DBSC
// #define RCAR_UCIE_2HDMA_4DBSC
// #define RCAR_UCIE_2HDMA_2DBSC
#define RCAR_UCIE_1HDMA_1DBSC

/*-----------------------------------------------------------*/
// UCIe RT-DMA test cases
/*-----------------------------------------------------------*/
// #define RCAR_UCIE_1RTDMA_1DBSC
#ifdef RCAR_UCIE_1RTDMA_1DBSC
// RT-DMAC drv
#include "ucie_dmac.h"
#include "reg_x5h_dmac.h"
#endif
/*-----------------------------------------------------------*/

#define main_ucie_TASK_PRIORITY        (tskIDLE_PRIORITY + 1)

/* Memory size macros */
#define SIZE_64MB           (0x4000000)
#define SIZE_32MB           (0x2000000)
#define SIZE_16MB           (0x1000000)
#define SIZE_8MB            (0x800000)
#define SIZE_1MB            (0x100000)
#define SIZE_256KB          (0x40000)
#define SIZE_1KB            (0x400)
#define DMA_SIZE_PER_CHAN	(SIZE_8MB)

/*
In current configuration, all of the dbsc channel is binded to the dedicated address range:
in the case of 4x4k split (4ch interleaving):
    0x0000_0000_0000 ~ 0x0000_FFFF_FFFF：DBSC0/1/2/3   <--- Used by ISP/IMR/CR
    0x0010_0000_0000 ~ 0x0017_FFFF_FFFF : DBSC0/1/2/3
    0x0018_0000_0000 ~ 0x001F_FFFF_FFFF : DBSC 4/5/6/7    <--- Used by CMN/CA

in the case of 2x2k split (2ch interleaving):
    0x0000_0000_0000 ~ 0x0000_FFFF_FFFF：DBSC0/1   <--- Used by ISP/IMR/CR
    0x0010_0000_0000 ~ 0x0013_FFFF_FFFF : DBSC0/1
    0x0014_0000_0000 ~ 0x0017_FFFF_FFFF : DBSC2/3
    0x0018_0000_0000 ~ 0x001B_FFFF_FFFF : DBSC4/5   <--- Used by CMN/CA
    0x001C_0000_0000 ~ 0x001F_FFFF_FFFF : DBSC6/7

in the case of Linear mode:
    0x0000_0000_0000 ~ 0x0000_FFFF_FFFF：DBSC0   <--- Used by ISP/IMR/CR
    0x0010_0000_0000 ~ 0x0011_FFFF_FFFF : DBSC0
    0x0012_0000_0000 ~ 0x0013_FFFF_FFFF : DBSC1
    0x0014_0000_0000 ~ 0x0015_FFFF_FFFF : DBSC2
    0x0016_0000_0000 ~ 0x0017_FFFF_FFFF : DBSC3
    0x0018_0000_0000 ~ 0x0019_FFFF_FFFF : DBSC4   <--- Used by CMN/CA
    0x001A_0000_0000 ~ 0x001B_FFFF_FFFF : DBSC5
    0x001C_0000_0000 ~ 0x001D_FFFF_FFFF : DBSC6
    0x001E_0000_0000 ~ 0x001F_FFFF_FFFF : DBSC7
*/
#define DRAM_DBSC01_ADDR    (0xB0000000)    // DRAM area for DBSC0/1 - Local SRAM-like region
#define DRAM_DBSC23_ADDR    (0x145A000000)  // DRAM area for DBSC2/3
#define DRAM_DBSC45_ADDR    (0x185A000000)  // DRAM area for DBSC4/5
#define DRAM_DBSC67_ADDR    (0x1C5A000000)  // DRAM area for DBSC6/7

#define DBSC01_HDMA(n)      (DRAM_DBSC01_ADDR + (n) * DMA_SIZE_PER_CHAN)
#define DBSC23_HDMA(n)      (DRAM_DBSC23_ADDR + (n) * DMA_SIZE_PER_CHAN)
#define DBSC45_HDMA(n)      (DRAM_DBSC45_ADDR + (n) * DMA_SIZE_PER_CHAN)
#define DBSC67_HDMA(n)      (DRAM_DBSC67_ADDR + (n) * DMA_SIZE_PER_CHAN)

#define CHUNK_SIZE_BYTES    SIZE_1MB
#define WORDS_PER_CHUNK     (CHUNK_SIZE_BYTES / sizeof(uint32_t))

#ifdef RCAR_UCIE_1RTDMA_1DBSC
const static struct dmac_info_ex rtdmac_tbl_rd[] = {
	/*	instance	chNo	sar				dar				dmsm */
	// {	0,			0,		0x1858000000,	0x20000000000,	0x4	},	// Dst Increment, Src Fix
	{	0,			0,		0x20000000000,	DBSC01_HDMA(2),	0x5	},	// Dst,Src Increment
	{	0,			0,		0x0,			0x0,			0	},	// End Of Table
};
#endif /* #ifdef RCAR_UCIE_1RTDMA_1DBSC */

const uint32_t TEST_DATA_RC[] = {
    0x11111111, 0xFEDCBA98, 0xA5A5A5A5, 0x5A5A5A5A, 0x11223344, 0x55667788, 0xAABBCCDD, 0xEEFF0011,
    // 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777, 0x88888888,
    // 0xCAFED00D, 0xBADC0FFE, 0xABADBABE, 0x1234ABCD, 0xABCDEFAB, 0x89ABCDEF, 0x76543210, 0xABADBEEF,
    // 0x00110011, 0x00220022, 0x00330033, 0x00440044, 0x00550055, 0x00660066, 0x00770077, 0x00880088,
};

const uint32_t TEST_DATA_EP[] = {
    // 0x12345678, 0xFEDCBA98, 0xA5A5A5A5, 0x5A5A5A5A, 0x11223344, 0x55667788, 0xAABBCCDD, 0xEEFF0011,
    // 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777, 0x88888888,
    0x12345678, 0xBADC0FFE, 0xABADBABE, 0x1234ABCD, 0xABCDEFAB, 0x89ABCDEF, 0x76543210, 0xABADBEEF,
    // 0x00110011, 0x00220022, 0x00330033, 0x00440044, 0x00550055, 0x00660066, 0x00770077, 0x00880088,
};

const uint32_t START_PATTERN = 0xDEADBEEF;
const uint32_t END_PATTERN = 0xBEEFDEAD;

/*-----------------------------------------------------------*/

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void ucie_comm_task( void *pvParameters );

/*-----------------------------------------------------------*/

/*
 * Processes DMA_SIZE_PER_CHAN in total
 * Processes memory in 1MB chunks
 * Processes memory less than 1MB chunks
 * Writes 8 words (32 bytes) at a time using an unrolled loop
 * Finishes any remaining words in the chunk with a modulo-based write
 */
static void memset_fast32(uint32_t dest, const uint32_t *pattern, uint32_t size_bytes)
{
    const uint32_t pattern_size = 8;
    uint32_t total_words        = size_bytes / sizeof(uint32_t);
    uint32_t chunks             = total_words / WORDS_PER_CHUNK;
    uint32_t remaining_words    = total_words % WORDS_PER_CHUNK;
    volatile uint32_t *buffer   = (volatile uint32_t *)(uintptr_t)dest;

    // Process full chunks, for large size
    if (chunks > 1)
    {
        for (uint32_t c = 0; c < chunks; ++c) {
            volatile uint32_t *chunk_base = buffer + c * WORDS_PER_CHUNK;
            uint32_t i = 0;

            // Unrolled loop to write 8 words at a time
            for (; i + 7 < WORDS_PER_CHUNK; i += 8) {
                chunk_base[i + 0] = pattern[0];
                chunk_base[i + 1] = pattern[1];
                chunk_base[i + 2] = pattern[2];
                chunk_base[i + 3] = pattern[3];
                chunk_base[i + 4] = pattern[4];
                chunk_base[i + 5] = pattern[5];
                chunk_base[i + 6] = pattern[6];
                chunk_base[i + 7] = pattern[7];
            }

            // Write remaining elements
            // All elements handled in unrolled loop (WORDS_PER_CHUNK is divisible by 8)
            for (; i < WORDS_PER_CHUNK; ++i) {
                chunk_base[i] = pattern[i % pattern_size];
            }
        }

        /*
        * Process remaining words outside full chunks
        * the size_bytes is exactly divisible by CHUNK_SIZE_BYTES
        * There will be no leftover words
        */
        volatile uint32_t *tail = buffer + chunks * WORDS_PER_CHUNK;
        for (uint32_t i = 0; i < remaining_words; ++i) {
            tail[i] = pattern[i % pattern_size];
        }
    } // end of if
    else // for small size, less than 1MB
    {
        uint32_t i = 0;
        // Unrolled loop to write 8 words at a time
        for (; i + 7 < total_words; i += 8) {
            buffer[i + 0] = pattern[0];
            buffer[i + 1] = pattern[1];
            buffer[i + 2] = pattern[2];
            buffer[i + 3] = pattern[3];
            buffer[i + 4] = pattern[4];
            buffer[i + 5] = pattern[5];
            buffer[i + 6] = pattern[6];
            buffer[i + 7] = pattern[7];
        }

        // Handle remaining words after unrolled loop
        for (; i < total_words; ++i) {
            buffer[i] = pattern[i % pattern_size];
        }
    } // end of else

    /* Place START_PATTERN and END_PATTERN for each 1MB block
     * 0x3FFFF, 0x7FFFF, 0xFFFFF, 0x13FFFF, 0x17FFFF, 0x1BFFFF, 0x1FFFFF */
    buffer[0] = START_PATTERN;
    for (size_t i = 1; i <= chunks; i++)
    {
        buffer[i * WORDS_PER_CHUNK - 1] = END_PATTERN;
    }
}

uint32_t verify_end_pattern(const uint32_t received_buffer, uint32_t size_bytes)
{
    uint32_t ret        = NORMAL_END;
    uint32_t total_mb   = size_bytes / SIZE_1MB;
    uint32_t *buffer    = (uint32_t *)(uintptr_t)received_buffer;
    uint32_t index = 0;

    // Check START_PATTERN at beginning of buffer
    if (buffer[0] != START_PATTERN)
    {
        // Data is not ready, return 
        printf("received_buffer[0]: 0x%08x\n", buffer[0]);
        ret = ERROR_END;
    }

    // Check END_PATTERN at the end of each 1MB block
    if (ret != ERROR_END)
    {
        for (uint32_t i = 1; i <= total_mb; i++)
        {
            index = i * WORDS_PER_CHUNK - 1;  // last word of each 1MB
            if (buffer[index] != END_PATTERN)
            {
                printf("[ERR] END_PATTERN mismatch at MB %u: index 0x%X = 0x%08X\n",
                    i, index, buffer[index]);
                ret = ERROR_END;
            }
            else
            {
                printf("[OK ] MB %u: END_PATTERN found at index 0x%X\n", i, index);
            }
        }
    }

    return ret;
}

static uint32_t compare_buffer(const uint32_t dest, const uint32_t src, uint32_t size_bytes)
{
    uint32_t ret = NORMAL_END;

    uint32_t *buf1 = (uint32_t *)(uintptr_t)dest;
    uint32_t *buf2 = (uint32_t *)(uintptr_t)src;
    uint32_t count = size_bytes / sizeof(uint32_t);
    uint32_t error_count = 0;

    printf("Buffer at 0x%08X vs 0x%08X ", dest, src);

    for (uint32_t i = 0; i < count; i++)
    {
        if (buf1[i] != buf2[i])
        {
            // printf("[ERR] index %u: dest=0x%08X: src=0x%08X\n", i, buf1[i], buf2[i]);
            error_count++;
            ret |= ERROR_END;
        }

        // if (error_count == 3) break;
    }

    if (error_count == 0)
    {
        printf("[INF] VERIFY OKAY \n");
    }
    else
    {
        printf("[ERR] VERIFY FAILED: %u mismatches\n", error_count);
        // printf("[ERR] VERIFY FAILED \n");
    }

    return ret;
}

static void print_buffer(uint32_t addr, uint32_t size_bytes)
{
    printf("Buffer at 0x%08X: ", addr);

    uint32_t *buffer = (uint32_t *)(uintptr_t)addr;
    uint32_t count = size_bytes / sizeof(uint32_t);
    uint32_t i = 0;

    for (i = 0; i < count; i++)
    {
        if (i % 8 == 0)
        {
            printf("\n0x%08X: ", addr + i * 4);
        }
        printf("%08X ", buffer[i]);
    }
    printf("\n");
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
    uint64_t ucie0_mem;
    uint32_t ucie1_tgt;
    uint32_t timeout;

    uint8_t uice_chn = UCIE_CH1;

    // uint32_t *w;
	// w = (uint32_t *)(0xbffff000);

	// ucie_hwemu_hw_ai_acc_init();
	// printf("[AI-ACC] EP mdlc_ms_init(0)\n");

	// // Wait and notify RC to proceed
	// vTaskDelay(w[0]); // delay 10ms
    // vTaskDelay(w[1]); // delay 2ms
    // vTaskDelay(w[2]); // slot
    // vTaskDelay(w[3]); // slot

    printf("[X5H TEG] EP SETUP START\n");

    /* ucie_chan = 0 - Endpoint */
    bool is_rootport = false;
    ret = Ucie_Setup_ep(uice_chn);
    // ret = ucie_hwemu_hw_init_ep(uice_chn, MODE_EP, UCIE_CH_EP_WITH);
    if (ret == ERROR_END)
    {
       printf("[X5H TEG] EP FAILED\n");
       // Still continue even if init might timeout in EP mode
    }
    printf("[X5H TEG] EP SETUP DONE \n");

    // For debugging
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
    ucie0_mem = 0x20000000000;  // D2D (UCIe ch1/no coherent (CXL))
    ucie1_tgt = 0x4E000000;     // UCIE0 INBOUND: translate from 0x20000000000 to 0x4E000000
 
	/*====================================*/
	/* Setup inbound region               */
	/*====================================*/
#if 0
    ucie_hwemu_init_inbound(0, base_addr, target_addr);
#else
   	/*** ch0 ***/
    val = mem_read32(UCIE_AXI_BASE(uice_chn) + 0x300510) | 0x00FFFFF0;		    //; IATU LIMIT_ADDR_OFF_INBOUND_2
    // val = mem_read32(UCIE_AXI_BASE(uice_chn) + 0x300510) | 0x000FFFF0;		    //; IATU LIMIT_ADDR_OFF_INBOUND_2
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300510, val);			            //; LIMIT_ADD_RW -> 16MB
    mem_read32(UCIE_AXI_BASE(uice_chn) + 0x300510);

    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300508, ucie0_mem & 0xFFFFFFFF);	//; IATU LWR_BASE_ADDR_OFF_INBOUND_2
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x30050C, ucie0_mem >> 32);		    //; IATU UPPER_BASE_ADDR_OFF_INBOUND_2
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300514, ucie1_tgt);                //; IATU LWR_TARGET_ADDR_OFF_INBOUND_2
	mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300518, 0x00000000);               //; IATU UPPER_TARGET_ADDR_OFF_INBOUND_2
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300500, 0x00000000);			    //; IATU REGION_CTRL_1_OFF_INBOUND_2
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300504, 0x80000000);			    //; IATU REGION_CTRL_2_OFF_INBOUND_2: [31] REDION_EN=1, [30] MATCH_MODE=0 (Address Match Mode)
#endif

    // for RC reading
    uint32_t *p_buf;
    p_buf = (uint32_t *)TEST_DATA_EP;
    memset_fast32((uint32_t)DBSC01_HDMA(1), p_buf, DMA_SIZE_PER_CHAN);

    // for Comparing
    p_buf = (uint32_t *)TEST_DATA_RC;
    memset_fast32((uint32_t)DBSC01_HDMA(0), p_buf, DMA_SIZE_PER_CHAN);

#ifdef RCAR_UCIE_1HDMA_1DBSC
    printf("[X5H TEG] UCIE EP: HDMA data recieve wait loop from RC - START\n");

    timeout = 0U;
	// while (mem_read32(MSI_STOP_BASE) != 0xA5A5)
	// {
	// 	wait_time(1);    // Wait for data to arrive
	// }

    while (1)
    {
        val = mem_read32(0x7FFF0000);  //MSI_STOP_BASE
        printf("MSI_STOP_BASE: 0x%x \n", val);

        if (verify_end_pattern(DBSC01_HDMA(2), DMA_SIZE_PER_CHAN) != ERROR_END)
        {
            printf("\n[X5H TEG] UCIE EP: HDMA CHECKING RESULT\n");
            ret = compare_buffer(DBSC01_HDMA(2), DBSC01_HDMA(0), DMA_SIZE_PER_CHAN);

            // Print START_PATTERN
            p_buf = (uint32_t *)(uintptr_t)DBSC01_HDMA(2);
            printf("p_buf[0]: 0x%08x\n", p_buf[0]);

            // Print END_PATTERN locations for each 1MB segment
            for (uint32_t i = 1; i <= (DMA_SIZE_PER_CHAN / SIZE_1MB); i++)
            {
                uint32_t index = i * WORDS_PER_CHUNK - 1;  // last word of each 1MB
                printf("p_buf[0x%08x]: 0x%08x\n", index, p_buf[index]);
            }
            break;
        }
        else
        {
            wait_time(1);    // Wait for data to arrive
            // timeout++;
        }

        // if (timeout >= 50u) 
        // {
        //     printf("\n[ERR] Timeout waiting for END_PATTERN\n");
        //     ret = ERROR_END;
        //     break;
        // }
    }
#endif /* #ifdef RCAR_UCIE_1HDMA_1DBSC */

#ifdef RCAR_UCIE_1RTDMA_1DBSC
    // Reading end pattern from RC buf and compare
    // ret = Rtdmac_Transfer(rtdmac_tbl_rd);
    p_buf = (uint32_t *)(uintptr_t)ucie1_tgt;
    printf("[AI-ACC] UCIE: HDMA data recieve wait loop from X5H - START\n");
    printf("\nUCIE EP CHECKING RESULT\n");
    while (1)
    {
        if (START_PATTERN == p_buf[0])
        {
            wait_time(3);   // wait for receving data from RC
            ret = compare_buffer(ucie1_tgt, DBSC01_HDMA(0), SIZE_1MB);
            printf("p_buf[0xFFF]: 0x%08x\n", p_buf[0xFFF]);
            printf("p_buf[0xFFFF]: 0x%08x\n", p_buf[0xFFFF]);
            printf("p_buf[0x1FFFF]: 0x%08x\n", p_buf[0x1FFFF]);
            printf("p_buf[0x2FFFF]: 0x%08x\n", p_buf[0x2FFFF]);
            printf("p_buf[0x3FFFF]: 0x%08x\n", p_buf[0x3FFFF]);
            printf("\n");
            break;
        }
        else
        {
            printf(". ");
            wait_time(1);   // wait for receving data from RC
        }
    }
#endif /* #ifndef RCAR_UCIE_1RTDMA_1DBSC */

    if (ret != NORMAL_END)
    {
        printf("[X5H TEG] UCIE EP: RECEIVE FAILED\n");
        // goto LABEL_ERROR;
    }
    else
    {
        printf("[X5H TEG] UCIE EP: RECEIVE PASSED\n");
    }

    while(1);
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
