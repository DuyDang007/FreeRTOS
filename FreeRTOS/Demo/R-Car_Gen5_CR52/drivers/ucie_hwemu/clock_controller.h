/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 * Copyright 2021-2022 Renesas Electronics Corporation All rights reserved.
 *******************************************************************************/

/*******************************************************************************
 * DESCRIPTION   : Clock Controller driver header
 ******************************************************************************/
#ifndef CLOCK_CONTROLLER_H_
#define CLOCK_CONTROLLER_H_

#include <stdint.h>
#include <clock_controller_register.h>

typedef struct
{
    const char *cpgm_name;   /* CPGM name */
} CPGM_NAME_TABLE;

#define PLL_SUCCESS     (0U)
#define PLL_ERR         (1U)
#define PLL_RETRY_MAX   (1000U)

/* Maxium number of PLLs */
#define PLL_MAX     (30U)
#define REG_ADDR    (0U)
#define REG_VAL     (1U)

#define PLL1_0      (0U)    /* TOP Hierarchy */
#define PLL1_1      (1U)    /* TOP Hierarchy */
#define PLL2_0      (2U)    /* TOP Hierarchy */
#define PLL2_1      (3U)    /* TOP Hierarchy */
#define PLL2_2      (4U)    /* TOP Hierarchy */
#define PLL2_3      (5U)    /* TOP Hierarchy */
#define PLL2_4      (6U)    /* TOP Hierarchy */
#define PLL2_5      (7U)    /* TOP Hierarchy */
#define PLL2_6      (8U)    /* TOP Hierarchy */
#define PLL2_7      (9U)    /* TOP Hierarchy */
#define PLL3_0      (10U)   /* TOP Hierarchy */
#define PLL3_1      (11U)   /* TOP Hierarchy */
#define PLL3_2      (12U)   /* TOP Hierarchy */
#define PLL3_3      (13U)   /* TOP Hierarchy */
#define PLL4        (14U)   /* TOP Hierarchy */
#define PLL5        (15U)   /* TOP Hierarchy */
#define PLL6        (16U)   /* TOP Hierarchy */
#define PLL7        (17U)   /* TOP Hierarchy */
#define PLL8        (18U)   /* TOP Hierarchy */
#define PLL9_0      (19U)   /* HSCS Hierarchy */
#define PLL9_1      (20U)   /* HSCS Hierarchy */
#define PLL10       (21U)   /* TOP Hierarchy */
#define PLL11       (22U)   /* TOP Hierarchy */
#define PLL12       (23U)   /* TOP Hierarchy */
#define PLL13       (24U)   /* SCP (PSO) Hierarchy */
#define PLL14       (25U)   /* TOP Hierarchy */
#define PLL15_0     (26U)   /* TOP Hierarchy */
#define PLL15_1     (27U)   /* TOP Hierarchy */
#define PLL15_2     (28U)   /* TOP Hierarchy */
#define PLL15_3     (29U)   /* TOP Hierarchy */

