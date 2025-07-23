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

// UCIe drv
#if 0
#include "ucie_reg.h"
#include "ucie_drv.h"
#include "ucie_drv_port.h"
#endif
#include "ucie_teg/uciedrv.h"

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

#ifdef RCAR_UCIE_4HDMA_8DBSC
// For write test data
const struct hdma_info_ex hdma_tbl_wrtest_dt[] = {	// UCIE0 WRCHx2
	/*	ucieCh	dmaCh	sar					dar					llp			rw	size		        tc	weight	*/
	{	0,		0,		DBSC01_HDMA(0),		DBSC23_HDMA(0),		0x7FF00000,	0,	DMA_SIZE_PER_CHAN,	0,	0x00 },
	{	0,		1,		DBSC01_HDMA(1),		DBSC23_HDMA(1),		0x7FF01000,	0,	DMA_SIZE_PER_CHAN,	0,	0x01 },
	{	0,		0,		0x0,				0x0,				        0x0,		0,	0,			        0,	0    },	// End Of Table
};

// For UCIe0 Read bw monitor, using x8 DBSC channels
const struct hdma_info_ex hdma_tbl_wr[] = {	// UCIE0 WRCHx4
	/*	ucieCh	dmaCh	sar					dar					llp			rw	size		        tc	weight	*/
	{	0,		0,		DBSC01_HDMA(0),		DBSC45_HDMA(0),		0x7FF02000,	0,	DMA_SIZE_PER_CHAN,	0,	0x02 }, // DBSC0/1 → DBSC4/5
	{	0,		1,		DBSC01_HDMA(1),		DBSC45_HDMA(1),		0x7FF03000,	0,	DMA_SIZE_PER_CHAN,	0,	0x03 }, // DBSC0/1 → DBSC4/5
	{	0,		2,		DBSC23_HDMA(0),		DBSC67_HDMA(0),		0x7FF04000,	0,	DMA_SIZE_PER_CHAN,	0,	0x04 }, // DBSC2/3 → DBSC6/7
	{	0,		3,		DBSC23_HDMA(1),		DBSC67_HDMA(1),		0x7FF05000,	0,	DMA_SIZE_PER_CHAN,	0,	0x05 }, // DBSC3/3 → DBSC6/7
	{	0,		0,		0x0,				0x0,				0x0,		0,	0,			        0,	0    },	// End Of Table
};

// For UCIe0 Write bw monitor, using x8 DBSC channels
const struct hdma_info_ex hdma_tbl_rd[] = {	// UCIE0 RDCHx4
	/*	ucieCh	dmaCh	sar					dar					llp			rw	size		        tc	weight	*/
	{	0,		0,		DBSC45_HDMA(0),		DBSC01_HDMA(2),		0x7FF02000,	1,	DMA_SIZE_PER_CHAN,	0,	0x02 }, // DBSC4/5 → DBSC0/1
	{	0,		1,		DBSC45_HDMA(1),		DBSC01_HDMA(3),		0x7FF03000,	1,	DMA_SIZE_PER_CHAN,	0,	0x03 }, // DBSC4/5 → DBSC0/1
	{	0,		2,		DBSC67_HDMA(0),		DBSC23_HDMA(2),		0x7FF04000,	1,	DMA_SIZE_PER_CHAN,	0,	0x04 }, // DBSC6/7 → DBSC2/3
	{	0,		3,		DBSC67_HDMA(1),		DBSC23_HDMA(3),		0x7FF05000,	1,	DMA_SIZE_PER_CHAN,	0,	0x05 }, // DBSC6/7 → DBSC2/3
	{	0,		0,		0x0,				0x0,				0x0,		0,	0,			        0,	0    },	// End Of Table
};

// For data verify
const struct hdma_info_ex hdma_tbl_rd_verify_dt[] = {	// UCIE0 RDCHx2
	/*	ucieCh	dmaCh	sar					dar					llp			rw: RDCH(1), WRCH(0)	size		        tc	weight	*/
	{	0,		0,		DBSC01_HDMA(4),		DBSC23_HDMA(2),		0x7FF00000,	1,	                    DMA_SIZE_PER_CHAN,	0,	0x00 },
	{	0,		1,		DBSC01_HDMA(5),		DBSC23_HDMA(3),		0x7FF01000,	1,	                    DMA_SIZE_PER_CHAN,	0,	0x01 },
	{	0,		0,		0x0,				0x0,				0x0,		0,	                    0,			        0,	0    },	// End Of Table
};
#endif /* #ifdef RCAR_UCIE_4HDMA_8DBSC */

