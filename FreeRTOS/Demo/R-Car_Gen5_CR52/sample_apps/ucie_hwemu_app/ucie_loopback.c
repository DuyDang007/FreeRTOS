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
#include "ucie_hwemu.h"

#include <stdlib.h>
// #include <time.h> //srand()

#define main_ucie_TASK_PRIORITY        (tskIDLE_PRIORITY + 1)

#define SIZE_64MB           (0x4000000)
#define SIZE_32MB           (0x2000000)
#define SIZE_16MB           (0x1000000)
#define SIZE_1MB            (0x100000)
#define SIZE_256KB          (0x40000)
#define SIZE_1KB            (0x400)
#define DMA_SIZE_PER_CHAN	SIZE_16MB

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
// UCIE AICC 2x2k split

#if 0
#define INRANGE_DBSC0_ADDR	    (0xB0000000)        // In range DBSC 0/1/2/3, memory is SRAM_ATTR
#define INRANGE_DBSC4_ADDR	    (0x185A000000)      // In range DBSC 4/5/6/7

#define DBSC0_HDMA(n)		    (INRANGE_DBSC0_ADDR + (n) * DMA_SIZE_PER_CHAN)
#define DBSC4_HDMA(n)		    (INRANGE_DBSC4_ADDR + (n) * DMA_SIZE_PER_CHAN)

const struct hdma_info_ex hdma_tbl_wr[] = {	// UCIE0 WRCHx2
	/*	ucieCh	dmaCh	sar					dar					llp			rw	size		        tc	weight	*/
	{	0,		0,		DBSC0_HDMA(0),		DBSC4_HDMA(0),		0x7FF00000,	0,	DMA_SIZE_PER_CHAN,	0,	0x00 },
	{	0,		1,		DBSC0_HDMA(1),		DBSC4_HDMA(1),		0x7FF01000,	0,	DMA_SIZE_PER_CHAN,	0,	0x01 },
	{	0,		0,		0x0,				0x0,				0x0,		0,	0,			        0,	0    },	// End Of Table
};

const struct hdma_info_ex hdma_tbl_rd[] = {	// UCIE0 RDCHx2
	/*	ucieCh	dmaCh	sar					dar					llp			rw	size		        tc	weight	*/
	{	0,		0,		DBSC4_HDMA(0),		DBSC0_HDMA(2),		0x7FF00000,	1,	DMA_SIZE_PER_CHAN,	0,	0x00 },
	{	0,		1,		DBSC4_HDMA(1),		DBSC0_HDMA(3),		0x7FF01000,	1,	DMA_SIZE_PER_CHAN,	0,	0x01 },
	{	0,		0,		0x0,				0x0,				0x0,		0,	0,			        0,	0    },	// End Of Table
};
#else
#define DRAM_DBSC01_ADDR    (0xB0000000)    // DRAM area for DBSC0/1 - Local SRAM-like region
#define DRAM_DBSC23_ADDR    (0x145A000000)  // DRAM area for DBSC2/3
#define DRAM_DBSC45_ADDR    (0x185A000000)  // DRAM area for DBSC4/5
#define DRAM_DBSC67_ADDR    (0x1C5A000000)  // DRAM area for DBSC6/7

#define DBSC01_HDMA(n)      (DRAM_DBSC01_ADDR + (n) * DMA_SIZE_PER_CHAN)
#define DBSC23_HDMA(n)      (DRAM_DBSC23_ADDR + (n) * DMA_SIZE_PER_CHAN)
#define DBSC45_HDMA(n)      (DRAM_DBSC45_ADDR + (n) * DMA_SIZE_PER_CHAN)
#define DBSC67_HDMA(n)      (DRAM_DBSC67_ADDR + (n) * DMA_SIZE_PER_CHAN)

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
	{	0,		3,		DBSC01_HDMA(0),		DBSC45_HDMA(0),		0x7FF02000,	0,	DMA_SIZE_PER_CHAN,	0,	0x02 }, // DBSC0/1 → DBSC4/5
	{	0,		4,		DBSC01_HDMA(1),		DBSC45_HDMA(1),		0x7FF03000,	0,	DMA_SIZE_PER_CHAN,	0,	0x03 }, // DBSC0/1 → DBSC4/5
	{	0,		5,		DBSC23_HDMA(0),		DBSC67_HDMA(0),		0x7FF04000,	0,	DMA_SIZE_PER_CHAN,	0,	0x04 }, // DBSC2/3 → DBSC6/7
	{	0,		6,		DBSC23_HDMA(1),		DBSC67_HDMA(1),		0x7FF05000,	0,	DMA_SIZE_PER_CHAN,	0,	0x05 }, // DBSC3/3 → DBSC6/7
	{	0,		0,		0x0,				0x0,				0x0,		0,	0,			        0,	0    },	// End Of Table
};