/* CPGM neme */
#define CPGM_NAME_PLL1_0    "CPGMA0_0 (PLL1_0)"
#define CPGM_NAME_PLL1_1    "CPGMA0_1 (PLL1_1)"
#define CPGM_NAME_PLL2_0    "CPGMK0 (PLL2_0)"
#define CPGM_NAME_PLL2_1    "CPGMK1 (PLL2_1)"
#define CPGM_NAME_PLL2_2    "CPGMK2 (PLL2_2)"
#define CPGM_NAME_PLL2_3    "CPGMK3 (PLL2_3)"
#define CPGM_NAME_PLL2_4    "CPGMK4 (PLL2_4)"
#define CPGM_NAME_PLL2_5    "CPGMK5 (PLL2_5)"
#define CPGM_NAME_PLL2_6    "CPGMK6 (PLL2_6)"
#define CPGM_NAME_PLL2_7    "CPGMK7 (PLL2_7)"
#define CPGM_NAME_PLL3_0    "CPGMD0 (PLL3_0)"
#define CPGM_NAME_PLL3_1    "CPGMD1 (PLL3_1)"
#define CPGM_NAME_PLL3_2    "CPGMD2 (PLL3_2)"
#define CPGM_NAME_PLL3_3    "CPGMD3 (PLL3_3)"
#define CPGM_NAME_PLL4      "CPGMG (PLL4)"
#define CPGM_NAME_PLL5      "CPGMA1 (PLL5)"
#define CPGM_NAME_PLL6      "CPGMR (PLL6)"
#define CPGM_NAME_PLL7      "CPGMRSW0 (PLL7)"
#define CPGM_NAME_PLL8      "CPGMEMMC (PLL8)"
#define CPGM_NAME_PLL9_0    "CPGMUCI0 (PLL9_0)"
#define CPGM_NAME_PLL9_1    "CPGMUCI1 (PLL9_1)"
#define CPGM_NAME_PLL10     "CPGMVCON (PLL10)"
#define CPGM_NAME_PLL11     "CPGMPCI (PLL11)"
#define CPGM_NAME_PLL12     "CPGMRSW1 (PLL12)"
#define CPGM_NAME_PLL13     "CPGMSCP (PLL13)"
#define CPGM_NAME_PLL14     "CPGMCMN (PLL14)"
#define CPGM_NAME_PLL15_0   "CPGMDSU0 (PLL15_0)"
#define CPGM_NAME_PLL15_1   "CPGMDSU1 (PLL15_1)"
#define CPGM_NAME_PLL15_2   "CPGMDSU2 (PLL15_2)"
#define CPGM_NAME_PLL15_3   "CPGMDSU3 (PLL15_3)"

/* PLLn_CR status */
#define PLL_CR2_PLLENTRG_START      (0x10000000U)
#define PLL_CR2_PLLCLKSTAB_MASK     (0x80000000U)
#define PLL_CR2_PLLCLKSTAB_UNSTABLE (0x00000000U)
#define PLL_CR2_PLLCLKSTAB_STABLE   (0x80000000U)

/* PLLn_SCR status */
#define PLL_SCR_PLLSELID_MASK       (0x00000001U)
#define PLL_SCR_PLLSELID_CLK_PLL    (0x00000000U)
#define PLL_SCR_PLLSELID_CLK_IOSC   (0x00000001U)
#define PLL_SCR_PLLSELACT_MASK      (0x00010000U)
#define PLL_SCR_PLLSELACT_CLK_PLL   (0x00000000U)
#define PLL_SCR_PLLSELACT_CLK_IOSC  (0x00010000U)

/* CPGM name array. */
static const CPGM_NAME_TABLE cpgm_name_table[PLL_MAX] =
{   /*              *cpgm_name */
    [PLL1_0]    =   {CPGM_NAME_PLL1_0},
    [PLL1_1]    =   {CPGM_NAME_PLL1_1},
    [PLL2_0]    =   {CPGM_NAME_PLL2_0},
    [PLL2_1]    =   {CPGM_NAME_PLL2_1},
    [PLL2_2]    =   {CPGM_NAME_PLL2_2},
    [PLL2_3]    =   {CPGM_NAME_PLL2_3},
    [PLL2_4]    =   {CPGM_NAME_PLL2_4},
    [PLL2_5]    =   {CPGM_NAME_PLL2_5},
    [PLL2_6]    =   {CPGM_NAME_PLL2_6},
    [PLL2_7]    =   {CPGM_NAME_PLL2_7},
    [PLL3_0]    =   {CPGM_NAME_PLL3_0},
    [PLL3_1]    =   {CPGM_NAME_PLL3_1},
    [PLL3_2]    =   {CPGM_NAME_PLL3_2},
    [PLL3_3]    =   {CPGM_NAME_PLL3_3},
    [PLL4]      =   {CPGM_NAME_PLL4},
    [PLL5]      =   {CPGM_NAME_PLL5},
    [PLL6]      =   {CPGM_NAME_PLL6},
    [PLL7]      =   {CPGM_NAME_PLL7},
    [PLL8]      =   {CPGM_NAME_PLL8},
    [PLL9_0]    =   {CPGM_NAME_PLL9_0},
    [PLL9_1]    =   {CPGM_NAME_PLL9_1},
    [PLL10]     =   {CPGM_NAME_PLL10},
    [PLL11]     =   {CPGM_NAME_PLL11},
    [PLL12]     =   {CPGM_NAME_PLL12},
    [PLL13]     =   {CPGM_NAME_PLL13},
    [PLL14]     =   {CPGM_NAME_PLL14},
    [PLL15_0]   =   {CPGM_NAME_PLL15_0},
    [PLL15_1]   =   {CPGM_NAME_PLL15_1},
    [PLL15_2]   =   {CPGM_NAME_PLL15_2},
    [PLL15_3]   =   {CPGM_NAME_PLL15_3}
};

