/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef UCIE_HDMA_H_
#define UCIE_HDMA_H_

#include <stdint.h>
#include <stdbool.h>

// #include "ucie_drv_port.h"

// -----------------------------------------------------------------------------
// UCIE DMA Transfer Info Structure
// -----------------------------------------------------------------------------
/**
 * @brief Extended structure to describe a UCIE HDMA transaction.
 */
struct hdma_info_ex {
	uint32_t ucieCh;	// 0 or 1
	uint32_t dmaCh;		// 0 - 31
	uint64_t sar;
	uint64_t dar;
	uint64_t llp;		// Linked List Pointer Address
	uint32_t rw;		// Use RDCH(1), WRCH(0)
	uint32_t size;		// Transfer Size (byte)
	uint8_t  tc;		// Traffic Class (0 - 7)
	uint8_t  weight;	// weight (0 - 31)
};

// -----------------------------------------------------------------------------
// Return Status Codes
// -----------------------------------------------------------------------------
// #define	NORMAL_END	(0)
// #define	ERROR		(1)
// #define	ERROR_END	(1)

// -----------------------------------------------------------------------------
// UCIE Channel Definitions
// -----------------------------------------------------------------------------
// #define UCIE_CH0    (0)
// #define UCIE_CH1    (1)

// #define UCIE_EP		(0)
// #define	UCIE_RC		(1)

// // -----------------------------------------------------------------------------
// // Time Conversion
// // -----------------------------------------------------------------------------
// // #define MS_TO_TICKS(ms)   ((ms * configTICK_RATE_HZ) / 1000)

// // -----------------------------------------------------------------------------
// // Address Mapping Macros
// // -----------------------------------------------------------------------------
// #define UCIE_AXI_BASE(n)	(0xD8000000 + (n) * 0x1000000)
// #define UCIE_APB_BASE(n)	(0xDC000000 + (n) * 0x1000000)

// -----------------------------------------------------------------------------
// Function Declarations
// -----------------------------------------------------------------------------

void ucie_pwrmngctrl_reg_modify(uint32_t address);

/**
 * @brief Write a 32-bit value to a UCIE register.
 * 
 * @param addr Register address
 * @param val  Value to write
 */
void ucie_hwemu_write(uint32_t addr, uint32_t val);

/**
 * @brief Read a 32-bit value from a UCIE register.
 * 
 * @param addr Register address
 * @return uint32_t Value read
 */
uint32_t ucie_hwemu_read(uint32_t addr);

/**
 * @brief Delay function
 * 
 * @param count Loop count
 * @return none
 */
void wait_time(uint32_t count);

// /**
//  * @brief Initialize the UCIE hardware as Root Complex (RC) or Endpoint (EP).
//  * 
//  * @param ucie_chan Channel index (0 or 1)
//  * @param is_rc     true for RC mode, false for EP mode
//  * @return int      0 on success, 1 on error
//  */
// uint32_t ucie_hwemu_hw_init(uint32_t ucie_chan, bool is_rc);

// /**
//  * @brief Initialize the UCIE hardware clock.
//  * 
//  * @return none
//  */
// void ucie_hwemu_hw_ai_acc_init(void);

// /**
//  * @brief Initialize UCIE AI-ACC stand-alone in loopback mode
//  * 
//  * @return int 0 on success, 1 on error
//  */
// uint32_t ucie_hwemu_hw_init_loopback(void);

/**
 * @brief Start a HDMA transfer.
 * 
 * @param ucie_chan UCIE channel
 * @param dma_chan  DMA channel
 * @param rw        Direction: 1 = Read, 0 = Write
 */
void ucie_hwemu_hdma_start(uint32_t ucie_chan, uint32_t dma_chan, uint32_t rw);

/**
 * @brief Configure HDMA registers using a structured configuration.
 * 
 * @param info Pointer to the HDMA info structure
 */
void ucie_hwemu_hdma_set_regsLL(const struct hdma_info_ex *info, uint32_t custom_size);

/**
 * @brief Stop a HDMA transfer.
 * 
 * @param ucie_chan UCIE channel
 * @param dma_chan  DMA channel
 * @param rw        Direction: 1 = Read, 0 = Write
 */
void ucie_hwemu_hdma_stop(uint32_t ucie_chan, uint32_t dma_chan, uint32_t rw);

/**
 * @brief Wait until HDMA transfer is complete and the channel has stopped.
 * 
 * @param ucie_chan UCIE channel
 * @param dma_chan  DMA channel
 * @param rw        Direction: 1 = Read, 0 = Write
 * @return int      0 if completed successfully, 1 on timeout/error
 */
uint32_t ucie_hwemu_hdma_wait_stop(uint32_t ucie_chan, uint32_t dma_chan, uint32_t rw);

#endif // UCIE_HDMA_H_