// For UCIe0 Write bw monitor, using x8 DBSC channels
const struct hdma_info_ex hdma_tbl_rd[] = {	// UCIE0 RDCHx4
	/*	ucieCh	dmaCh	sar					dar					llp			rw	size		        tc	weight	*/
	{	0,		3,		DBSC45_HDMA(0),		DBSC01_HDMA(2),		0x7FF02000,	1,	DMA_SIZE_PER_CHAN,	0,	0x02 }, // DBSC4/5 → DBSC0/1
	{	0,		4,		DBSC45_HDMA(1),		DBSC01_HDMA(3),		0x7FF03000,	1,	DMA_SIZE_PER_CHAN,	0,	0x03 }, // DBSC4/5 → DBSC0/1
	{	0,		5,		DBSC67_HDMA(0),		DBSC23_HDMA(2),		0x7FF04000,	1,	DMA_SIZE_PER_CHAN,	0,	0x04 }, // DBSC6/7 → DBSC2/3
	{	0,		6,		DBSC67_HDMA(1),		DBSC23_HDMA(3),		0x7FF05000,	1,	DMA_SIZE_PER_CHAN,	0,	0x05 }, // DBSC6/7 → DBSC2/3
	{	0,		0,		0x0,				0x0,				0x0,		0,	0,			        0,	0    },	// End Of Table
};

// For data verify
const struct hdma_info_ex hdma_tbl_wr_verify_dt[] = {	// UCIE0 WRCHx2
	/*	ucieCh	dmaCh	sar					dar					llp			rw	size		        tc	weight	*/
	{	0,		0,		DBSC01_HDMA(4),		DBSC23_HDMA(2),		0x7FF00000,	0,	DMA_SIZE_PER_CHAN,	0,	0x00 },
	{	0,		1,		DBSC01_HDMA(5),		DBSC23_HDMA(3),		0x7FF01000,	0,	DMA_SIZE_PER_CHAN,	0,	0x01 },
	{	0,		0,		0x0,				0x0,				        0x0,		0,	0,			        0,	0    },	// End Of Table
};
#endif

const uint32_t TEST_DATA0[8U] = {0x12345678, 0xFEDCBA98, 0xA5A5A5A5, 0x5A5A5A5A,
							        0x11223344, 0x55667788, 0xAABBCCDD, 0xEEFF0011};

const uint32_t TEST_DATA1[8U] = {0x11111111, 0x22222222, 0x33333333, 0x44444444,
							        0x55555555, 0x66666666, 0x77777777, 0x88888888};

const uint32_t ZERO_DATA[8U] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

// struct hdma_info {
// 	uint32_t ucie_chan;
// 	uint32_t dma_chan;
// 	uint64_t sar;
// 	uint64_t dar;
// 	uint32_t rw;
// };

/* Used by the pseudo random number generator. */
// static size_t uxNextRand = 0;
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
}

/*-----------------------------------------------------------*/

/*
 * Local pseudo random number seed and return functions.  Used to avoid calls
 * to the standard library.
 */
// static size_t ucie_Rand( void )
// {
// 	uxNextRand = ( uxNextRand * ( size_t ) 1103515245 ) + ( size_t ) 12345;
// 	// return ( uxNextRand / ( size_t ) 65536 ) % ( size_t ) 32768;
//     return uxNextRand;
// }

// static void ucie_SRand( size_t uxSeed )
// {
// 	uxNextRand = uxSeed;
// }