#ifdef RCAR_UCIE_2HDMA_4DBSC
// For write test data
const struct hdma_info_ex hdma_tbl_wr_2ch[] = {	// UCIE0 WRCHx2
	/*	ucieCh	dmaCh	sar					dar					llp			rw	                    size		        tc	weight	*/
	{	0,		0,		DBSC01_HDMA(0),		0x20000000000,		0x7FF00000,	0,	                    DMA_SIZE_PER_CHAN,	0,	0x00 },
	{	0,		1,		DBSC01_HDMA(1),		0x20000100000,		0x7FF01000,	0,	                    DMA_SIZE_PER_CHAN,	0,	0x01 },
	{	0,		0,		0x0,				0x0,				        0x0,		                0,	0,			    0,	0    },	// End Of Table
};

// For data verify
const struct hdma_info_ex hdma_tbl_rd_2ch[] = {	// UCIE0 RDCHx2
	/*	ucieCh	dmaCh	sar					dar					llp			rw: RDCH(1), WRCH(0)	size		        tc	weight	*/
	{	0,		0,		0x20000000000,		DBSC01_HDMA(2),		0x7FF00000,	1,	                    DMA_SIZE_PER_CHAN,	0,	0x00 },
	{	0,		1,		0x20000100000,		DBSC01_HDMA(3),		0x7FF01000,	1,	                    DMA_SIZE_PER_CHAN,	0,	0x01 },
	{	0,		0,		0x0,				0x0,				0x0,		0,	                    0,			        0,	0    },	// End Of Table
};
#endif /* #ifdef RCAR_UCIE_2HDMA_4DBSC */

#ifdef RCAR_UCIE_2HDMA_2DBSC
// For write test data
const struct hdma_info_ex hdma_tbl_wr_2dbsc[] = {	// UCIE0 WRCHx2
	/*	ucieCh	dmaCh	sar					dar					llp			rw	                    size		        tc	weight	*/
	{	0,		0,		DBSC01_HDMA(0),		DBSC01_HDMA(2),		0x7FF00000,	0,	                    DMA_SIZE_PER_CHAN,	0,	0x00 },
	{	0,		1,		DBSC01_HDMA(1),		DBSC01_HDMA(3),		0x7FF01000,	0,	                    DMA_SIZE_PER_CHAN,	0,	0x01 },
	{	0,		0,		0x0,				0x0,				        0x0,		                0,	0,			    0,	0    },	// End Of Table
};

// For data verify
const struct hdma_info_ex hdma_tbl_rd_2dbsc[] = {	// UCIE0 RDCHx2
	/*	ucieCh	dmaCh	sar					dar					llp			rw: RDCH(1), WRCH(0)	size		        tc	weight	*/
	{	0,		0,		DBSC01_HDMA(2),		DBSC01_HDMA(4),		0x7FF00000,	1,	                    DMA_SIZE_PER_CHAN,	0,	0x00 },
	{	0,		1,		DBSC01_HDMA(3),		DBSC01_HDMA(5),		0x7FF01000,	1,	                    DMA_SIZE_PER_CHAN,	0,	0x01 },
	{	0,		0,		0x0,				0x0,				0x0,		0,	                    0,			        0,	0    },	// End Of Table
};
#endif /* #ifdef RCAR_UCIE_2HDMA_4DBSC */

#ifdef RCAR_UCIE_1HDMA_1DBSC
// For write test data
struct hdma_info_ex hdma_tbl_wr_1hdma_1dbsc[] = {	// UCIE0 WRCHx1
	/*	ucieCh	dmaCh	sar					dar					llp			rw	                    size		        tc	weight	*/
	{	0,		0,		DBSC01_HDMA(0),		DBSC01_HDMA(2),		0x7FF00000,	0,	                    DMA_SIZE_PER_CHAN,	0,	0x00 },
	{	0,		0,		0x0,				0x0,				0x0,		0,	                    0,			        0,	0    },	// End Of Table
};

