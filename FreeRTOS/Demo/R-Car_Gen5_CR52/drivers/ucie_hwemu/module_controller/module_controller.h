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
 * DESCRIPTION   : Module Controller driver header
 ******************************************************************************/

#ifndef MODULE_CONTROLLER_H_
#define MODULE_CONTROLLER_H_

#include <stdint.h>
#include <module_controller_register.h>

/* CF-compliant (TS Rev.0.40) */

typedef struct
{
    const char *hier_name;      /* PD_hier name */
} MDLC_HIER_NAME_TABLE;

/* Module Power RUN transition execution information. */
typedef struct
{
    uint32_t hier_pdid_max;     /* Max value of PDID for each PD_hier. */
    uint32_t pd_init_stat;      /* Initial status of MPG at Full-RUN and Sentry startup. */
    uint32_t mpg_run_skip_flag; /* Flag to skip transition to Module Power RUN. */
} MDLC_MPG_EXEC_TABLE;


/* Module RUN transition execution information. */
typedef struct
{
    uint32_t mpg_reg_addr;      /* Module Power Domain Gating Register address */
    uint32_t mpg_stat_reg_addr; /* Module Power Domain Gating Status Register address */
    uint32_t pdr_assign;        /* MPG PDR assignment information */
    uint32_t set_val;           /* Register setting value */
} MDLC_MPG_CFG_TABLE;


/* Module RUN transition execution information. */
typedef struct
{
    uint32_t ms_reg_addr;       /* Module System Reset Register address */
    uint32_t ms_stat_reg_addr;  /* Module System Reset Status Register address */
    uint32_t bit_assign;        /* MS bit assignment information */
    uint32_t set_val;           /* Register setting value */
} MDLC_MS_CFG_TABLE;

#define BIT_1_32BIT             (0x1U)

/* MPG and MS result status */
#define MDLC_SUCCESS            (0U)
#define MDLC_ERR                (1U)
#if 0
#define MDLC_RETRY_MAX          (32U)
#else
#define MDLC_RETRY_MAX          (128U)
#endif

/* Max number of registers. */
#define PD_HIER_MAX             (PD_HIER_MAX_NON_SCP + PD_HIER_MAX_SCP)
#define PD_HIER_MAX_NON_SCP     (26U)
#define PD_HIER_MAX_SCP         (2U)
#define MPG_REG_NUM_MAX         (64U)
#define MS_REG_NUM_MAX_NON_SCP  (24U)
#define MS_REG_NUM_MAX_SCP      (22U)

#ifdef BIT0
#undef BIT0
#endif

#define BIT0                    (0x00000001U)

#define PD_HIER_NOT_IMPL        (0x0U)
#define PD_HIER_IMPL            (0x1U)

/* MPG config table number */
#define MPG_CFG_TABLE_MAX       (3U)

/* Status of MPG */
#define MPG_GATING              (0x00000000U)   /* Module Power Gating */
#define MPG_RESET               (0x00000001U)   /* Module Power Reset */
#define MPG_RUN                 (0x00000003U)   /* Module Power RUN */
#define NON_PSWC                (MPG_GATING)    /* MPG not implemented */

/* Module Power RUN skip flag */
#define MPG_EXEC                (0U)
#define MPG_RUN_SKIP            (1U)

/* MPG bit assignment */
#define MPG_RESERVED            (0x00000000U)
#define MPG_MPGDTR              (0x00000003U)
#define MPG_RUN                 (0x00000003U)

/* Status of MS */
#define MS_BIT_MASK             (0x00000003U)   /* Module bit mask  */
#define MS_STANDBY              (0x0U)          /* Module Standby   */
#define MS_RESET                (0x1U)          /* Module Reset     */
#define MS_STOP                 (0x2U)          /* Module STOP      */
#define MS_RUN                  (0x3U)          /* Module RUN       */
#define MS_BIT_MASK             (0x00000003U)   /* Module bit mask  */
#define MS_STANDBY              (0x0U)          /* Module Standby   */
#define MS_RESET                (0x1U)          /* Module Reset     */
#define MS_STOP                 (0x2U)          /* Module STOP      */
#define MS_RUN                  (0x3U)          /* Module RUN       */

/* MPG bit assignment */
#define MS_RESERVED             (0x00000000U)

/* Module RUN skip flag */
#define MS_EXEC                 (0U)
#define MS_RUN_SKIP             (1U)

/* PD_hier number */
#define PD_HIER_VIPN        (0U)
#define PD_HIER_VIPS        (1U)
#define PD_HIER_VIO         (2U)
#define PD_HIER_PERE        (3U)
#define PD_HIER_PERW        (4U)
#define PD_HIER_DDR0        (5U)
#define PD_HIER_DDR1        (6U)
#define PD_HIER_DDR2        (7U)
#define PD_HIER_DDR3        (8U)
#define PD_HIER_DDR4        (9U)
#define PD_HIER_DDR5        (10U)
#define PD_HIER_DDR6        (11U)
#define PD_HIER_DDR7        (12U)
#define PD_HIER_HSCN        (13U)
#define PD_HIER_RT          (14U)
#define PD_HIER_TOP         (15U)
#define PD_HIER_HSCS        (16U)
#define PD_HIER_IMN         (17U)
#define PD_HIER_IMS         (18U)
#define PD_HIER_GPC         (19U)
#define PD_HIER_DSP         (20U)
#define PD_HIER_MM          (21U)
#define PD_HIER_NPU0        (22U)
#define PD_HIER_NPU1        (23U)
#define PD_HIER_APUN        (24U)
#define PD_HIER_APUS        (25U)
#define PD_HIER_SCP         (26U)
#define PD_HIER_AON         (27U)

/* MPG and MS register setting value */
#define WRITE_KEY_CODE_DIS      (0xA5A5A500U)
#define WRITE_KEY_CODE_EN       (0xA5A5A501U)
#define COMPL_INT_DIS           (0x00000000U)
#define COMPL_INT_REQ_MASK      (0xFFFFFFFFU)




/* NEW */

typedef struct
{
    const char *name;               /* Classification Group name                    */
} MDLC_CLASS_GROUP_NAME_TABLE;

typedef struct
{
    uint32_t mpg_exec_flag;         /* MPG  execution flag                          */
    uint32_t ms_exec_flag;          /* MS execution flag                            */
} MDLC_MODULE_NUM_EXEC_TABLE;

typedef struct
{
    uint32_t mpg_gating_reg_addr;   /* Module Power Domain Gating Register          */
    uint32_t mpg_stat_reg_addr;     /* Module Power Domain Gating Status Register   */
    uint32_t mpg_pdr_assign;        /* MPG PDR assign information                   */
} MDLC_MPG_REG_TABLE;

typedef struct
{
    uint32_t bit_assign_info;   /* MS bit assign information                        */
} MDLC_MS_BIT_ASSIGN_TABLE;

typedef struct
{
    uint32_t ms_reset_reg_addr; /* Module System Reset Register                     */
    uint32_t ms_stat_reg_addr;  /* Module System Reset Status Register address      */
    uint32_t ms_reg_bit_assign; /* MS bit assign information                        */
} MDLC_MS_REG_TABLE;

/* Target Classification Groups of Module Power Gating and Module Standby           */

# define REG_NUM_HSCS       (2U)

/* PD Hier virtual pin address */
#define PD_HIER_VR_PIN_ADDR (0x34004020U)

/* MDUC-Region base address of Hier Structure. */
#if 1
#define BASE_ADDR_HIER_VIPN (0xC3060000U)   /* Hier Structure: 00, VIPN */
#define BASE_ADDR_HIER_VIPS (0xC3460000U)   /* Hier Structure: 01, VIPS */
#define BASE_ADDR_HIER_VIO  (0xC5000000U)   /* Hier Structure: 02, VIO */
#define BASE_ADDR_HIER_PERE (0xC08F0000U)   /* Hier Structure: 03, PERE */
#define BASE_ADDR_HIER_PERW (0xC05D0000U)   /* Hier Structure: 04, PERW */
#define BASE_ADDR_HIER_DDR0 (0xE8000000U)   /* Hier Structure: 05, DDR0 */
#define BASE_ADDR_HIER_DDR1 (0xE8080000U)   /* Hier Structure: 06, DDR1 */
#define BASE_ADDR_HIER_DDR2 (0xE8100000U)   /* Hier Structure: 07, DDR2 */
#define BASE_ADDR_HIER_DDR3 (0xE8180000U)   /* Hier Structure: 08, DDR3 */
#define BASE_ADDR_HIER_DDR4 (0xE8200000U)   /* Hier Structure: 09, DDR4 */
#define BASE_ADDR_HIER_DDR5 (0xE8280000U)   /* Hier Structure: 10, DDR5 */
#define BASE_ADDR_HIER_DDR6 (0xE8300000U)   /* Hier Structure: 11, DDR6 */
#define BASE_ADDR_HIER_DDR7 (0xE8380000U)   /* Hier Structure: 12, DDR7 */
#define BASE_ADDR_HIER_HSCN (0xC9C90000U)   /* Hier Structure: 13, HSCN */
#define BASE_ADDR_HIER_RT   (0x19440000U)   /* Hier Structure: 14, RT */
#define BASE_ADDR_HIER_TOP  (0xC6480000U)   /* Hier Structure: 15, TOP */
#define BASE_ADDR_HIER_HSCS (0xDE200000U)   /* Hier Structure: 16, HSCS */
#define BASE_ADDR_HIER_IMN  (0xC1990000U)   /* Hier Structure: 17, IMN */
#define BASE_ADDR_HIER_IMS  (0xC1D90000U)   /* Hier Structure: 18, IMS */
#define BASE_ADDR_HIER_GPC  (0xCB510000U)   /* Hier Structure: 19, GPC */
#define BASE_ADDR_HIER_DSP  (0xCBE90000U)   /* Hier Structure: 20, DSP */
#define BASE_ADDR_HIER_MM   (0xE9980000U)   /* Hier Structure: 21, MM */
#define BASE_ADDR_HIER_NPU0 (0xD2C30000U)   /* Hier Structure: 22, NPU0 */
#define BASE_ADDR_HIER_NPU1 (0xD6C30000U)   /* Hier Structure: 23, NPU1 */
#define BASE_ADDR_HIER_CMNN (0xCA410000U)   /* Hier Structure: 24, CMNN */
#define BASE_ADDR_HIER_CMNS (0xCA510000U)   /* Hier Structure: 25, CMNS */
#define BASE_ADDR_HIER_SCP  (0xC1330000U)   /* Hier Structure: 26, SCP */
#define BASE_ADDR_HIER_AON  (0xC1338000U)   /* Hier Structure: 27, AON */
#endif

#define MODULE_NUM_MAX              (30U)

#define MODULE_NUM_VIPN             (0U)
#define MODULE_NUM_VIPS             (1U)
#define MODULE_NUM_VIO              (2U)
#define MODULE_NUM_PERE             (3U)
#define MODULE_NUM_PERW             (4U)
#define MODULE_NUM_DDR0             (5U)
#define MODULE_NUM_DDR1             (6U)
#define MODULE_NUM_DDR2             (7U)
#define MODULE_NUM_DDR3             (8U)
#define MODULE_NUM_DDR4             (9U)
#define MODULE_NUM_DDR5             (10U)
#define MODULE_NUM_DDR6             (11U)
#define MODULE_NUM_DDR7             (12U)
#define MODULE_NUM_HSCN             (13U)
#define MODULE_NUM_RT               (14U)
#define MODULE_NUM_TOP              (15U)
#define MODULE_NUM_HSCS             (16U)
#define MODULE_NUM_IMN              (17U)
#define MODULE_NUM_IMS              (18U)
#define MODULE_NUM_GPC              (19U)
#define MODULE_NUM_DSP              (20U)
#define MODULE_NUM_MM               (21U)
#define MODULE_NUM_NPU0             (22U)
#define MODULE_NUM_NPU1             (23U)
#define MODULE_NUM_CMNN             (24U)
#define MODULE_NUM_CMNS             (25U)
#define MODULE_NUM_SCP              (26U)
#define MODULE_NUM_AON              (27U)
#define MODULE_NUM_NPU2             (28U)
#define MODULE_NUM_NPU3             (29U)

/* Classification Group Name                                                    */
#define CLASS_GROUP_NAME_VIPN       "Module Controller (VIPN)"
#define CLASS_GROUP_NAME_VIPS       "Module Controller (VIPS)"
#define CLASS_GROUP_NAME_VIO        "Module Controller (VIO )"
#define CLASS_GROUP_NAME_PERE       "Module Controller (PERE)"
#define CLASS_GROUP_NAME_PERW       "Module Controller (PERW)"
#define CLASS_GROUP_NAME_DDR0       "Module Controller (DDR0)"
#define CLASS_GROUP_NAME_DDR1       "Module Controller (DDR1)"
#define CLASS_GROUP_NAME_DDR2       "Module Controller (DDR2)"
#define CLASS_GROUP_NAME_DDR3       "Module Controller (DDR3)"
#define CLASS_GROUP_NAME_DDR4       "Module Controller (DDR4)"
#define CLASS_GROUP_NAME_DDR5       "Module Controller (DDR5)"
#define CLASS_GROUP_NAME_DDR6       "Module Controller (DDR6)"
#define CLASS_GROUP_NAME_DDR7       "Module Controller (DDR7)"
#define CLASS_GROUP_NAME_HSCN       "Module Controller (HSCN)"
#define CLASS_GROUP_NAME_RT         "Module Controller (RT)  "
#define CLASS_GROUP_NAME_TOP        "Module Controller (TOP) "
#define CLASS_GROUP_NAME_HSCS       "Module Controller (HSCS)"
#define CLASS_GROUP_NAME_IMN        "Module Controller (IMN) "
#define CLASS_GROUP_NAME_IMS        "Module Controller (IMS) "
#define CLASS_GROUP_NAME_GPC        "Module Controller (GPC) "
#define CLASS_GROUP_NAME_DSP        "Module Controller (DSP) "
#define CLASS_GROUP_NAME_MM         "Module Controller (MM)  "
#define CLASS_GROUP_NAME_NPU0       "Module Controller (NPU0)"
#define CLASS_GROUP_NAME_NPU1       "Module Controller (NPU1)"
#define CLASS_GROUP_NAME_APUN       "Module Controller (APUN)"
#define CLASS_GROUP_NAME_APUS       "Module Controller (APUS)"
#define CLASS_GROUP_NAME_SCP        "Module Controller (SCP) "
#define CLASS_GROUP_NAME_AON        "Module Controller (AON) "
#define CLASS_GROUP_NAME_NPU2       "Module Controller (NPU2) "
#define CLASS_GROUP_NAME_NPU3       "Module Controller (NPU3) "



/* MPG and MS execution flag for Target Classification Groups                   */
#define CLASS_GROUP_MPG_RESERVED    (0U)
#define CLASS_GROUP_MPG_SKIP        (CLASS_GROUP_MPG_RESERVED)
#define CLASS_GROUP_MPG_EXEC        (1U)
#define CLASS_GROUP_MS_RESERVED     (0U)
#define CLASS_GROUP_MS_SKIP         (CLASS_GROUP_MS_RESERVED)
#define CLASS_GROUP_MS_EXEC         (1U)

/* Module Standby bit assign                                                    */
#define MPG_REG_MAX_NUM             (21U)
#define MPG_PDR_RESERVED            (0x00000000U)
#define MPG_PDR_SKIP                (MPG_PDR_RESERVED)
#define MPG_PDR_ASSIGNED            (0x00000003U)

/* Module Standby bit assign                                                    */
#define MS_REG_MAX_NUM              (22U)
#define MS_BIT_RESERVED             (0x00000000U)
#define MS_BIT_SKIP                 (MS_BIT_RESERVED)
#define MS_BIT_ASSIGNED             (0x00000003U)
#define MS_BIT_31_30                (MS_BIT_ASSIGNED << 30U)
#define MS_BIT_29_28                (MS_BIT_ASSIGNED << 28U)
#define MS_BIT_27_26                (MS_BIT_ASSIGNED << 26U)
#define MS_BIT_25_24                (MS_BIT_ASSIGNED << 24U)
#define MS_BIT_23_22                (MS_BIT_ASSIGNED << 22U)
#define MS_BIT_21_20                (MS_BIT_ASSIGNED << 20U)
#define MS_BIT_19_18                (MS_BIT_ASSIGNED << 18U)
#define MS_BIT_17_16                (MS_BIT_ASSIGNED << 16U)
#define MS_BIT_15_14                (MS_BIT_ASSIGNED << 14U)
#define MS_BIT_13_12                (MS_BIT_ASSIGNED << 12U)
#define MS_BIT_11_10                (MS_BIT_ASSIGNED << 10U)
#define MS_BIT_09_08                (MS_BIT_ASSIGNED << 8U)
#define MS_BIT_07_06                (MS_BIT_ASSIGNED << 6U)
#define MS_BIT_05_04                (MS_BIT_ASSIGNED << 4U)
#define MS_BIT_03_02                (MS_BIT_ASSIGNED << 2U)
#define MS_BIT_01_00                (MS_BIT_ASSIGNED << 0U)

static const uint32_t mdlcnpkcprot1_reg[PD_HIER_MAX] =
{
    MDLC00PKCPROT1,   /* Hier Structure: 00, VIPN */
    MDLC01PKCPROT1,   /* Hier Structure: 01, VIPS */
    MDLC02PKCPROT1,   /* Hier Structure: 02, VIO */
    MDLC03PKCPROT1,   /* Hier Structure: 03, PERE */
    MDLC04PKCPROT1,   /* Hier Structure: 04, PERW */
    MDLC05PKCPROT1,   /* Hier Structure: 05, DDR0 */
    MDLC06PKCPROT1,   /* Hier Structure: 06, DDR1 */
    MDLC07PKCPROT1,   /* Hier Structure: 07, DDR2 */
    MDLC08PKCPROT1,   /* Hier Structure: 08, DDR3 */
    MDLC09PKCPROT1,   /* Hier Structure: 09, DDR4 */
    MDLC10PKCPROT1,   /* Hier Structure: 10, DDR5 */
    MDLC11PKCPROT1,   /* Hier Structure: 11, DDR6 */
    MDLC12PKCPROT1,   /* Hier Structure: 12, DDR7 */
    MDLC13PKCPROT1,   /* Hier Structure: 13, HSCN */
    MDLC14PKCPROT1,   /* Hier Structure: 14, RT */
    MDLC15PKCPROT1,   /* Hier Structure: 15, TOP */
    MDLC16PKCPROT1,   /* Hier Structure: 16, HSCS */
    MDLC17PKCPROT1,   /* Hier Structure: 17, IMN */
    MDLC18PKCPROT1,   /* Hier Structure: 18, IMS */
    MDLC19PKCPROT1,   /* Hier Structure: 19, GPC */
    MDLC20PKCPROT1,   /* Hier Structure: 20, DSP */
    MDLC21PKCPROT1,   /* Hier Structure: 21, MM */
    MDLC22PKCPROT1,   /* Hier Structure: 22, NPU0 */
    MDLC23PKCPROT1,   /* Hier Structure: 23, NPU1 */
    MDLC24PKCPROT1,   /* Hier Structure: 24, APUN */
    MDLC25PKCPROT1,   /* Hier Structure: 25, APUS */
    MDLC26PKCPROT1,   /* Hier Structure: 26, SCP */
    MDLC27PKCPROT1    /* Hier Structure: 27, AON */
};

