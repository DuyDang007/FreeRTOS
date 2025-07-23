/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

// #include "rcar_ucie.h"
// #include "module_controller.h"
// #include "clock_controller.h"

#include "reg_x5h_dmac.h"
#include "ucie_dmac.h"
#include "ucie_hwemu.h"

// -----------------------------------------------------------------------------
// Timeout Configuration
// -----------------------------------------------------------------------------
#define	TIMEOUT_EXECUTE_LOOP_COUNT (30000U)	/**< Timeout loop count */

// -----------------------------------------------------------------------------
// /* RT-DMAC Register Bit */
// -----------------------------------------------------------------------------
// Single BIT
#define BIT0		0x0000000000000001
#define BIT1		0x0000000000000002
#define BIT2		0x0000000000000004
#define BIT3		0x0000000000000008
#define BIT4		0x0000000000000010
#define BIT5		0x0000000000000020
#define BIT6		0x0000000000000040
#define BIT7		0x0000000000000080
#define BIT8		0x0000000000000100
#define BIT9		0x0000000000000200
#define BIT10		0x0000000000000400
#define BIT11		0x0000000000000800
#define BIT12		0x0000000000001000
#define BIT13		0x0000000000002000
#define BIT14		0x0000000000004000
#define BIT15		0x0000000000008000
#define BIT16		0x0000000000010000
#define BIT17		0x0000000000020000
#define BIT18		0x0000000000040000
#define BIT19		0x0000000000080000
#define BIT20		0x0000000000100000

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
// Function Body
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
void Rtdmac_SetRegsEx(const struct dmac_info_ex *info)
{
	uint32_t i, ch;
	uint32_t val;

	i  = info->instance;
	ch = info->chNo;

	/* channel clear */
	ucie_hwemu_write(DMAC_RDM_CHCR(i,ch), 0x0);
	ucie_hwemu_write(DMAC_RDM_CHCLR(i,ch), RTDMAC_CHCLR_CLR);

	/* Set RT-DMAC */
	ucie_hwemu_write(DMAC_RDM_SAR(i,ch), (uint32_t)info->sar);						// SAR(Lower)
	ucie_hwemu_write(DMAC_RDM_FIXSAR(i,ch), (uint32_t)((info->sar >> 32) & 0x7FF));	// SAR(Upper)
	ucie_hwemu_write(DMAC_RDM_DAR(i,ch), (uint32_t)info->dar);						// DAR(Lower)
	ucie_hwemu_write(DMAC_RDM_FIXDAR(i,ch),	(uint32_t)((info->dar >> 32) & 0x7FF));	// DAR(Upper)
	ucie_hwemu_write(DMAC_RDM_TCR(i,ch), 0x4000);									// TCR(=1MB/64)

	val  = ucie_hwemu_read(DMAC_RDM_RDMBUFCR(i,ch)) & (0x000007FF);
	val |= 0x01000000;													// MBU=256byte
	ucie_hwemu_write(DMAC_RDM_RDMBUFCR(i,ch), val);

	val  = RTDMAC_CHCR_TS_64;											// 64-byte units transfer
	val |= (info->dmsm <<12);											// Dst,Src address mode
	val |= RTDMAC_CHCR_RS_AUTOREQ;										// Resource selection - 0100: Auto request
	ucie_hwemu_write(DMAC_RDM_CHCR(i,ch), val);

	/* Enable channel */
	val |= RTDMAC_CHCR_DE;
	ucie_hwemu_write(DMAC_RDM_CHCR(i,ch), val);
}

/*******************************************************************************
 * NAME     : Rtdmac_WaitTE
 * FUNCTION : Wait RT-DMAC transfer end
 * INPUT    : i			RT-DMAC instance (0-3)
 *          : chNo		RT-DMAC ch (0-15)
 * OUTPUT   : none
 * RETURN   : NORMAL_END	success
 *          : ERROR_END		transfer timeout
*******************************************************************************/
uint32_t Rtdmac_WaitTE(uint32_t i, uint32_t chNo)
{
    uint32_t ret = NORMAL_END;
    uint32_t timeout = 0U;
    uint32_t reg_val;

    /* Wait for Transfer End (TE) flag */
    while (1)
	{
        reg_val = ucie_hwemu_read(DMAC_RDM_CHCR(i, chNo));
        if ((reg_val & RTDMAC_CHCR_TE) != RTDMAC_CHCR_TE)
		{
            break;  // TE detected
        }
        if (timeout >= TIMEOUT_EXECUTE_LOOP_COUNT)
		{
            printf("** RT-DMAC%d ch%d Transfer Timeout (CHCR=0x%08X)\n", i, chNo, reg_val);
            ret = ERROR_END;
            break;
        }
		timeout++;
    }

    return ret;
}

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
// uint32_t Rtdmac_Transfer(uint32_t chNo, uint64_t srcAddr, uint64_t dstAddr, uint32_t dataSize, uint32_t dataUnit, uint32_t mbu)
uint32_t Rtdmac_Transfer(const struct dmac_info_ex *rtdma_tbl)
{
	uint32_t ret = NORMAL_END;
	uint32_t timeout = 0U;
	uint32_t index = 0U;

	/* Setup RT-DMAC */
	ucie_hwemu_write16(DMAC_RDMOR, 0x0000);			// DMA Master Disable

	index = 0;
	while (rtdma_tbl[index].sar != 0x0)
	{
		Rtdmac_SetRegsEx(&rtdma_tbl[index]);
		index++;
	}

	/* Start */
	ucie_hwemu_write16(DMAC_RDMOR, RTDMAC_DMOR_DME);	// DMA Master Enable

	/* Wait RT-DMAC Transfer End */
	index = 0;
	while (rtdma_tbl[index].sar != 0x0)
	{
		Rtdmac_WaitTE(rtdma_tbl[index].instance, rtdma_tbl[index].chNo);
		index++;
	}

	/* Stop DMA */
	ucie_hwemu_write16(DMAC_RDMOR, 0x0000);			// DMA Master Disable

	return ret;
}

// -----------------------------------------------------------------------------
// END of FILE
// -----------------------------------------------------------------------------
