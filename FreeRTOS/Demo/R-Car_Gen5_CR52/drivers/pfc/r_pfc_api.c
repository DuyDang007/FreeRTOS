/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "stdio.h"
#include "r_pfc_api.h"

#define PFC_INVALID_ADDR           0x0

/* PFC base adrress */
#define PFC_BASE_OFFSET    0x000
#define PFC_GR_0           (0xC1080000 + PFC_BASE_OFFSET)
#define PFC_GR_1           (0xC1080800 + PFC_BASE_OFFSET)
#define PFC_GR_2           (0xC1081000 + PFC_BASE_OFFSET)
#define PFC_GR_3           (0xC0800000 + PFC_BASE_OFFSET)
#define PFC_GR_4           (0xC0800800 + PFC_BASE_OFFSET)
#define PFC_GR_5           (0xC0400000 + PFC_BASE_OFFSET)
#define PFC_GR_6           (0xC0400800 + PFC_BASE_OFFSET)
#define PFC_GR_7           (0xC0401000 + PFC_BASE_OFFSET)
#define PFC_GR_8           (0xC0401800 + PFC_BASE_OFFSET)
#define PFC_GR_9           (0xC9B00000 + PFC_BASE_OFFSET)
#define PFC_GR_10          (0xC9B00800 + PFC_BASE_OFFSET)

/* PFC register: offset address */
#define GP_PMMR             0x000
#define GP_PMMER            0x004
#define GP_PSER             0x008
#define GP_PSSR0            0x00C
#define GP_PSSR1            0x010
#define GP_DMPR0            0x020
#define GP_DMPR1            0x024
#define GP_DMPR2            0x028
#define GP_DMPR3            0x02C
#define GP_GPSR             0x040
#define GP_ALTSEL0          0x060
#define GP_ALTSEL1          0x064
#define GP_ALTSEL2          0x068
#define GP_ALTSEL3          0x06C
#define GP_DRVCTRL0         0x080
#define GP_DRVCTRL1         0x084
#define GP_DRVCTRL2         0x088
#define GP_TDSEL0           0x094
#define GP_TDSEL1           0x098
#define GP_PULLEN           0x0C0
#define GP_PUDSEL           0x0C4
#define GP_MODSEL           0x100

/** Pin function mode */
enum e_rcar_pfc_mode {
    PFC_PERIPHERAL = 0,
    PFC_GPIO,
    PFC_ENABLE_PULL,
    PFC_DISABLE_PULL
};

static uint32_t getPfcRegister(rcar_pfc_group_t grp, uint32_t offset);

static void pfcWrite(rcar_pfc_group_t grp, uint32_t addr, uint32_t val);

static void pfcSetGPSR(uint8_t mode, rcar_pfc_group_t grp, rcar_pfc_pin_t pin);

static void writel(const uint32_t value, const uintptr_t address);

static uint32_t readl(const uintptr_t Address);

static void setbit_l(uint32_t addr, uint32_t pos);

static uint32_t getbit_l(uint32_t addr, uint32_t pos);

static void clearbit_l(uint32_t addr, uint32_t pos);

static void writel(const uint32_t value, const uintptr_t address)
{
    *((volatile unsigned int*) address)  = value;
}

static uint32_t readl(const uintptr_t address)
{
    return *((volatile unsigned int*)address);
}

static void setbit_l(uint32_t addr, uint32_t pos)
{
    writel(readl(addr) | BIT(pos), addr);
}

static uint32_t getbit_l(uint32_t addr, uint32_t pos)
{
    return !!(readl(addr) & BIT(pos));
}

static void clearbit_l(uint32_t addr, uint32_t pos)
{
    uint32_t val = readl(addr);

    writel(val &= ~BIT(pos), addr);
}

static uint32_t getPfcRegister(rcar_pfc_group_t grp, uint32_t offset)
{
    uint32_t base_addr;
    uint32_t reg_addr;

    switch (grp) {
    case 0:
        base_addr = PFC_GR_0;
        break;
    case 1:
        base_addr = PFC_GR_1;
        break;
    case 2:
        base_addr = PFC_GR_2;
        break;
    case 3:
        base_addr = PFC_GR_3;
        break;
    case 4:
        base_addr = PFC_GR_4;
        break;
    case 5:
        base_addr = PFC_GR_5;
        break;
    case 6:
        base_addr = PFC_GR_6;
        break;
    case 7:
        base_addr = PFC_GR_7;
        break;
    case 8:
        base_addr = PFC_GR_8;
        break;
    case 9:
        base_addr = PFC_GR_9;
        break;
    case 10:
        base_addr = PFC_GR_10;
        break;
    default:
        printf("PFC group %d not exist!\n", grp);
        return PFC_INVALID_ADDR;
    }

    reg_addr = base_addr + offset;

    return reg_addr;
}

static void pfcWrite(rcar_pfc_group_t grp, uint32_t addr, uint32_t val)
{
    writel(~val, getPfcRegister(grp, GP_PMMR));
    writel(val, addr);
}

static void pfcSetGPSR(uint8_t gpio, rcar_pfc_group_t grp, rcar_pfc_pin_t pin)
{
    uint32_t val, reg_addr;

    reg_addr = getPfcRegister(grp, GP_GPSR);
    val = readl(reg_addr);
    val = gpio ? val & ~BIT(pin) : val | BIT(pin);
    pfcWrite(grp, reg_addr, val);
}

int pfcSetGPIO(rcar_pfc_group_t grp, rcar_pfc_pin_t pin)
{
    pfcSetGPSR(PFC_GPIO, grp, pin);
    return 0;
}

int pfcSetPeripheral(rcar_pfc_group_t grp, rcar_pfc_pin_t pin)
{
    pfcSetGPSR(PFC_PERIPHERAL, grp, pin);
    return 0;
}

static void pfcPullMode(uint8_t enable, rcar_pfc_group_t grp, rcar_pfc_pin_t pin)
{
    uint32_t val, reg_addr;

    reg_addr = getPfcRegister(grp, GP_PULLEN);
    val = readl(reg_addr);
    if (enable == PFC_ENABLE_PULL)
        val |= BIT(pin);
    if (enable == PFC_DISABLE_PULL)
        val &= ~BIT(pin);
    pfcWrite(grp, reg_addr, val);
}

static void pfcSetPullType(uint8_t option, rcar_pfc_group_t grp, rcar_pfc_pin_t pin)
{
    uint32_t val, reg_addr;

    reg_addr = getPfcRegister(grp, GP_PUDSEL);
    val = readl(reg_addr);
    if (option == RCAR_PFC_PULL_UP)
        val |= BIT(pin);
    if (option == RCAR_PFC_PULL_DOWN)
        val &= ~BIT(pin);
    pfcWrite(grp, reg_addr, val);
}

int pfcSetPullDown(rcar_pfc_group_t grp, rcar_pfc_pin_t pin)
{
    pfcSetPullType(RCAR_PFC_PULL_DOWN, grp, pin);
    pfcPullMode(PFC_ENABLE_PULL, grp, pin);
    return 0;
}

int pfcSetPullUp(rcar_pfc_group_t grp, rcar_pfc_pin_t pin)
{
    pfcSetPullType(RCAR_PFC_PULL_UP, grp, pin);
    pfcPullMode(PFC_ENABLE_PULL, grp, pin);
    return 0;
}

int pfcSetNoPull(rcar_pfc_group_t grp, rcar_pfc_pin_t pin)
{
    pfcPullMode(PFC_DISABLE_PULL, grp, pin);
    return 0;
}