// For data verify
struct hdma_info_ex hdma_tbl_rd_1hdma_1dbsc[] = {	// UCIE0 RDCHx1
	/*	ucieCh	dmaCh	sar					dar					llp			rw: RDCH(1), WRCH(0)	size		        tc	weight	*/
	{	0,		0,		DBSC01_HDMA(1),		DBSC01_HDMA(3),		0x7FF01000,	1,	                    DMA_SIZE_PER_CHAN,	0,	0x00 },
	{	0,		0,		0x0,				0x0,				0x0,		0,	                    0,			        0,	0    },	// End Of Table
};
#endif /* #ifdef RCAR_UCIE_1HDMA_1DBSC */

#ifdef RCAR_UCIE_1RTDMA_1DBSC
// struct dmac_info_ex {
// 	uint32_t instance;		// RT-DMAC0-3
// 	uint32_t chNo;			// ch0-15
// 	uint64_t sar;
// 	uint64_t dar;
// 	uint8_t dmsm;			// Destination address mode(2bit) + Source address mode(2bit)
// };

const struct dmac_info_ex rtdmac_tbl_wr[] = {
	// /*	instance	chNo	sar				dar				dmsm */
	// {	0,			0,		0x1858000000,	0x20000000000,	0x4	},	// Dst Increment, Src Fix
	{	0,			0,		DBSC01_HDMA(0),	0x20000000000,	0x5	},	// Dst,Src Increment
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

#define CHUNK_SIZE_BYTES    SIZE_1MB
#define WORDS_PER_CHUNK     (CHUNK_SIZE_BYTES / sizeof(uint32_t))

/*-----------------------------------------------------------*/

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void ucie_comm_task( void *pvParameters );

/*-----------------------------------------------------------*/

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
    if (buffer[index] != START_PATTERN)
    {
        // Data is not ready, return 
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

/*******************************************************
 * NAME		: ucie_xfer
 * FUNCTION	: Root Complex UCIE HDMA transfer test
 * INPUT	: hdma_info_ex *hdma_tbl
 * OUTPUT	: none
 * RETURN	: none
 ******************************************************/
static void ucie_xfer(struct hdma_info_ex *hdma_tbl, uint32_t custom_size)
{
    uint32_t index;

    /* move to outside */
    // printf("UCIE generates random test data %d \n",index);
    // while (hdma_tbl[index].llp != 0x0)
    // {
    //     /* Clear data */
    //     printf("UCIE memset 0 for SAR and DAR buffers %d \n",index);
    //     memset((void *)(uintptr_t)hdma_tbl[index].sar, 0, DMA_SIZE_PER_CHAN);
    //     memset((void *)(uintptr_t)hdma_tbl[index].dar, 0, DMA_SIZE_PER_CHAN);

	//     /* Fill test data for SAR */
    //     ucie_get_random_data((uint32_t)hdma_tbl[index].sar, DMA_SIZE_PER_CHAN);
    //     index++;
    // }

    /* Create Linked List and Set HDMA Registers */
    printf("UCIE setting HDMA Registers\n");
    index = 0;
    while (hdma_tbl[index].llp != 0x0)
    {
        hdma_tbl[index].size = custom_size;
        // ucie_hwemu_hdma_set_regsLL(&hdma_tbl[index]);
        Ucie_hdma_SetRegsLL(&hdma_tbl[index]);
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

    /* move to outside */
    // printf("-------- CHECK RESULT --------\n");
    // index = 0;
    // while (hdma_tbl[index].llp != 0x0)
    // {
    //     // Print first 32 bytes of SAR buffer to verify
    //     print_buffer((uint32_t)hdma_tbl[index].sar, 32);
    //     print_buffer((uint32_t)hdma_tbl[index].dar, 32);
	//     index++;
    // }
}

/*-----------------------------------------------------------*/

static void ucie_comm_task(void *pvParameters)
{
    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    uint32_t ret = NORMAL_END;
    uint64_t ucie1_mem;
	uint32_t ucie0_tgt;
    uint32_t val;

    uint8_t uice_chn = UCIE_CH1;

    // uint32_t *w;

    // struct hdma_info_ex *hdma_tbl_wr = (struct hdma_info_ex *) pvPortMalloc(3 * sizeof(struct hdma_info_ex));
    // struct hdma_info_ex *hdma_tbl_rd = (struct hdma_info_ex *) pvPortMalloc(3 * sizeof(struct hdma_info_ex));

// #if 0 /* Try to use UCIE read and write on seperate DBSCs */
//     /*	 ucieCh	dmaCh	sar			   dar			  llp			rw	size		tc	weight	*/
//     hdma_tbl_wr[0] = (struct hdma_info_ex) {0, 0, DBSC0_HDMA(1), DBSC0_HDMA(2), 0x7FF00000, 0, DMA_SIZE_PER_CHAN, 0, 0x00};
//     hdma_tbl_wr[1] = (struct hdma_info_ex) {0, 1, DBSC0_HDMA(3), DBSC0_HDMA(4), 0x7FF01000, 0, DMA_SIZE_PER_CHAN, 0, 0x01};
//     hdma_tbl_wr[2] = (struct hdma_info_ex) {0, 0, 0x0, 0x0, 0x0, 0, 0, 0, 0}; // End marker

//     hdma_tbl_rd[0] = (struct hdma_info_ex) {0, 0, DBSC0_HDMA(5), DBSC0_HDMA(6), 0x7FF00000, 1, DMA_SIZE_PER_CHAN, 0, 0x00};
//     hdma_tbl_rd[1] = (struct hdma_info_ex) {0, 1, DBSC0_HDMA(7), DBSC0_HDMA(8), 0x7FF01000, 1, DMA_SIZE_PER_CHAN, 0, 0x01};
//     hdma_tbl_rd[2] = (struct hdma_info_ex) {0, 0, 0x0, 0x0, 0x0, 0, 0, 0, 0}; // End marker
// #else 
//     hdma_tbl_wr[0] = (struct hdma_info_ex) {0, 0, DBSC0_HDMA(0), DBSC4_HDMA(0), 0x7FF00000, 0, DMA_SIZE_PER_CHAN, 0, 0x00};
//     hdma_tbl_wr[1] = (struct hdma_info_ex) {0, 1, DBSC0_HDMA(1), DBSC4_HDMA(1), 0x7FF01000, 0, DMA_SIZE_PER_CHAN, 0, 0x01};
//     hdma_tbl_wr[2] = (struct hdma_info_ex) {0, 0, 0x0, 0x0, 0x0, 0, 0, 0, 0};

//     hdma_tbl_rd[0] = (struct hdma_info_ex) {0, 0, DBSC4_HDMA(0), DBSC0_HDMA(2), 0x7FF00000, 1, DMA_SIZE_PER_CHAN, 0, 0x00};
//     hdma_tbl_rd[1] = (struct hdma_info_ex) {0, 1, DBSC4_HDMA(1), DBSC0_HDMA(3), 0x7FF01000, 1, DMA_SIZE_PER_CHAN, 0, 0x01};
//     hdma_tbl_rd[2] = (struct hdma_info_ex) {0, 0, 0x0, 0x0, 0x0, 0, 0, 0, 0};  
// #endif
	// w = (uint32_t *)(0xbffff000);
    vTaskDelay(pdMS_TO_TICKS(1));
    printf("[X5H TEG] RC SETUP START\n");
 
    /* UCIe_chan = 0 - Root-Complex */
    bool is_rootport = true;
    ret = Ucie_Setup_rc(uice_chn);
    // ret = ucie_hwemu_hw_init_rc(uice_chn, MODE_RC, UCIE_CH_RC_WITH);
    if (ret != NORMAL_END)
    {
        printf("[X5H TEG] RC FAILED\n");
	    goto LABEL_ERROR;
    }
    printf("[X5H TEG] RC SETUP DONE \n");

    // For debugging
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x000004, 0x00110007); // ???
    mem_read32(UCIE_AXI_BASE(uice_chn) + 0x000004);

	/*====================================*/
	/* UCIE memory area                   */
	/*====================================*/
	ucie1_mem = 0x20000000000;  // D2D (UCIe ch0/no coherent (CXL))
	ucie0_tgt = 0x4F000000;     // UCIE1 INBOUND: translate from 0x20000000000 to 0x4F000000

    /*====================================*/
	/* UCIE inbound setting                */
	/*====================================*/
#if 0
    ucie_hwemu_init_inbound(0, base_addr, target_addr);
#else 
	/*** ch0 ***/
	val = mem_read32(UCIE_AXI_BASE(uice_chn) + 0x300510) | 0x00FFFFF0; // IATU LIMIT_ADDR_OFF_INBOUND_2
	// val = mem_read32(UCIE_AXI_BASE(uice_chn) + 0x300510) | 0x000FFFF0; // IATU LIMIT_ADDR_OFF_INBOUND_2
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300510, val);			    // LIMIT_ADD_RW -> 16MB
    mem_read32(UCIE_AXI_BASE(uice_chn) + 0x300510);

    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300508, ucie1_mem & 0xFFFFFFFF);	//; IATU LWR_BASE_ADDR_OFF_INBOUND_2
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x30050C, ucie1_mem >> 32);		    //; IATU UPPER_BASE_ADDR_OFF_INBOUND_2
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300514, ucie0_tgt);                //; IATU LWR_TARGET_ADDR_OFF_INBOUND_2
	mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300518, 0x00000000);               //; IATU UPPER_TARGET_ADDR_OFF_INBOUND_2
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300500, 0x00000000);			    //; IATU REGION_CTRL_1_OFF_INBOUND_2
    mem_write32(UCIE_AXI_BASE(uice_chn) + 0x300504, 0x80000000);			    //; IATU REGION_CTRL_2_OFF_INBOUND_2: [31] REDION_EN=1, [30] MATCH_MODE=0 (Address Match Mode)

    wait_time(2);
