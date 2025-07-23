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

#include "rcar_ucie.h"
#include "module_controller.h"
#include "clock_controller.h"
#include "ucie_hwemu.h"
#include "ucie_drv_port.h"

#include "ucie_rtdmac.h"

// -----------------------------------------------------------------------------
//  Macro definitions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Timeout Configuration
// -----------------------------------------------------------------------------
#define	TIMEOUT_EXECUTE_LOOP_COUNT (30000U)	/**< Timeout loop count */

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
	mem_write32(DMAC_RDM_CHCR(i,ch),		0x0);
	mem_write32(DMAC_RDM_CHCLR(i,ch),	RTDMAC_CHCLR_CLR);

	/* Set RT-DMAC */
	mem_write32(DMAC_RDM_SAR(i,ch),		(uint32_t)info->sar);					// SAR(Lower)
	mem_write32(DMAC_RDM_FIXSAR(i,ch),	(uint32_t)((info->sar >> 32) & 0x7FF));	// SAR(Upper)
	mem_write32(DMAC_RDM_DAR(i,ch),		(uint32_t)info->dar);					// DAR(Lower)
	mem_write32(DMAC_RDM_FIXDAR(i,ch),	(uint32_t)((info->dar >> 32) & 0x7FF));	// DAR(Upper)
	mem_write32(DMAC_RDM_TCR(i,ch),		0x4000);								// TCR(=1MB/64)

	val  = mem_read32(DMAC_RDM_RDMBUFCR(i,ch)) & (0x000007FF);
	val |= 0x01000000;													// MBU=256byte
	mem_write32(DMAC_RDM_RDMBUFCR(i,ch), val);

	val  = RTDMAC_CHCR_TS_64;											// 64-byte units transfer
	val |= (info->dmsm <<12);											// Dst,Src address mode
	val |= RTDMAC_CHCR_RS_AUTOREQ;										// Resource selection - 0100: Auto request
	mem_write32(DMAC_RDM_CHCR(i,ch), val);

	/* Enable channel */
	val |= RTDMAC_CHCR_DE;
	mem_write32(DMAC_RDM_CHCR(i,ch), val);
}

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
{
	uint32_t ret = NORMAL_END;
	uint32_t timeout = 0U;

	/* Wait TE */
	while ((ucie_hwemu_read(DMAC_RDM_CHCR(i, chNo)) & RTDMAC_CHCR_TE) != RTDMAC_CHCR_TE)
	{
		if (timeout >= TIMEOUT_EXECUTE_LOOP_COUNT) 
		{
			printf("** RT-DMAC%d ch%d Transfer Timeout **\n", i, chNo);
			ret = ERROR_END;
			break;
		}
        if ((timeout % 10000U) == 0) 
        {  // Print dot every 10000 iterations
            printf(". ");
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
uint32_t Rtdmac_Transfer(uint32_t chNo, uint64_t srcAddr, uint64_t dstAddr, uint32_t dataSize, uint32_t dataUnit, uint32_t mbu);
{
	uint32_t ret = NORMAL_END;
	uint32_t timeout;

	/* Setup RT-DMAC */
	mem_write16(DMAC_RDMOR, 0x0000);			// DMA Master Disable
	Rtdmac_SetRegs(chNo, srcAddr, dstAddr, dataSize, dataUnit, mbu);

	/* Start */
	mem_write16(DMAC_RDMOR, RTDMAC_DMOR_DME);	// DMA Master Enable

	/* Wait TE */
	timeout = 1000;
	while ((mem_read32(DMAC_RDM0_CHCR(chNo)) & RTDMAC_CHCR_TE) != RTDMAC_CHCR_TE) {
		timeout--;
		if (timeout == 0) {
			mprintf("** RT-DMAC Transfer Timeout\n");
			ret = ERROR_END;
			break;
		}
	}

	/* Stop DMA */
	mem_write16(DMAC_RDMOR, 0x0000);			// DMA Master Disable

	return ret;	
}

// -----------------------------------------------------------------------------
// END of FILE
// -----------------------------------------------------------------------------