/*
 * Processes 32MB in total
 * Processes memory in 1MB chunks
 * Writes 8 words (32 bytes) at a time using an unrolled loop
 * Finishes any remaining words in the chunk with a modulo-based write
 */
static void memset_fast32(uint32_t dest, const uint32_t *pattern, uint32_t size_bytes)
{
    const uint32_t pattern_size = 8;
    uint32_t total_words        = size_bytes / sizeof(uint32_t);
    uint32_t chunks             = total_words / WORDS_PER_CHUNK;
    volatile uint32_t *buffer   = (volatile uint32_t *)(uintptr_t)dest;

    // Process full chunks
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
}

// // // static void memset_fast32(uint32_t *destination, uint32_t *source, uint32_t size_bytes)
// // // {
// // //     // volatile uint32_t *buffer = (volatile uint32_t *)(uintptr_t)addr;
// // //     uint32_t count = size_bytes / sizeof(uint32_t);
// // //     uint32_t i = 0;

// // //     // Write blocks of 8 for speed
// // //     for (i = 0; i + 7 < count; i += 8)
// // //     {
// // //         buffer[i] = source[i];
// // //         buffer[i + 1] = source[i + 1];
// // //         buffer[i + 2] = source[i + 2];
// // //         buffer[i + 3] = source[i + 3];
// // //         buffer[i + 4] = source[i + 4];
// // //         buffer[i + 5] = source[i + 5];
// // //         buffer[i + 6] = source[i + 6];
// // //         buffer[i + 7] = source[i + 7];
// // //     }

// // //     // // Write remaining elements if count not divisible by 4
// // //     // for (; i < count; i++)
// // //     // {
// // //     //     buffer[i] = value;
// // //     // }
// // // }

// // static void ucie_get_random_data(uint32_t addr, uint32_t size_bytes)
// // {
// //     // Call this once before using rand()
// //     // Seed the random generator with FreeRTOS tick count
// //     ucie_SRand((size_t)xTaskGetTickCount());

// //     volatile uint32_t *buffer = (volatile uint32_t *)(uintptr_t)addr;
// //     uint32_t count = size_bytes / sizeof(uint32_t);
// //     uint32_t i = 0;

// //     for (i = 0; i < count; i++)
// //     {
// //         uint32_t data = (uint32_t)ucie_Rand();
// //         buffer[i] = data;
// //         // Uncomment for debug
// //         // printf("Write 0x%08X to SAR[0x%08X]\n", data, addr + (i * 4));
// //     }
// // }

// static void ucie_fill_test_pattern(uint32_t *destination, uint32_t *source, uint32_t size_bytes)
// {
//     // uint32_t *buffer = (uint32_t *)(uintptr_t)destination;
//     uint32_t count = size_bytes / sizeof(uint32_t); // Number of uint32_t words
//     uint32_t i = 0;

//     // Unroll loop: write 4 values per iteration
//     for (; i + 3 < count; i += 4)
//     {
//         buffer[i]     = source[(i + 0) % 8] + i + 0;
//         buffer[i + 1] = source[(i + 1) % 8] + i + 1;
//         buffer[i + 2] = source[(i + 2) % 8] + i + 2;
//         buffer[i + 3] = source[(i + 3) % 8] + i + 3;
//     }
// }

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

/*-----------------------------------------------------------*/

/*******************************************************
 * NAME		: ucie_xfer
 * FUNCTION	: Root Complex UCIE HDMA transfer test
 * INPUT	: hdma_info_ex *hdma_tbl
 * OUTPUT	: none
 * RETURN	: none
 ******************************************************/