/* PD_hier neme */
#define PD_HIER_NAME_VIPN   "VIPN"
#define PD_HIER_NAME_VIPS   "VIPS"
#define PD_HIER_NAME_VIO    "VIO"
#define PD_HIER_NAME_PERE   "PERE"
#define PD_HIER_NAME_PERW   "PERW"
#define PD_HIER_NAME_DDR0   "DDR0"
#define PD_HIER_NAME_DDR1   "DDR1"
#define PD_HIER_NAME_DDR2   "DDR2"
#define PD_HIER_NAME_DDR3   "DDR3"
#define PD_HIER_NAME_DDR4   "DDR4"
#define PD_HIER_NAME_DDR5   "DDR5"
#define PD_HIER_NAME_DDR6   "DDR6"
#define PD_HIER_NAME_DDR7   "DDR7"
#define PD_HIER_NAME_HSCN   "HSCN"
#define PD_HIER_NAME_RT     "RT"
#define PD_HIER_NAME_TOP    "TOP"
#define PD_HIER_NAME_HSCS   "HSCS"
#define PD_HIER_NAME_IMN    "IMN"
#define PD_HIER_NAME_IMS    "IMS"
#define PD_HIER_NAME_GPC    "GPC"
#define PD_HIER_NAME_DSP    "DSP"
#define PD_HIER_NAME_MM     "MM"
#define PD_HIER_NAME_NPU0   "NPU0"
#define PD_HIER_NAME_NPU1   "NPU1"
#define PD_HIER_NAME_APUN   "APUN"
#define PD_HIER_NAME_APUS   "APUS"
#define PD_HIER_NAME_SCP    "SCP"
#define PD_HIER_NAME_AON    "AON"

/* PD_hier name array. */
static const MDLC_HIER_NAME_TABLE hier_name_table[PD_HIER_MAX] =
{   /*                  *hier_name */
    [PD_HIER_VIPN]  =   {PD_HIER_NAME_VIPN},
    [PD_HIER_VIPS]  =   {PD_HIER_NAME_VIPS},
    [PD_HIER_VIO]   =   {PD_HIER_NAME_VIO},
    [PD_HIER_PERE]  =   {PD_HIER_NAME_PERE},
    [PD_HIER_PERW]  =   {PD_HIER_NAME_PERW},
    [PD_HIER_DDR0]  =   {PD_HIER_NAME_DDR0},
    [PD_HIER_DDR1]  =   {PD_HIER_NAME_DDR1},
    [PD_HIER_DDR2]  =   {PD_HIER_NAME_DDR2},
    [PD_HIER_DDR3]  =   {PD_HIER_NAME_DDR3},
    [PD_HIER_DDR4]  =   {PD_HIER_NAME_DDR4},
    [PD_HIER_DDR5]  =   {PD_HIER_NAME_DDR5},
    [PD_HIER_DDR6]  =   {PD_HIER_NAME_DDR6},
    [PD_HIER_DDR7]  =   {PD_HIER_NAME_DDR7},
    [PD_HIER_HSCN]  =   {PD_HIER_NAME_HSCN},
    [PD_HIER_RT]    =   {PD_HIER_NAME_RT},
    [PD_HIER_TOP]   =   {PD_HIER_NAME_TOP},
    [PD_HIER_HSCS]  =   {PD_HIER_NAME_HSCS},
    [PD_HIER_IMN]   =   {PD_HIER_NAME_IMN},
    [PD_HIER_IMS]   =   {PD_HIER_NAME_IMS},
    [PD_HIER_GPC]   =   {PD_HIER_NAME_GPC},
    [PD_HIER_DSP]   =   {PD_HIER_NAME_DSP},
    [PD_HIER_MM]    =   {PD_HIER_NAME_MM},
    [PD_HIER_NPU0]  =   {PD_HIER_NAME_NPU0},
    [PD_HIER_NPU1]  =   {PD_HIER_NAME_NPU1},
    [PD_HIER_APUN]  =   {PD_HIER_NAME_APUN},
    [PD_HIER_APUS]  =   {PD_HIER_NAME_APUS},
    [PD_HIER_SCP]   =   {PD_HIER_NAME_SCP},
    [PD_HIER_AON]   =   {PD_HIER_NAME_AON}
};

static const MDLC_MODULE_NUM_EXEC_TABLE mpg_ms_exec_flag[MODULE_NUM_MAX] =
{
    /*                      mpg_exec_flag,              ms_exec_flag            */
    [MODULE_NUM_VIPN] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_RESERVED },
    [MODULE_NUM_VIPS] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_RESERVED },
    [MODULE_NUM_VIO ] =    {CLASS_GROUP_MPG_EXEC,       CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_PERE] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_RESERVED },
    [MODULE_NUM_PERW] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_RESERVED },
    [MODULE_NUM_DDR0] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_DDR1] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_DDR2] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_DDR3] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_DDR4] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_DDR5] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_DDR6] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_DDR7] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_HSCN] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_RESERVED },
    [MODULE_NUM_RT  ] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_TOP ] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_HSCS] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_IMN ] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_RESERVED },
    [MODULE_NUM_IMS ] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_RESERVED },
    [MODULE_NUM_GPC ] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_RESERVED },
    [MODULE_NUM_DSP ] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_RESERVED },
    [MODULE_NUM_MM  ] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_NPU0] =    {CLASS_GROUP_MPG_EXEC,       CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_NPU1] =    {CLASS_GROUP_MPG_EXEC,       CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_CMNN] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_RESERVED },
    [MODULE_NUM_CMNS] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_RESERVED },
    [MODULE_NUM_SCP ] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_EXEC     },
    [MODULE_NUM_AON ] =    {CLASS_GROUP_MPG_RESERVED,   CLASS_GROUP_MS_RESERVED },
};

static const MDLC_CLASS_GROUP_NAME_TABLE class_group_name_table[MODULE_NUM_MAX] =
{
    /*                      *class_group_name                                  */
    [MODULE_NUM_VIPN] =    {CLASS_GROUP_NAME_VIPN   },
    [MODULE_NUM_VIPS] =    {CLASS_GROUP_NAME_VIPS   },
    [MODULE_NUM_VIO ] =    {CLASS_GROUP_NAME_VIO    },
    [MODULE_NUM_PERE] =    {CLASS_GROUP_NAME_PERE   },
    [MODULE_NUM_PERW] =    {CLASS_GROUP_NAME_PERW   },
    [MODULE_NUM_DDR0] =    {CLASS_GROUP_NAME_DDR0   },
    [MODULE_NUM_DDR1] =    {CLASS_GROUP_NAME_DDR1   },
    [MODULE_NUM_DDR2] =    {CLASS_GROUP_NAME_DDR2   },
    [MODULE_NUM_DDR3] =    {CLASS_GROUP_NAME_DDR3   },
    [MODULE_NUM_DDR4] =    {CLASS_GROUP_NAME_DDR4   },
    [MODULE_NUM_DDR5] =    {CLASS_GROUP_NAME_DDR5   },
    [MODULE_NUM_DDR6] =    {CLASS_GROUP_NAME_DDR6   },
    [MODULE_NUM_DDR7] =    {CLASS_GROUP_NAME_DDR7   },
    [MODULE_NUM_HSCN] =    {CLASS_GROUP_NAME_HSCN   },
    [MODULE_NUM_RT  ] =    {CLASS_GROUP_NAME_RT     },
    [MODULE_NUM_TOP ] =    {CLASS_GROUP_NAME_TOP    },
    [MODULE_NUM_HSCS] =    {CLASS_GROUP_NAME_HSCS   },
    [MODULE_NUM_IMN ] =    {CLASS_GROUP_NAME_IMN    },
    [MODULE_NUM_IMS ] =    {CLASS_GROUP_NAME_IMS    },
    [MODULE_NUM_GPC ] =    {CLASS_GROUP_NAME_GPC    },
    [MODULE_NUM_DSP ] =    {CLASS_GROUP_NAME_DSP    },
    [MODULE_NUM_MM  ] =    {CLASS_GROUP_NAME_MM     },
    [MODULE_NUM_NPU0] =    {CLASS_GROUP_NAME_NPU0   },
    [MODULE_NUM_NPU1] =    {CLASS_GROUP_NAME_NPU1   },
    [MODULE_NUM_CMNN] =    {CLASS_GROUP_NAME_APUN   },
    [MODULE_NUM_CMNS] =    {CLASS_GROUP_NAME_APUS   },
    [MODULE_NUM_SCP ] =    {CLASS_GROUP_NAME_SCP    },
    [MODULE_NUM_AON ] =    {CLASS_GROUP_NAME_AON    },
};

static const MDLC_MS_BIT_ASSIGN_TABLE ms_reg_bit_assign[MODULE_NUM_MAX][MS_REG_MAX_NUM] =
{
    /* Module Number 00, Module Controller (VIPN)   */
    [MODULE_NUM_VIPN] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 01, Module Controller (VIPS)   */
    [MODULE_NUM_VIPS] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 02, Module Controller (VIO)    */
    [MODULE_NUM_VIO] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_27_26      | MS_BIT_25_24      | MS_BIT_23_22      | MS_BIT_21_20      | MS_BIT_19_18      | MS_BIT_17_16      \
                    |   MS_BIT_15_14    | MS_BIT_13_12      | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_19_18      | MS_BIT_17_16      \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_03_02      | MS_BIT_01_00      },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_03_02      | MS_BIT_01_00      },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_27_26      | MS_BIT_25_24      | MS_BIT_23_22      | MS_BIT_21_20      | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_31_30    | MS_BIT_29_28      | MS_BIT_27_26      | MS_BIT_25_24      | MS_BIT_23_22      | MS_BIT_21_20      | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_31_30    | MS_BIT_29_28      | MS_BIT_27_26      | MS_BIT_25_24      | MS_BIT_23_22      | MS_BIT_21_20      | MS_BIT_19_18      | MS_BIT_17_16      \
                    |   MS_BIT_15_14    | MS_BIT_13_12      | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_03_02      | MS_BIT_01_00      },

        [10]    = {     MS_BIT_31_30    | MS_BIT_29_28      | MS_BIT_27_26      | MS_BIT_25_24      | MS_BIT_23_22      | MS_BIT_21_20      | MS_BIT_19_18      | MS_BIT_17_16      \
                    |   MS_BIT_15_14    | MS_BIT_13_12      | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_03_02      | MS_BIT_01_00      },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_27_26      | MS_BIT_25_24      | MS_BIT_23_22      | MS_BIT_21_20      | MS_BIT_19_18      | MS_BIT_17_16      \
                    |   MS_BIT_15_14    | MS_BIT_13_12      | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_03_02      | MS_BIT_01_00      },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_31_30    | MS_BIT_29_28      | MS_BIT_27_26      | MS_BIT_25_24      | MS_BIT_23_22      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_21_20      | MS_BIT_19_18      | MS_BIT_17_16      \
                    |   MS_BIT_15_14    | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 03, Module Controller (PERE)   */
    [MODULE_NUM_PERE] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

    },

    /* Module Number 04, Module Controller (PERW)   */
    [MODULE_NUM_PERW] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 05, Module Controller (DDR0)   */
    [MODULE_NUM_DDR0] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 06, Module Controller (DDR1)   */
    [MODULE_NUM_DDR1] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 07, Module Controller (DDR2)   */
    [MODULE_NUM_DDR2] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 08, Module Controller (DDR3)   */
    [MODULE_NUM_DDR3] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 09, Module Controller (DDR4)   */
    [MODULE_NUM_DDR4] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 10, Module Controller (DDR5)   */
    [MODULE_NUM_DDR5] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 11, Module Controller (DDR6)   */
    [MODULE_NUM_DDR6] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 12, Module Controller (DDR7)   */
    [MODULE_NUM_DDR7] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 13, Module Controller (HSCN)   */
    [MODULE_NUM_HSCN] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 14, Module Controller (RT)     */
    [MODULE_NUM_RT] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_19_18      | MS_BIT_17_16      \
                    |   MS_BIT_15_14    | MS_BIT_13_12      | MS_BIT_11_10      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_03_02      | MS_BIT_01_00      },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_03_02      | MS_BIT_01_00      },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_03_02      | MS_BIT_01_00      },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_05_04      | MS_BIT_03_02      | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_31_30    | MS_BIT_29_28      | MS_BIT_27_26      | MS_BIT_25_24      | MS_BIT_23_22      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_21_20      | MS_BIT_19_18      | MS_BIT_17_16      \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_17_16      \
                    |   MS_BIT_15_14    | MS_BIT_RESERVED   | MS_BIT_11_10      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_31_30    | MS_BIT_29_28      | MS_BIT_27_26      | MS_BIT_25_24      | MS_BIT_23_22      | MS_BIT_21_20      | MS_BIT_19_18      | MS_BIT_17_16      \
                    |   MS_BIT_15_14    | MS_BIT_13_12      | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_03_02      | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_03_02      | MS_BIT_01_00      },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_09_08      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_23_22      | MS_BIT_21_20      | MS_BIT_19_18      | MS_BIT_17_16      \
                    |   MS_BIT_RESERVED | MS_BIT_13_12      | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_03_02      | MS_BIT_01_00      },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },


    /* Module Number 15, Module Controller (TOP)    */
    [MODULE_NUM_TOP] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_27_26      | MS_BIT_25_24      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_15_14    | MS_BIT_13_12      | MS_BIT_11_10      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_05_04      | MS_BIT_03_02      | MS_BIT_01_00      },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_27_26      | MS_BIT_25_24      | MS_BIT_23_22      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_15_14    | MS_BIT_13_12      | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_03_02      | MS_BIT_01_00      },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_13_12      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_05_04      | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_03_02      | MS_BIT_01_00      },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_01_00      },
    },

    /* Module Number 16, Module Controller (HSCS)   */
    [MODULE_NUM_HSCS] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_03_02      | MS_BIT_01_00      },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 17, Module Controller (IMN)    */
    [MODULE_NUM_IMN] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 18, Module Controller (IMS)    */
    [MODULE_NUM_IMS] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 19, Module Controller (GPC)    */
    [MODULE_NUM_GPC] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 20, Module Controller (DSP)   */
    [MODULE_NUM_DSP] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_23_22      | MS_BIT_21_20      | MS_BIT_19_18      | MS_BIT_17_16      \
                    |   MS_BIT_15_14    | MS_BIT_13_12      | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_03_02      | MS_BIT_01_00      },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },


    /* Module Number 21, Module Controller (MM)   */
    [MODULE_NUM_MM] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_31_30    | MS_BIT_29_28      | MS_BIT_27_26      | MS_BIT_25_24      | MS_BIT_23_22      | MS_BIT_21_20      | MS_BIT_19_18      | MS_BIT_17_16      \
                    |   MS_BIT_15_14    | MS_BIT_13_12      | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_03_02      | MS_BIT_01_00      },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 22, Module Controller (NPU0)   */
    [MODULE_NUM_NPU0] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_05_04      | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [6]     = {     MS_BIT_31_30    | MS_BIT_29_28      | MS_BIT_27_26      | MS_BIT_25_24      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_17_16      \
                    |   MS_BIT_15_14    | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_29_28      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_19_18      | MS_BIT_17_16      \
                    |   MS_BIT_15_14    | MS_BIT_13_12      | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_03_02      | MS_BIT_01_00      },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_19_18      | MS_BIT_RESERVED   \
                    |   MS_BIT_15_14    | MS_BIT_RESERVED   | MS_BIT_11_10      | MS_BIT_RESERVED   | MS_BIT_07_06      | MS_BIT_RESERVED   | MS_BIT_03_02      | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 23, Module Controller (NPU1)   */
    [MODULE_NUM_NPU1] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_05_04      | MS_BIT_RESERVED   | MS_BIT_01_00      },

        [6]     = {     MS_BIT_31_30    | MS_BIT_29_28      | MS_BIT_27_26      | MS_BIT_25_24      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_17_16      \
                    |   MS_BIT_15_14    | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_29_28      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_19_18      | MS_BIT_17_16      \
                    |   MS_BIT_15_14    | MS_BIT_13_12      | MS_BIT_11_10      | MS_BIT_09_08      | MS_BIT_07_06      | MS_BIT_05_04      | MS_BIT_03_02      | MS_BIT_01_00      },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_19_18      | MS_BIT_RESERVED   \
                    |   MS_BIT_15_14    | MS_BIT_RESERVED   | MS_BIT_11_10      | MS_BIT_RESERVED   | MS_BIT_07_06      | MS_BIT_RESERVED   | MS_BIT_03_02      | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 24, Module Controller (CMNN)   */
    [MODULE_NUM_CMNN] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 25, Module Controller (CMNS)   */
    [MODULE_NUM_CMNS] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 26, Module Controller (SCP)    */
    [MODULE_NUM_SCP] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_03_02      | MS_BIT_RESERVED   },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_SKIP       },      /* [01:00] SCP  */

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_07_06      | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    },

    /* Module Number 27, Module Controller (AON)   */
    [MODULE_NUM_AON] =
    {   /*          bit_assign_info                                                                                                                                                     *
         *              [31:30]	        | [29:28]           | [27:26]           | [25:24]           | [23:22]           | [21:20]           | [19:18]           | [17:16]           \   *
         *          |   [15:14]         | [13:12]           | [11:10]           | [9:8]             | [7:6]             | [5:4]             | [3:2]             | [1:0]                 */
        [0]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [1]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [2]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [3]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [4]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [5]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [6]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [7]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [8]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [9]     = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [10]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [11]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [12]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [13]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [14]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [15]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [16]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [17]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [18]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [19]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [20]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },

        [21]    = {     MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   \
                    |   MS_BIT_RESERVED | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   | MS_BIT_RESERVED   },
    }
};