/* PLLn Control Register 2 list */
static const uint32_t pll_ctrl_reg2[PLL_MAX] =
{   /* Register address */
    PLL1_0_CR2,
    PLL1_1_CR2,
    PLL2_0_CR2,
    PLL2_1_CR2,
    PLL2_2_CR2,
    PLL2_3_CR2,
    PLL2_4_CR2,
    PLL2_5_CR2,
    PLL2_6_CR2,
    PLL2_7_CR2,
    PLL3_0_CR2,
    PLL3_1_CR2,
    PLL3_2_CR2,
    PLL3_3_CR2,
    PLL4_CR2,
    PLL5_CR2,
    PLL6_CR2,
    PLL7_CR2,
    PLL8_CR2,
    PLL9_0_CR2,
    PLL9_1_CR2,
    PLL10_CR2,
    PLL11_CR2,
    PLL12_CR2,
    PLL13_CR2,
    PLL14_CR2,
    PLL15_0_CR2,
    PLL15_1_CR2,
    PLL15_2_CR2,
    PLL15_3_CR2
};

/* PLLn Selector Control Register list */
static const uint32_t pll_sel_ctrl_reg[PLL_MAX][2U] =
{   /* Register address, Setting value */
    {PLL1_0SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL1_1SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL2_0SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL2_1SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL2_2SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL2_3SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL2_4SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL2_5SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL2_6SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL2_7SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL3_0SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL3_1SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL3_2SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL3_3SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL4SCR,       PLL_SCR_PLLSELID_CLK_PLL},
    {PLL5SCR,       PLL_SCR_PLLSELID_CLK_PLL},
    {PLL6SCR,       PLL_SCR_PLLSELID_CLK_PLL},
    {PLL7SCR,       PLL_SCR_PLLSELID_CLK_PLL},
    {PLL8SCR,       PLL_SCR_PLLSELID_CLK_PLL},
    {PLL9_0SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL9_1SCR,     PLL_SCR_PLLSELID_CLK_PLL},
    {PLL10SCR,      PLL_SCR_PLLSELID_CLK_PLL},
    {PLL11SCR,      PLL_SCR_PLLSELID_CLK_PLL},
    {PLL12SCR,      PLL_SCR_PLLSELID_CLK_PLL},
    {PLL13SCR,      PLL_SCR_PLLSELID_CLK_PLL},
    {PLL14SCR,      PLL_SCR_PLLSELID_CLK_PLL},
    {PLL15_0SCR,    PLL_SCR_PLLSELID_CLK_PLL},
    {PLL15_1SCR,    PLL_SCR_PLLSELID_CLK_PLL},
    {PLL15_2SCR,    PLL_SCR_PLLSELID_CLK_PLL},
    {PLL15_3SCR,    PLL_SCR_PLLSELID_CLK_PLL}
};

//void clock_controller_init(void);
//void pll_init(void);
uint32_t switch_clock_source_pll(uint32_t pll_num);
void clock_controller_reg_write(uint32_t pd_hier, uint32_t reg_addr, uint32_t reg_val);
void pll_reg_write(uint32_t pll_num);
uint32_t get_pd_hier_from_pll(uint32_t pll_num);
const char* get_cpgm_name(uint32_t pll_num);

#endif /* CLOCK_CONTROLLER_H_ */
