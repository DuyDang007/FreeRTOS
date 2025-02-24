/*
* Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "pcie/r_pcie_ep.h"
#include "ucie.h"
#include <string.h>

/* Implement functions for Endpoint */

static void rcar_ucie_ep_hw_enable(uint16_t channel)
{
    rcar_ucie_reg_write32(channel, false, true, IMP_CORECONFIG_CONFIG0, UCIECTL_DEF_EP_EN);

    rcar_ucie_controller_enable(channel);

    //rcar_ucie_phy_enable(channel);
}

int R_PCIE_EP_TransferDataDMA(struct st_pcie_ep *ep, uint64_t pcie_addr,
                          uintptr_t *local_addr, uint32_t size,
                          enum pcie_ob_mem_type ob_mem_type,
                          enum xfer_direction dir)
{
    uint32_t i;
    uint16_t channel = 1;
    uintptr_t addr = (uintptr_t)pcie_addr;
    uint8_t *pcie_data = (uint8_t *)addr;

    if (dir == DEVICE_TO_HOST) {
	/* DMA Write channel 0 enable */
	R_UCIE_RegWrite32(channel, UCIE_DMA_WR_EN, 0x1);

	/* DMA write channel interrupt setup */
	R_UCIE_RegWrite32(channel, UCIE_DMA_WR_INT_SET, 0x50);

	/* DMA write channel control setting 1 */
	R_UCIE_RegWrite32(channel, UCIE_DMA_WR_CTL, 0x2);

	/* DMA write channel function number */
	R_UCIE_RegWrite32(channel, UCIE_DMA_WR_FUNC_NUM, 0x0);

	/* DMA write channel QOS setting */
	R_UCIE_RegWrite32(channel, UCIE_DMA_WR_QOS, 0x0);

	/* DMA write channel transfer size to 1KB */
	R_UCIE_RegWrite32(channel, UCIE_DMA_WR_SIZE, 0x00000400);

	/* DMA write channel SAR low */
	R_UCIE_RegWrite32(channel, UCIE_DMA_WR_SAR_LOW, (uint32_t)local_addr & 0xFFFFFFFF);

	/* DMA write channel SAR high */
	R_UCIE_RegWrite32(channel, UCIE_DMA_WR_SAR_HIGH, 0x00000000);

	/* DMA write channel DAR low */
	R_UCIE_RegWrite32(channel, UCIE_DMA_WR_DAR_LOW, UCIE_D2D_CH0_LOWER);

	/* DMA write channel DAR high */
	R_UCIE_RegWrite32(channel, UCIE_DMA_WR_DAR_HIGH, UCIE_D2D_CH0_UPPER);

	/* DMA write channel 0 doorbell */
	R_UCIE_RegWrite32(channel, UCIE_DMA_WR_DOORBELL, 0x00000001);

	printf_delay("WAIT_INT_WRITE\n");
	while (R_UCIE_RegRead32(channel, UCIE_DMA_WR_INT_STT) && 0x1 == 0x0) {
		i++;
		if (i == 10) {
			printf_delay("TIMEOUT\n");
			break;
		}
	}
	/* Clear interrupt */
	R_UCIE_RegWrite32(channel, UCIE_DMA_WR_INT_CLR, 0x1);

    } else {
	/* DMA Read channel 0 enable */
	R_UCIE_RegWrite32(channel, UCIE_DMA_RD_EN, 0x1);

	/* DMA read channel interrupt setup */
        R_UCIE_RegWrite32(channel, UCIE_DMA_RD_INT_SET, 0x50);

	/* DMA read channel control setting 1 */
        R_UCIE_RegWrite32(channel, UCIE_DMA_RD_CTL, 0x2);

	/* DMA read channel function number */
        R_UCIE_RegWrite32(channel, UCIE_DMA_RD_FUNC_NUM, 0x0);

	/* DMA read channel QOS setting */
        R_UCIE_RegWrite32(channel, UCIE_DMA_RD_QOS, 0x0);

	/* DMA read channel transfer size */
        R_UCIE_RegWrite32(channel, UCIE_DMA_RD_SIZE, 0x00000400);

	/* DMA read channel SAR low */
        R_UCIE_RegWrite32(channel, UCIE_DMA_RD_SAR_LOW, UCIE_D2D_CH0_LOWER);

	/* DMA read channel SAR high  */
        R_UCIE_RegWrite32(channel, UCIE_DMA_RD_SAR_HIGH, UCIE_D2D_CH0_UPPER);

	/* DMA read channel DAR low */
        R_UCIE_RegWrite32(channel, UCIE_DMA_RD_DAR_LOW, (uint32_t)local_addr & 0xFFFFFFFF);

	/* DMA read channel DAR high */
        R_UCIE_RegWrite32(channel, UCIE_DMA_RD_DAR_HIGH, 0x00000000);

	/* DMA read channel 0 doorbell */
        R_UCIE_RegWrite32(channel, UCIE_DMA_RD_DOORBELL, 0x00000001);

	printf_delay("WAIT_INT_READ\n");
	while (R_UCIE_RegRead32(channel, UCIE_DMA_RD_INT_STT) && 0x1 == 0x0) {
                i++;
                if (i == 10) {
                        printf_delay("TIMEOUT\n");
                        break;
		}
        }
	/* Clear interrupt */
        R_UCIE_RegWrite32(channel, UCIE_DMA_RD_INT_CLR, 0x1);
    }