static const MDLC_MS_REG_TABLE ms_reg_table[MODULE_NUM_MAX][MS_REG_MAX_NUM] =
{
    /* Module Number 00, Module Controller (VIPN)   */
    [MODULE_NUM_VIPN] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC00MSRES00,      MDLC00MSRESS00,     ms_reg_bit_assign[MODULE_NUM_VIPN][0U].bit_assign_info     },
        [1]     = { MDLC00MSRES01,      MDLC00MSRESS01,     ms_reg_bit_assign[MODULE_NUM_VIPN][1U].bit_assign_info     },
        [2]     = { MDLC00MSRES02,      MDLC00MSRESS02,     ms_reg_bit_assign[MODULE_NUM_VIPN][2U].bit_assign_info     },
        [3]     = { MDLC00MSRES03,      MDLC00MSRESS03,     ms_reg_bit_assign[MODULE_NUM_VIPN][3U].bit_assign_info     },
        [4]     = { MDLC00MSRES04,      MDLC00MSRESS04,     ms_reg_bit_assign[MODULE_NUM_VIPN][4U].bit_assign_info     },
        [5]     = { MDLC00MSRES05,      MDLC00MSRESS05,     ms_reg_bit_assign[MODULE_NUM_VIPN][5U].bit_assign_info     },
        [6]     = { MDLC00MSRES06,      MDLC00MSRESS06,     ms_reg_bit_assign[MODULE_NUM_VIPN][6U].bit_assign_info     },
        [7]     = { MDLC00MSRES07,      MDLC00MSRESS07,     ms_reg_bit_assign[MODULE_NUM_VIPN][7U].bit_assign_info     },
        [8]     = { MDLC00MSRES08,      MDLC00MSRESS08,     ms_reg_bit_assign[MODULE_NUM_VIPN][8U].bit_assign_info     },
        [9]     = { MDLC00MSRES09,      MDLC00MSRESS09,     ms_reg_bit_assign[MODULE_NUM_VIPN][9U].bit_assign_info     },
        [10]    = { MDLC00MSRES10,      MDLC00MSRESS10,     ms_reg_bit_assign[MODULE_NUM_VIPN][10U].bit_assign_info    },
        [11]    = { MDLC00MSRES11,      MDLC00MSRESS11,     ms_reg_bit_assign[MODULE_NUM_VIPN][11U].bit_assign_info    },
        [12]    = { MDLC00MSRES12,      MDLC00MSRESS12,     ms_reg_bit_assign[MODULE_NUM_VIPN][12U].bit_assign_info    },
        [13]    = { MDLC00MSRES13,      MDLC00MSRESS13,     ms_reg_bit_assign[MODULE_NUM_VIPN][13U].bit_assign_info    },
        [14]    = { MDLC00MSRES14,      MDLC00MSRESS14,     ms_reg_bit_assign[MODULE_NUM_VIPN][14U].bit_assign_info    },
        [15]    = { MDLC00MSRES15,      MDLC00MSRESS15,     ms_reg_bit_assign[MODULE_NUM_VIPN][15U].bit_assign_info    },
        [16]    = { MDLC00MSRES16,      MDLC00MSRESS16,     ms_reg_bit_assign[MODULE_NUM_VIPN][16U].bit_assign_info    },
        [17]    = { MDLC00MSRES17,      MDLC00MSRESS17,     ms_reg_bit_assign[MODULE_NUM_VIPN][17U].bit_assign_info    },
        [18]    = { MDLC00MSRES18,      MDLC00MSRESS18,     ms_reg_bit_assign[MODULE_NUM_VIPN][18U].bit_assign_info    },
        [19]    = { MDLC00MSRES19,      MDLC00MSRESS19,     ms_reg_bit_assign[MODULE_NUM_VIPN][19U].bit_assign_info    },
        [20]    = { MDLC00MSRES20,      MDLC00MSRESS20,     ms_reg_bit_assign[MODULE_NUM_VIPN][20U].bit_assign_info    },
        [21]    = { MDLC00MSRES21,      MDLC00MSRESS21,     ms_reg_bit_assign[MODULE_NUM_VIPN][21U].bit_assign_info    },
    },

    /* Module Number 01, Module Controller (VIPS)   */
    [MODULE_NUM_VIPS] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC01MSRES00,      MDLC01MSRESS00,     ms_reg_bit_assign[MODULE_NUM_VIPS][0U].bit_assign_info     },
        [1]     = { MDLC01MSRES01,      MDLC01MSRESS01,     ms_reg_bit_assign[MODULE_NUM_VIPS][1U].bit_assign_info     },
        [2]     = { MDLC01MSRES02,      MDLC01MSRESS02,     ms_reg_bit_assign[MODULE_NUM_VIPS][2U].bit_assign_info     },
        [3]     = { MDLC01MSRES03,      MDLC01MSRESS03,     ms_reg_bit_assign[MODULE_NUM_VIPS][3U].bit_assign_info     },
        [4]     = { MDLC01MSRES04,      MDLC01MSRESS04,     ms_reg_bit_assign[MODULE_NUM_VIPS][4U].bit_assign_info     },
        [5]     = { MDLC01MSRES05,      MDLC01MSRESS05,     ms_reg_bit_assign[MODULE_NUM_VIPS][5U].bit_assign_info     },
        [6]     = { MDLC01MSRES06,      MDLC01MSRESS06,     ms_reg_bit_assign[MODULE_NUM_VIPS][6U].bit_assign_info     },
        [7]     = { MDLC01MSRES07,      MDLC01MSRESS07,     ms_reg_bit_assign[MODULE_NUM_VIPS][7U].bit_assign_info     },
        [8]     = { MDLC01MSRES08,      MDLC01MSRESS08,     ms_reg_bit_assign[MODULE_NUM_VIPS][8U].bit_assign_info     },
        [9]     = { MDLC01MSRES09,      MDLC01MSRESS09,     ms_reg_bit_assign[MODULE_NUM_VIPS][9U].bit_assign_info     },
        [10]    = { MDLC01MSRES10,      MDLC01MSRESS10,     ms_reg_bit_assign[MODULE_NUM_VIPS][10U].bit_assign_info    },
        [11]    = { MDLC01MSRES11,      MDLC01MSRESS11,     ms_reg_bit_assign[MODULE_NUM_VIPS][11U].bit_assign_info    },
        [12]    = { MDLC01MSRES12,      MDLC01MSRESS12,     ms_reg_bit_assign[MODULE_NUM_VIPS][12U].bit_assign_info    },
        [13]    = { MDLC01MSRES13,      MDLC01MSRESS13,     ms_reg_bit_assign[MODULE_NUM_VIPS][13U].bit_assign_info    },
        [14]    = { MDLC01MSRES14,      MDLC01MSRESS14,     ms_reg_bit_assign[MODULE_NUM_VIPS][14U].bit_assign_info    },
        [15]    = { MDLC01MSRES15,      MDLC01MSRESS15,     ms_reg_bit_assign[MODULE_NUM_VIPS][15U].bit_assign_info    },
        [16]    = { MDLC01MSRES16,      MDLC01MSRESS16,     ms_reg_bit_assign[MODULE_NUM_VIPS][16U].bit_assign_info    },
        [17]    = { MDLC01MSRES17,      MDLC01MSRESS17,     ms_reg_bit_assign[MODULE_NUM_VIPS][17U].bit_assign_info    },
        [18]    = { MDLC01MSRES18,      MDLC01MSRESS18,     ms_reg_bit_assign[MODULE_NUM_VIPS][18U].bit_assign_info    },
        [19]    = { MDLC01MSRES19,      MDLC01MSRESS19,     ms_reg_bit_assign[MODULE_NUM_VIPS][19U].bit_assign_info    },
        [20]    = { MDLC01MSRES20,      MDLC01MSRESS20,     ms_reg_bit_assign[MODULE_NUM_VIPS][20U].bit_assign_info    },
        [21]    = { MDLC01MSRES21,      MDLC01MSRESS21,     ms_reg_bit_assign[MODULE_NUM_VIPS][21U].bit_assign_info    },
    },

    /* Module Number 02, Module Controller (VIO)    */
    [MODULE_NUM_VIO] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC02MSRES00,      MDLC02MSRESS00,     ms_reg_bit_assign[MODULE_NUM_VIO][0U].bit_assign_info      },
        [1]     = { MDLC02MSRES01,      MDLC02MSRESS01,     ms_reg_bit_assign[MODULE_NUM_VIO][1U].bit_assign_info      },
        [2]     = { MDLC02MSRES02,      MDLC02MSRESS02,     ms_reg_bit_assign[MODULE_NUM_VIO][2U].bit_assign_info      },
        [3]     = { MDLC02MSRES03,      MDLC02MSRESS03,     ms_reg_bit_assign[MODULE_NUM_VIO][3U].bit_assign_info      },
        [4]     = { MDLC02MSRES04,      MDLC02MSRESS04,     ms_reg_bit_assign[MODULE_NUM_VIO][4U].bit_assign_info      },
        [5]     = { MDLC02MSRES05,      MDLC02MSRESS05,     ms_reg_bit_assign[MODULE_NUM_VIO][5U].bit_assign_info      },
        [6]     = { MDLC02MSRES06,      MDLC02MSRESS06,     ms_reg_bit_assign[MODULE_NUM_VIO][6U].bit_assign_info      },
        [7]     = { MDLC02MSRES07,      MDLC02MSRESS07,     ms_reg_bit_assign[MODULE_NUM_VIO][7U].bit_assign_info      },
        [8]     = { MDLC02MSRES08,      MDLC02MSRESS08,     ms_reg_bit_assign[MODULE_NUM_VIO][8U].bit_assign_info      },
        [9]     = { MDLC02MSRES09,      MDLC02MSRESS09,     ms_reg_bit_assign[MODULE_NUM_VIO][9U].bit_assign_info      },
        [10]    = { MDLC02MSRES10,      MDLC02MSRESS10,     ms_reg_bit_assign[MODULE_NUM_VIO][10U].bit_assign_info     },
        [11]    = { MDLC02MSRES11,      MDLC02MSRESS11,     ms_reg_bit_assign[MODULE_NUM_VIO][11U].bit_assign_info     },
        [12]    = { MDLC02MSRES12,      MDLC02MSRESS12,     ms_reg_bit_assign[MODULE_NUM_VIO][12U].bit_assign_info     },
        [13]    = { MDLC02MSRES13,      MDLC02MSRESS13,     ms_reg_bit_assign[MODULE_NUM_VIO][13U].bit_assign_info     },
        [14]    = { MDLC02MSRES14,      MDLC02MSRESS14,     ms_reg_bit_assign[MODULE_NUM_VIO][14U].bit_assign_info     },
        [15]    = { MDLC02MSRES15,      MDLC02MSRESS15,     ms_reg_bit_assign[MODULE_NUM_VIO][15U].bit_assign_info     },
        [16]    = { MDLC02MSRES16,      MDLC02MSRESS16,     ms_reg_bit_assign[MODULE_NUM_VIO][16U].bit_assign_info     },
        [17]    = { MDLC02MSRES17,      MDLC02MSRESS17,     ms_reg_bit_assign[MODULE_NUM_VIO][17U].bit_assign_info     },
        [18]    = { MDLC02MSRES18,      MDLC02MSRESS18,     ms_reg_bit_assign[MODULE_NUM_VIO][18U].bit_assign_info     },
        [19]    = { MDLC02MSRES19,      MDLC02MSRESS19,     ms_reg_bit_assign[MODULE_NUM_VIO][19U].bit_assign_info     },
        [20]    = { MDLC02MSRES20,      MDLC02MSRESS20,     ms_reg_bit_assign[MODULE_NUM_VIO][20U].bit_assign_info     },
        [21]    = { MDLC02MSRES21,      MDLC02MSRESS21,     ms_reg_bit_assign[MODULE_NUM_VIO][21U].bit_assign_info     },
    },

    /* Module Number 03, Module Controller (PERE)   */
    [MODULE_NUM_PERE] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC03MSRES00,      MDLC03MSRESS00,     ms_reg_bit_assign[MODULE_NUM_PERE][0U].bit_assign_info     },
        [1]     = { MDLC03MSRES01,      MDLC03MSRESS01,     ms_reg_bit_assign[MODULE_NUM_PERE][1U].bit_assign_info     },
        [2]     = { MDLC03MSRES02,      MDLC03MSRESS02,     ms_reg_bit_assign[MODULE_NUM_PERE][2U].bit_assign_info     },
        [3]     = { MDLC03MSRES03,      MDLC03MSRESS03,     ms_reg_bit_assign[MODULE_NUM_PERE][3U].bit_assign_info     },
        [4]     = { MDLC03MSRES04,      MDLC03MSRESS04,     ms_reg_bit_assign[MODULE_NUM_PERE][4U].bit_assign_info     },
        [5]     = { MDLC03MSRES05,      MDLC03MSRESS05,     ms_reg_bit_assign[MODULE_NUM_PERE][5U].bit_assign_info     },
        [6]     = { MDLC03MSRES06,      MDLC03MSRESS06,     ms_reg_bit_assign[MODULE_NUM_PERE][6U].bit_assign_info     },
        [7]     = { MDLC03MSRES07,      MDLC03MSRESS07,     ms_reg_bit_assign[MODULE_NUM_PERE][7U].bit_assign_info     },
        [8]     = { MDLC03MSRES08,      MDLC03MSRESS08,     ms_reg_bit_assign[MODULE_NUM_PERE][8U].bit_assign_info     },
        [9]     = { MDLC03MSRES09,      MDLC03MSRESS09,     ms_reg_bit_assign[MODULE_NUM_PERE][9U].bit_assign_info     },
        [10]    = { MDLC03MSRES10,      MDLC03MSRESS10,     ms_reg_bit_assign[MODULE_NUM_PERE][10U].bit_assign_info    },
        [11]    = { MDLC03MSRES11,      MDLC03MSRESS11,     ms_reg_bit_assign[MODULE_NUM_PERE][11U].bit_assign_info    },
        [12]    = { MDLC03MSRES12,      MDLC03MSRESS12,     ms_reg_bit_assign[MODULE_NUM_PERE][12U].bit_assign_info    },
        [13]    = { MDLC03MSRES13,      MDLC03MSRESS13,     ms_reg_bit_assign[MODULE_NUM_PERE][13U].bit_assign_info    },
        [14]    = { MDLC03MSRES14,      MDLC03MSRESS14,     ms_reg_bit_assign[MODULE_NUM_PERE][14U].bit_assign_info    },
        [15]    = { MDLC03MSRES15,      MDLC03MSRESS15,     ms_reg_bit_assign[MODULE_NUM_PERE][15U].bit_assign_info    },
        [16]    = { MDLC03MSRES16,      MDLC03MSRESS16,     ms_reg_bit_assign[MODULE_NUM_PERE][16U].bit_assign_info    },
        [17]    = { MDLC03MSRES17,      MDLC03MSRESS17,     ms_reg_bit_assign[MODULE_NUM_PERE][17U].bit_assign_info    },
        [18]    = { MDLC03MSRES18,      MDLC03MSRESS18,     ms_reg_bit_assign[MODULE_NUM_PERE][18U].bit_assign_info    },
        [19]    = { MDLC03MSRES19,      MDLC03MSRESS19,     ms_reg_bit_assign[MODULE_NUM_PERE][19U].bit_assign_info    },
        [20]    = { MDLC03MSRES20,      MDLC03MSRESS20,     ms_reg_bit_assign[MODULE_NUM_PERE][20U].bit_assign_info    },
        [21]    = { MDLC03MSRES21,      MDLC03MSRESS21,     ms_reg_bit_assign[MODULE_NUM_PERE][21U].bit_assign_info    },
    },

    /* Module Number 04, Module Controller (PERW)   */
    [MODULE_NUM_PERW] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC04MSRES00,      MDLC04MSRESS00,     ms_reg_bit_assign[MODULE_NUM_PERW][0U].bit_assign_info     },
        [1]     = { MDLC04MSRES01,      MDLC04MSRESS01,     ms_reg_bit_assign[MODULE_NUM_PERW][1U].bit_assign_info     },
        [2]     = { MDLC04MSRES02,      MDLC04MSRESS02,     ms_reg_bit_assign[MODULE_NUM_PERW][2U].bit_assign_info     },
        [3]     = { MDLC04MSRES03,      MDLC04MSRESS03,     ms_reg_bit_assign[MODULE_NUM_PERW][3U].bit_assign_info     },
        [4]     = { MDLC04MSRES04,      MDLC04MSRESS04,     ms_reg_bit_assign[MODULE_NUM_PERW][4U].bit_assign_info     },
        [5]     = { MDLC04MSRES05,      MDLC04MSRESS05,     ms_reg_bit_assign[MODULE_NUM_PERW][5U].bit_assign_info     },
        [6]     = { MDLC04MSRES06,      MDLC04MSRESS06,     ms_reg_bit_assign[MODULE_NUM_PERW][6U].bit_assign_info     },
        [7]     = { MDLC04MSRES07,      MDLC04MSRESS07,     ms_reg_bit_assign[MODULE_NUM_PERW][7U].bit_assign_info     },
        [8]     = { MDLC04MSRES08,      MDLC04MSRESS08,     ms_reg_bit_assign[MODULE_NUM_PERW][8U].bit_assign_info     },
        [9]     = { MDLC04MSRES09,      MDLC04MSRESS09,     ms_reg_bit_assign[MODULE_NUM_PERW][9U].bit_assign_info     },
        [10]    = { MDLC04MSRES10,      MDLC04MSRESS10,     ms_reg_bit_assign[MODULE_NUM_PERW][10U].bit_assign_info    },
        [11]    = { MDLC04MSRES11,      MDLC04MSRESS11,     ms_reg_bit_assign[MODULE_NUM_PERW][11U].bit_assign_info    },
        [12]    = { MDLC04MSRES12,      MDLC04MSRESS12,     ms_reg_bit_assign[MODULE_NUM_PERW][12U].bit_assign_info    },
        [13]    = { MDLC04MSRES13,      MDLC04MSRESS13,     ms_reg_bit_assign[MODULE_NUM_PERW][13U].bit_assign_info    },
        [14]    = { MDLC04MSRES14,      MDLC04MSRESS14,     ms_reg_bit_assign[MODULE_NUM_PERW][14U].bit_assign_info    },
        [15]    = { MDLC04MSRES15,      MDLC04MSRESS15,     ms_reg_bit_assign[MODULE_NUM_PERW][15U].bit_assign_info    },
        [16]    = { MDLC04MSRES16,      MDLC04MSRESS16,     ms_reg_bit_assign[MODULE_NUM_PERW][16U].bit_assign_info    },
        [17]    = { MDLC04MSRES17,      MDLC04MSRESS17,     ms_reg_bit_assign[MODULE_NUM_PERW][17U].bit_assign_info    },
        [18]    = { MDLC04MSRES18,      MDLC04MSRESS18,     ms_reg_bit_assign[MODULE_NUM_PERW][18U].bit_assign_info    },
        [19]    = { MDLC04MSRES19,      MDLC04MSRESS19,     ms_reg_bit_assign[MODULE_NUM_PERW][19U].bit_assign_info    },
        [20]    = { MDLC04MSRES20,      MDLC04MSRESS20,     ms_reg_bit_assign[MODULE_NUM_PERW][20U].bit_assign_info    },
        [21]    = { MDLC04MSRES21,      MDLC04MSRESS21,     ms_reg_bit_assign[MODULE_NUM_PERW][21U].bit_assign_info    },
    },

    /* Module Number 05, Module Controller (DDR0)   */
    [MODULE_NUM_DDR0] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC05MSRES00,      MDLC05MSRESS00,     ms_reg_bit_assign[MODULE_NUM_DDR0][0U].bit_assign_info     },
        [1]     = { MDLC05MSRES01,      MDLC05MSRESS01,     ms_reg_bit_assign[MODULE_NUM_DDR0][1U].bit_assign_info     },
        [2]     = { MDLC05MSRES02,      MDLC05MSRESS02,     ms_reg_bit_assign[MODULE_NUM_DDR0][2U].bit_assign_info     },
        [3]     = { MDLC05MSRES03,      MDLC05MSRESS03,     ms_reg_bit_assign[MODULE_NUM_DDR0][3U].bit_assign_info     },
        [4]     = { MDLC05MSRES04,      MDLC05MSRESS04,     ms_reg_bit_assign[MODULE_NUM_DDR0][4U].bit_assign_info     },
        [5]     = { MDLC05MSRES05,      MDLC05MSRESS05,     ms_reg_bit_assign[MODULE_NUM_DDR0][5U].bit_assign_info     },
        [6]     = { MDLC05MSRES06,      MDLC05MSRESS06,     ms_reg_bit_assign[MODULE_NUM_DDR0][6U].bit_assign_info     },
        [7]     = { MDLC05MSRES07,      MDLC05MSRESS07,     ms_reg_bit_assign[MODULE_NUM_DDR0][7U].bit_assign_info     },
        [8]     = { MDLC05MSRES08,      MDLC05MSRESS08,     ms_reg_bit_assign[MODULE_NUM_DDR0][8U].bit_assign_info     },
        [9]     = { MDLC05MSRES09,      MDLC05MSRESS09,     ms_reg_bit_assign[MODULE_NUM_DDR0][9U].bit_assign_info     },
        [10]    = { MDLC05MSRES10,      MDLC05MSRESS10,     ms_reg_bit_assign[MODULE_NUM_DDR0][10U].bit_assign_info    },
        [11]    = { MDLC05MSRES11,      MDLC05MSRESS11,     ms_reg_bit_assign[MODULE_NUM_DDR0][11U].bit_assign_info    },
        [12]    = { MDLC05MSRES12,      MDLC05MSRESS12,     ms_reg_bit_assign[MODULE_NUM_DDR0][12U].bit_assign_info    },
        [13]    = { MDLC05MSRES13,      MDLC05MSRESS13,     ms_reg_bit_assign[MODULE_NUM_DDR0][13U].bit_assign_info    },
        [14]    = { MDLC05MSRES14,      MDLC05MSRESS14,     ms_reg_bit_assign[MODULE_NUM_DDR0][14U].bit_assign_info    },
        [15]    = { MDLC05MSRES15,      MDLC05MSRESS15,     ms_reg_bit_assign[MODULE_NUM_DDR0][15U].bit_assign_info    },
        [16]    = { MDLC05MSRES16,      MDLC05MSRESS16,     ms_reg_bit_assign[MODULE_NUM_DDR0][16U].bit_assign_info    },
        [17]    = { MDLC05MSRES17,      MDLC05MSRESS17,     ms_reg_bit_assign[MODULE_NUM_DDR0][17U].bit_assign_info    },
        [18]    = { MDLC05MSRES18,      MDLC05MSRESS18,     ms_reg_bit_assign[MODULE_NUM_DDR0][18U].bit_assign_info    },
        [19]    = { MDLC05MSRES19,      MDLC05MSRESS19,     ms_reg_bit_assign[MODULE_NUM_DDR0][19U].bit_assign_info    },
        [20]    = { MDLC05MSRES20,      MDLC05MSRESS20,     ms_reg_bit_assign[MODULE_NUM_DDR0][20U].bit_assign_info    },
        [21]    = { MDLC05MSRES21,      MDLC05MSRESS21,     ms_reg_bit_assign[MODULE_NUM_DDR0][21U].bit_assign_info    },
    },

    /* Module Number 06, Module Controller (DDR1)   */
    [MODULE_NUM_DDR1] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC06MSRES00,      MDLC06MSRESS00,     ms_reg_bit_assign[MODULE_NUM_DDR1][0U].bit_assign_info     },
        [1]     = { MDLC06MSRES01,      MDLC06MSRESS01,     ms_reg_bit_assign[MODULE_NUM_DDR1][1U].bit_assign_info     },
        [2]     = { MDLC06MSRES02,      MDLC06MSRESS02,     ms_reg_bit_assign[MODULE_NUM_DDR1][2U].bit_assign_info     },
        [3]     = { MDLC06MSRES03,      MDLC06MSRESS03,     ms_reg_bit_assign[MODULE_NUM_DDR1][3U].bit_assign_info     },
        [4]     = { MDLC06MSRES04,      MDLC06MSRESS04,     ms_reg_bit_assign[MODULE_NUM_DDR1][4U].bit_assign_info     },
        [5]     = { MDLC06MSRES05,      MDLC06MSRESS05,     ms_reg_bit_assign[MODULE_NUM_DDR1][5U].bit_assign_info     },
        [6]     = { MDLC06MSRES06,      MDLC06MSRESS06,     ms_reg_bit_assign[MODULE_NUM_DDR1][6U].bit_assign_info     },
        [7]     = { MDLC06MSRES07,      MDLC06MSRESS07,     ms_reg_bit_assign[MODULE_NUM_DDR1][7U].bit_assign_info     },
        [8]     = { MDLC06MSRES08,      MDLC06MSRESS08,     ms_reg_bit_assign[MODULE_NUM_DDR1][8U].bit_assign_info     },
        [9]     = { MDLC06MSRES09,      MDLC06MSRESS09,     ms_reg_bit_assign[MODULE_NUM_DDR1][9U].bit_assign_info     },
        [10]    = { MDLC06MSRES10,      MDLC06MSRESS10,     ms_reg_bit_assign[MODULE_NUM_DDR1][10U].bit_assign_info    },
        [11]    = { MDLC06MSRES11,      MDLC06MSRESS11,     ms_reg_bit_assign[MODULE_NUM_DDR1][11U].bit_assign_info    },
        [12]    = { MDLC06MSRES12,      MDLC06MSRESS12,     ms_reg_bit_assign[MODULE_NUM_DDR1][12U].bit_assign_info    },
        [13]    = { MDLC06MSRES13,      MDLC06MSRESS13,     ms_reg_bit_assign[MODULE_NUM_DDR1][13U].bit_assign_info    },
        [14]    = { MDLC06MSRES14,      MDLC06MSRESS14,     ms_reg_bit_assign[MODULE_NUM_DDR1][14U].bit_assign_info    },
        [15]    = { MDLC06MSRES15,      MDLC06MSRESS15,     ms_reg_bit_assign[MODULE_NUM_DDR1][15U].bit_assign_info    },
        [16]    = { MDLC06MSRES16,      MDLC06MSRESS16,     ms_reg_bit_assign[MODULE_NUM_DDR1][16U].bit_assign_info    },
        [17]    = { MDLC06MSRES17,      MDLC06MSRESS17,     ms_reg_bit_assign[MODULE_NUM_DDR1][17U].bit_assign_info    },
        [18]    = { MDLC06MSRES18,      MDLC06MSRESS18,     ms_reg_bit_assign[MODULE_NUM_DDR1][18U].bit_assign_info    },
        [19]    = { MDLC06MSRES19,      MDLC06MSRESS19,     ms_reg_bit_assign[MODULE_NUM_DDR1][19U].bit_assign_info    },
        [20]    = { MDLC06MSRES20,      MDLC06MSRESS20,     ms_reg_bit_assign[MODULE_NUM_DDR1][20U].bit_assign_info    },
        [21]    = { MDLC06MSRES21,      MDLC06MSRESS21,     ms_reg_bit_assign[MODULE_NUM_DDR1][21U].bit_assign_info    },
    },

    /* Module Number 07, Module Controller (DDR2)   */
    [MODULE_NUM_DDR2] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC07MSRES00,      MDLC07MSRESS00,     ms_reg_bit_assign[MODULE_NUM_DDR2][0U].bit_assign_info     },
        [1]     = { MDLC07MSRES01,      MDLC07MSRESS01,     ms_reg_bit_assign[MODULE_NUM_DDR2][1U].bit_assign_info     },
        [2]     = { MDLC07MSRES02,      MDLC07MSRESS02,     ms_reg_bit_assign[MODULE_NUM_DDR2][2U].bit_assign_info     },
        [3]     = { MDLC07MSRES03,      MDLC07MSRESS03,     ms_reg_bit_assign[MODULE_NUM_DDR2][3U].bit_assign_info     },
        [4]     = { MDLC07MSRES04,      MDLC07MSRESS04,     ms_reg_bit_assign[MODULE_NUM_DDR2][4U].bit_assign_info     },
        [5]     = { MDLC07MSRES05,      MDLC07MSRESS05,     ms_reg_bit_assign[MODULE_NUM_DDR2][5U].bit_assign_info     },
        [6]     = { MDLC07MSRES06,      MDLC07MSRESS06,     ms_reg_bit_assign[MODULE_NUM_DDR2][6U].bit_assign_info     },
        [7]     = { MDLC07MSRES07,      MDLC07MSRESS07,     ms_reg_bit_assign[MODULE_NUM_DDR2][7U].bit_assign_info     },
        [8]     = { MDLC07MSRES08,      MDLC07MSRESS08,     ms_reg_bit_assign[MODULE_NUM_DDR2][8U].bit_assign_info     },
        [9]     = { MDLC07MSRES09,      MDLC07MSRESS09,     ms_reg_bit_assign[MODULE_NUM_DDR2][9U].bit_assign_info     },
        [10]    = { MDLC07MSRES10,      MDLC07MSRESS10,     ms_reg_bit_assign[MODULE_NUM_DDR2][10U].bit_assign_info    },
        [11]    = { MDLC07MSRES11,      MDLC07MSRESS11,     ms_reg_bit_assign[MODULE_NUM_DDR2][11U].bit_assign_info    },
        [12]    = { MDLC07MSRES12,      MDLC07MSRESS12,     ms_reg_bit_assign[MODULE_NUM_DDR2][12U].bit_assign_info    },
        [13]    = { MDLC07MSRES13,      MDLC07MSRESS13,     ms_reg_bit_assign[MODULE_NUM_DDR2][13U].bit_assign_info    },
        [14]    = { MDLC07MSRES14,      MDLC07MSRESS14,     ms_reg_bit_assign[MODULE_NUM_DDR2][14U].bit_assign_info    },
        [15]    = { MDLC07MSRES15,      MDLC07MSRESS15,     ms_reg_bit_assign[MODULE_NUM_DDR2][15U].bit_assign_info    },
        [16]    = { MDLC07MSRES16,      MDLC07MSRESS16,     ms_reg_bit_assign[MODULE_NUM_DDR2][16U].bit_assign_info    },
        [17]    = { MDLC07MSRES17,      MDLC07MSRESS17,     ms_reg_bit_assign[MODULE_NUM_DDR2][17U].bit_assign_info    },
        [18]    = { MDLC07MSRES18,      MDLC07MSRESS18,     ms_reg_bit_assign[MODULE_NUM_DDR2][18U].bit_assign_info    },
        [19]    = { MDLC07MSRES19,      MDLC07MSRESS19,     ms_reg_bit_assign[MODULE_NUM_DDR2][19U].bit_assign_info    },
        [20]    = { MDLC07MSRES20,      MDLC07MSRESS20,     ms_reg_bit_assign[MODULE_NUM_DDR2][20U].bit_assign_info    },
        [21]    = { MDLC07MSRES21,      MDLC07MSRESS21,     ms_reg_bit_assign[MODULE_NUM_DDR2][21U].bit_assign_info    },
    },

    /* Module Number 08, Module Controller (DDR3)   */
    [MODULE_NUM_DDR3] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC08MSRES00,      MDLC08MSRESS00,     ms_reg_bit_assign[MODULE_NUM_DDR3][0U].bit_assign_info     },
        [1]     = { MDLC08MSRES01,      MDLC08MSRESS01,     ms_reg_bit_assign[MODULE_NUM_DDR3][1U].bit_assign_info     },
        [2]     = { MDLC08MSRES02,      MDLC08MSRESS02,     ms_reg_bit_assign[MODULE_NUM_DDR3][2U].bit_assign_info     },
        [3]     = { MDLC08MSRES03,      MDLC08MSRESS03,     ms_reg_bit_assign[MODULE_NUM_DDR3][3U].bit_assign_info     },
        [4]     = { MDLC08MSRES04,      MDLC08MSRESS04,     ms_reg_bit_assign[MODULE_NUM_DDR3][4U].bit_assign_info     },
        [5]     = { MDLC08MSRES05,      MDLC08MSRESS05,     ms_reg_bit_assign[MODULE_NUM_DDR3][5U].bit_assign_info     },
        [6]     = { MDLC08MSRES06,      MDLC08MSRESS06,     ms_reg_bit_assign[MODULE_NUM_DDR3][6U].bit_assign_info     },
        [7]     = { MDLC08MSRES07,      MDLC08MSRESS07,     ms_reg_bit_assign[MODULE_NUM_DDR3][7U].bit_assign_info     },
        [8]     = { MDLC08MSRES08,      MDLC08MSRESS08,     ms_reg_bit_assign[MODULE_NUM_DDR3][8U].bit_assign_info     },
        [9]     = { MDLC08MSRES09,      MDLC08MSRESS09,     ms_reg_bit_assign[MODULE_NUM_DDR3][9U].bit_assign_info     },
        [10]    = { MDLC08MSRES10,      MDLC08MSRESS10,     ms_reg_bit_assign[MODULE_NUM_DDR3][10U].bit_assign_info    },
        [11]    = { MDLC08MSRES11,      MDLC08MSRESS11,     ms_reg_bit_assign[MODULE_NUM_DDR3][11U].bit_assign_info    },
        [12]    = { MDLC08MSRES12,      MDLC08MSRESS12,     ms_reg_bit_assign[MODULE_NUM_DDR3][12U].bit_assign_info    },
        [13]    = { MDLC08MSRES13,      MDLC08MSRESS13,     ms_reg_bit_assign[MODULE_NUM_DDR3][13U].bit_assign_info    },
        [14]    = { MDLC08MSRES14,      MDLC08MSRESS14,     ms_reg_bit_assign[MODULE_NUM_DDR3][14U].bit_assign_info    },
        [15]    = { MDLC08MSRES15,      MDLC08MSRESS15,     ms_reg_bit_assign[MODULE_NUM_DDR3][15U].bit_assign_info    },
        [16]    = { MDLC08MSRES16,      MDLC08MSRESS16,     ms_reg_bit_assign[MODULE_NUM_DDR3][16U].bit_assign_info    },
        [17]    = { MDLC08MSRES17,      MDLC08MSRESS17,     ms_reg_bit_assign[MODULE_NUM_DDR3][17U].bit_assign_info    },
        [18]    = { MDLC08MSRES18,      MDLC08MSRESS18,     ms_reg_bit_assign[MODULE_NUM_DDR3][18U].bit_assign_info    },
        [19]    = { MDLC08MSRES19,      MDLC08MSRESS19,     ms_reg_bit_assign[MODULE_NUM_DDR3][19U].bit_assign_info    },
        [20]    = { MDLC08MSRES20,      MDLC08MSRESS20,     ms_reg_bit_assign[MODULE_NUM_DDR3][20U].bit_assign_info    },
        [21]    = { MDLC08MSRES21,      MDLC08MSRESS21,     ms_reg_bit_assign[MODULE_NUM_DDR3][21U].bit_assign_info    },
    },

    /* Module Number 09, Module Controller (DDR4)   */
    [MODULE_NUM_DDR4] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC09MSRES00,      MDLC09MSRESS00,     ms_reg_bit_assign[MODULE_NUM_DDR4][0U].bit_assign_info     },
        [1]     = { MDLC09MSRES01,      MDLC09MSRESS01,     ms_reg_bit_assign[MODULE_NUM_DDR4][1U].bit_assign_info     },
        [2]     = { MDLC09MSRES02,      MDLC09MSRESS02,     ms_reg_bit_assign[MODULE_NUM_DDR4][2U].bit_assign_info     },
        [3]     = { MDLC09MSRES03,      MDLC09MSRESS03,     ms_reg_bit_assign[MODULE_NUM_DDR4][3U].bit_assign_info     },
        [4]     = { MDLC09MSRES04,      MDLC09MSRESS04,     ms_reg_bit_assign[MODULE_NUM_DDR4][4U].bit_assign_info     },
        [5]     = { MDLC09MSRES05,      MDLC09MSRESS05,     ms_reg_bit_assign[MODULE_NUM_DDR4][5U].bit_assign_info     },
        [6]     = { MDLC09MSRES06,      MDLC09MSRESS06,     ms_reg_bit_assign[MODULE_NUM_DDR4][6U].bit_assign_info     },
        [7]     = { MDLC09MSRES07,      MDLC09MSRESS07,     ms_reg_bit_assign[MODULE_NUM_DDR4][7U].bit_assign_info     },
        [8]     = { MDLC09MSRES08,      MDLC09MSRESS08,     ms_reg_bit_assign[MODULE_NUM_DDR4][8U].bit_assign_info     },
        [9]     = { MDLC09MSRES09,      MDLC09MSRESS09,     ms_reg_bit_assign[MODULE_NUM_DDR4][9U].bit_assign_info     },
        [10]    = { MDLC09MSRES10,      MDLC09MSRESS10,     ms_reg_bit_assign[MODULE_NUM_DDR4][10U].bit_assign_info    },
        [11]    = { MDLC09MSRES11,      MDLC09MSRESS11,     ms_reg_bit_assign[MODULE_NUM_DDR4][11U].bit_assign_info    },
        [12]    = { MDLC09MSRES12,      MDLC09MSRESS12,     ms_reg_bit_assign[MODULE_NUM_DDR4][12U].bit_assign_info    },
        [13]    = { MDLC09MSRES13,      MDLC09MSRESS13,     ms_reg_bit_assign[MODULE_NUM_DDR4][13U].bit_assign_info    },
        [14]    = { MDLC09MSRES14,      MDLC09MSRESS14,     ms_reg_bit_assign[MODULE_NUM_DDR4][14U].bit_assign_info    },
        [15]    = { MDLC09MSRES15,      MDLC09MSRESS15,     ms_reg_bit_assign[MODULE_NUM_DDR4][15U].bit_assign_info    },
        [16]    = { MDLC09MSRES16,      MDLC09MSRESS16,     ms_reg_bit_assign[MODULE_NUM_DDR4][16U].bit_assign_info    },
        [17]    = { MDLC09MSRES17,      MDLC09MSRESS17,     ms_reg_bit_assign[MODULE_NUM_DDR4][17U].bit_assign_info    },
        [18]    = { MDLC09MSRES18,      MDLC09MSRESS18,     ms_reg_bit_assign[MODULE_NUM_DDR4][18U].bit_assign_info    },
        [19]    = { MDLC09MSRES19,      MDLC09MSRESS19,     ms_reg_bit_assign[MODULE_NUM_DDR4][19U].bit_assign_info    },
        [20]    = { MDLC09MSRES20,      MDLC09MSRESS20,     ms_reg_bit_assign[MODULE_NUM_DDR4][20U].bit_assign_info    },
        [21]    = { MDLC09MSRES21,      MDLC09MSRESS21,     ms_reg_bit_assign[MODULE_NUM_DDR4][21U].bit_assign_info    },
    },

    /* Module Number 10, Module Controller (DDR5)   */
    [MODULE_NUM_DDR5] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC10MSRES00,      MDLC10MSRESS00,     ms_reg_bit_assign[MODULE_NUM_DDR5][0U].bit_assign_info     },
        [1]     = { MDLC10MSRES01,      MDLC10MSRESS01,     ms_reg_bit_assign[MODULE_NUM_DDR5][1U].bit_assign_info     },
        [2]     = { MDLC10MSRES02,      MDLC10MSRESS02,     ms_reg_bit_assign[MODULE_NUM_DDR5][2U].bit_assign_info     },
        [3]     = { MDLC10MSRES03,      MDLC10MSRESS03,     ms_reg_bit_assign[MODULE_NUM_DDR5][3U].bit_assign_info     },
        [4]     = { MDLC10MSRES04,      MDLC10MSRESS04,     ms_reg_bit_assign[MODULE_NUM_DDR5][4U].bit_assign_info     },
        [5]     = { MDLC10MSRES05,      MDLC10MSRESS05,     ms_reg_bit_assign[MODULE_NUM_DDR5][5U].bit_assign_info     },
        [6]     = { MDLC10MSRES06,      MDLC10MSRESS06,     ms_reg_bit_assign[MODULE_NUM_DDR5][6U].bit_assign_info     },
        [7]     = { MDLC10MSRES07,      MDLC10MSRESS07,     ms_reg_bit_assign[MODULE_NUM_DDR5][7U].bit_assign_info     },
        [8]     = { MDLC10MSRES08,      MDLC10MSRESS08,     ms_reg_bit_assign[MODULE_NUM_DDR5][8U].bit_assign_info     },
        [9]     = { MDLC10MSRES09,      MDLC10MSRESS09,     ms_reg_bit_assign[MODULE_NUM_DDR5][9U].bit_assign_info     },
        [10]    = { MDLC10MSRES10,      MDLC10MSRESS10,     ms_reg_bit_assign[MODULE_NUM_DDR5][10U].bit_assign_info    },
        [11]    = { MDLC10MSRES11,      MDLC10MSRESS11,     ms_reg_bit_assign[MODULE_NUM_DDR5][11U].bit_assign_info    },
        [12]    = { MDLC10MSRES12,      MDLC10MSRESS12,     ms_reg_bit_assign[MODULE_NUM_DDR5][12U].bit_assign_info    },
        [13]    = { MDLC10MSRES13,      MDLC10MSRESS13,     ms_reg_bit_assign[MODULE_NUM_DDR5][13U].bit_assign_info    },
        [14]    = { MDLC10MSRES14,      MDLC10MSRESS14,     ms_reg_bit_assign[MODULE_NUM_DDR5][14U].bit_assign_info    },
        [15]    = { MDLC10MSRES15,      MDLC10MSRESS15,     ms_reg_bit_assign[MODULE_NUM_DDR5][15U].bit_assign_info    },
        [16]    = { MDLC10MSRES16,      MDLC10MSRESS16,     ms_reg_bit_assign[MODULE_NUM_DDR5][16U].bit_assign_info    },
        [17]    = { MDLC10MSRES17,      MDLC10MSRESS17,     ms_reg_bit_assign[MODULE_NUM_DDR5][17U].bit_assign_info    },
        [18]    = { MDLC10MSRES18,      MDLC10MSRESS18,     ms_reg_bit_assign[MODULE_NUM_DDR5][18U].bit_assign_info    },
        [19]    = { MDLC10MSRES19,      MDLC10MSRESS19,     ms_reg_bit_assign[MODULE_NUM_DDR5][19U].bit_assign_info    },
        [20]    = { MDLC10MSRES20,      MDLC10MSRESS20,     ms_reg_bit_assign[MODULE_NUM_DDR5][20U].bit_assign_info    },
        [21]    = { MDLC10MSRES21,      MDLC10MSRESS21,     ms_reg_bit_assign[MODULE_NUM_DDR5][21U].bit_assign_info    },
    },

    /* Module Number 11, Module Controller (DDR6)   */
    [MODULE_NUM_DDR6] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC11MSRES00,      MDLC11MSRESS00,     ms_reg_bit_assign[MODULE_NUM_DDR6][0U].bit_assign_info     },
        [1]     = { MDLC11MSRES01,      MDLC11MSRESS01,     ms_reg_bit_assign[MODULE_NUM_DDR6][1U].bit_assign_info     },
        [2]     = { MDLC11MSRES02,      MDLC11MSRESS02,     ms_reg_bit_assign[MODULE_NUM_DDR6][2U].bit_assign_info     },
        [3]     = { MDLC11MSRES03,      MDLC11MSRESS03,     ms_reg_bit_assign[MODULE_NUM_DDR6][3U].bit_assign_info     },
        [4]     = { MDLC11MSRES04,      MDLC11MSRESS04,     ms_reg_bit_assign[MODULE_NUM_DDR6][4U].bit_assign_info     },
        [5]     = { MDLC11MSRES05,      MDLC11MSRESS05,     ms_reg_bit_assign[MODULE_NUM_DDR6][5U].bit_assign_info     },
        [6]     = { MDLC11MSRES06,      MDLC11MSRESS06,     ms_reg_bit_assign[MODULE_NUM_DDR6][6U].bit_assign_info     },
        [7]     = { MDLC11MSRES07,      MDLC11MSRESS07,     ms_reg_bit_assign[MODULE_NUM_DDR6][7U].bit_assign_info     },
        [8]     = { MDLC11MSRES08,      MDLC11MSRESS08,     ms_reg_bit_assign[MODULE_NUM_DDR6][8U].bit_assign_info     },
        [9]     = { MDLC11MSRES09,      MDLC11MSRESS09,     ms_reg_bit_assign[MODULE_NUM_DDR6][9U].bit_assign_info     },
        [10]    = { MDLC11MSRES10,      MDLC11MSRESS10,     ms_reg_bit_assign[MODULE_NUM_DDR6][10U].bit_assign_info    },
        [11]    = { MDLC11MSRES11,      MDLC11MSRESS11,     ms_reg_bit_assign[MODULE_NUM_DDR6][11U].bit_assign_info    },
        [12]    = { MDLC11MSRES12,      MDLC11MSRESS12,     ms_reg_bit_assign[MODULE_NUM_DDR6][12U].bit_assign_info    },
        [13]    = { MDLC11MSRES13,      MDLC11MSRESS13,     ms_reg_bit_assign[MODULE_NUM_DDR6][13U].bit_assign_info    },
        [14]    = { MDLC11MSRES14,      MDLC11MSRESS14,     ms_reg_bit_assign[MODULE_NUM_DDR6][14U].bit_assign_info    },
        [15]    = { MDLC11MSRES15,      MDLC11MSRESS15,     ms_reg_bit_assign[MODULE_NUM_DDR6][15U].bit_assign_info    },
        [16]    = { MDLC11MSRES16,      MDLC11MSRESS16,     ms_reg_bit_assign[MODULE_NUM_DDR6][16U].bit_assign_info    },
        [17]    = { MDLC11MSRES17,      MDLC11MSRESS17,     ms_reg_bit_assign[MODULE_NUM_DDR6][17U].bit_assign_info    },
        [18]    = { MDLC11MSRES18,      MDLC11MSRESS18,     ms_reg_bit_assign[MODULE_NUM_DDR6][18U].bit_assign_info    },
        [19]    = { MDLC11MSRES19,      MDLC11MSRESS19,     ms_reg_bit_assign[MODULE_NUM_DDR6][19U].bit_assign_info    },
        [20]    = { MDLC11MSRES20,      MDLC11MSRESS20,     ms_reg_bit_assign[MODULE_NUM_DDR6][20U].bit_assign_info    },
        [21]    = { MDLC11MSRES21,      MDLC11MSRESS21,     ms_reg_bit_assign[MODULE_NUM_DDR6][21U].bit_assign_info    },
    },

    /* Module Number 12, Module Controller (DDR7)   */
    [MODULE_NUM_DDR7] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC12MSRES00,      MDLC12MSRESS00,     ms_reg_bit_assign[MODULE_NUM_DDR7][0U].bit_assign_info     },
        [1]     = { MDLC12MSRES01,      MDLC12MSRESS01,     ms_reg_bit_assign[MODULE_NUM_DDR7][1U].bit_assign_info     },
        [2]     = { MDLC12MSRES02,      MDLC12MSRESS02,     ms_reg_bit_assign[MODULE_NUM_DDR7][2U].bit_assign_info     },
        [3]     = { MDLC12MSRES03,      MDLC12MSRESS03,     ms_reg_bit_assign[MODULE_NUM_DDR7][3U].bit_assign_info     },
        [4]     = { MDLC12MSRES04,      MDLC12MSRESS04,     ms_reg_bit_assign[MODULE_NUM_DDR7][4U].bit_assign_info     },
        [5]     = { MDLC12MSRES05,      MDLC12MSRESS05,     ms_reg_bit_assign[MODULE_NUM_DDR7][5U].bit_assign_info     },
        [6]     = { MDLC12MSRES06,      MDLC12MSRESS06,     ms_reg_bit_assign[MODULE_NUM_DDR7][6U].bit_assign_info     },
        [7]     = { MDLC12MSRES07,      MDLC12MSRESS07,     ms_reg_bit_assign[MODULE_NUM_DDR7][7U].bit_assign_info     },
        [8]     = { MDLC12MSRES08,      MDLC12MSRESS08,     ms_reg_bit_assign[MODULE_NUM_DDR7][8U].bit_assign_info     },
        [9]     = { MDLC12MSRES09,      MDLC12MSRESS09,     ms_reg_bit_assign[MODULE_NUM_DDR7][9U].bit_assign_info     },
        [10]    = { MDLC12MSRES10,      MDLC12MSRESS10,     ms_reg_bit_assign[MODULE_NUM_DDR7][10U].bit_assign_info    },
        [11]    = { MDLC12MSRES11,      MDLC12MSRESS11,     ms_reg_bit_assign[MODULE_NUM_DDR7][11U].bit_assign_info    },
        [12]    = { MDLC12MSRES12,      MDLC12MSRESS12,     ms_reg_bit_assign[MODULE_NUM_DDR7][12U].bit_assign_info    },
        [13]    = { MDLC12MSRES13,      MDLC12MSRESS13,     ms_reg_bit_assign[MODULE_NUM_DDR7][13U].bit_assign_info    },
        [14]    = { MDLC12MSRES14,      MDLC12MSRESS14,     ms_reg_bit_assign[MODULE_NUM_DDR7][14U].bit_assign_info    },
        [15]    = { MDLC12MSRES15,      MDLC12MSRESS15,     ms_reg_bit_assign[MODULE_NUM_DDR7][15U].bit_assign_info    },
        [16]    = { MDLC12MSRES16,      MDLC12MSRESS16,     ms_reg_bit_assign[MODULE_NUM_DDR7][16U].bit_assign_info    },
        [17]    = { MDLC12MSRES17,      MDLC12MSRESS17,     ms_reg_bit_assign[MODULE_NUM_DDR7][17U].bit_assign_info    },
        [18]    = { MDLC12MSRES18,      MDLC12MSRESS18,     ms_reg_bit_assign[MODULE_NUM_DDR7][18U].bit_assign_info    },
        [19]    = { MDLC12MSRES19,      MDLC12MSRESS19,     ms_reg_bit_assign[MODULE_NUM_DDR7][19U].bit_assign_info    },
        [20]    = { MDLC12MSRES20,      MDLC12MSRESS20,     ms_reg_bit_assign[MODULE_NUM_DDR7][20U].bit_assign_info    },
        [21]    = { MDLC12MSRES21,      MDLC12MSRESS21,     ms_reg_bit_assign[MODULE_NUM_DDR7][21U].bit_assign_info    },
    },

    /* Module Number 13, Module Controller (HSCN)   */
    [MODULE_NUM_HSCN] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC13MSRES00,      MDLC13MSRESS00,     ms_reg_bit_assign[MODULE_NUM_HSCN][0U].bit_assign_info     },
        [1]     = { MDLC13MSRES01,      MDLC13MSRESS01,     ms_reg_bit_assign[MODULE_NUM_HSCN][1U].bit_assign_info     },
        [2]     = { MDLC13MSRES02,      MDLC13MSRESS02,     ms_reg_bit_assign[MODULE_NUM_HSCN][2U].bit_assign_info     },
        [3]     = { MDLC13MSRES03,      MDLC13MSRESS03,     ms_reg_bit_assign[MODULE_NUM_HSCN][3U].bit_assign_info     },
        [4]     = { MDLC13MSRES04,      MDLC13MSRESS04,     ms_reg_bit_assign[MODULE_NUM_HSCN][4U].bit_assign_info     },
        [5]     = { MDLC13MSRES05,      MDLC13MSRESS05,     ms_reg_bit_assign[MODULE_NUM_HSCN][5U].bit_assign_info     },
        [6]     = { MDLC13MSRES06,      MDLC13MSRESS06,     ms_reg_bit_assign[MODULE_NUM_HSCN][6U].bit_assign_info     },
        [7]     = { MDLC13MSRES07,      MDLC13MSRESS07,     ms_reg_bit_assign[MODULE_NUM_HSCN][7U].bit_assign_info     },
        [8]     = { MDLC13MSRES08,      MDLC13MSRESS08,     ms_reg_bit_assign[MODULE_NUM_HSCN][8U].bit_assign_info     },
        [9]     = { MDLC13MSRES09,      MDLC13MSRESS09,     ms_reg_bit_assign[MODULE_NUM_HSCN][9U].bit_assign_info     },
        [10]    = { MDLC13MSRES10,      MDLC13MSRESS10,     ms_reg_bit_assign[MODULE_NUM_HSCN][10U].bit_assign_info    },
        [11]    = { MDLC13MSRES11,      MDLC13MSRESS11,     ms_reg_bit_assign[MODULE_NUM_HSCN][11U].bit_assign_info    },
        [12]    = { MDLC13MSRES12,      MDLC13MSRESS12,     ms_reg_bit_assign[MODULE_NUM_HSCN][12U].bit_assign_info    },
        [13]    = { MDLC13MSRES13,      MDLC13MSRESS13,     ms_reg_bit_assign[MODULE_NUM_HSCN][13U].bit_assign_info    },
        [14]    = { MDLC13MSRES14,      MDLC13MSRESS14,     ms_reg_bit_assign[MODULE_NUM_HSCN][14U].bit_assign_info    },
        [15]    = { MDLC13MSRES15,      MDLC13MSRESS15,     ms_reg_bit_assign[MODULE_NUM_HSCN][15U].bit_assign_info    },
        [16]    = { MDLC13MSRES16,      MDLC13MSRESS16,     ms_reg_bit_assign[MODULE_NUM_HSCN][16U].bit_assign_info    },
        [17]    = { MDLC13MSRES17,      MDLC13MSRESS17,     ms_reg_bit_assign[MODULE_NUM_HSCN][17U].bit_assign_info    },
        [18]    = { MDLC13MSRES18,      MDLC13MSRESS18,     ms_reg_bit_assign[MODULE_NUM_HSCN][18U].bit_assign_info    },
        [19]    = { MDLC13MSRES19,      MDLC13MSRESS19,     ms_reg_bit_assign[MODULE_NUM_HSCN][19U].bit_assign_info    },
        [20]    = { MDLC13MSRES20,      MDLC13MSRESS20,     ms_reg_bit_assign[MODULE_NUM_HSCN][20U].bit_assign_info    },
        [21]    = { MDLC13MSRES21,      MDLC13MSRESS21,     ms_reg_bit_assign[MODULE_NUM_HSCN][21U].bit_assign_info    },
    },

    /* Module Number 14, Module Controller (RT)     */
    [MODULE_NUM_RT] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC14MSRES00,      MDLC14MSRESS00,     ms_reg_bit_assign[MODULE_NUM_RT][0U].bit_assign_info       },
        [1]     = { MDLC14MSRES01,      MDLC14MSRESS01,     ms_reg_bit_assign[MODULE_NUM_RT][1U].bit_assign_info       },
        [2]     = { MDLC14MSRES02,      MDLC14MSRESS02,     ms_reg_bit_assign[MODULE_NUM_RT][2U].bit_assign_info       },
        [3]     = { MDLC14MSRES03,      MDLC14MSRESS03,     ms_reg_bit_assign[MODULE_NUM_RT][3U].bit_assign_info       },
        [4]     = { MDLC14MSRES04,      MDLC14MSRESS04,     ms_reg_bit_assign[MODULE_NUM_RT][4U].bit_assign_info       },
        [5]     = { MDLC14MSRES05,      MDLC14MSRESS05,     ms_reg_bit_assign[MODULE_NUM_RT][5U].bit_assign_info       },
        [6]     = { MDLC14MSRES06,      MDLC14MSRESS06,     ms_reg_bit_assign[MODULE_NUM_RT][6U].bit_assign_info       },
        [7]     = { MDLC14MSRES07,      MDLC14MSRESS07,     ms_reg_bit_assign[MODULE_NUM_RT][7U].bit_assign_info       },
        [8]     = { MDLC14MSRES08,      MDLC14MSRESS08,     ms_reg_bit_assign[MODULE_NUM_RT][8U].bit_assign_info       },
        [9]     = { MDLC14MSRES09,      MDLC14MSRESS09,     ms_reg_bit_assign[MODULE_NUM_RT][9U].bit_assign_info       },
        [10]    = { MDLC14MSRES10,      MDLC14MSRESS10,     ms_reg_bit_assign[MODULE_NUM_RT][10U].bit_assign_info      },
        [11]    = { MDLC14MSRES11,      MDLC14MSRESS11,     ms_reg_bit_assign[MODULE_NUM_RT][11U].bit_assign_info      },
        [12]    = { MDLC14MSRES12,      MDLC14MSRESS12,     ms_reg_bit_assign[MODULE_NUM_RT][12U].bit_assign_info      },
        [13]    = { MDLC14MSRES13,      MDLC14MSRESS13,     ms_reg_bit_assign[MODULE_NUM_RT][13U].bit_assign_info      },
        [14]    = { MDLC14MSRES14,      MDLC14MSRESS14,     ms_reg_bit_assign[MODULE_NUM_RT][14U].bit_assign_info      },
        [15]    = { MDLC14MSRES15,      MDLC14MSRESS15,     ms_reg_bit_assign[MODULE_NUM_RT][15U].bit_assign_info      },
        [16]    = { MDLC14MSRES16,      MDLC14MSRESS16,     ms_reg_bit_assign[MODULE_NUM_RT][16U].bit_assign_info      },
        [17]    = { MDLC14MSRES17,      MDLC14MSRESS17,     ms_reg_bit_assign[MODULE_NUM_RT][17U].bit_assign_info      },
        [18]    = { MDLC14MSRES18,      MDLC14MSRESS18,     ms_reg_bit_assign[MODULE_NUM_RT][18U].bit_assign_info      },
        [19]    = { MDLC14MSRES19,      MDLC14MSRESS19,     ms_reg_bit_assign[MODULE_NUM_RT][19U].bit_assign_info      },
        [20]    = { MDLC14MSRES20,      MDLC14MSRESS20,     ms_reg_bit_assign[MODULE_NUM_RT][20U].bit_assign_info      },
        [21]    = { MDLC14MSRES21,      MDLC14MSRESS21,     ms_reg_bit_assign[MODULE_NUM_RT][21U].bit_assign_info      },
    },

    /* Module Number 15, Module Controller (TOP)    */
    [MODULE_NUM_TOP] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC15MSRES00,      MDLC15MSRESS00,     ms_reg_bit_assign[MODULE_NUM_TOP][0U].bit_assign_info      },
        [1]     = { MDLC15MSRES01,      MDLC15MSRESS01,     ms_reg_bit_assign[MODULE_NUM_TOP][1U].bit_assign_info      },
        [2]     = { MDLC15MSRES02,      MDLC15MSRESS02,     ms_reg_bit_assign[MODULE_NUM_TOP][2U].bit_assign_info      },
        [3]     = { MDLC15MSRES03,      MDLC15MSRESS03,     ms_reg_bit_assign[MODULE_NUM_TOP][3U].bit_assign_info      },
        [4]     = { MDLC15MSRES04,      MDLC15MSRESS04,     ms_reg_bit_assign[MODULE_NUM_TOP][4U].bit_assign_info      },
        [5]     = { MDLC15MSRES05,      MDLC15MSRESS05,     ms_reg_bit_assign[MODULE_NUM_TOP][5U].bit_assign_info      },
        [6]     = { MDLC15MSRES06,      MDLC15MSRESS06,     ms_reg_bit_assign[MODULE_NUM_TOP][6U].bit_assign_info      },
        [7]     = { MDLC15MSRES07,      MDLC15MSRESS07,     ms_reg_bit_assign[MODULE_NUM_TOP][7U].bit_assign_info      },
        [8]     = { MDLC15MSRES08,      MDLC15MSRESS08,     ms_reg_bit_assign[MODULE_NUM_TOP][8U].bit_assign_info      },
        [9]     = { MDLC15MSRES09,      MDLC15MSRESS09,     ms_reg_bit_assign[MODULE_NUM_TOP][9U].bit_assign_info      },
        [10]    = { MDLC15MSRES10,      MDLC15MSRESS10,     ms_reg_bit_assign[MODULE_NUM_TOP][10U].bit_assign_info     },
        [11]    = { MDLC15MSRES11,      MDLC15MSRESS11,     ms_reg_bit_assign[MODULE_NUM_TOP][11U].bit_assign_info     },
        [12]    = { MDLC15MSRES12,      MDLC15MSRESS12,     ms_reg_bit_assign[MODULE_NUM_TOP][12U].bit_assign_info     },
        [13]    = { MDLC15MSRES13,      MDLC15MSRESS13,     ms_reg_bit_assign[MODULE_NUM_TOP][13U].bit_assign_info     },
        [14]    = { MDLC15MSRES14,      MDLC15MSRESS14,     ms_reg_bit_assign[MODULE_NUM_TOP][14U].bit_assign_info     },
        [15]    = { MDLC15MSRES15,      MDLC15MSRESS15,     ms_reg_bit_assign[MODULE_NUM_TOP][15U].bit_assign_info     },
        [16]    = { MDLC15MSRES16,      MDLC15MSRESS16,     ms_reg_bit_assign[MODULE_NUM_TOP][16U].bit_assign_info     },
        [17]    = { MDLC15MSRES17,      MDLC15MSRESS17,     ms_reg_bit_assign[MODULE_NUM_TOP][17U].bit_assign_info     },
        [18]    = { MDLC15MSRES18,      MDLC15MSRESS18,     ms_reg_bit_assign[MODULE_NUM_TOP][18U].bit_assign_info     },
        [19]    = { MDLC15MSRES19,      MDLC15MSRESS19,     ms_reg_bit_assign[MODULE_NUM_TOP][19U].bit_assign_info     },
        [20]    = { MDLC15MSRES20,      MDLC15MSRESS20,     ms_reg_bit_assign[MODULE_NUM_TOP][20U].bit_assign_info     },
        [21]    = { MDLC15MSRES21,      MDLC15MSRESS21,     ms_reg_bit_assign[MODULE_NUM_TOP][21U].bit_assign_info     },
    },

    /* Module Number 16, Module Controller (HSCS)   */
    [MODULE_NUM_HSCS] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC16MSRES00,      MDLC16MSRESS00,     ms_reg_bit_assign[MODULE_NUM_HSCS][0U].bit_assign_info     },
        [1]     = { MDLC16MSRES01,      MDLC16MSRESS01,     ms_reg_bit_assign[MODULE_NUM_HSCS][1U].bit_assign_info     },
        [2]     = { MDLC16MSRES02,      MDLC16MSRESS02,     ms_reg_bit_assign[MODULE_NUM_HSCS][2U].bit_assign_info     },
        [3]     = { MDLC16MSRES03,      MDLC16MSRESS03,     ms_reg_bit_assign[MODULE_NUM_HSCS][3U].bit_assign_info     },
        [4]     = { MDLC16MSRES04,      MDLC16MSRESS04,     ms_reg_bit_assign[MODULE_NUM_HSCS][4U].bit_assign_info     },
        [5]     = { MDLC16MSRES05,      MDLC16MSRESS05,     ms_reg_bit_assign[MODULE_NUM_HSCS][5U].bit_assign_info     },
        [6]     = { MDLC16MSRES06,      MDLC16MSRESS06,     ms_reg_bit_assign[MODULE_NUM_HSCS][6U].bit_assign_info     },
        [7]     = { MDLC16MSRES07,      MDLC16MSRESS07,     ms_reg_bit_assign[MODULE_NUM_HSCS][7U].bit_assign_info     },
        [8]     = { MDLC16MSRES08,      MDLC16MSRESS08,     ms_reg_bit_assign[MODULE_NUM_HSCS][8U].bit_assign_info     },
        [9]     = { MDLC16MSRES09,      MDLC16MSRESS09,     ms_reg_bit_assign[MODULE_NUM_HSCS][9U].bit_assign_info     },
        [10]    = { MDLC16MSRES10,      MDLC16MSRESS10,     ms_reg_bit_assign[MODULE_NUM_HSCS][10U].bit_assign_info    },
        [11]    = { MDLC16MSRES11,      MDLC16MSRESS11,     ms_reg_bit_assign[MODULE_NUM_HSCS][11U].bit_assign_info    },
        [12]    = { MDLC16MSRES12,      MDLC16MSRESS12,     ms_reg_bit_assign[MODULE_NUM_HSCS][12U].bit_assign_info    },
        [13]    = { MDLC16MSRES13,      MDLC16MSRESS13,     ms_reg_bit_assign[MODULE_NUM_HSCS][13U].bit_assign_info    },
        [14]    = { MDLC16MSRES14,      MDLC16MSRESS14,     ms_reg_bit_assign[MODULE_NUM_HSCS][14U].bit_assign_info    },
        [15]    = { MDLC16MSRES15,      MDLC16MSRESS15,     ms_reg_bit_assign[MODULE_NUM_HSCS][15U].bit_assign_info    },
        [16]    = { MDLC16MSRES16,      MDLC16MSRESS16,     ms_reg_bit_assign[MODULE_NUM_HSCS][16U].bit_assign_info    },
        [17]    = { MDLC16MSRES17,      MDLC16MSRESS17,     ms_reg_bit_assign[MODULE_NUM_HSCS][17U].bit_assign_info    },
        [18]    = { MDLC16MSRES18,      MDLC16MSRESS18,     ms_reg_bit_assign[MODULE_NUM_HSCS][18U].bit_assign_info    },
        [19]    = { MDLC16MSRES19,      MDLC16MSRESS19,     ms_reg_bit_assign[MODULE_NUM_HSCS][19U].bit_assign_info    },
        [20]    = { MDLC16MSRES20,      MDLC16MSRESS20,     ms_reg_bit_assign[MODULE_NUM_HSCS][20U].bit_assign_info    },
        [21]    = { MDLC16MSRES21,      MDLC16MSRESS21,     ms_reg_bit_assign[MODULE_NUM_HSCS][21U].bit_assign_info    },
    },

    /* Module Number 17, Module Controller (IMN)    */
    [MODULE_NUM_IMN] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC17MSRES00,      MDLC17MSRESS00,     ms_reg_bit_assign[MODULE_NUM_IMN][0U].bit_assign_info      },
        [1]     = { MDLC17MSRES01,      MDLC17MSRESS01,     ms_reg_bit_assign[MODULE_NUM_IMN][1U].bit_assign_info      },
        [2]     = { MDLC17MSRES02,      MDLC17MSRESS02,     ms_reg_bit_assign[MODULE_NUM_IMN][2U].bit_assign_info      },
        [3]     = { MDLC17MSRES03,      MDLC17MSRESS03,     ms_reg_bit_assign[MODULE_NUM_IMN][3U].bit_assign_info      },
        [4]     = { MDLC17MSRES04,      MDLC17MSRESS04,     ms_reg_bit_assign[MODULE_NUM_IMN][4U].bit_assign_info      },
        [5]     = { MDLC17MSRES05,      MDLC17MSRESS05,     ms_reg_bit_assign[MODULE_NUM_IMN][5U].bit_assign_info      },
        [6]     = { MDLC17MSRES06,      MDLC17MSRESS06,     ms_reg_bit_assign[MODULE_NUM_IMN][6U].bit_assign_info      },
        [7]     = { MDLC17MSRES07,      MDLC17MSRESS07,     ms_reg_bit_assign[MODULE_NUM_IMN][7U].bit_assign_info      },
        [8]     = { MDLC17MSRES08,      MDLC17MSRESS08,     ms_reg_bit_assign[MODULE_NUM_IMN][8U].bit_assign_info      },
        [9]     = { MDLC17MSRES09,      MDLC17MSRESS09,     ms_reg_bit_assign[MODULE_NUM_IMN][9U].bit_assign_info      },
        [10]    = { MDLC17MSRES10,      MDLC17MSRESS10,     ms_reg_bit_assign[MODULE_NUM_IMN][10U].bit_assign_info     },
        [11]    = { MDLC17MSRES11,      MDLC17MSRESS11,     ms_reg_bit_assign[MODULE_NUM_IMN][11U].bit_assign_info     },
        [12]    = { MDLC17MSRES12,      MDLC17MSRESS12,     ms_reg_bit_assign[MODULE_NUM_IMN][12U].bit_assign_info     },
        [13]    = { MDLC17MSRES13,      MDLC17MSRESS13,     ms_reg_bit_assign[MODULE_NUM_IMN][13U].bit_assign_info     },
        [14]    = { MDLC17MSRES14,      MDLC17MSRESS14,     ms_reg_bit_assign[MODULE_NUM_IMN][14U].bit_assign_info     },
        [15]    = { MDLC17MSRES15,      MDLC17MSRESS15,     ms_reg_bit_assign[MODULE_NUM_IMN][15U].bit_assign_info     },
        [16]    = { MDLC17MSRES16,      MDLC17MSRESS16,     ms_reg_bit_assign[MODULE_NUM_IMN][16U].bit_assign_info     },
        [17]    = { MDLC17MSRES17,      MDLC17MSRESS17,     ms_reg_bit_assign[MODULE_NUM_IMN][17U].bit_assign_info     },
        [18]    = { MDLC17MSRES18,      MDLC17MSRESS18,     ms_reg_bit_assign[MODULE_NUM_IMN][18U].bit_assign_info     },
        [19]    = { MDLC17MSRES19,      MDLC17MSRESS19,     ms_reg_bit_assign[MODULE_NUM_IMN][19U].bit_assign_info     },
        [20]    = { MDLC17MSRES20,      MDLC17MSRESS20,     ms_reg_bit_assign[MODULE_NUM_IMN][20U].bit_assign_info     },
        [21]    = { MDLC17MSRES21,      MDLC17MSRESS21,     ms_reg_bit_assign[MODULE_NUM_IMN][21U].bit_assign_info     },
    },

    /* Module Number 18, Module Controller (IMS)    */
    [MODULE_NUM_IMS] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC18MSRES00,      MDLC18MSRESS00,     ms_reg_bit_assign[MODULE_NUM_IMS][0U].bit_assign_info      },
        [1]     = { MDLC18MSRES01,      MDLC18MSRESS01,     ms_reg_bit_assign[MODULE_NUM_IMS][1U].bit_assign_info      },
        [2]     = { MDLC18MSRES02,      MDLC18MSRESS02,     ms_reg_bit_assign[MODULE_NUM_IMS][2U].bit_assign_info      },
        [3]     = { MDLC18MSRES03,      MDLC18MSRESS03,     ms_reg_bit_assign[MODULE_NUM_IMS][3U].bit_assign_info      },
        [4]     = { MDLC18MSRES04,      MDLC18MSRESS04,     ms_reg_bit_assign[MODULE_NUM_IMS][4U].bit_assign_info      },
        [5]     = { MDLC18MSRES05,      MDLC18MSRESS05,     ms_reg_bit_assign[MODULE_NUM_IMS][5U].bit_assign_info      },
        [6]     = { MDLC18MSRES06,      MDLC18MSRESS06,     ms_reg_bit_assign[MODULE_NUM_IMS][6U].bit_assign_info      },
        [7]     = { MDLC18MSRES07,      MDLC18MSRESS07,     ms_reg_bit_assign[MODULE_NUM_IMS][7U].bit_assign_info      },
        [8]     = { MDLC18MSRES08,      MDLC18MSRESS08,     ms_reg_bit_assign[MODULE_NUM_IMS][8U].bit_assign_info      },
        [9]     = { MDLC18MSRES09,      MDLC18MSRESS09,     ms_reg_bit_assign[MODULE_NUM_IMS][9U].bit_assign_info      },
        [10]    = { MDLC18MSRES10,      MDLC18MSRESS10,     ms_reg_bit_assign[MODULE_NUM_IMS][10U].bit_assign_info     },
        [11]    = { MDLC18MSRES11,      MDLC18MSRESS11,     ms_reg_bit_assign[MODULE_NUM_IMS][11U].bit_assign_info     },
        [12]    = { MDLC18MSRES12,      MDLC18MSRESS12,     ms_reg_bit_assign[MODULE_NUM_IMS][12U].bit_assign_info     },
        [13]    = { MDLC18MSRES13,      MDLC18MSRESS13,     ms_reg_bit_assign[MODULE_NUM_IMS][13U].bit_assign_info     },
        [14]    = { MDLC18MSRES14,      MDLC18MSRESS14,     ms_reg_bit_assign[MODULE_NUM_IMS][14U].bit_assign_info     },
        [15]    = { MDLC18MSRES15,      MDLC18MSRESS15,     ms_reg_bit_assign[MODULE_NUM_IMS][15U].bit_assign_info     },
        [16]    = { MDLC18MSRES16,      MDLC18MSRESS16,     ms_reg_bit_assign[MODULE_NUM_IMS][16U].bit_assign_info     },
        [17]    = { MDLC18MSRES17,      MDLC18MSRESS17,     ms_reg_bit_assign[MODULE_NUM_IMS][17U].bit_assign_info     },
        [18]    = { MDLC18MSRES18,      MDLC18MSRESS18,     ms_reg_bit_assign[MODULE_NUM_IMS][18U].bit_assign_info     },
        [19]    = { MDLC18MSRES19,      MDLC18MSRESS19,     ms_reg_bit_assign[MODULE_NUM_IMS][19U].bit_assign_info     },
        [20]    = { MDLC18MSRES20,      MDLC18MSRESS20,     ms_reg_bit_assign[MODULE_NUM_IMS][20U].bit_assign_info     },
        [21]    = { MDLC18MSRES21,      MDLC18MSRESS21,     ms_reg_bit_assign[MODULE_NUM_IMS][21U].bit_assign_info     },
    },

    /* Module Number 19, Module Controller (GPC)    */
    [MODULE_NUM_GPC] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC19MSRES00,      MDLC19MSRESS00,     ms_reg_bit_assign[MODULE_NUM_GPC][0U].bit_assign_info      },
        [1]     = { MDLC19MSRES01,      MDLC19MSRESS01,     ms_reg_bit_assign[MODULE_NUM_GPC][1U].bit_assign_info      },
        [2]     = { MDLC19MSRES02,      MDLC19MSRESS02,     ms_reg_bit_assign[MODULE_NUM_GPC][2U].bit_assign_info      },
        [3]     = { MDLC19MSRES03,      MDLC19MSRESS03,     ms_reg_bit_assign[MODULE_NUM_GPC][3U].bit_assign_info      },
        [4]     = { MDLC19MSRES04,      MDLC19MSRESS04,     ms_reg_bit_assign[MODULE_NUM_GPC][4U].bit_assign_info      },
        [5]     = { MDLC19MSRES05,      MDLC19MSRESS05,     ms_reg_bit_assign[MODULE_NUM_GPC][5U].bit_assign_info      },
        [6]     = { MDLC19MSRES06,      MDLC19MSRESS06,     ms_reg_bit_assign[MODULE_NUM_GPC][6U].bit_assign_info      },
        [7]     = { MDLC19MSRES07,      MDLC19MSRESS07,     ms_reg_bit_assign[MODULE_NUM_GPC][7U].bit_assign_info      },
        [8]     = { MDLC19MSRES08,      MDLC19MSRESS08,     ms_reg_bit_assign[MODULE_NUM_GPC][8U].bit_assign_info      },
        [9]     = { MDLC19MSRES09,      MDLC19MSRESS09,     ms_reg_bit_assign[MODULE_NUM_GPC][9U].bit_assign_info      },
        [10]    = { MDLC19MSRES10,      MDLC19MSRESS10,     ms_reg_bit_assign[MODULE_NUM_GPC][10U].bit_assign_info     },
        [11]    = { MDLC19MSRES11,      MDLC19MSRESS11,     ms_reg_bit_assign[MODULE_NUM_GPC][11U].bit_assign_info     },
        [12]    = { MDLC19MSRES12,      MDLC19MSRESS12,     ms_reg_bit_assign[MODULE_NUM_GPC][12U].bit_assign_info     },
        [13]    = { MDLC19MSRES13,      MDLC19MSRESS13,     ms_reg_bit_assign[MODULE_NUM_GPC][13U].bit_assign_info     },
        [14]    = { MDLC19MSRES14,      MDLC19MSRESS14,     ms_reg_bit_assign[MODULE_NUM_GPC][14U].bit_assign_info     },
        [15]    = { MDLC19MSRES15,      MDLC19MSRESS15,     ms_reg_bit_assign[MODULE_NUM_GPC][15U].bit_assign_info     },
        [16]    = { MDLC19MSRES16,      MDLC19MSRESS16,     ms_reg_bit_assign[MODULE_NUM_GPC][16U].bit_assign_info     },
        [17]    = { MDLC19MSRES17,      MDLC19MSRESS17,     ms_reg_bit_assign[MODULE_NUM_GPC][17U].bit_assign_info     },
        [18]    = { MDLC19MSRES18,      MDLC19MSRESS18,     ms_reg_bit_assign[MODULE_NUM_GPC][18U].bit_assign_info     },
        [19]    = { MDLC19MSRES19,      MDLC19MSRESS19,     ms_reg_bit_assign[MODULE_NUM_GPC][19U].bit_assign_info     },
        [20]    = { MDLC19MSRES20,      MDLC19MSRESS20,     ms_reg_bit_assign[MODULE_NUM_GPC][20U].bit_assign_info     },
        [21]    = { MDLC19MSRES21,      MDLC19MSRESS21,     ms_reg_bit_assign[MODULE_NUM_GPC][21U].bit_assign_info     },
    },

    /* Module Number 20, Module Controller (DSP)    */
    [MODULE_NUM_DSP] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC20MSRES00,      MDLC20MSRESS00,     ms_reg_bit_assign[MODULE_NUM_DSP][0U].bit_assign_info      },
        [1]     = { MDLC20MSRES01,      MDLC20MSRESS01,     ms_reg_bit_assign[MODULE_NUM_DSP][1U].bit_assign_info      },
        [2]     = { MDLC20MSRES02,      MDLC20MSRESS02,     ms_reg_bit_assign[MODULE_NUM_DSP][2U].bit_assign_info      },
        [3]     = { MDLC20MSRES03,      MDLC20MSRESS03,     ms_reg_bit_assign[MODULE_NUM_DSP][3U].bit_assign_info      },
        [4]     = { MDLC20MSRES04,      MDLC20MSRESS04,     ms_reg_bit_assign[MODULE_NUM_DSP][4U].bit_assign_info      },
        [5]     = { MDLC20MSRES05,      MDLC20MSRESS05,     ms_reg_bit_assign[MODULE_NUM_DSP][5U].bit_assign_info      },
        [6]     = { MDLC20MSRES06,      MDLC20MSRESS06,     ms_reg_bit_assign[MODULE_NUM_DSP][6U].bit_assign_info      },
        [7]     = { MDLC20MSRES07,      MDLC20MSRESS07,     ms_reg_bit_assign[MODULE_NUM_DSP][7U].bit_assign_info      },
        [8]     = { MDLC20MSRES08,      MDLC20MSRESS08,     ms_reg_bit_assign[MODULE_NUM_DSP][8U].bit_assign_info      },
        [9]     = { MDLC20MSRES09,      MDLC20MSRESS09,     ms_reg_bit_assign[MODULE_NUM_DSP][9U].bit_assign_info      },
        [10]    = { MDLC20MSRES10,      MDLC20MSRESS10,     ms_reg_bit_assign[MODULE_NUM_DSP][10U].bit_assign_info     },
        [11]    = { MDLC20MSRES11,      MDLC20MSRESS11,     ms_reg_bit_assign[MODULE_NUM_DSP][11U].bit_assign_info     },
        [12]    = { MDLC20MSRES12,      MDLC20MSRESS12,     ms_reg_bit_assign[MODULE_NUM_DSP][12U].bit_assign_info     },
        [13]    = { MDLC20MSRES13,      MDLC20MSRESS13,     ms_reg_bit_assign[MODULE_NUM_DSP][13U].bit_assign_info     },
        [14]    = { MDLC20MSRES14,      MDLC20MSRESS14,     ms_reg_bit_assign[MODULE_NUM_DSP][14U].bit_assign_info     },
        [15]    = { MDLC20MSRES15,      MDLC20MSRESS15,     ms_reg_bit_assign[MODULE_NUM_DSP][15U].bit_assign_info     },
        [16]    = { MDLC20MSRES16,      MDLC20MSRESS16,     ms_reg_bit_assign[MODULE_NUM_DSP][16U].bit_assign_info     },
        [17]    = { MDLC20MSRES17,      MDLC20MSRESS17,     ms_reg_bit_assign[MODULE_NUM_DSP][17U].bit_assign_info     },
        [18]    = { MDLC20MSRES18,      MDLC20MSRESS18,     ms_reg_bit_assign[MODULE_NUM_DSP][18U].bit_assign_info     },
        [19]    = { MDLC20MSRES19,      MDLC20MSRESS19,     ms_reg_bit_assign[MODULE_NUM_DSP][19U].bit_assign_info     },
        [20]    = { MDLC20MSRES20,      MDLC20MSRESS20,     ms_reg_bit_assign[MODULE_NUM_DSP][20U].bit_assign_info     },
        [21]    = { MDLC20MSRES21,      MDLC20MSRESS21,     ms_reg_bit_assign[MODULE_NUM_DSP][21U].bit_assign_info     },
    },

    /* Module Number 21, Module Controller (MM)     */
    [MODULE_NUM_MM] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC21MSRES00,      MDLC21MSRESS00,     ms_reg_bit_assign[MODULE_NUM_MM][0U].bit_assign_info       },
        [1]     = { MDLC21MSRES01,      MDLC21MSRESS01,     ms_reg_bit_assign[MODULE_NUM_MM][1U].bit_assign_info       },
        [2]     = { MDLC21MSRES02,      MDLC21MSRESS02,     ms_reg_bit_assign[MODULE_NUM_MM][2U].bit_assign_info       },
        [3]     = { MDLC21MSRES03,      MDLC21MSRESS03,     ms_reg_bit_assign[MODULE_NUM_MM][3U].bit_assign_info       },
        [4]     = { MDLC21MSRES04,      MDLC21MSRESS04,     ms_reg_bit_assign[MODULE_NUM_MM][4U].bit_assign_info       },
        [5]     = { MDLC21MSRES05,      MDLC21MSRESS05,     ms_reg_bit_assign[MODULE_NUM_MM][5U].bit_assign_info       },
        [6]     = { MDLC21MSRES06,      MDLC21MSRESS06,     ms_reg_bit_assign[MODULE_NUM_MM][6U].bit_assign_info       },
        [7]     = { MDLC21MSRES07,      MDLC21MSRESS07,     ms_reg_bit_assign[MODULE_NUM_MM][7U].bit_assign_info       },
        [8]     = { MDLC21MSRES08,      MDLC21MSRESS08,     ms_reg_bit_assign[MODULE_NUM_MM][8U].bit_assign_info       },
        [9]     = { MDLC21MSRES09,      MDLC21MSRESS09,     ms_reg_bit_assign[MODULE_NUM_MM][9U].bit_assign_info       },
        [10]    = { MDLC21MSRES10,      MDLC21MSRESS10,     ms_reg_bit_assign[MODULE_NUM_MM][10U].bit_assign_info      },
        [11]    = { MDLC21MSRES11,      MDLC21MSRESS11,     ms_reg_bit_assign[MODULE_NUM_MM][11U].bit_assign_info      },
        [12]    = { MDLC21MSRES12,      MDLC21MSRESS12,     ms_reg_bit_assign[MODULE_NUM_MM][12U].bit_assign_info      },
        [13]    = { MDLC21MSRES13,      MDLC21MSRESS13,     ms_reg_bit_assign[MODULE_NUM_MM][13U].bit_assign_info      },
        [14]    = { MDLC21MSRES14,      MDLC21MSRESS14,     ms_reg_bit_assign[MODULE_NUM_MM][14U].bit_assign_info      },
        [15]    = { MDLC21MSRES15,      MDLC21MSRESS15,     ms_reg_bit_assign[MODULE_NUM_MM][15U].bit_assign_info      },
        [16]    = { MDLC21MSRES16,      MDLC21MSRESS16,     ms_reg_bit_assign[MODULE_NUM_MM][16U].bit_assign_info      },
        [17]    = { MDLC21MSRES17,      MDLC21MSRESS17,     ms_reg_bit_assign[MODULE_NUM_MM][17U].bit_assign_info      },
        [18]    = { MDLC21MSRES18,      MDLC21MSRESS18,     ms_reg_bit_assign[MODULE_NUM_MM][18U].bit_assign_info      },
        [19]    = { MDLC21MSRES19,      MDLC21MSRESS19,     ms_reg_bit_assign[MODULE_NUM_MM][19U].bit_assign_info      },
        [20]    = { MDLC21MSRES20,      MDLC21MSRESS20,     ms_reg_bit_assign[MODULE_NUM_MM][20U].bit_assign_info      },
        [21]    = { MDLC21MSRES21,      MDLC21MSRESS21,     ms_reg_bit_assign[MODULE_NUM_MM][21U].bit_assign_info      },
    },

    /* Module Number 22, Module Controller (NPU0)   */
    [MODULE_NUM_NPU0] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC22MSRES00,      MDLC22MSRESS00,     ms_reg_bit_assign[MODULE_NUM_NPU0][0U].bit_assign_info     },
        [1]     = { MDLC22MSRES01,      MDLC22MSRESS01,     ms_reg_bit_assign[MODULE_NUM_NPU0][1U].bit_assign_info     },
        [2]     = { MDLC22MSRES02,      MDLC22MSRESS02,     ms_reg_bit_assign[MODULE_NUM_NPU0][2U].bit_assign_info     },
        [3]     = { MDLC22MSRES03,      MDLC22MSRESS03,     ms_reg_bit_assign[MODULE_NUM_NPU0][3U].bit_assign_info     },
        [4]     = { MDLC22MSRES04,      MDLC22MSRESS04,     ms_reg_bit_assign[MODULE_NUM_NPU0][4U].bit_assign_info     },
        [5]     = { MDLC22MSRES05,      MDLC22MSRESS05,     ms_reg_bit_assign[MODULE_NUM_NPU0][5U].bit_assign_info     },
        [6]     = { MDLC22MSRES06,      MDLC22MSRESS06,     ms_reg_bit_assign[MODULE_NUM_NPU0][6U].bit_assign_info     },
        [7]     = { MDLC22MSRES07,      MDLC22MSRESS07,     ms_reg_bit_assign[MODULE_NUM_NPU0][7U].bit_assign_info     },
        [8]     = { MDLC22MSRES08,      MDLC22MSRESS08,     ms_reg_bit_assign[MODULE_NUM_NPU0][8U].bit_assign_info     },
        [9]     = { MDLC22MSRES09,      MDLC22MSRESS09,     ms_reg_bit_assign[MODULE_NUM_NPU0][9U].bit_assign_info     },
        [10]    = { MDLC22MSRES10,      MDLC22MSRESS10,     ms_reg_bit_assign[MODULE_NUM_NPU0][10U].bit_assign_info    },
        [11]    = { MDLC22MSRES11,      MDLC22MSRESS11,     ms_reg_bit_assign[MODULE_NUM_NPU0][11U].bit_assign_info    },
        [12]    = { MDLC22MSRES12,      MDLC22MSRESS12,     ms_reg_bit_assign[MODULE_NUM_NPU0][12U].bit_assign_info    },
        [13]    = { MDLC22MSRES13,      MDLC22MSRESS13,     ms_reg_bit_assign[MODULE_NUM_NPU0][13U].bit_assign_info    },
        [14]    = { MDLC22MSRES14,      MDLC22MSRESS14,     ms_reg_bit_assign[MODULE_NUM_NPU0][14U].bit_assign_info    },
        [15]    = { MDLC22MSRES15,      MDLC22MSRESS15,     ms_reg_bit_assign[MODULE_NUM_NPU0][15U].bit_assign_info    },
        [16]    = { MDLC22MSRES16,      MDLC22MSRESS16,     ms_reg_bit_assign[MODULE_NUM_NPU0][16U].bit_assign_info    },
        [17]    = { MDLC22MSRES17,      MDLC22MSRESS17,     ms_reg_bit_assign[MODULE_NUM_NPU0][17U].bit_assign_info    },
        [18]    = { MDLC22MSRES18,      MDLC22MSRESS18,     ms_reg_bit_assign[MODULE_NUM_NPU0][18U].bit_assign_info    },
        [19]    = { MDLC22MSRES19,      MDLC22MSRESS19,     ms_reg_bit_assign[MODULE_NUM_NPU0][19U].bit_assign_info    },
        [20]    = { MDLC22MSRES20,      MDLC22MSRESS20,     ms_reg_bit_assign[MODULE_NUM_NPU0][20U].bit_assign_info    },
        [21]    = { MDLC22MSRES21,      MDLC22MSRESS21,     ms_reg_bit_assign[MODULE_NUM_NPU0][21U].bit_assign_info    },
    },

    /* Module Number 23, Module Controller (NPU1)   */
    [MODULE_NUM_NPU1] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC23MSRES00,      MDLC23MSRESS00,     ms_reg_bit_assign[MODULE_NUM_NPU1][0U].bit_assign_info     },
        [1]     = { MDLC23MSRES01,      MDLC23MSRESS01,     ms_reg_bit_assign[MODULE_NUM_NPU1][1U].bit_assign_info     },
        [2]     = { MDLC23MSRES02,      MDLC23MSRESS02,     ms_reg_bit_assign[MODULE_NUM_NPU1][2U].bit_assign_info     },
        [3]     = { MDLC23MSRES03,      MDLC23MSRESS03,     ms_reg_bit_assign[MODULE_NUM_NPU1][3U].bit_assign_info     },
        [4]     = { MDLC23MSRES04,      MDLC23MSRESS04,     ms_reg_bit_assign[MODULE_NUM_NPU1][4U].bit_assign_info     },
        [5]     = { MDLC23MSRES05,      MDLC23MSRESS05,     ms_reg_bit_assign[MODULE_NUM_NPU1][5U].bit_assign_info     },
        [6]     = { MDLC23MSRES06,      MDLC23MSRESS06,     ms_reg_bit_assign[MODULE_NUM_NPU1][6U].bit_assign_info     },
        [7]     = { MDLC23MSRES07,      MDLC23MSRESS07,     ms_reg_bit_assign[MODULE_NUM_NPU1][7U].bit_assign_info     },
        [8]     = { MDLC23MSRES08,      MDLC23MSRESS08,     ms_reg_bit_assign[MODULE_NUM_NPU1][8U].bit_assign_info     },
        [9]     = { MDLC23MSRES09,      MDLC23MSRESS09,     ms_reg_bit_assign[MODULE_NUM_NPU1][9U].bit_assign_info     },
        [10]    = { MDLC23MSRES10,      MDLC23MSRESS10,     ms_reg_bit_assign[MODULE_NUM_NPU1][10U].bit_assign_info    },
        [11]    = { MDLC23MSRES11,      MDLC23MSRESS11,     ms_reg_bit_assign[MODULE_NUM_NPU1][11U].bit_assign_info    },
        [12]    = { MDLC23MSRES12,      MDLC23MSRESS12,     ms_reg_bit_assign[MODULE_NUM_NPU1][12U].bit_assign_info    },
        [13]    = { MDLC23MSRES13,      MDLC23MSRESS13,     ms_reg_bit_assign[MODULE_NUM_NPU1][13U].bit_assign_info    },
        [14]    = { MDLC23MSRES14,      MDLC23MSRESS14,     ms_reg_bit_assign[MODULE_NUM_NPU1][14U].bit_assign_info    },
        [15]    = { MDLC23MSRES15,      MDLC23MSRESS15,     ms_reg_bit_assign[MODULE_NUM_NPU1][15U].bit_assign_info    },
        [16]    = { MDLC23MSRES16,      MDLC23MSRESS16,     ms_reg_bit_assign[MODULE_NUM_NPU1][16U].bit_assign_info    },
        [17]    = { MDLC23MSRES17,      MDLC23MSRESS17,     ms_reg_bit_assign[MODULE_NUM_NPU1][17U].bit_assign_info    },
        [18]    = { MDLC23MSRES18,      MDLC23MSRESS18,     ms_reg_bit_assign[MODULE_NUM_NPU1][18U].bit_assign_info    },
        [19]    = { MDLC23MSRES19,      MDLC23MSRESS19,     ms_reg_bit_assign[MODULE_NUM_NPU1][19U].bit_assign_info    },
        [20]    = { MDLC23MSRES20,      MDLC23MSRESS20,     ms_reg_bit_assign[MODULE_NUM_NPU1][20U].bit_assign_info    },
        [21]    = { MDLC23MSRES21,      MDLC23MSRESS21,     ms_reg_bit_assign[MODULE_NUM_NPU1][21U].bit_assign_info    },
    },

    /* Module Number 24, Module Controller (CMNN)   */
    [MODULE_NUM_CMNN] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC24MSRES00,      MDLC24MSRESS00,     ms_reg_bit_assign[MODULE_NUM_CMNN][0U].bit_assign_info     },
        [1]     = { MDLC24MSRES01,      MDLC24MSRESS01,     ms_reg_bit_assign[MODULE_NUM_CMNN][1U].bit_assign_info     },
        [2]     = { MDLC24MSRES02,      MDLC24MSRESS02,     ms_reg_bit_assign[MODULE_NUM_CMNN][2U].bit_assign_info     },
        [3]     = { MDLC24MSRES03,      MDLC24MSRESS03,     ms_reg_bit_assign[MODULE_NUM_CMNN][3U].bit_assign_info     },
        [4]     = { MDLC24MSRES04,      MDLC24MSRESS04,     ms_reg_bit_assign[MODULE_NUM_CMNN][4U].bit_assign_info     },
        [5]     = { MDLC24MSRES05,      MDLC24MSRESS05,     ms_reg_bit_assign[MODULE_NUM_CMNN][5U].bit_assign_info     },
        [6]     = { MDLC24MSRES06,      MDLC24MSRESS06,     ms_reg_bit_assign[MODULE_NUM_CMNN][6U].bit_assign_info     },
        [7]     = { MDLC24MSRES07,      MDLC24MSRESS07,     ms_reg_bit_assign[MODULE_NUM_CMNN][7U].bit_assign_info     },
        [8]     = { MDLC24MSRES08,      MDLC24MSRESS08,     ms_reg_bit_assign[MODULE_NUM_CMNN][8U].bit_assign_info     },
        [9]     = { MDLC24MSRES09,      MDLC24MSRESS09,     ms_reg_bit_assign[MODULE_NUM_CMNN][9U].bit_assign_info     },
        [10]    = { MDLC24MSRES10,      MDLC24MSRESS10,     ms_reg_bit_assign[MODULE_NUM_CMNN][10U].bit_assign_info    },
        [11]    = { MDLC24MSRES11,      MDLC24MSRESS11,     ms_reg_bit_assign[MODULE_NUM_CMNN][11U].bit_assign_info    },
        [12]    = { MDLC24MSRES12,      MDLC24MSRESS12,     ms_reg_bit_assign[MODULE_NUM_CMNN][12U].bit_assign_info    },
        [13]    = { MDLC24MSRES13,      MDLC24MSRESS13,     ms_reg_bit_assign[MODULE_NUM_CMNN][13U].bit_assign_info    },
        [14]    = { MDLC24MSRES14,      MDLC24MSRESS14,     ms_reg_bit_assign[MODULE_NUM_CMNN][14U].bit_assign_info    },
        [15]    = { MDLC24MSRES15,      MDLC24MSRESS15,     ms_reg_bit_assign[MODULE_NUM_CMNN][15U].bit_assign_info    },
        [16]    = { MDLC24MSRES16,      MDLC24MSRESS16,     ms_reg_bit_assign[MODULE_NUM_CMNN][16U].bit_assign_info    },
        [17]    = { MDLC24MSRES17,      MDLC24MSRESS17,     ms_reg_bit_assign[MODULE_NUM_CMNN][17U].bit_assign_info    },
        [18]    = { MDLC24MSRES18,      MDLC24MSRESS18,     ms_reg_bit_assign[MODULE_NUM_CMNN][18U].bit_assign_info    },
        [19]    = { MDLC24MSRES19,      MDLC24MSRESS19,     ms_reg_bit_assign[MODULE_NUM_CMNN][19U].bit_assign_info    },
        [20]    = { MDLC24MSRES20,      MDLC24MSRESS20,     ms_reg_bit_assign[MODULE_NUM_CMNN][20U].bit_assign_info    },
        [21]    = { MDLC24MSRES21,      MDLC24MSRESS21,     ms_reg_bit_assign[MODULE_NUM_CMNN][21U].bit_assign_info    },
    },

    /* Module Number 25, Module Controller (CMNS)   */
    [MODULE_NUM_CMNS] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                          */
        [0]     = { MDLC25MSRES00,      MDLC25MSRESS00,     ms_reg_bit_assign[MODULE_NUM_CMNS][0U].bit_assign_info     },
        [1]     = { MDLC25MSRES01,      MDLC25MSRESS01,     ms_reg_bit_assign[MODULE_NUM_CMNS][1U].bit_assign_info     },
        [2]     = { MDLC25MSRES02,      MDLC25MSRESS02,     ms_reg_bit_assign[MODULE_NUM_CMNS][2U].bit_assign_info     },
        [3]     = { MDLC25MSRES03,      MDLC25MSRESS03,     ms_reg_bit_assign[MODULE_NUM_CMNS][3U].bit_assign_info     },
        [4]     = { MDLC25MSRES04,      MDLC25MSRESS04,     ms_reg_bit_assign[MODULE_NUM_CMNS][4U].bit_assign_info     },
        [5]     = { MDLC25MSRES05,      MDLC25MSRESS05,     ms_reg_bit_assign[MODULE_NUM_CMNS][5U].bit_assign_info     },
        [6]     = { MDLC25MSRES06,      MDLC25MSRESS06,     ms_reg_bit_assign[MODULE_NUM_CMNS][6U].bit_assign_info     },
        [7]     = { MDLC25MSRES07,      MDLC25MSRESS07,     ms_reg_bit_assign[MODULE_NUM_CMNS][7U].bit_assign_info     },
        [8]     = { MDLC25MSRES08,      MDLC25MSRESS08,     ms_reg_bit_assign[MODULE_NUM_CMNS][8U].bit_assign_info     },
        [9]     = { MDLC25MSRES09,      MDLC25MSRESS09,     ms_reg_bit_assign[MODULE_NUM_CMNS][9U].bit_assign_info     },
        [10]    = { MDLC25MSRES10,      MDLC25MSRESS10,     ms_reg_bit_assign[MODULE_NUM_CMNS][10U].bit_assign_info    },
        [11]    = { MDLC25MSRES11,      MDLC25MSRESS11,     ms_reg_bit_assign[MODULE_NUM_CMNS][11U].bit_assign_info    },
        [12]    = { MDLC25MSRES12,      MDLC25MSRESS12,     ms_reg_bit_assign[MODULE_NUM_CMNS][12U].bit_assign_info    },
        [13]    = { MDLC25MSRES13,      MDLC25MSRESS13,     ms_reg_bit_assign[MODULE_NUM_CMNS][13U].bit_assign_info    },
        [14]    = { MDLC25MSRES14,      MDLC25MSRESS14,     ms_reg_bit_assign[MODULE_NUM_CMNS][14U].bit_assign_info    },
        [15]    = { MDLC25MSRES15,      MDLC25MSRESS15,     ms_reg_bit_assign[MODULE_NUM_CMNS][15U].bit_assign_info    },
        [16]    = { MDLC25MSRES16,      MDLC25MSRESS16,     ms_reg_bit_assign[MODULE_NUM_CMNS][16U].bit_assign_info    },
        [17]    = { MDLC25MSRES17,      MDLC25MSRESS17,     ms_reg_bit_assign[MODULE_NUM_CMNS][17U].bit_assign_info    },
        [18]    = { MDLC25MSRES18,      MDLC25MSRESS18,     ms_reg_bit_assign[MODULE_NUM_CMNS][18U].bit_assign_info    },
        [19]    = { MDLC25MSRES19,      MDLC25MSRESS19,     ms_reg_bit_assign[MODULE_NUM_CMNS][19U].bit_assign_info    },
        [20]    = { MDLC25MSRES20,      MDLC25MSRESS20,     ms_reg_bit_assign[MODULE_NUM_CMNS][20U].bit_assign_info    },
        [21]    = { MDLC25MSRES21,      MDLC25MSRESS21,     ms_reg_bit_assign[MODULE_NUM_CMNS][21U].bit_assign_info    },
    },

    /* Module Number 26, Module Controller (SCP)   */
    [MODULE_NUM_SCP] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                           */
        [0]     = { MDLC26MSRES00,      MDLC26MSRESS00,     ms_reg_bit_assign[MODULE_NUM_SCP][0U].bit_assign_info      },
        [1]     = { MDLC26MSRES01,      MDLC26MSRESS01,     ms_reg_bit_assign[MODULE_NUM_SCP][1U].bit_assign_info      },
        [2]     = { MDLC26MSRES02,      MDLC26MSRESS02,     ms_reg_bit_assign[MODULE_NUM_SCP][2U].bit_assign_info      },
        [3]     = { MDLC26MSRES03,      MDLC26MSRESS03,     ms_reg_bit_assign[MODULE_NUM_SCP][3U].bit_assign_info      },
        [4]     = { MDLC26MSRES04,      MDLC26MSRESS04,     ms_reg_bit_assign[MODULE_NUM_SCP][4U].bit_assign_info      },
        [5]     = { MDLC26MSRES05,      MDLC26MSRESS05,     ms_reg_bit_assign[MODULE_NUM_SCP][5U].bit_assign_info      },
        [6]     = { MDLC26MSRES06,      MDLC26MSRESS06,     ms_reg_bit_assign[MODULE_NUM_SCP][6U].bit_assign_info      },
        [7]     = { MDLC26MSRES07,      MDLC26MSRESS07,     ms_reg_bit_assign[MODULE_NUM_SCP][7U].bit_assign_info      },
        [8]     = { MDLC26MSRES08,      MDLC26MSRESS08,     ms_reg_bit_assign[MODULE_NUM_SCP][8U].bit_assign_info      },
        [9]     = { MDLC26MSRES09,      MDLC26MSRESS09,     ms_reg_bit_assign[MODULE_NUM_SCP][9U].bit_assign_info      },
        [10]    = { MDLC26MSRES10,      MDLC26MSRESS10,     ms_reg_bit_assign[MODULE_NUM_SCP][10U].bit_assign_info     },
        [11]    = { MDLC26MSRES11,      MDLC26MSRESS11,     ms_reg_bit_assign[MODULE_NUM_SCP][11U].bit_assign_info     },
        [12]    = { MDLC26MSRES12,      MDLC26MSRESS12,     ms_reg_bit_assign[MODULE_NUM_SCP][12U].bit_assign_info     },
        [13]    = { MDLC26MSRES13,      MDLC26MSRESS13,     ms_reg_bit_assign[MODULE_NUM_SCP][13U].bit_assign_info     },
        [14]    = { MDLC26MSRES14,      MDLC26MSRESS14,     ms_reg_bit_assign[MODULE_NUM_SCP][14U].bit_assign_info     },
        [15]    = { MDLC26MSRES15,      MDLC26MSRESS15,     ms_reg_bit_assign[MODULE_NUM_SCP][15U].bit_assign_info     },
        [16]    = { MDLC26MSRES16,      MDLC26MSRESS16,     ms_reg_bit_assign[MODULE_NUM_SCP][16U].bit_assign_info     },
        [17]    = { MDLC26MSRES17,      MDLC26MSRESS17,     ms_reg_bit_assign[MODULE_NUM_SCP][17U].bit_assign_info     },
        [18]    = { MDLC26MSRES18,      MDLC26MSRESS18,     ms_reg_bit_assign[MODULE_NUM_SCP][18U].bit_assign_info     },
        [19]    = { MDLC26MSRES19,      MDLC26MSRESS19,     ms_reg_bit_assign[MODULE_NUM_SCP][19U].bit_assign_info     },
        [20]    = { MDLC26MSRES20,      MDLC26MSRESS20,     ms_reg_bit_assign[MODULE_NUM_SCP][20U].bit_assign_info     },
        [21]    = { MDLC26MSRES21,      MDLC26MSRESS21,     ms_reg_bit_assign[MODULE_NUM_SCP][21U].bit_assign_info     },
    },

    /* Module Number 27, Module Controller (AON)    */
    [MODULE_NUM_AON] =
    {   /*          ms_reset_reg_addr,  ms_stat_reg_addr,   ms_reg_bit_assign                                           */
        [0]     = { MDLC27MSRES00,      MDLC27MSRESS00,     ms_reg_bit_assign[MODULE_NUM_AON][0U].bit_assign_info      },
        [1]     = { MDLC27MSRES01,      MDLC27MSRESS01,     ms_reg_bit_assign[MODULE_NUM_AON][1U].bit_assign_info      },
        [2]     = { MDLC27MSRES02,      MDLC27MSRESS02,     ms_reg_bit_assign[MODULE_NUM_AON][2U].bit_assign_info      },
        [3]     = { MDLC27MSRES03,      MDLC27MSRESS03,     ms_reg_bit_assign[MODULE_NUM_AON][3U].bit_assign_info      },
        [4]     = { MDLC27MSRES04,      MDLC27MSRESS04,     ms_reg_bit_assign[MODULE_NUM_AON][4U].bit_assign_info      },
        [5]     = { MDLC27MSRES05,      MDLC27MSRESS05,     ms_reg_bit_assign[MODULE_NUM_AON][5U].bit_assign_info      },
        [6]     = { MDLC27MSRES06,      MDLC27MSRESS06,     ms_reg_bit_assign[MODULE_NUM_AON][6U].bit_assign_info      },
        [7]     = { MDLC27MSRES07,      MDLC27MSRESS07,     ms_reg_bit_assign[MODULE_NUM_AON][7U].bit_assign_info      },
        [8]     = { MDLC27MSRES08,      MDLC27MSRESS08,     ms_reg_bit_assign[MODULE_NUM_AON][8U].bit_assign_info      },
        [9]     = { MDLC27MSRES09,      MDLC27MSRESS09,     ms_reg_bit_assign[MODULE_NUM_AON][9U].bit_assign_info      },
        [10]    = { MDLC27MSRES10,      MDLC27MSRESS10,     ms_reg_bit_assign[MODULE_NUM_AON][10U].bit_assign_info     },
        [11]    = { MDLC27MSRES11,      MDLC27MSRESS11,     ms_reg_bit_assign[MODULE_NUM_AON][11U].bit_assign_info     },
        [12]    = { MDLC27MSRES12,      MDLC27MSRESS12,     ms_reg_bit_assign[MODULE_NUM_AON][12U].bit_assign_info     },
        [13]    = { MDLC27MSRES13,      MDLC27MSRESS13,     ms_reg_bit_assign[MODULE_NUM_AON][13U].bit_assign_info     },
        [14]    = { MDLC27MSRES14,      MDLC27MSRESS14,     ms_reg_bit_assign[MODULE_NUM_AON][14U].bit_assign_info     },
        [15]    = { MDLC27MSRES15,      MDLC27MSRESS15,     ms_reg_bit_assign[MODULE_NUM_AON][15U].bit_assign_info     },
        [16]    = { MDLC27MSRES16,      MDLC27MSRESS16,     ms_reg_bit_assign[MODULE_NUM_AON][16U].bit_assign_info     },
        [17]    = { MDLC27MSRES17,      MDLC27MSRESS17,     ms_reg_bit_assign[MODULE_NUM_AON][17U].bit_assign_info     },
        [18]    = { MDLC27MSRES18,      MDLC27MSRESS18,     ms_reg_bit_assign[MODULE_NUM_AON][18U].bit_assign_info     },
        [19]    = { MDLC27MSRES19,      MDLC27MSRESS19,     ms_reg_bit_assign[MODULE_NUM_AON][19U].bit_assign_info     },
        [20]    = { MDLC27MSRES20,      MDLC27MSRESS20,     ms_reg_bit_assign[MODULE_NUM_AON][20U].bit_assign_info     },
        [21]    = { MDLC27MSRES21,      MDLC27MSRESS21,     ms_reg_bit_assign[MODULE_NUM_AON][21U].bit_assign_info     },
    }
};


