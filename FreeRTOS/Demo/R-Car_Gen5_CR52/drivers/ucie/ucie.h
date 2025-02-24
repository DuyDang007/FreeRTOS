/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

/* Define common, private things here */

#include <stdbool.h>

#define UCIE_CXL_CH0_BASE (0xd8000000u)
#define UCIE_CXL_CH1_BASE (0xd9000000u)

#define UCIE_D2D_CH0_LOWER		(0x00000000U)
#define UCIE_D2D_CH0_UPPER		(0x00000200U)
#define UCIE_D2D_CH1_LOWER		(0x00000000U)
#define UCIE_D2D_CH1_UPPER		(0x00000240U)

#define OPCODE				0x1F
#define OPCODE_MEM_READ32		0
#define OPCODE_MEM_WRITE32		0x1
#define OPCODE_CONF_READ32		0x4
#define OPCODE_CONF_WRITE32		0x5

#define BYTE_ENABLES			0x3FC000
#define BYTE_ENABLES_32			(0xf << 14)

#define SRCID				0xE0000000
#define SRCID_PROTO_STACK0_ACCESS	(0 << 29)

#define DSTID				0x7000000
#define DSTID_PROTO_STACK_ACCESS	(0x1 << 24)
#define DSTID_PHY_STACK_ACCESS		(0x2 << 24)

#define CONTROL_PARITY(n)		((n) << 30)
#define DATA_PARITY(n)			((n) << 31)

/* APB registers */
#define APB_BRIDGE_CTL0			0x0100
#define APB_BRIDGE_CTL1			0x0104
#define APB_BRIDGE_CTL2			0x0108
#define APB_BRIDGE_CTL3			0x010c

/* Adapter registers */
#define IMP_CORECONFIG_CONFIG0		0x280030
#define UCIECTL_DEF_RP_EN		0x1
#define UCIECTL_DEF_EP_EN		0x2

#define IMP_SB_CONFIG0			0x282000
#define IMP_SB_CONFIG2			0x282008
#define IMP_SB_CONFIG4			0x282010

/* DVSEC_UNIT base: Addr[23:0] should be 0x000_0000 instead of 0x1000_0000 */
#define DVSEC_UCIE_LINK_CONTROL		0x000010

#define UCIE_PORT_DEBUG1		0x72C
#define UCIE_PORT_DEBUG1_LINK_UP	0x10
#define UCIE_PORT_DEBUG1_LINK_TRAINING	0x20000000

/* DBI registers */
#define UCIE_EXCAP2			0x00078
#define UCIE_PRTLGC24			0x0081c

#define UCIE_PCICONF1			0x00004
#define UCIE_PCICONF7			0x0001c
#define UCIE_PCICONF8			0x00020
#define UCIE_PCICONF9			0x00024
#define UCIE_PCICONF10			0x00028
#define UCIE_PCICONF11			0x0002C
#define UCIE_PCICONF12			0x00030

/* ATU registers */
#define UCIE_IB_ATU_LOWER_BASE		0x300108
#define UCIE_IB_ATU_UPPER_BASE		0x30010c
#define UCIE_IB_ATU_LIMIT_BASE		0x300110
#define UCIE_IB_ATU_LOWER_TARGET	0x300114
#define UCIE_IB_ATU_UPPER_TARGET	0x300118
#define UCIE_IB_REGION_CTL1		0x300100
#define UCIE_IB_REGION_CTL2		0x300104

#define UCIE_OB_ATU_LOWER_BASE		0x300008
#define UCIE_OB_ATU_UPPER_BASE		0x30000c
#define UCIE_OB_ATU_LIMIT_BASE		0x300010
#define UCIE_OB_ATU_LOWER_TARGET	0x300014
#define UCIE_OB_ATU_UPPER_TARGET	0x300018
#define UCIE_OB_REGION_CTL1		0x300000
#define UCIE_OB_REGION_CTL2		0x300004

/* DMA registers */
#define UCIE_DMA_WR_EN			0x380000
#define UCIE_DMA_WR_INT_SET		0x380088
#define UCIE_DMA_WR_CTL			0x380034
#define UCIE_DMA_WR_FUNC_NUM		0x380038
#define UCIE_DMA_WR_QOS			0x38003c
#define UCIE_DMA_WR_SIZE		0x38001c
#define UCIE_DMA_WR_SAR_LOW		0x380020
#define UCIE_DMA_WR_SAR_HIGH		0x380024
#define UCIE_DMA_WR_DAR_LOW		0x380028
#define UCIE_DMA_WR_DAR_HIGH		0x38002c
#define UCIE_DMA_WR_DOORBELL		0x380004
#define UCIE_DMA_WR_INT_STT		0x380084
#define UCIE_DMA_WR_INT_CLR		0x38008c
#define UCIE_DMA_RD_EN			0x380400
#define UCIE_DMA_RD_INT_SET		0x380488
#define UCIE_DMA_RD_CTL			0x380434
#define UCIE_DMA_RD_FUNC_NUM		0x380438
#define UCIE_DMA_RD_QOS			0x38043c
#define UCIE_DMA_RD_SIZE		0x38041c
#define UCIE_DMA_RD_SAR_LOW             0x380420
#define UCIE_DMA_RD_SAR_HIGH            0x380424
#define UCIE_DMA_RD_DAR_LOW             0x380428
#define UCIE_DMA_RD_DAR_HIGH            0x38042c
#define UCIE_DMA_RD_DOORBELL            0x380404
#define UCIE_DMA_RD_INT_STT		0x380484
#define UCIE_DMA_RD_INT_CLR             0x38048c

#define SIZE_IN_BYTE			1024 //1KB

extern int printf_delay(const char *format, ...);

void writel(const uint32_t Value, const uintptr_t Address);

uint32_t readl(const uintptr_t Address);

void R_UCIE_RegWrite32(uint16_t channel, uint32_t Offset, uint32_t Value);

uint32_t R_UCIE_RegRead32(uint16_t channel, uint32_t Offset);

bool rcar_ucie_calc_even_parity(uint64_t data);

void rcar_ucie_reg_write32(uint32_t channel, bool phy, bool mem, uint32_t reg, uint32_t data);

void rcar_ucie_controller_enable(uint32_t channel);
/* .... */