#endif

    uint32_t *p_test_dt;
    uint32_t xfer_size[] = {DMA_SIZE_PER_CHAN, DMA_SIZE_PER_CHAN, DMA_SIZE_PER_CHAN};
    uint32_t iter = 0;

    /* Fill random test data */
    printf("\n[X5H TEG] RC: Starting UCIE generate data\n");
    p_test_dt = (uint32_t *)TEST_DATA_RC;
    memset_fast32((uint32_t)DBSC01_HDMA(0), p_test_dt, DMA_SIZE_PER_CHAN);

    p_test_dt = (uint32_t *)TEST_DATA_EP;
    memset_fast32((uint32_t)DBSC01_HDMA(5), p_test_dt, DMA_SIZE_PER_CHAN);
    
    // while (iter < 3)
    {
        /* Summary of Loop Stages
         * Generate test pattern via memset_fast32()
         * Perform WRCHx2 → WRCHx4 → RDCHx4 → WRCHx2
         * Validate with compare_buffer()
         * Print a few sample values with print_buffer()
         * Increase transfer size to 2Kb, 4Kb, 8Kb, 16Kb, 32kb, 64kb, 128Kb, 512Kb, 1024Mb (Maxium)
         */

        printf("\nIteration %d, %d Kb\n", iter, (xfer_size[iter] / 1024));

        // p_test_dt++;
        // printf("UCIE generate data - END \n");

#ifdef RCAR_UCIE_4HDMA_8DBSC
        /* WRCHx2
        *    UCIE write: from 0xB0000000 to 0x145A000000
        *                from 0xB1000000 to 0x145B000000
        */
        printf("\nStarting UCIe HDMA WRCHx2\n");
        ucie_xfer(hdma_tbl_wrtest_dt, xfer_size);

        /* WRCHx4
        *    UCIE write: from 0xB0000000 to 0x185A000000
        *                from 0xB1000000 to 0x185B000000
        *                from 0x145A000000 to 0x1C5A000000
        *                from 0x145B000000 to 0x1C5B000000
        */
        printf("\nStarting UCIe HDMA WRCHx4\n");
        ucie_xfer(hdma_tbl_wr, xfer_size);

        /* RDCHx4
        *    UCIE read: from 0x185A000000 to 0xB2000000
        *                from 0x185B000000 to 0xB3000000
        *                from 0x1C5A000000 to 0x145C000000
        *                from 0x1C5B000000 to 0x145D000000
        */
        printf("\nStarting UCIe HDMA RDCHx4\n");
        ucie_xfer(hdma_tbl_rd, xfer_size);

        /* WRCHx2
        *    UCIE read: to 0xB4000000 from 0x145C000000
        *               to 0xB5000000 from 0x145D000000
        */
        printf("\nStarting UCIe HDMA RDCHx2\n");
        ucie_xfer(hdma_tbl_rd_verify_dt, xfer_size);

        printf("\nCHECKING RESULT\n");
        /* Expected result: DBSC01_HDMA(0) = DBSC01_HDMA(2) =  DBSC01_HDMA(4)
         *                  DBSC01_HDMA(1) = DBSC01_HDMA(3) =  DBSC01_HDMA(5) */

        ret = compare_buffer(DBSC01_HDMA(0), DBSC01_HDMA(2), xfer_size);
        ret |= compare_buffer(DBSC01_HDMA(2), DBSC01_HDMA(4), xfer_size);
        ret |= compare_buffer(DBSC01_HDMA(1), DBSC01_HDMA(3), xfer_size);
        ret |= compare_buffer(DBSC01_HDMA(1), DBSC01_HDMA(5), xfer_size);
        print_buffer((uint32_t)DBSC01_HDMA(0), 16);
        print_buffer((uint32_t)DBSC01_HDMA(2), 16);
        print_buffer((uint32_t)DBSC01_HDMA(4), 16);
        printf("\n");
        print_buffer((uint32_t)DBSC01_HDMA(1), 16);
        print_buffer((uint32_t)DBSC01_HDMA(3), 16);
        print_buffer((uint32_t)DBSC01_HDMA(5), 16);
        printf("\n");
#endif /* #ifdef RCAR_UCIE_4HDMA_8DBSC */

#ifdef RCAR_UCIE_2HDMA_4DBSC
        printf("\nStarting UCIe HDMA WRCHx2\n");
        ucie_xfer(hdma_tbl_wr_2ch, xfer_size);

        printf("\nStarting UCIe HDMA RDCHx2\n");
        ucie_xfer(hdma_tbl_rd_2ch, xfer_size);

        printf("\nCHECKING RESULT\n");
        ret = compare_buffer(DBSC01_HDMA(0), DBSC01_HDMA(2), xfer_size);
        ret |= compare_buffer(DBSC01_HDMA(1), DBSC01_HDMA(3), xfer_size);
        print_buffer((uint32_t)DBSC01_HDMA(0), 16);
        print_buffer((uint32_t)DBSC01_HDMA(2), 16);
        printf("\n");
        print_buffer((uint32_t)DBSC01_HDMA(1), 16);
        print_buffer((uint32_t)DBSC01_HDMA(3), 16);
#endif /* RCAR_UCIE_2HDMA_4DBSC */

#ifdef RCAR_UCIE_2HDMA_2DBSC
        printf("\nStarting UCIe HDMA WRCHx2\n");
        ucie_xfer(hdma_tbl_wr_2dbsc, xfer_size[iter]);

        printf("\nCHECKING RESULT\n");
        ret = compare_buffer(DBSC01_HDMA(0), DBSC01_HDMA(2), xfer_size[iter]);
        ret |= compare_buffer(DBSC01_HDMA(1), DBSC01_HDMA(3), xfer_size[iter]);
        print_buffer((uint32_t)DBSC01_HDMA(0), 16);
        print_buffer((uint32_t)DBSC01_HDMA(2), 16);
        printf("\n");
        print_buffer((uint32_t)DBSC01_HDMA(1), 16);
        print_buffer((uint32_t)DBSC01_HDMA(3), 16);

        printf("\nStarting UCIe HDMA RDCHx2\n");
        ucie_xfer(hdma_tbl_rd_2dbsc, xfer_size[iter]);

        printf("\nCHECKING RESULT\n");
        ret = compare_buffer(DBSC01_HDMA(2), DBSC01_HDMA(4), xfer_size[iter]);
        ret |= compare_buffer(DBSC01_HDMA(3), DBSC01_HDMA(5), xfer_size[iter]);
        print_buffer((uint32_t)DBSC01_HDMA(2), 16);
        print_buffer((uint32_t)DBSC01_HDMA(4), 16);
        printf("\n");
        print_buffer((uint32_t)DBSC01_HDMA(3), 16);
        print_buffer((uint32_t)DBSC01_HDMA(5), 16);
#endif /* RCAR_UCIE_2HDMA_4DBSC */

#ifdef RCAR_UCIE_1HDMA_1DBSC
        printf("\n[X5H TEG] RC: Starting UCIe HDMA WRCHx1\n");
        ucie_xfer(hdma_tbl_wr_1hdma_1dbsc, xfer_size[iter]);
        wait_time(1);

        printf("\n[X5H TEG] RC: Starting UCIe HDMA RDCHx1\n");
        ucie_xfer(hdma_tbl_rd_1hdma_1dbsc, xfer_size[iter]);
        wait_time(1);

        // Reading end pattern from RC buf and compare
        printf("\n[X5H TEG] RC: UCIE HDMA CHECKING RESULT\n");
        ret = compare_buffer(DBSC01_HDMA(3), DBSC01_HDMA(5), xfer_size[iter]);

        // Print START_PATTERN
        p_test_dt = (uint32_t *)(uintptr_t)DBSC01_HDMA(3);
        printf("p_buf[0]: 0x%08x\n", p_test_dt[0]);

        // Print END_PATTERN locations for each 1MB segment
        for (uint32_t i = 1; i <= (DMA_SIZE_PER_CHAN / SIZE_1MB); i++)
        {
            uint32_t index = i * WORDS_PER_CHUNK - 1;  // last word of each 1MB
            printf("p_buf[0x%08x]: 0x%08x\n", index, p_test_dt[index]);
        }
        printf("\n");
#endif /* #ifdef RCAR_UCIE_1HDMA_1DBSC */

#ifdef RCAR_UCIE_1RTDMA_1DBSC
        printf("\nStarting UCIe RT-DMA write to UCIE0 memory\n");
        ret = Rtdmac_Transfer(rtdmac_tbl_wr);
        // Reading end pattern from RC buf and compare
        if (ret == NORMAL_END)
        {
            printf("\nCHECKING RESULT\n");
            ret = compare_buffer(ucie0_tgt, DBSC01_HDMA(0), xfer_size[iter]);

            // Print to check end pattern
            p_test_dt = (uint32_t *)(uintptr_t)ucie0_tgt;
            printf("p_buf[0xFFF]: 0x%08x\n", p_test_dt[0xFFF]);
            printf("p_buf[0xFFFF]: 0x%08x\n", p_test_dt[0xFFFF]);
            printf("p_buf[0x1FFFF]: 0x%08x\n", p_test_dt[0x1FFFF]);
            printf("p_buf[0x2FFFF]: 0x%08x\n", p_test_dt[0x2FFFF]);
            printf("p_buf[0x3FFFF]: 0x%08x\n", p_test_dt[0x3FFFF]);
            printf("\n");
        }
#endif /* #ifdef RCAR_UCIE_1RTDMA_1DBSC */

        if (ret != NORMAL_END)
        {
            printf("[X5H TEG] RC: TRANSFER FAILED\n");
            // goto LABEL_ERROR;
        }
        else
        {
            printf("[X5H TEG] RC: TRANSFER PASSED\n");
        }

        // Increase test size to 1MB
        // xfer_size += SIZE_1MB;
        ++iter;
        vTaskDelay(pdMS_TO_TICKS(1));
    }

    printf("[X5H TEG] RC: TEST DONE\n");

