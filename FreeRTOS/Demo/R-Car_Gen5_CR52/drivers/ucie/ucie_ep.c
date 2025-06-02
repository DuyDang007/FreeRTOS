/*
* Copyright (c) 2025 Renesas Electronics Corporation
*
* SPDX-License-Identifier: MIT
*/

#include "pcie/r_pcie_ep.h"
#include "ucie.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"

#define MSI_LOCAL_ADDR   ((volatile uint32_t *)0x60000000)
#define MSI_MSG_DATA     0x0000

/* Implement functions for Endpoint */

int R_PCIE_EP_Outbound_ATU(struct st_pcie_ep *ep, uint16_t channel)
{
    uint32_t free_win = 0;
    uint32_t index, bit_pos;

    /* Outbound ATU configuration */
    R_UCIE_RegWrite32(channel, UCIE_OB_ATU_LOWER_BASE, 0x40000000);
    R_UCIE_RegWrite32(channel, UCIE_OB_ATU_UPPER_BASE, 0);
    R_UCIE_RegWrite32(channel, UCIE_OB_ATU_LIMIT_BASE, 0x40000000 + 0xffff);
    R_UCIE_RegWrite32(channel, UCIE_OB_ATU_LOWER_TARGET, UCIE_D2D_CH1_LOWER);
    R_UCIE_RegWrite32(channel, UCIE_OB_ATU_UPPER_TARGET, UCIE_D2D_CH1_UPPER);
    R_UCIE_RegWrite32(channel, UCIE_OB_REGION_CTL1, 0);
    R_UCIE_RegWrite32(channel, UCIE_OB_REGION_CTL2, 0x80000000);

    R_UCIE_RegWrite32(channel, UCIE_PCICONF1, 0x100007);
    
    return 0;
}

int R_PCIE_DMAtransfer(uint16_t channel, uint32_t dma_wr_sar, uint32_t dma_rd_dar)
{
    /* Write config */
    R_UCIE_RegWrite32(channel, UCIE_DMA_WR_EN, 0x1);
    R_UCIE_RegWrite32(channel, UCIE_DMA_WR_INT_SET, 0x50);
    R_UCIE_RegWrite32(channel, UCIE_DMA_WR_CTL, 0x2);
    R_UCIE_RegWrite32(channel, UCIE_DMA_WR_FUNC_NUM, 0x0);
    R_UCIE_RegWrite32(channel, UCIE_DMA_WR_QOS, 0x0);
    R_UCIE_RegWrite32(channel, UCIE_DMA_WR_SIZE, 0x00001000);
    R_UCIE_RegWrite32(channel, UCIE_DMA_WR_SAR_LOW, dma_wr_sar);
    R_UCIE_RegWrite32(channel, UCIE_DMA_WR_SAR_HIGH, 0);
    R_UCIE_RegWrite32(channel, UCIE_DMA_WR_DAR_LOW, UCIE_D2D_CH0_LOWER);
    R_UCIE_RegWrite32(channel, UCIE_DMA_WR_DAR_HIGH, UCIE_D2D_CH0_UPPER);

    /* DoorBell to start transfer */
    /* DMA write channel 0 doorbell */
    R_UCIE_RegWrite32(channel, UCIE_DMA_WR_DOORBELL, 0x00000001);

    /* Clear interrupt */
    R_UCIE_RegWrite32(channel, UCIE_DMA_WR_INT_CLR, 0x1);

    /* Read config */
    R_UCIE_RegWrite32(channel, UCIE_DMA_RD_EN, 0x1);
    R_UCIE_RegWrite32(channel, UCIE_DMA_RD_INT_SET, 0x50);
    R_UCIE_RegWrite32(channel, UCIE_DMA_RD_CTL, 0x2);
    R_UCIE_RegWrite32(channel, UCIE_DMA_RD_FUNC_NUM, 0x0);
    R_UCIE_RegWrite32(channel, UCIE_DMA_RD_QOS, 0x0);
    R_UCIE_RegWrite32(channel, UCIE_DMA_RD_SIZE, 0x00001000);
    R_UCIE_RegWrite32(channel, UCIE_DMA_RD_SAR_LOW, UCIE_D2D_CH0_LOWER);
    R_UCIE_RegWrite32(channel, UCIE_DMA_RD_SAR_HIGH, UCIE_D2D_CH0_UPPER);
    R_UCIE_RegWrite32(channel, UCIE_DMA_RD_DAR_LOW, dma_rd_dar);
    R_UCIE_RegWrite32(channel, UCIE_DMA_RD_DAR_HIGH, 0);

    /* DoorBell to start transfer */
    /* DMA read channel 0 doorbell */
    R_UCIE_RegWrite32(channel, UCIE_DMA_RD_DOORBELL, 0x00000001);

    /* Clear interrupt */
    R_UCIE_RegWrite32(channel, UCIE_DMA_RD_INT_CLR, 0x1);

    return 0;
}

int R_PCIE_EP_Init(struct st_pcie_ep *ep, uint16_t channel)
{
    uint32_t val;
    uint32_t scaling_factor = 0x3 << 29;

    // FAST_LINK_SCALING_FACTOR
    val = R_UCIE_RegRead32(channel, UCIE_TIMERCTL_MAXFUNC) & ~(FASTLINK_SCALING_FACTOR);
    val |= scaling_factor;
    R_UCIE_RegWrite32(channel, UCIE_TIMERCTL_MAXFUNC, val);

    // set PORT_LINK_CTRL.LINK_CAPABLE[21:16] = 0x7 & set FAST_LINK_MODE
    val = R_UCIE_RegRead32(channel, UCIE_PORT_LINK_CONTROL) & ~(PORT_LINK_MODE_MASK);
    val |= UCIE_LINK_X4;
    val |= UCIE_FAST_LINK_MODE;
    R_UCIE_RegWrite32(channel, UCIE_PORT_LINK_CONTROL, val);

    // set GEN2_CTRL.NUM_OF_LANES[12:8] = 0x400
    val = R_UCIE_RegRead32(channel, UCIE_LINK_WIDTH_SPEED_CONTROL) & ~(PORT_LOGIC_LINK_WIDTH_MASK);
    val |= PORT_LOGIC_LINK_WIDTH_4_LANES;
    R_UCIE_RegWrite32(channel, UCIE_LINK_WIDTH_SPEED_CONTROL, val);

    // set INSERT_LANE_SKEW
    R_UCIE_Write_1(UCIE1_CXL_AXI_BASE+ UCIE_LANE_SKEW, UCIE_DUAL_LANE_MOD);

    R_UCIE_Write_1(UCIE1_CXL_AXI_BASE + UCIE_MISC_CONTROL_1_OFF, UCIE_DBI_RO_WR_EN);
    R_UCIE_Write_1(UCIE1_CXL_AXI_BASE + UCIE_VENDOR_EXT_CAP, 0x02);

    /* Enable Interrupt smlh_link_up enable and Interrupt rdlh_link_up */
    R_UCIE_Write_1(UCIE0_CXL_APB_BASE + 0x002C0, 0x000000C0);
    R_UCIE_Write_1(UCIE1_CXL_APB_BASE + 0x002C0, 0x000000C0);

    /* Enable ltssm */
    R_UCIE_Write_1(UCIE0_CXL_APB_BASE + 0x0001C, 0x1);
    R_UCIE_Write_1(UCIE1_CXL_APB_BASE + 0x0001C, 0x1);

    return 0;
}
