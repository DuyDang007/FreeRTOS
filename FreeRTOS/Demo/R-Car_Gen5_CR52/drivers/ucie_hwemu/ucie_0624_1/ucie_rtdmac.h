/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef UCIE_RTDMA_H_
#define UCIE_RTDMA_H_

#include <stdint.h>
#include <stdbool.h>

// #include "ucie_drv_port.h"

// -----------------------------------------------------------------------------
// RT-DMA Transfer Info Structure
// -----------------------------------------------------------------------------
/**
 * @brief Extended structure to describe a UCIE HDMA transaction.
 */
struct dmac_info_ex {
	uint32_t instance;		// RT-DMAC0-3
	uint32_t chNo;			// ch0-15
	uint64_t sar;
	uint64_t dar;
	uint8_t dmsm;			// Destination address mode(2bit) + Source address mode(2bit)
};

// -----------------------------------------------------------------------------
// Return Status Codes
// -----------------------------------------------------------------------------
#define	NORMAL_END	(0)
#define	ERROR_END	(1)

// -----------------------------------------------------------------------------
//  Macro definitions
// -----------------------------------------------------------------------------

/* RT-DMAC Register Bit */
#define RTDMAC_DMOR_DME			BIT0
#define RTDMAC_CHCR_TS_01		(0x0)
#define RTDMAC_CHCR_TS_02		(BIT3)
#define RTDMAC_CHCR_TS_04		(BIT4)
#define RTDMAC_CHCR_TS_08		(BIT20 | BIT4_3)
#define RTDMAC_CHCR_TS_16		(BIT4_3)
#define RTDMAC_CHCR_TS_32		(BIT20)
#define RTDMAC_CHCR_TS_64		(BIT20 | BIT3)
#define RTDMAC_CHCR_DM_DSTINC	BIT14
#define RTDMAC_CHCR_SM_SRCINC	BIT12
#define RTDMAC_CHCR_RS_AUTOREQ	BIT10
#define RTDMAC_CHCR_TE			BIT1
#define RTDMAC_CHCR_DE			BIT0
#define RTDMAC_CHCLR_CLR		BIT0

// -----------------------------------------------------------------------------
// Function Declarations
// -----------------------------------------------------------------------------

/*******************************************************************************
 * NAME     : Rtdmac_SetRegsEx
 * FUNCTION : Set RT-DMAC registers
 * INPUT    : info		DMAC parameter
 * OUTPUT   : none
 * RETURN   : none
 * REMARK   : transfer size [Byte] is 1MB fixed
 *          : transfer unit size [Byte] is 64byte fixed
 *          : Maximum burst unit [Byte] is 256byte fixed
*******************************************************************************/
void Rtdmac_SetRegsEx(const struct dmac_info_ex *info);

/*******************************************************************************
 * NAME     : Rtdmac_WaitTE
 * FUNCTION : Wait RT-DMAC transfer end
 * INPUT    : i			RT-DMAC instance (0-1)
 *          : chNo		RT-DMAC ch (0-15)
 * OUTPUT   : none
 * RETURN   : NORMAL_END	success
 *          : ERROR_END		transfer timeout
*******************************************************************************/
uint32_t Rtdmac_WaitTE(uint32_t i, uint32_t chNo);

/*******************************************************************************
 * NAME     : Rtdmac_Transfer
 * FUNCTION : Execute RT-DMAC transfer
 * INPUT    : chNo		RT-DMAC00 ch (0-15)
 *          : srcAddr	SAR
 *          : dstAddr	DAR
 *          : dataSize	transfer size [Byte]
 *          : dataUnit	transfer unit size [Byte]
 *          : mbu		Maximum burst unit [Byte]
 * OUTPUT   : none
 * RETURN   : NORMAL_END	success
 *          : ERROR_END		transfer timeout
*******************************************************************************/
uint32_t Rtdmac_Transfer(uint32_t chNo, uint64_t srcAddr, uint64_t dstAddr, uint32_t dataSize, uint32_t dataUnit, uint32_t mbu);

#endif // UCIE_RTDMA_H_
