/*
* Copyright (c) 2025 Renesas Electronics Corporation
*
* SPDX-License-Identifier: MIT
*/
#include <stdbool.h>
#include "pcie/r_pcie_ctrl.h"
#include "ucie.h"

/* Implement API functions here */

void writel(const uint32_t Value, const uintptr_t Address)
{
    *((volatile unsigned int*) Address)  = Value;
}

uint32_t readl(const uintptr_t Address)
{
    return *((volatile unsigned int*)Address);
}

void R_UCIE_RegWrite32(uint16_t channel, uint32_t Offset, uint32_t Value)
{
    uint32_t regAddr;

    regAddr = (channel == 0) ? (UCIE_CXL_CH0_BASE + Offset) : (UCIE_CXL_CH1_BASE + Offset);
    *(volatile uint32_t*)regAddr = Value;
}

uint32_t R_UCIE_RegRead32(uint16_t channel, uint32_t Offset)
{
    uint32_t regAddr;

    regAddr = (channel == 0) ? (UCIE_CXL_CH0_BASE + Offset) : (UCIE_CXL_CH1_BASE + Offset);
    return *(volatile uint32_t*)regAddr;
}

bool rcar_ucie_calc_even_parity(uint64_t data)
{
    int i;

    for (i = 32; i > 0; i /= 2)
        data ^= data >> i;

    return (data & 1);
}

void rcar_ucie_reg_write32(uint32_t channel, bool phy, bool mem, uint32_t reg, uint32_t data)
{
    uint32_t phase0, phase1;
    uint64_t val;

    phase0 = mem ? OPCODE_MEM_WRITE32 : OPCODE_CONF_WRITE32;
    phase0 |= BYTE_ENABLES_32 | SRCID_PROTO_STACK0_ACCESS;

    R_UCIE_RegWrite32(channel, APB_BRIDGE_CTL0, phase0);

    phase1 = phy ? DSTID_PHY_STACK_ACCESS : DSTID_PROTO_STACK_ACCESS;
    phase1 |= reg;

    val = ((uint64_t)phase1 << 32) | phase0;
    phase1 |= CONTROL_PARITY(rcar_ucie_calc_even_parity(val));
    phase1 |= DATA_PARITY(rcar_ucie_calc_even_parity(data));

    R_UCIE_RegWrite32(channel, APB_BRIDGE_CTL1, phase1);
    R_UCIE_RegWrite32(channel, APB_BRIDGE_CTL2, data);
    R_UCIE_RegWrite32(channel, APB_BRIDGE_CTL3, 0);
}

void rcar_ucie_controller_enable(uint32_t channel)
{
    /* Write to memory register */
    rcar_ucie_reg_write32(channel, false, true, IMP_SB_CONFIG0, 0xa0190);
    rcar_ucie_reg_write32(channel, false, true, IMP_SB_CONFIG2, 0xa0190);
    rcar_ucie_reg_write32(channel, false, true, IMP_SB_CONFIG4, 0x91);

    /* Write to configuration register */
    rcar_ucie_reg_write32(channel, false, false, DVSEC_UCIE_LINK_CONTROL, 0x01);
}

void R_UCIE_ControllerInit(uint32_t channel, struct st_pcie_ctrl *ctrl)
{
    /* Config UCIe base address, configuration space, BAR address and range,... */
}
