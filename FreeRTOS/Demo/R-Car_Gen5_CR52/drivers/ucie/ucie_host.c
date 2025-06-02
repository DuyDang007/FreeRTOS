/*
* Copyright (c) 2025 Renesas Electronics Corporation
*
* SPDX-License-Identifier: MIT
*/

#include "pcie/r_pcie_host.h"
#include "ucie.h"

#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "interrupts.h"
/* Implement functions for Host */

int ucie_link_up(uint16_t channel)
{
    uint32_t val;

    val = R_UCIE_RegRead32(channel, UCIE_PORT_DEBUG1);
    return ((val & UCIE_PORT_DEBUG1_LINK_UP) &&
              (!(val & UCIE_PORT_DEBUG1_LINK_TRAINING)));
}

int R_PCIE_Host_Inbound_ATU(uint16_t channel)
{
    /* set max payload to 1024byte */
    R_UCIE_RegWrite32(channel, UCIE_EXCAP2, 0x102970);

    /* PORT_LOGIC TRGT_MAP_CTRL_OFF */
    R_UCIE_RegWrite32(channel, UCIE_PRTLGC24, 0x40);

    /* Inbound ATU configuration */
    R_UCIE_RegWrite32(channel, UCIE_IB_ATU_LOWER_BASE, UCIE_D2D_CH0_LOWER);
    R_UCIE_RegWrite32(channel, UCIE_IB_ATU_UPPER_BASE, UCIE_D2D_CH0_UPPER);
    R_UCIE_RegWrite32(channel, UCIE_IB_ATU_LIMIT_BASE, UCIE_D2D_CH0_LOWER + 0xffff);
    R_UCIE_RegWrite32(channel, UCIE_IB_ATU_LOWER_TARGET, 0x50000000);
    R_UCIE_RegWrite32(channel, UCIE_IB_ATU_UPPER_TARGET, 0);
    R_UCIE_RegWrite32(channel, UCIE_IB_REGION_CTL1, 0x00000000);
    R_UCIE_RegWrite32(channel, UCIE_IB_REGION_CTL2, 0x80000000);

    /* bus master enable , memory space enable , IO space enable */
    R_UCIE_RegWrite32(channel, UCIE_PCICONF1, 0x100007);

    return 0;
}

int R_PCIE_InitHost(struct st_pcie_host *host, uint16_t channel)
{
    uint32_t ret, val;
    uint32_t scaling_factor = 0x3 << 29;
    uint32_t dma_rd_dar, dma_wr_sar;

    // set PCIE_CAP_TARGET_LINK_SPEED to Gen6 speed
    val = R_UCIE_RegRead32(channel, EXPCAP12) & ~(UCIE_EXP_TARGET_SPEED);
    val |= 0x6;
    R_UCIE_RegWrite32(channel, EXPCAP12, val);

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
    R_UCIE_Write_1(UCIE0_CXL_AXI_BASE + UCIE_LANE_SKEW, UCIE_DUAL_LANE_MOD);

    R_UCIE_Write_1(UCIE0_CXL_AXI_BASE + UCIE_MISC_CONTROL_1_OFF, UCIE_DBI_RO_WR_EN);
    R_UCIE_Write_1(UCIE0_CXL_AXI_BASE + UCIE_VENDOR_EXT_CAP, 0x02);

    return 0;
}