static void ucie_xfer(const struct hdma_info_ex *hdma_tbl)
{
    uint32_t index;

    /* move to outside */
    // uint32_t index = 0;
    // while (hdma_tbl[index].llp != 0x0)
    // {
    //     /* Clear data */
    //     printf("UCIE memset 0 for SAR and DAR buffers %d \n",index);
    //     memset((void *)(uintptr_t)hdma_tbl[index].sar, 0, DMA_SIZE_PER_CHAN);
    //     memset((void *)(uintptr_t)hdma_tbl[index].dar, 0, DMA_SIZE_PER_CHAN);

	//     /* Fill test data for SAR */
    //     printf("UCIE generates random test data for SAR(%d) \n",index);
    //     ucie_get_random_data((uint32_t)hdma_tbl[index].sar, DMA_SIZE_PER_CHAN);
    //     index++;
    // }

    /* Create Linked List and Set HDMA Registers */
    printf("UCIE setting HDMA Registers\n");
    index = 0;
    while (hdma_tbl[index].llp != 0x0) {
        // ucie_hwemu_hdma_set_regs(hdma_tbl[index].ucieCh, hdma_tbl[index].dmaCh, hdma_tbl[index].sar, hdma_tbl[index].dar, hdma_tbl[index].llp, hdma_tbl[index].rw, hdma_tbl[index].size, hdma_tbl[index].tc, hdma_tbl[index].weight);/////???
        ucie_hwemu_hdma_set_regsLL(&hdma_tbl[index]);
	    index++;
    }

    /* Start All CH Transfer */
    printf("UCIE HDMA STARTS\n");
    index = 0;
    while (hdma_tbl[index].llp != 0x0)
    {
	    ucie_hwemu_hdma_start(hdma_tbl[index].ucieCh, hdma_tbl[index].dmaCh, hdma_tbl[index].rw);
	    index++;
    }

    /* Wait All CH STOP */
    printf("UCIE wait for HDMA transfer\n");
    index = 0;
    while (hdma_tbl[index].llp != 0x0)
    {
        if (ERROR_END == ucie_hwemu_hdma_wait_stop(hdma_tbl[index].ucieCh, hdma_tbl[index].dmaCh, hdma_tbl[index].rw) == 1)
        {
            printf("transfer timeout\n");
        }
	    index++;
    }

    /* Stop All CH */
    printf("UCIE HDMA transfer STOPS\n");
    index = 0;
    while (hdma_tbl[index].llp != 0x0)
    {
        ucie_hwemu_hdma_stop(hdma_tbl[index].ucieCh, hdma_tbl[index].dmaCh, hdma_tbl[index].rw);
	    index++;
    }

    /* move to outside */
    // printf("-------- CHECK RESULT --------\n");
    // index = 0;
    // while (hdma_tbl[index].llp != 0x0)
    // {
    //     // Print first 16 bytes of SAR buffer to verify
    //     print_buffer((uint32_t)hdma_tbl[index].sar, 16);
    //     print_buffer((uint32_t)hdma_tbl[index].dar, 16);
	//     index++;
    // }
}

static void wait_loop(uint32_t count)
{
    printf("W:%d\n", count);
    volatile uint32_t dummy = 0;
    for (uint32_t i = 0; i < count; i++)
    {
        dummy++;
    }
    printf("!\n", count);
}