LABEL_ERROR:
    while (1);
}

#ifdef RCAR_UCIE_2RTDMA_2DBSC
static void prvDMACTask( void *pvParameters )
{
	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;
	uint16_t ret_dma;

	Context_t usr_context =
	{
		.ctx = &rDmacIrqHandler_t_irq,
	};

	printf("\nPROGRAM START \n");

	/* Device Driver Part */
	ret_dma = R_RTDMAC_RcarDmacCtrlInit(RT_DMAC0, DRV_RTDMAC_PRIO_FIX);
	printf("R_RTDMAC_RcarDmacCtrlInit(DMAC0) returned %d\n", ret_dma);

	/* Add source value */
	*(volatile uint32_t*)cfg0.mSrcAddr = 0x123;

	/* Set user callback */
	ret_dma = R_RTDMAC_RcarCallBackSet(&rDmacIrqHandler_t_irq, dmacUserCallback, &usr_context);
	printf("R_RTDMAC_RcarCallBackSet returned = %d\n", ret_dma);

	/* RT_DMAC0/CH3 memtomem execution */
	ret_dma = R_RTDMAC_RcarDmacExec(RT_DMAC0, DMAC_CH3, &cfg0, 0);
	printf("R_RTDMAC_RcarDmacExec(DMAC0_CH3) returned = %d\n", ret_dma);

	/* Verify destination data */
	uint32_t nSrcVal = *(volatile uint32_t*)cfg0.mSrcAddr;
	uint32_t nDestVal = *(volatile uint32_t*)cfg0.mDestAddr;
    ret_dma = (uint32_t)(nDestVal - nSrcVal);
    if (ret_dma)
    {
        printf("R_RTDMAC Failed Src 0x%x, Des 0x%x\r\n", nSrcVal, nDestVal);
    }
    else
    {
        printf("R_RTDMAC Passed Src 0x%x, Des 0x%x\r\n", nSrcVal, nDestVal);
    }
	printf("\nPROGRAM END \n");
    while (1);
}
#endif /* #ifdef RCAR_UCIE_2RTDMA_2DBSC */

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