/*****************************************************************************
 * Module Controller common
 *****************************************************************************/
const char* get_class_group_name(uint32_t module_num);


/*****************************************************************************
 * Module Controller common
 *****************************************************************************/
const char* get_class_group_name(uint32_t module_num);
uint32_t get_mx_setting(void);

/*****************************************************************************
 * Module Standby
 *****************************************************************************/
void mdlc_ms_init(uint32_t module_num, uint32_t reg_num, uint32_t *init_bit_array);
void mdlc_ms_init_class_group(uint32_t module_num, uint32_t reg_num, uint32_t *init_bit_array);
void mdlc_ms_module_run_class_group(uint32_t module_num, uint32_t reg_num, uint32_t *init_bit_array);
void mdlc_ms_init_reg(uint32_t module_num, uint32_t reg_num, uint32_t *init_bit_array);
void mdlc_ms_module_run_reg(uint32_t module_num, uint32_t reg_num, uint32_t *init_bit_array);
void mdlc_ms_module_run_bit(uint32_t module_num, uint32_t reg_num, uint32_t bit_num);

void mdlc_check_ms_status(uint32_t module_num, uint32_t reg_num, uint32_t bit_num);
void mdlc_transition_ms(uint32_t module_num, uint32_t reg_num, uint32_t bit_num, uint32_t ms_dest);
void mdlc_ms_reg_write(uint32_t module_num, uint32_t reg_addr, uint32_t reg_val);

#endif  /* MODULE_CONTROLLER_H_ */