static void ucie_comm_task(void *pvParameters)
{
    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;
    // uint64_t base_addr, target_addr;
	uint64_t ucie0_mem, ucie1_mem;
	uint32_t ucie0_tgt, ucie1_tgt;
    uint32_t val;
    int ret;

//     struct hdma_info_ex *hdma_tbl_wr = (struct hdma_info_ex *) pvPortMalloc(3 * sizeof(struct hdma_info_ex));
//     struct hdma_info_ex *hdma_tbl_rd = (struct hdma_info_ex *) pvPortMalloc(3 * sizeof(struct hdma_info_ex));

// #if 0 /* Try to use UCIE read and write on seperate DBSCs */
//     hdma_tbl_wr[0] = (struct hdma_info_ex) {0, 0, 0x80000000, 0x8000F000, 0x7FF00000, 0, DMA_SIZE_PER_CHAN, 0, 0x00};
//     //hdma_tbl_wr[0] = (struct hdma_info_ex) {0, 0, 0x1000000000, 0x1C00000000, 0x7FF00000, 0, SIZE_1MB, 0, 0x00};
//     //hdma_tbl_wr[1] = (struct hdma_info_ex) {0, 1, 0x1200008000, 0x1E00008000, 0x7FF01000, 0, SIZE_1MB, 0, 0x01};
//     hdma_tbl_wr[1] = (struct hdma_info_ex) {0, 1, 0x800F0000, 0x800FF000, 0x7FF01000, 0, DMA_SIZE_PER_CHAN, 0, 0x01};
//     hdma_tbl_wr[2] = (struct hdma_info_ex) {0, 0, 0x0, 0x0, 0x0, 0, 0, 0, 0};
//     //hdma_tbl_wr[1] = (struct hdma_info) {0, 4, DBSC0_HDMA(1), DBSC0_HDMA(5), 0};
//     //hdma_tbl_wr[2] = (struct hdma_info) {0, 5, DBSC0_HDMA(2), DBSC0_HDMA(6), 0};
//     //hdma_tbl_wr[3] = (struct hdma_info) {0, 6, DBSC0_HDMA(3), DBSC0_HDMA(7), 0};
//     //hdma_tbl_wr[1] = (struct hdma_info) {0, 0, 0, 0, 0};
//     //hdma_tbl_rd[0] = (struct hdma_info_ex) {0, 0, 0x1000000000, 0x1C00000000, 0x7FF00000, 1, SIZE_1MB, 0, 0x00};
//     //hdma_tbl_rd[1] = (struct hdma_info_ex) {0, 1, 0x1200008000, 0x1E00008000, 0x7FF01000, 1, SIZE_1MB, 0, 0x01};
//     hdma_tbl_rd[0] = (struct hdma_info_ex) {0, 0, 0x80000000, 0x8000F000, 0x7FF00000, 1, DMA_SIZE_PER_CHAN, 0, 0x00};
//     hdma_tbl_rd[1] = (struct hdma_info_ex) {0, 1, 0x800F0000, 0x800FF000, 0x7FF01000, 1, DMA_SIZE_PER_CHAN, 0, 0x01};
//     hdma_tbl_rd[2] = (struct hdma_info_ex) {0, 0, 0x0, 0x0, 0x0, 0, 0, 0, 0};
//     //hdma_tbl_rd[3] = (struct hdma_info) {0, 6, DBSC0_HDMA(3), DBSC0_HDMA(7), 1};
//     //hdma_tbl_rd[1] = (struct hdma_info) {0, 0, 0, 0, 0};
// #else
//     hdma_tbl_wr[0] = (struct hdma_info_ex) {0, 0, DBSC0_HDMA(0), DBSC4_HDMA(0), 0x7FF00000, 0, DMA_SIZE_PER_CHAN, 0, 0x00};
//     hdma_tbl_wr[1] = (struct hdma_info_ex) {0, 1, DBSC0_HDMA(1), DBSC4_HDMA(1), 0x7FF01000, 0, DMA_SIZE_PER_CHAN, 0, 0x01};
//     hdma_tbl_wr[2] = (struct hdma_info_ex) {0, 0, 0x0, 0x0, 0x0, 0, 0, 0, 0};

//     hdma_tbl_rd[0] = (struct hdma_info_ex) {0, 0, DBSC4_HDMA(0), DBSC0_HDMA(2), 0x7FF00000, 1, DMA_SIZE_PER_CHAN, 0, 0x00};
//     hdma_tbl_rd[1] = (struct hdma_info_ex) {0, 1, DBSC4_HDMA(1), DBSC0_HDMA(3), 0x7FF01000, 1, DMA_SIZE_PER_CHAN, 0, 0x01};
//     hdma_tbl_rd[2] = (struct hdma_info_ex) {0, 0, 0x0, 0x0, 0x0, 0, 0, 0, 0};  
// #endif

#if 0
    printf("UCIe hardware settings for loopback\n");

    ret = ucie_hwemu_hw_init_loopback();
    if (ret)
    {
        printf("---- INIT ERROR, TEST DONE -----\n");
	    goto error;
    }
#else
    printf("----- PROGRAM START -----\n");
	uint32_t *w;
	w = (uint32_t *)(0xbffff000);
	wait_time(w[0]);

    ucie_hwemu_hw_ai_acc_init();

	wait_time(w[1]);
	vTaskDelay(pdMS_TO_TICKS(1));

	wait_time(w[2]);
	vTaskDelay(pdMS_TO_TICKS(2));

	wait_time(w[3]);
    vTaskDelay(pdMS_TO_TICKS(5));

	wait_time(w[4]);
    vTaskDelay(pdMS_TO_TICKS(5));
    vTaskDelay(pdMS_TO_TICKS(5));

	wait_time(w[5]);
    vTaskDelay(pdMS_TO_TICKS(10));

	wait_time(w[6]);
    vTaskDelay(pdMS_TO_TICKS(10));
    vTaskDelay(pdMS_TO_TICKS(10));

	wait_time(w[7]);

    printf("----- PROGRAM END -----\n");
    goto error;
#endif

	/*====================================*/
	/* UCIE memory area                   */
	/*====================================*/
	ucie0_mem = 0x20000000000; // D2D (UCIe ch0/no coherent (CXL))
	ucie1_mem = 0x24000000000; // D2D (UCIe ch1/no coherent (CXL))

	ucie0_tgt = 0x4E000000; // UCIE0 INBOUND: translate from 0x24000000000 to 0x4E000000
	// ucie1_tgt = 0x4F000000; // UCIE1 INBOUND: translate from 0x20000000000 to 0x4F000000
    ucie1_tgt = 0x53000000; // UCIE1 INBOUND: translate from 0x20000000000 to 0x53000000

	/*====================================*/
	/* inbound setting                    */
	/*====================================*/

    /*** ch0 ***/
//+++<ANH001>
    /* ANH001: this function won't work because
    // SoC memory map limits CR52 to 32-bit physical address access (e.g. 4 GB range),
    // then passing addresses > 0xFFFFFFFF won’t work. */
    // base_addr = 0x24000000000;
    // target_addr = 0x4E000000;
    // ucie_hwemu_init_inbound(0, base_addr, target_addr);

	/*** ch0 ***/
	// val = ucie_hwemu_read(0xD8300510) | 0x00FFFFF0; // IATU LIMIT_ADDR_OFF_INBOUND_2
    val = ucie_hwemu_read(0xD8300510) | 0x04FFFFF0; // IATU LIMIT_ADDR_OFF_INBOUND_2
	ucie_hwemu_write(0xD8300510, val);			    // LIMIT_ADD_RW -> 80MB //16MB

	ucie_hwemu_write(0xD8300508, (ucie1_mem & 0xFFFFFFFF)); //; IATU LWR_BASE_ADDR_OFF_INBOUND_2
	ucie_hwemu_write(0xD830050C, (ucie1_mem >> 32));		  //; IATU UPPER_BASE_ADDR_OFF_INBOUND_2
	ucie_hwemu_write(0xD8300514, ucie0_tgt);				  //; IATU LWR_TARGET_ADDR_OFF_INBOUND_2
	ucie_hwemu_write(0xD8300518, 0x00000000);				  //; IATU UPPER_TARGET_ADDR_OFF_INBOUND_2
	ucie_hwemu_write(0xD8300500, 0x00000000);				  //; IATU REGION_CTRL_1_OFF_INBOUND_2
	ucie_hwemu_write(0xD8300504, 0x80000000);				  //; IATU REGION_CTRL_2_OFF_INBOUND_2
													  //;   [31] REDION_EN=1, [30] MATCH_MODE=0 (Address Match Mode)

    vTaskDelay(MS_TO_TICKS(1));
//---<ANH001>

//+++<ANH001>
    // base_addr = 0x20000000000;
    // target_addr = 0x4F000000;
    // ucie_hwemu_init_inbound(1, base_addr, target_addr);
	/*** ch1 RC ***/
	// val = ucie_hwemu_read(0xD9300510) | 0x00FFFFF0; // IATU LIMIT_ADDR_OFF_INBOUND_2
    val = ucie_hwemu_read(0xD8300510) | 0x04FFFFF0; // IATU LIMIT_ADDR_OFF_INBOUND_2
	ucie_hwemu_write(0xD9300510, val);			    // LIMIT_ADD_RW -> 80MB //16MB

	ucie_hwemu_write(0xD9300508, (ucie0_mem & 0xFFFFFFFF)); //; IATU LWR_BASE_ADDR_OFF_INBOUND_2
	ucie_hwemu_write(0xD930050C, (ucie0_mem >> 32));		  //; IATU UPPER_BASE_ADDR_OFF_INBOUND_2
	ucie_hwemu_write(0xD9300514, ucie1_tgt);				  //; IATU LWR_TARGET_ADDR_OFF_INBOUND_2
	ucie_hwemu_write(0xD9300518, 0x00000000);				  //; IATU UPPER_TARGET_ADDR_OFF_INBOUND_2
	ucie_hwemu_write(0xD9300500, 0x00000000);				  //; IATU REGION_CTRL_1_OFF_INBOUND_2
	ucie_hwemu_write(0xD9300504, 0x80000000);				  //; IATU REGION_CTRL_2_OFF_INBOUND_2
													  //;   [31] REDION_EN=1, [30] MATCH_MODE=0 (Address Match Mode)

    vTaskDelay(MS_TO_TICKS(1));
//---<ANH001>

    /* Fill random test data */
    // printf("UCIE memset 0 - START \n");
    // memset_fast32((uint32_t)DBSC0_HDMA(2), ZERO_DATA, DMA_SIZE_PER_CHAN);
    // memset_fast32((uint32_t)DBSC0_HDMA(3), ZERO_DATA, DMA_SIZE_PER_CHAN);
    // memset_fast32((uint32_t)0x80200000, 0, DMA_SIZE_PER_CHAN);
    // memset_fast32((uint32_t)0x80300000, 0, DMA_SIZE_PER_CHAN);

    printf("UCIE generate data - START \n");
    memset_fast32((uint32_t)DBSC01_HDMA(0), TEST_DATA0, DMA_SIZE_PER_CHAN);
    memset_fast32((uint32_t)DBSC01_HDMA(1), TEST_DATA1, DMA_SIZE_PER_CHAN);
    printf("UCIE generate data - END \n");
    vTaskDelay(pdMS_TO_TICKS(1));

    // wait_time(8192U);

    /* WRCHx2
     *    UCIE write: from 0xB0000000 to 0x145A000000
     *                from 0xB1000000 to 0x145B000000
     */
    printf("\nStarting UCIe HDMA WRCHx2\n");
    ucie_xfer(hdma_tbl_wrtest_dt);

    /* WRCHx4
     *    UCIE write: from 0xB0000000 to 0x185A000000
     *                from 0xB1000000 to 0x185B000000
     *                from 0x145A000000 to 0x1C5A000000
     *                from 0x145B000000 to 0x1C5B000000
     */
    printf("\nStarting UCIe HDMA WRCHx4\n");
    ucie_xfer(hdma_tbl_wr);

    /* RDCHx4
     *    UCIE read: from 0x185A000000 to 0xB2000000
     *                from 0x185B000000 to 0xB3000000
     *                from 0x1C5A000000 to 0x145C000000
     *                from 0x1C5B000000 to 0x145D000000
     */
    printf("\nStarting UCIe HDMA RDCHx4\n");
    ucie_xfer(hdma_tbl_rd);

    /* WRCHx2
     *    UCIE write: to 0xB4000000 from 0x145C000000
     *                to 0xB5000000 from 0x145D000000
     */
    printf("\nStarting UCIe HDMA WRCHx2\n");
    ucie_xfer(hdma_tbl_wr_verify_dt);

    printf("\n----- CHECK LOOPBACK RESULT -----\n");
    // Print first 16 bytes of buffer to verify
    // Expected result: DBSC01_HDMA(0) = DBSC01_HDMA(2) =  DBSC01_HDMA(4)
    //                  DBSC01_HDMA(1) = DBSC01_HDMA(3) =  DBSC01_HDMA(5)
    print_buffer((uint32_t)DBSC01_HDMA(0), 16);
    print_buffer((uint32_t)DBSC01_HDMA(2), 16);
    print_buffer((uint32_t)DBSC01_HDMA(4), 16);
    printf("\n");
    print_buffer((uint32_t)DBSC01_HDMA(1), 16);
    print_buffer((uint32_t)DBSC01_HDMA(3), 16);
    print_buffer((uint32_t)DBSC01_HDMA(5), 16);

    printf("----- TEST DONE -----\n");

error:
    // if(hdma_tbl_rd) vPortFree(hdma_tbl_rd);
    // if(hdma_tbl_wr) vPortFree(hdma_tbl_wr);
    while (1);
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