    if (memcmp(pcie_data, local_addr, size) == 0)
	printf_delay("PASS\n");
    else
	printf_delay("FAILED\n");
}

void R_PCIE_EP_Inbound_ATU(uint16_t channel)
{
    /* set max payload to 1024byte */
    R_UCIE_RegWrite32(channel, UCIE_EXCAP2, 0x102970);

    /* PORT_LOGIC TRGT_MAP_CTRL_OFF */
    R_UCIE_RegWrite32(channel, UCIE_PRTLGC24, 0x40);

    /* Inbound ATU configuration */
    R_UCIE_RegWrite32(channel, UCIE_IB_ATU_LOWER_BASE, UCIE_D2D_CH1_LOWER);
    R_UCIE_RegWrite32(channel, UCIE_IB_ATU_UPPER_BASE, UCIE_D2D_CH1_UPPER);
    R_UCIE_RegWrite32(channel, UCIE_IB_ATU_LIMIT_BASE, UCIE_D2D_CH1_LOWER + 0xffff);
    R_UCIE_RegWrite32(channel, UCIE_IB_ATU_LOWER_TARGET, 0x64000000);
    R_UCIE_RegWrite32(channel, UCIE_IB_ATU_UPPER_TARGET, 0);
    R_UCIE_RegWrite32(channel, UCIE_IB_REGION_CTL1, 0);
    R_UCIE_RegWrite32(channel, UCIE_IB_REGION_CTL2, 0x80000000);

    /* bus master enable , memory space enable , IO space enable */
    R_UCIE_RegWrite32(channel, UCIE_PCICONF1, 0x100007);
}

void R_PCIE_EP_Init(struct st_pcie_ep *ep, uint16_t channel)
{
    /* Init a controller in Endpoint mode */

    /* FIXME: Confirm the used of these registers */
/*  writel(0x00004141, 0xDCE005E8);
    writel(0x00004141, 0xDDE005E8);

    R_UCIE_RegWrite32(channel, 0xF01000, 0x08010000); //MmInitCtrl
    R_UCIE_RegWrite32(channel, 0xF0211C, 0x0003BF85); //AcsmLtmlndex0Var1
    R_UCIE_RegWrite32(channel, 0xF0212C, 0x00043FED); //AcsmLtmlndex0Var5
    R_UCIE_RegWrite32(channel, 0xF0214C, 0x000CF816); //AcsmLtmlndex0Var13

    R_UCIE_RegWrite32(channel, 0x118, 0x0000E1E0);
    R_UCIE_RegWrite32(channel, 0x10118, 0x0000E1E0);
    R_UCIE_RegWrite32(channel, 0, 0xABCD16C3);
    R_UCIE_RegWrite32(channel, 0x1C0, 0x00000200);
    R_UCIE_RegWrite32(channel, 0xC48, 0x00800000);
    R_UCIE_RegWrite32(channel, 0x448, 0x00000007);
    R_UCIE_RegWrite32(channel, 0x70, 0x8002B010);
    R_UCIE_RegWrite32(channel, 0x8BC, 0x040BFF48);
*/
    rcar_ucie_ep_hw_enable(channel);
}
