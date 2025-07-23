/*************************************************************************
 * LICENSE:
 *    This source code is copyright (c) Renesas Electronics Corporation.
 *    All rights reserved.
 *
 ************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

#include "ucie_drv_port.h"
#include "ucie_drv.h"
#include "ucie_reg.h"

#include "rcar_ucie.h"

/************************************************************************
 * Definitions
 ************************************************************************/
#define UCIE_STATE_ACTIVE           (0x00000016U) /*16? 15h ACTIVE in STANDARD*/
#define UCIE_STATE_TRAINERROR       (0x000000FFU)

// -----------------------------------------------------------------------------
// Timeout Configuration
// -----------------------------------------------------------------------------
#define	TIMEOUT_EXECUTE_LOOP_COUNT (30000U)	/**< Timeout loop count */

/************************************************************************
 * Global Variables
 ************************************************************************/

/************************************************************************
 * Prototypes
 ************************************************************************/
static void            Ucie_Setup_Reset(uint32_t ucie_ch);
static void            Ucie_Setup_Pre(uint32_t ucie_ch, uint32_t device_type, uint32_t conn_ver);
static void            Ucie_Setup_Start_Linkup(uint32_t ucie_ch, uint32_t device_type);
static uint32_t        Ucie_Setup_Wait_FreqChange_Req(uint32_t ucie_ch, uint32_t device_type);
static void            Ucie_Setup_Ack_FreqChange(uint32_t ucie_ch);
// static uint32_t        Ucie_Setup_Wait_Linkup(uint32_t ucie_ch);
static uint32_t        Ucie_Setup_Wait_Linkup(uint32_t ucie_ch, uint32_t device_type);
static void            Ucie_Setup_PCIE_Pre(uint32_t ucie_ch, uint32_t device_type);
static void            Ucie_Setup_PCIE_Start_LinkUp(uint32_t ucie_ch, uint32_t device_type);
static uint32_t        Ucie_Setup_PCIE_Wait_LinkUp(uint32_t ucie_ch, uint32_t device_type);
static void            Ucie_Setup_PCIE_Post(uint32_t ucie_ch, uint32_t device_type);
// static void            wait_time(uint32_t count); // for boot code
/************************************************************************
 * Functions
 ************************************************************************/

/*******************************************************
 * NAME		: Ucie_Setup_Reset
 * FUNCTION	: Reset UCIE and do settings during reset
 * INPUT	: ucie_ch
 * OUTPUT	: none
 * RETURN	: none
 ******************************************************/
static void Ucie_Setup_Reset(uint32_t ucie_ch)
{
    volatile uintptr_t uciepwrmngctrl;
    // uint32_t ucie_apb_base;
    uint32_t value;

    // if(UCIE_CH0==ucie_ch){
    //    ucie_apb_base=0xDC000000;
    // } else {
    //    ucie_apb_base=0xDD000000;
    // }

    /* move to RESET state */
    Ucie_clkcnt(ucie_ch, UCIE_CLK_STBY);

    Ucie_ms_transition(ucie_ch, UCIE_MS_AREA_CORE, UCIE_MS_ST_RESET);
    Ucie_ms_transition(ucie_ch, UCIE_MS_AREA_PERI, UCIE_MS_ST_RESET);


    /* move common area to RUN state */
    Ucie_ms_transition(ucie_ch, UCIE_MS_AREA_PERI, UCIE_MS_ST_RUN);

    /*UCIE_APB[0/1]_UCIEPWRMNGCTRL register modify*/
    uciepwrmngctrl = UCIE_APB_BASE(ucie_ch) + 0x00E00070U;

    value = ucie_hwemu_read(uciepwrmngctrl);
    value &= ~(1U<<4);                 // Clear sys_aux_pwr_det bit (bit 4 )
    value |=  (1U<<6);                 // Set app_ready_entr_l23 bit (bit 6)
    ucie_hwemu_write(uciepwrmngctrl, value); 

#ifdef UCIE_SETUP_MODE_HOLDLTSSM
    //UCIERSTCTRL1.app_ltssm_enable=1 : hold ltssm state transition
    printf("HOLDLTSSM\n");
    // ucie_hwemu_write( ucie_apb_base+0xE00014U, 0x00000001U );
    ucie_hwemu_write(UCIE_APB_BASE(ucie_ch) + 0xE00014, 0x00000001U);
#endif /* UCIE_SETUP_MODE_HOLDLTSSM */

#ifdef START_INPUT_CLK_500MHZ
    Ucie_clkcnt(ucie_ch, UCIE_CLK_SLOW);
#else
    Ucie_clkcnt(ucie_ch, UCIE_CLK_FAST);
#endif /* START_INPUT_CLK_500MHZ */
    
    /* move core area to RUN state */
    Ucie_ms_transition(ucie_ch, UCIE_MS_AREA_CORE, UCIE_MS_ST_RUN);
}
/* -----------------------------------------------------------------------------
 * End of function Ucie_Setup_Reset
 * -------------------------------------------------------------------------- */

#if 0
/*******************************************************
 * NAME		: Ucie_Setup_Seq
 * FUNCTION	: Setup UCIE
 * INPUT	: ucie_ch, device_type, conn_ver, seq (0..all, N>0..specified procedure only
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *              : ERROR_END     Fail
 ******************************************************/
uint32_t Ucie_Setup_Seq(uint32_t ucie_ch, uint32_t device_type, uint32_t conn_ver, uint32_t seq)
{
    uint32_t ret;

    /* */

    if ( 0U == seq || 1U == seq ) {
        Ucie_Setup_Reset(ucie_ch);
        Ucie_Setup_Pre(ucie_ch, device_type, conn_ver);
    }

    /* */
    if ( 0U == seq || 2U == seq ) {
        Ucie_Setup_Start_Linkup(ucie_ch, device_type);
    }

    /* */
    if ( 0U == seq || 3U == seq ) {
        ret = Ucie_Setup_Wait_FreqChange_Req(ucie_ch);
        if (ret != NORMAL_END)
        {
            return ret;
        }
        Ucie_Setup_Ack_FreqChange(ucie_ch);
    }
    
    if ( 0U == seq || 4U == seq ) {
        ret = Ucie_Setup_Wait_Linkup(ucie_ch);
        if (ret != NORMAL_END)
        {
            return ret;
        }
    }

    /* */
    if ( 0U == seq || 5U == seq ) {
        Ucie_Setup_PCIE_Pre(ucie_ch, device_type);
    }

    /* */
    if ( 0U == seq || 6U == seq ) {
        Ucie_Setup_PCIE_Start_LinkUp(ucie_ch, device_type);
    }

    /* */
    if ( 0U == seq || 7U == seq ) {
        ret = Ucie_Setup_PCIE_Wait_LinkUp(ucie_ch);
        if (ret != NORMAL_END)
        {
            return ret;
        }
    }

    /* */
    if ( 0U == seq || 8U == seq ) {
        Ucie_Setup_PCIE_Post(ucie_ch, device_type);
    }

    /* */
    return NORMAL_END;

}
/* -----------------------------------------------------------------------------
 * End of function Ucie_Setup_Seq
 * -------------------------------------------------------------------------- */
#endif

//+++<ANH001>
/*******************************************************
 * NAME        : Ucie_Setup_Pre
 * FUNCTION    : Setup UCIE pre-linkup
 * INPUT       : ucie_ch: ucie0/1
 *               device_type: rc/ep
 * OUTPUT      : none
 * RETURN      : void
 ******************************************************/
static void Ucie_Setup_Pre(uint32_t ucie_ch, uint32_t device_type, uint32_t conn_ver)
{
    uint32_t ucie_axi_base;
    uint32_t ucie_apb_base;
    uint32_t reg_val;

    // if ( UCIE_CH0 == ucie_ch )
    // {
    //     ucie_axi_base=0xD8000000U;
    //     ucie_apb_base=0xDC000000U;
    // }
    // else
    // {
    //     ucie_axi_base=0xD9000000U;
    //     ucie_apb_base=0xDD000000U;
    // }
    ucie_axi_base = UCIE_AXI_BASE(ucie_ch);
    ucie_apb_base = UCIE_APB_BASE(ucie_ch);

    /*====================================*/
    /* CT pattern                         */
    /*====================================*/
    /* ;--- 6.0 bypass mode --- */
    /* ;EMUL mode */
    /* ; axi adr = 41 */
#ifndef DIRECT_ADR
    ucie_hwemu_write( ucie_apb_base+0xE005E8, 0x00000000 );

    ucie_hwemu_write( ucie_apb_base+0xE005E8, 0x00004141 );
#else
    ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00030000U );
#endif /* DIRECT_ADR */

#ifdef USE_EMULATOR
    /* ;MmInitCtrl */
    ucie_hwemu_write( ucie_axi_base + MMPL_MMINITCTRL_ADD, 0x08010000U );
#endif /* USE_EMULATOR */

#ifdef STATE_SKIP
    /*  state skip off */
    /* ; AcsmLtmlndex0Var1 */
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMINDEX0VAR1_ADD, 0x0003BF85U );
    /* ; AcsmLtmlndex0Var5 */
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMINDEX0VAR5_ADD, 0x00043FEDU );
    /* ; AcsmLtmlndex0Var13 */
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMINDEX0VAR13_ADD, 0x000CF816U );
#endif /* STATE_SKIP */

    /* ; axi adr = 0 */
#ifndef DIRECT_ADR
    ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00000000U );
#endif /* DIRECT_ADR */

#ifdef USE_EMULATOR
    /* ; PLLBypen=1, phy_lclk_sel=1 */
    ucie_hwemu_write( ucie_apb_base+0xE22000U, 0x00000005U );
    ucie_hwemu_write( ucie_apb_base+0xE22000U, 0x00000005U );
#endif /* USE_EMULATOR */

    /* EP: setup start from SBINIT is off*/
#ifndef DIRECT_ADR
    ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00004141U );
#endif /* DIRECT_ADR */

    if ( MODE_RC != device_type )
    {
        reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMMISCCTRL_ADD);
        reg_val = reg_val &    ACSM_ACSMMISCCTRL_MASK;
        reg_val = reg_val & ~( ACSM_ACSMMISCCTRL_ACSMLTSMTRIGGEREN_MASK  << ACSM_ACSMMISCCTRL_ACSMLTSMTRIGGEREN_SHIFT);
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMMISCCTRL_ADD, reg_val );
    }

#ifndef DIRECT_ADR
    ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00000000U );
#endif /* DIRECT_ADR */


    /* ; UCIe0/1 apb setting */
    if ( MODE_RC == device_type )
    {
        /* ; UCIEFMIS */
        ucie_hwemu_write( ucie_apb_base+0xE00308U, 0x019A0000U );    /* UCIEFMIS */

        ucie_hwemu_write( ucie_apb_base+0xE00000U, 0x00000010U );    /*  UCIE0 -> RC */

        /* ; UCIEPCR00 */
        ucie_hwemu_write( ucie_apb_base+0xE21000U, 0x00000001U );    /* set UCIe device Endpoint enable */

        /* ; UCIECSR00 */
        ucie_hwemu_write( ucie_apb_base+0xE20000U, 0x00000001U );    /* select CXL mode */
    }
    else
    {
        /* ; UCIEFMIS */
        ucie_hwemu_write( ucie_apb_base+0xE00308U, 0x015C0000U );    /* UCIEFMIS */

        ucie_hwemu_write( ucie_apb_base+0xE00000U, 0x00000000U );

        /* ; UCIEPCR00 */
        ucie_hwemu_write( ucie_apb_base+0xE21000U, 0x00000002U );    /* set UCIe device Endpoint enable */

        /* ; UCIECSR00 */
        ucie_hwemu_write( ucie_apb_base+0xE20000U, 0x00000001U );    /* select CXL mode */
    }

    /* ; UCIEICR27 */
    ucie_hwemu_write( ucie_apb_base+0xE1007CU, 0x00000002U );    /* interrupt output enable freq_change_req */

    /* ; UCIe0/1 axi setting 00 */
    if ( MODE_RC == device_type )
    {
        /* ; LINK_CONTROL2_LINK_STATUS2_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_PCIE_CAP_LINK_CONTROL2_LINK_STATUS2_REG_ADD, 0x00000004U );
    }
    ucie_hwemu_write( ucie_axi_base + RCAR_UCIE_BASE_ADD(0x38CU), 0x00000000U );
    ucie_hwemu_write( ucie_axi_base + RCAR_UCIE_BASE_ADD(0x38CU), 0x00000000U );

    /* ; PL32G_CONTROL_REG */
    ucie_hwemu_write( ucie_axi_base + PF0_PL32G_CAP_PL32G_CONTROL_REG_ADD, 0x00000200U );    /* PL32G_CONTROL_REG. Modified TS Usage Mode Selected.Alternate Protocol Negtiation. */
    /* ; CXL_VLSM_CSR_REG_OFF */
    ucie_hwemu_write( ucie_axi_base + PF0_PORT_LOGIC_CXL_VLSM_CSR_REG_OFF_ADD, 0x00800000U );    /* CXL VLSM CSR REGISTER */

    /* ; RASDP_ERROR_TRACER_CAPABILITY_OFF */
    /*  VTB_adr : 0x00000420U, C code adr : 0xd8b003bcU */
    ucie_hwemu_write( ucie_axi_base + RCAR_UCIE_BASE_ADD(0x420U), 0x00000027U );

    if ( MODE_RC != device_type )
    {
        /* ; CXL_RCIEP_FLEXBUS_CNTRL_STATUS_OFF */
        ucie_hwemu_write( ucie_axi_base + TMP_CXL_RCIEP_FLEXBUS_CNTRL_STATUS_OFF, 0x00000007U );    /* CXL_RCIEP_FLEXBUS_CNTRL_STATUS_OFF */
        /* ; PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_PCIE_CAP_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_ADD, 0x8002D010U ); /*  PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG */
        /* ; MISC_CONTROL_1_OFF */
        ucie_hwemu_write( ucie_axi_base + PF0_PORT_LOGIC_MISC_CONTROL_1_OFF_ADD, 0x080BFF48U ); /*  MISC_CONTROL_1_OFF */
    }
    else
    {
        /* ; PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_PCIE_CAP_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_ADD, 0x8042D010U ); /*  PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG */
        /* ; MISC_CONTROL_1_OFF */
        ucie_hwemu_write( ucie_axi_base + PF0_PORT_LOGIC_MISC_CONTROL_1_OFF_ADD, 0x040BFF48U ); /*  MISC_CONTROL_1_OFF */
    }

    ucie_hwemu_write( ucie_axi_base + RCAR_UCIE_BASE_ADD(0x38cU), 0x00000000U );

    /* ; ULT_PLLCTRL4 -> PF0_MEMBAR0_RAS_CAP_MEMBAR0_RAS_UNCOR_ERROR_MASK_REG_OFF_ADD */
    ucie_hwemu_write( ucie_axi_base + PF0_MEMBAR0_RAS_CAP_MEMBAR0_RAS_UNCOR_ERROR_MASK_REG_OFF_ADD, 0x00000000U ); /* elbi */
    /* ; MEMBAR0_RAS_CORR_ERROR_MASK_REG_OFF */
    ucie_hwemu_write( ucie_axi_base + PF0_MEMBAR0_RAS_CAP_MEMBAR0_RAS_CORR_ERROR_MASK_REG_OFF_ADD, 0x00000000U ); /* elbi */

    /* ; UCIe0 axi setting 01 */
    /* ;  axi0 addres ON */
#ifndef DIRECT_ADR
    ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00004141U );
#endif /* DIRECT_ADR */

    /* ;  ULT_CTRL_IMP_SB_CONFIG3 */
    ucie_hwemu_write( ucie_axi_base + IMP_SPECIFIC_SB_UNIT_IMP_SB_CONFIG3_ADD, 0x00018001U );

    /* ;  ULT_CTRL_IMP_SB_CONFIG0 */
    ucie_hwemu_write( ucie_axi_base + IMP_SPECIFIC_SB_UNIT_IMP_SB_CONFIG0_ADD, 0x000a0190U );

    /* ;  ULT_CTRL_IMP_SB_CONFIG2 */
    ucie_hwemu_write( ucie_axi_base + IMP_SPECIFIC_SB_UNIT_IMP_SB_CONFIG2_ADD, 0x000a0190U );

    /* ;  ULT_CTRL_IMP_SB_CONFIG4 */
    if ( MODE_RC == device_type )
    {
        ucie_hwemu_write( ucie_axi_base + IMP_SPECIFIC_SB_UNIT_IMP_SB_CONFIG4_ADD, 0x002000AEU ); /* DSP */
    }
    else
    {
        ucie_hwemu_write( ucie_axi_base + IMP_SPECIFIC_SB_UNIT_IMP_SB_CONFIG4_ADD, 0x004000AEU ); /* USP */
    }
    ucie_hwemu_write( ucie_axi_base + IMP_SPECIFIC_SB_UNIT_IMP_SB_CONFIG5_ADD, 0x00000000U );

#ifndef DIRECT_ADR
    /* ;  axi0 addres OFF */
    ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00000000U );
#endif /* DIRECT_ADR */

    /* ;  ULT_CTRL_DVSEC_UCIE_LINK_CONTROL */
    ucie_hwemu_write( ucie_axi_base + DVSEC_UNIT_DSP_DVSEC_UCIE_LINK_CONTROL_ADD, 0x00004000U );

    /* ;  ULT_CTRL_CXL_DVSEC_FLEX_CTL_STATUS */
#ifdef RCAR_UCIE_V100
    ucie_hwemu_write( ucie_axi_base + CXL_DVSEC_UNIT_DSP_CXL_DVSEC_FLEX_CTL_STATUS_ADD, 0x00000027U );
#endif /* RCAR_UCIE_V100 */

#if defined(RCAR_UCIE_V101) || defined(RCAR_UCIE_V102)
    /*  VTB_adr : 0x0000042cU, C code adr : 0xd8000420U */
    ucie_hwemu_write( ucie_axi_base + RCAR_UCIE_BASE_ADD(0x42CU), 0x00000027U ); /*  from UT result */
#endif /* #if defined(RCAR_UCIE_V101) || defined(RCAR_UCIE_V102) */

    /* ; UCIe0 axi setting 02 */
#ifndef DIRECT_ADR
    /* ;  axi addres ON */
    ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00004141U );
#endif /* DIRECT_ADR */

#if defined(RCAR_UCIE_V101) || defined(RCAR_UCIE_V102)
    /* ;  ULT_CTRL_IMP_MB_CONFIG11 */
    ucie_hwemu_write( ucie_axi_base + IMP_SPECIFIC_MB_UNIT_IMP_MB_CONFIG11_ADD, 0x00103001U );

    /* ;  ULT_CTRL_IMP_MB_CONFIG11 */
    ucie_hwemu_write( ucie_axi_base + IMP_SPECIFIC_MB_UNIT_IMP_MB_CONFIG11_ADD, 0x00103001U );
#endif /* defined(RCAR_UCIE_V101) || defined(RCAR_UCIE_V102) */

#ifdef RCAR_UCIE_V100
    /* ;  ULT_CTRL_IMP_MB_CONFIG11 */
    ucie_hwemu_write( ucie_axi_base + IMP_SPECIFIC_MB_UNIT_IMP_MB_CONFIG11_ADD, 0x00103000U );
    /* ;  ULT_CTRL_IMP_MB_CONFIG11 */
    ucie_hwemu_write( ucie_axi_base + IMP_SPECIFIC_MB_UNIT_IMP_MB_CONFIG11_ADD, 0x00103000U ); /*  X5H: bit[0] = 0 */
#endif /* RCAR_UCIE_V100 */

#ifdef RCAR_UCIE_V100
    /* ;  ULT_MMTRKEN */
    ucie_hwemu_write( ucie_axi_base + MMPL_MMTRKCTRL_ADD, 0x00000002U );

    /* ;  ULT_MODULEDISABLESTATUS */
    ucie_hwemu_write( ucie_axi_base + MMPL_MODULEDEGRADESTATUS_ADD, 0x0000FFFCU );

    /* ;  ULT_ACSMWAITDLY0 */
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMWAITDLY0_ADD, 0x000007D0U );

    /* ;  ULT_ACSMWAITDLY1 */
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMWAITDLY1_ADD, 0x00004A38U );

    /* ;  ULT_ZCALCOMPSTARTUPTIME */
    ucie_hwemu_write( ucie_axi_base + MMPL_ZCALCTRL0_ADD, 0x641F43E8U );

    /* ;  ULT_ZCALSAMPLETIME */
    ucie_hwemu_write( ucie_axi_base + MMPL_ZCALCTRL0_ADD, 0x640FA3E8U );

    /* ;  ULT_ZCALOFFSETSAMPLETIME */
    ucie_hwemu_write( ucie_axi_base + MMPL_ZCALCTRL0_ADD, 0x320FA3E8U );

    /* ;  ULT_TXZCALNOFFSET */
    ucie_hwemu_write( ucie_axi_base + MMPL_ZCALCTRL1_ADD, 0x0C219800U );

    /* ;  ULT_TXZCALPOFFSET */
    ucie_hwemu_write( ucie_axi_base + MMPL_ZCALCTRL1_ADD, 0x0C219CC0U );

    /* ;  ULT_PLLCPINTCTRL */
    ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL0_ADD, 0x2C7C0C03U ); /*  7'b111_1111, 7'b000_0011 */

    /* ;  ULT_PLLCPPROPCTRL */
    ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL0_ADD, 0x2C7C0B03U ); /*  7'b111_1111<<8, 7'b000_1011<<8 */

    /* ;  ULT_PLLCPINTGSCTRL */
    ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL0_ADD, 0x2C7F0B03U ); /*  7'b111_1111<<16, 7'b111_1111<<16 */

    /* ;  ULT_PLLCPPROPGSCTRL */
    ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL0_ADD, 0x287F0B03U ); /*  7'b111_1111<<24, 7'b010_1000<<24 */

    /* ;  ULT_PLLDIVSEL */
    ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL1_ADD, 0x00002BCEU );

    /* ;  ULT_PLLV2IMODE */
    ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL1_ADD, 0x000033CEU );

    /* ;  ULT_PLLVCOLOWFREQ */
    ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL1_ADD, 0x000013CEU );

    /* ;  ULT_PLLCTRL3 */
    ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL3_ADD, 0x50601009U );

    /* ;  ULT_PLLCTRL4 */
    ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL4_ADD, 0x041B6001U );

    /* ;  ULT_FREQCHANGETYPE */
    ucie_hwemu_write( ucie_axi_base + MMPL_MMMODECTRL_ADD, 0x00000012U );
#endif /* RCAR_UCIE_V100 */

    /* ;  ULT_DWMODECTRL0 */
    ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMODECTRL0_ADD, 0x0600000CU );

    /* ;  ULT_DWDCDSETTLETIME */
    ucie_hwemu_write( ucie_axi_base + DWORD_0_DWDCCCTRL1_ADD, 0x00C86432U );

    /* ;  ULT_DWDCASETTLETIME */
    ucie_hwemu_write( ucie_axi_base + DWORD_0_DWDCCCTRL1_ADD, 0x00C81532U );

    /* ;  ULT_DWDCDSAMPLETIME */
    ucie_hwemu_write( ucie_axi_base + DWORD_0_DWDCCCTRL1_ADD, 0x00141532U );

    /* ;  ULT_DWMODULEDISABLESTATUS */
    ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMODULEDEGRADESTATUS_ADD, 0x0000FFFCU );

    /* ;  ULT_DWVREFVARMAX */
    ucie_hwemu_write( ucie_axi_base + DWORD_0_DWVREFVAR_ADD, 0x00800001U );

    /* ;  ULT_DWVREFVARMIN */
    ucie_hwemu_write( ucie_axi_base + DWORD_0_DWVREFVAR_ADD, 0x0080007EU );

#if 0 /* <ANH001> from ucie_0624_1: not use any more */
    /* ;  ULT_DWTXCKPARKLEVEL */
    ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMISCCTRL0_ADD, 0x3D00A001U );
#endif

    /* ;  ULT_DWMODECTRL0 */
    ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMODECTRL0_ADD, 0x0600000CU );

    /* ;  ULT_DWDCDSETTLETIME_DW1 */
    ucie_hwemu_write( ucie_axi_base + DWORD_1_DWDCCCTRL1_ADD, 0x00C86432U );

    /* ;  ULT_DWDCASETTLETIME_DW1 */
    ucie_hwemu_write( ucie_axi_base + DWORD_1_DWDCCCTRL1_ADD, 0x00C81532U );

    /* ;  ULT_DWDCDSAMPLETIME_DW1 */
    ucie_hwemu_write( ucie_axi_base + DWORD_1_DWDCCCTRL1_ADD, 0x00141532U );

    /* ;  ULT_DWMODULEDISABLESTATUS_DW1 */
    ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMODULEDEGRADESTATUS_ADD, 0x0000FFFCU );

    /* ;  ULT_DWVREFVARMAX_DW1 */
    ucie_hwemu_write( ucie_axi_base + DWORD_1_DWVREFVAR_ADD, 0x00800001U );

    /* ;  ULT_DWVREFVARMIN_DW1 */
    ucie_hwemu_write( ucie_axi_base + DWORD_1_DWVREFVAR_ADD, 0x0080007EU );

#if 0 /* <ANH001> from ucie_0624_1: not use any more */
    /* ;  ULT_DWTXCKPARKLEVEL_DW1 */
    ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMISCCTRL0_ADD, 0x3D00E001U );
#endif

    /* default value as [31]=0, [15]=0 
     * [31] DWPHYCOMPLIANCE_SHIFT : 1 for V102_W_V100 case
     * [15] DWTXCKPARKLEVEL_SHIFT : 1 for V100, and V102_W_100 case
      */
    ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMISCCTRL0_ADD, 0x3D002001U );
    ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMISCCTRL0_ADD, 0x3D002001U );

#if defined(RCAR_UCIE_V101) || defined(RCAR_UCIE_V102)
    if ( UCIE_SETUP_VER_V100 == conn_ver )
    {
        /*  Setting CSR for reversalMB */
        ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG25_ADD, 0x83000061U);
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR4_ADD, 0x48E23803U);
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR5_ADD, 0x48E23803U);
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR6_ADD, 0x48E39805U);
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR7_ADD, 0x4BDFD805U);

        /* PARKLVL is 1 */
        reg_val = ucie_hwemu_read(ucie_axi_base + MMPL_MMMISCCTRL_ADD);
        reg_val = reg_val & MMPL_MMMISCCTRL_MASK;
        reg_val = reg_val &      ~( MMPL_MMMISCCTRL_MMPHYCOMPLIANCE_MASK  << MMPL_MMMISCCTRL_MMPHYCOMPLIANCE_SHIFT);
        reg_val = reg_val | ((0x1U & MMPL_MMMISCCTRL_MMPHYCOMPLIANCE_MASK) << MMPL_MMMISCCTRL_MMPHYCOMPLIANCE_SHIFT);
        ucie_hwemu_write( ucie_axi_base + MMPL_MMMISCCTRL_ADD, reg_val );
    
        reg_val = ucie_hwemu_read(ucie_axi_base + DWORD_0_DWMISCCTRL0_ADD);
        reg_val = reg_val & DWORD_0_DWMISCCTRL0_MASK;
        reg_val = reg_val &      ~( DWORD_0_DWMISCCTRL0_DWPHYCOMPLIANCE_MASK  << DWORD_0_DWMISCCTRL0_DWPHYCOMPLIANCE_SHIFT);
        reg_val = reg_val | ((0x1U & DWORD_0_DWMISCCTRL0_DWPHYCOMPLIANCE_MASK) << DWORD_0_DWMISCCTRL0_DWPHYCOMPLIANCE_SHIFT);
        ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMISCCTRL0_ADD, reg_val );
    
        reg_val = ucie_hwemu_read(ucie_axi_base + DWORD_1_DWMISCCTRL0_ADD);
        reg_val = reg_val & DWORD_1_DWMISCCTRL0_MASK;
        reg_val = reg_val &      ~( DWORD_1_DWMISCCTRL0_DWPHYCOMPLIANCE_MASK  << DWORD_1_DWMISCCTRL0_DWPHYCOMPLIANCE_SHIFT);
        reg_val = reg_val | ((0x1U & DWORD_1_DWMISCCTRL0_DWPHYCOMPLIANCE_MASK) << DWORD_1_DWMISCCTRL0_DWPHYCOMPLIANCE_SHIFT);
        ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMISCCTRL0_ADD, reg_val );
    
        reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMTRAINVAR0I0_ADD);
        reg_val = reg_val & ACSM_ACSMTRAINVAR0I0_MASK;
        reg_val = reg_val | (0x1U << 16);
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMTRAINVAR0I0_ADD, reg_val );
    
        reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMTRAINVAR0I1_ADD);
        reg_val = reg_val & ACSM_ACSMTRAINVAR0I1_MASK;
        reg_val = reg_val | (0x1U << 16);
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMTRAINVAR0I1_ADD, reg_val );
    
        reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMTRAINVAR0I2_ADD);
        reg_val = reg_val & ACSM_ACSMTRAINVAR0I2_MASK;
        reg_val = reg_val | (0x1U << 16);
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMTRAINVAR0I2_ADD, reg_val );
    
        reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMTRAINVAR1I1_ADD);
        reg_val = reg_val & ACSM_ACSMTRAINVAR1I1_MASK;
        reg_val = reg_val & 0xFFFFFEFFU;
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMTRAINVAR1I1_ADD, reg_val );

        /*  Setting PLL clock for X5H connection */
        ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL0_P0_ADD  , 0x3F3F100CU);
        ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL0_P1_ADD  , 0x3F3F100CU);
        ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL0_P2_ADD  , 0x303F0808U);
        ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL0_P3_ADD  , 0x3F3F1004U);
        ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL1_P0_ADD  , 0x14830C55U);
        ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL1_P1_ADD  , 0x14870C04U);
        ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL1_P2_ADD  , 0x03870C04U);
        ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL1_P3_ADD  , 0x06C70C04U);
        ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL3_ADD     , 0x04236B49U);
        ucie_hwemu_write( ucie_axi_base + MMPL_PLLCTRL4_ADD     , 0x50601009U);
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMWAITDLY0_ADD , 0x000007D0U);
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMWAITDLY1_ADD , 0x00004A38U);
    }
#endif /* #if defined(RCAR_UCIE_V101) || defined(RCAR_UCIE_V102) */

#ifdef RCAR_UCIE_V100
    /*  Setting CSR for reversalMB */
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG95_ADD      , 0x000FFF53U);
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG96_ADD      , 0x000FFFB3U);
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG97_ADD      , 0x00080023U);
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG98_ADD      , 0x0000B00BU);
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG99_ADD      , 0x03970B6AU);

    reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMLTSMINDEX0VAR14_ADD);
    reg_val = reg_val & ACSM_ACSMLTSMINDEX0VAR14_MASK;
    reg_val = reg_val | (0x1U << 20);
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMINDEX0VAR14_ADD, reg_val );

    reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMLTSMINDEX0VAR16_ADD);
    reg_val = reg_val & ACSM_ACSMLTSMINDEX0VAR16_MASK;
    reg_val = reg_val | (0x1U << 20);
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMINDEX0VAR16_ADD, reg_val );

    reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMTRAINVAR0I0_ADD);
    reg_val = reg_val & ACSM_ACSMTRAINVAR0I0_MASK;
    reg_val = reg_val | (0x1U << 16);
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMTRAINVAR0I0_ADD, reg_val );

    reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMTRAINVAR0I1_ADD);
    reg_val = reg_val & ACSM_ACSMTRAINVAR0I1_MASK;
    reg_val = reg_val | (0x1U << 16);
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMTRAINVAR0I1_ADD, reg_val );

    reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMTRAINVAR0I2_ADD);
    reg_val = reg_val & ACSM_ACSMTRAINVAR0I2_MASK;
    reg_val = reg_val | (0x1U << 16);
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMTRAINVAR0I2_ADD, reg_val );

    reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMTRAINVAR1I1_ADD);
    reg_val = reg_val & ACSM_ACSMTRAINVAR1I1_MASK;
    reg_val = reg_val & 0xFFFFFEFFU;
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMTRAINVAR1I1_ADD, reg_val );

    /* setup_highspeed_link_initialization_and_training_steps_x5h(); tmk+:AIACC:20250407 */
    /* 2. Setup high-speed link initialization and training steps. */
    /* a. Write MmTrkCtrl.MmTrkEn = 1'b0 to disable periodic track run time calibration. */
    ucie_hwemu_write( ucie_axi_base + RCAR_UCIE_BASE_ADD(0xF01128U), 0x00000000U );

    /* b. Write DwRxLatCtrl.DwRxVldMargin = 1 to setup VLD lane training variables. */
    reg_val = ucie_hwemu_read(ucie_axi_base + DWORD_0_DWRXLATCTRL_ADD);
    reg_val = reg_val & DWORD_0_DWRXLATCTRL_MASK;
    reg_val = reg_val &      ~( DWORD_0_DWRXLATCTRL_DWRXVLDMARGIN_MASK  << DWORD_0_DWRXLATCTRL_DWRXVLDMARGIN_SHIFT);
    reg_val = reg_val | ((0x1U & DWORD_0_DWRXLATCTRL_DWRXVLDMARGIN_MASK) << DWORD_0_DWRXLATCTRL_DWRXVLDMARGIN_SHIFT);
    ucie_hwemu_write( ucie_axi_base + DWORD_0_DWRXLATCTRL_ADD, reg_val );

    reg_val = ucie_hwemu_read(ucie_axi_base + DWORD_1_DWRXLATCTRL_ADD);
    reg_val = reg_val & DWORD_1_DWRXLATCTRL_MASK;
    reg_val = reg_val &      ~( DWORD_1_DWRXLATCTRL_DWRXVLDMARGIN_MASK  << DWORD_1_DWRXLATCTRL_DWRXVLDMARGIN_SHIFT);
    reg_val = reg_val | ((0x1U & DWORD_1_DWRXLATCTRL_DWRXVLDMARGIN_MASK) << DWORD_1_DWRXLATCTRL_DWRXVLDMARGIN_SHIFT);
    ucie_hwemu_write( ucie_axi_base + DWORD_1_DWRXLATCTRL_ADD, reg_val );

    /* d. Write 00800000 to UcieTrainingSetup1 to program PPGC pattern.ã??*/
#ifndef DIRECT_ADR
    ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00004040U );
#endif /* DIRECT_ADR */

    /*  adr 0x4070_1*** is none in .h file */
    ucie_hwemu_write( ucie_axi_base + RCAR_UCIE_BASE_ADD(0x40701000U) + 0x010U, 0x00800000U );
    ucie_hwemu_write( ucie_axi_base + RCAR_UCIE_BASE_ADD(0x40701000U) + 0x014U, 0x00800000U );

    /* e. Write 00010000U to UcieTrainingSetup2 to program PPGC pattern. */
    /*  adr 0x4070_1*** is none in .h file */
    ucie_hwemu_write( ucie_axi_base + RCAR_UCIE_BASE_ADD(0x40701000U) + 0x020U, 0x00010000U );
    ucie_hwemu_write( ucie_axi_base + RCAR_UCIE_BASE_ADD(0x40701000U) + 0x024U, 0x00010000U );

    /* f. Write DwMiscCtrl0.DwTxCkParkLevel=1'b1, clock is parked at its inactive level. */
#ifndef DIRECT_ADR
    ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00004141U );
#endif /* DIRECT_ADR */

    reg_val = ucie_hwemu_read(ucie_axi_base + DWORD_0_DWMISCCTRL0_ADD);
    reg_val = reg_val & DWORD_0_DWMISCCTRL0_MASK;
    reg_val = reg_val &      ~( DWORD_0_DWMISCCTRL0_DWTXCKPARKLEVEL_MASK  << DWORD_0_DWMISCCTRL0_DWTXCKPARKLEVEL_SHIFT);
    reg_val = reg_val | ((0x1U & DWORD_0_DWMISCCTRL0_DWTXCKPARKLEVEL_MASK) << DWORD_0_DWMISCCTRL0_DWTXCKPARKLEVEL_SHIFT);
    ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMISCCTRL0_ADD, reg_val );

    reg_val = ucie_hwemu_read(ucie_axi_base + DWORD_1_DWMISCCTRL0_ADD);
    reg_val = reg_val & DWORD_1_DWMISCCTRL0_MASK;
    reg_val = reg_val &      ~( DWORD_1_DWMISCCTRL0_DWTXCKPARKLEVEL_MASK  << DWORD_1_DWMISCCTRL0_DWTXCKPARKLEVEL_SHIFT);
    reg_val = reg_val | ((0x1U & DWORD_1_DWMISCCTRL0_DWTXCKPARKLEVEL_MASK) << DWORD_1_DWMISCCTRL0_DWTXCKPARKLEVEL_SHIFT);
    ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMISCCTRL0_ADD, reg_val );

    /* g. Program DwModeCtrl0 with the following values. */
    /* i. Write DwModeCtrl0.DwRxCtlClkSel=0x1U to select the receive clock. */
    /* ii. Write DwModeCtrl0.DwRxLatAlign=0x1U to select the read latency alignment type. */
    reg_val = ucie_hwemu_read(ucie_axi_base + DWORD_0_DWMODECTRL0_ADD);
    reg_val = reg_val & DWORD_0_DWMODECTRL0_MASK;
    reg_val = reg_val &      ~( DWORD_0_DWMODECTRL0_DWRXCTLCLKSEL_MASK  << DWORD_0_DWMODECTRL0_DWRXCTLCLKSEL_SHIFT);
    reg_val = reg_val | ((0x1U & DWORD_0_DWMODECTRL0_DWRXCTLCLKSEL_MASK) << DWORD_0_DWMODECTRL0_DWRXCTLCLKSEL_SHIFT);
    reg_val = reg_val &      ~( DWORD_0_DWMODECTRL0_DWRXLATALIGN_MASK  << DWORD_0_DWMODECTRL0_DWRXLATALIGN_SHIFT);
    reg_val = reg_val | ((0x1U & DWORD_0_DWMODECTRL0_DWRXLATALIGN_MASK) << DWORD_0_DWMODECTRL0_DWRXLATALIGN_SHIFT);
    ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMODECTRL0_ADD, reg_val );

    reg_val = ucie_hwemu_read(ucie_axi_base + DWORD_1_DWMODECTRL0_ADD);
    reg_val = reg_val & DWORD_1_DWMODECTRL0_MASK;
    reg_val = reg_val &      ~( DWORD_1_DWMODECTRL0_DWRXCTLCLKSEL_MASK  << DWORD_1_DWMODECTRL0_DWRXCTLCLKSEL_SHIFT);
    reg_val = reg_val | ((0x1U & DWORD_1_DWMODECTRL0_DWRXCTLCLKSEL_MASK) << DWORD_1_DWMODECTRL0_DWRXCTLCLKSEL_SHIFT);
    reg_val = reg_val &      ~( DWORD_1_DWMODECTRL0_DWRXLATALIGN_MASK  << DWORD_1_DWMODECTRL0_DWRXLATALIGN_SHIFT);
    reg_val = reg_val | ((0x1U & DWORD_1_DWMODECTRL0_DWRXLATALIGN_MASK) << DWORD_1_DWMODECTRL0_DWRXLATALIGN_SHIFT);
    ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMODECTRL0_ADD, reg_val );

    /* h. Write AcsmLtsmIndex0Var14=0x14efefU to enable 1D Vref training. */
    reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMLTSMINDEX0VAR14_ADD);
    reg_val = reg_val & ACSM_ACSMLTSMINDEX0VAR14_MASK;
    reg_val = reg_val &           ~( ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_MASK  << ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_SHIFT);
    reg_val = reg_val | ((0x14EFEFU & ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_MASK) << ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_SHIFT);
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMINDEX0VAR14_ADD, reg_val );

    /*  adr symbol is none */
    reg_val = ucie_hwemu_read(ucie_axi_base + (RCAR_UCIE_BASE_ADD(0x41F12000U)+0x150U) );
    reg_val = reg_val & ACSM_ACSMLTSMINDEX0VAR14_MASK;
    reg_val = reg_val &           ~( ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_MASK  << ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_SHIFT);
    reg_val = reg_val | ((0x14EFEFU & ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_MASK) << ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_SHIFT);
    ucie_hwemu_write( ucie_axi_base + (RCAR_UCIE_BASE_ADD(0x41F12000U)+0x150U), reg_val );

    /* i. Write AcsmLtsmIndex0Var16=0x10f7f1U to enable 1D Vref training. */
    reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMLTSMINDEX0VAR16_ADD );
    reg_val = reg_val & ACSM_ACSMLTSMINDEX0VAR16_MASK;
    reg_val = reg_val &           ~( ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_MASK  << ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_SHIFT);
    reg_val = reg_val | ((0x10F7F1U & ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_MASK) << ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_SHIFT);
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMINDEX0VAR16_ADD, reg_val );

    /*  adr symbol is none */
    reg_val = ucie_hwemu_read(ucie_axi_base + (RCAR_UCIE_BASE_ADD(0x41F12000U)+0x158U) );
    reg_val = reg_val & ACSM_ACSMLTSMINDEX0VAR16_MASK;
    reg_val = reg_val &           ~( ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_MASK  << ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_SHIFT);
    reg_val = reg_val | ((0x10F7F1U & ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_MASK) << ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_SHIFT);
    ucie_hwemu_write( ucie_axi_base + (RCAR_UCIE_BASE_ADD(0x41F12000U)+0x158U), reg_val );

    /* j. Write 00001600U to AcsmLoopVar1 so that LTSM will go from RESET to LINKINIT */
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLOOPVAR1_ADD, 0x0001600U );


    /* update_instr_code_x5h(); */
    /* Instruction registers */
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG5_ADD,   0x03910133U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG6_ADD,   0x022403A3U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG7_ADD,   0x03910E33U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG8_ADD,   0x022404A3U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG9_ADD,   0x02210143U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG10_ADD,  0x02200000U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG11_ADD,  0x83100011U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG12_ADD,  0x0110C007U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG13_ADD,  0x02711003U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG14_ADD,  0xA3100801U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG15_ADD,  0xA3100011U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG16_ADD,  0xA3100021U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG17_ADD,  0x63970181U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG18_ADD,  0xA3100031U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG19_ADD,  0xA3100041U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG20_ADD,  0xA3100851U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG21_ADD,  0x22712003U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG22_ADD,  0x000122C9U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG23_ADD,  0x000C4125U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG24_ADD,  0x83045062U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG25_ADD,  0x000100C6U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG26_ADD,  0x83200091U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG27_ADD,  0x02500000U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG28_ADD,  0x83100041U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG29_ADD,  0x02714003U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG30_ADD,  0x02210243U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG31_ADD,  0x00042EC9U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG32_ADD,  0x000300A6U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG33_ADD,  0x02210443U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG34_ADD,  0x8300D0C1U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG35_ADD,  0x832000D1U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG36_ADD,  0x830001D1U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG37_ADD,  0x83102071U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG38_ADD,  0x00034125U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG39_ADD,  0x000F7155U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG40_ADD,  0x00021006U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG68_ADD,  0x00080423U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG69_ADD,  0x0004FE53U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG70_ADD,  0x0001CFB3U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG71_ADD,  0x03973C6AU );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG72_ADD,  0x834090F1U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG73_ADD,  0x80006081U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG74_ADD,  0x80000281U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG75_ADD,  0x83209081U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG76_ADD,  0x83000281U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG77_ADD,  0x80004081U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG78_ADD,  0x80006481U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG79_ADD,  0x8000A181U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG80_ADD,  0x83000001U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG81_ADD,  0x0397606AU );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG82_ADD,  0x83200001U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG83_ADD,  0x83000101U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG84_ADD,  0x83005181U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG85_ADD,  0x80004381U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG86_ADD,  0x000F5135U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG87_ADD,  0x00016145U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG88_ADD,  0x03979B6AU );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG89_ADD,  0x03978D6AU );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG91_ADD,  0x0397406AU );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG92_ADD,  0x01180873U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG93_ADD,  0x02780863U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG94_ADD,  0x01180073U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG95_ADD,  0x000FFF53U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG96_ADD,  0x000FFFB3U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG97_ADD,  0x00080023U );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG98_ADD,  0x0000B00BU );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG99_ADD,  0x03970B6AU );
    ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG100_ADD, 0x00020173U );

    /* Mask registers */
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR0_ADD,     0x1f9U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR1_ADD,     0x2623001U  );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR2_ADD,     0x5U        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR3_ADD,     0x1e1U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR4_ADD,     0x3727003U  );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR5_ADD,     0x3727003U  );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR6_ADD,     0x2733005U  );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR7_ADD,     0x36fb005U  );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR8_ADD,     0xde673e05U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR9_ADD,     0xde673e05U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR10_ADD,    0xbU        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR11_ADD,    0x1e3U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR12_ADD,    0xc0000607U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR13_ADD,    0x32673805U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR14_ADD,    0xde673e05U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR15_ADD,    0x32673805U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR16_ADD,    0xde673e05U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR17_ADD,    0x5U        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR18_ADD,    0x32673805U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR19_ADD,    0x26fb007U  );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR20_ADD,    0x1603005U  );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR21_ADD,    0xcec203U   );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR22_ADD,    0xc208985U  );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR23_ADD,    0x1020605U  );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR24_ADD,    0x4030005U  );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR25_ADD,    0xf8100673U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR26_ADD,    0xf8100673U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR27_ADD,    0x8800c277U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR28_ADD,    0x8800c275U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR29_ADD,    0x8800c277U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR30_ADD,    0x7300000dU );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR0_ADD,     0x140U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR1_ADD,     0x120U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR2_ADD,     0x100U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR3_ADD,     0x120U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR4_ADD,     0x120U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR5_ADD,     0x120U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR6_ADD,     0x120U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR7_ADD,     0x120U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR8_ADD,     0x123U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR9_ADD,     0x123U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR10_ADD,    0x120U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR11_ADD,    0x120U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR12_ADD,    0x123U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR13_ADD,    0x120U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR14_ADD,    0x123U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR15_ADD,    0x120U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR16_ADD,    0x123U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR17_ADD,    0x120U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR18_ADD,    0x120U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR19_ADD,    0x1dcU      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR20_ADD,    0x120U      );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR21_ADD,    0x4U        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR22_ADD,    0x18U       );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR23_ADD,    0x4U        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR24_ADD,    0x8U        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR25_ADD,    0x3U        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR26_ADD,    0x3U        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR27_ADD,    0xbU        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR28_ADD,    0xbU        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR29_ADD,    0xbU        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR30_ADD,    0x4U        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR21ALT_ADD, 0xcc800bU   );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR22ALT_ADD, 0xc208985U  );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR23ALT_ADD, 0x100210dU  );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR24ALT_ADD, 0x4020005U  );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR25ALT_ADD, 0xf820a173U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR26ALT_ADD, 0xf820a173U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR27ALT_ADD, 0x88008077U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR28ALT_ADD, 0x88008075U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR29ALT_ADD, 0x88008077U );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR30ALT_ADD, 0x7300000dU );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR21ALT_ADD, 0x4U        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR22ALT_ADD, 0x18U       );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR23ALT_ADD, 0x4U        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR24ALT_ADD, 0x8U        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR25ALT_ADD, 0x3U        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR26ALT_ADD, 0x3U        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR27ALT_ADD, 0xbU        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR28ALT_ADD, 0xbU        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR29ALT_ADD, 0xbU        );
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK1VAR30ALT_ADD, 0x4U        );

    /* Address registers */
    ucie_hwemu_write( ucie_axi_base + MMPL_CSRADDR2_ADD,  0x304004U );
    ucie_hwemu_write( ucie_axi_base + MMPL_CSRADDR5_ADD,  0x304018U );
    ucie_hwemu_write( ucie_axi_base + MMPL_CSRADDR10_ADD, 0x3042BCU );
    ucie_hwemu_write( ucie_axi_base + MMPL_CSRADDR11_ADD, 0x304020U );

    reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMSEQ0CTRL_ADD);
    reg_val = reg_val & ACSM_ACSMSEQ0CTRL_MASK;
    reg_val = reg_val &       ~( ACSM_ACSMSEQ0CTRL_ACSMSEQ0STOPADDR_MASK  << ACSM_ACSMSEQ0CTRL_ACSMSEQ0STOPADDR_SHIFT);
    reg_val = reg_val | ((0x28U & ACSM_ACSMSEQ0CTRL_ACSMSEQ0STOPADDR_MASK) << ACSM_ACSMSEQ0CTRL_ACSMSEQ0STOPADDR_SHIFT);
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMSEQ0CTRL_ADD, reg_val );

    reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMSEQ1CTRL_ADD);
    reg_val = reg_val & ACSM_ACSMSEQ1CTRL_MASK;
    reg_val = reg_val &       ~( ACSM_ACSMSEQ1CTRL_ACSMSEQ1STOPADDR_MASK  << ACSM_ACSMSEQ1CTRL_ACSMSEQ1STOPADDR_SHIFT);
    reg_val = reg_val | ((0x64U & ACSM_ACSMSEQ1CTRL_ACSMSEQ1STOPADDR_MASK) << ACSM_ACSMSEQ1CTRL_ACSMSEQ1STOPADDR_SHIFT);
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMSEQ1CTRL_ADD, reg_val );

    reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMCTRL_ADD);
    reg_val = reg_val & ACSM_ACSMCTRL_MASK;
    reg_val = reg_val &       ~( ACSM_ACSMCTRL_ACSMSTOPADDR_MASK  << ACSM_ACSMCTRL_ACSMSTOPADDR_SHIFT);
    reg_val = reg_val | ((0x28U & ACSM_ACSMCTRL_ACSMSTOPADDR_MASK) << ACSM_ACSMCTRL_ACSMSTOPADDR_SHIFT);
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMCTRL_ADD, reg_val ); 
#endif /* #ifdef RCAR_UCIE_V100 */
  
#if defined(RCAR_UCIE_V101) || defined(RCAR_UCIE_V102)
    if ( UCIE_SETUP_VER_V100 == conn_ver )
    {
        // PHY N3A for N3E
        ucie_hwemu_write( ucie_axi_base + DVSEC_DVSECLINKCONTROL_ADD  , 0x00000010U );
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR4_ADD   , 0x48e23803U );
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR5_ADD   , 0x48e23803U );
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR6_ADD   , 0x48e39805U );
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMLTSMMSK0VAR7_ADD   , 0x4bdfd805U );
        
        ucie_hwemu_write( ucie_axi_base + MMPL_MMTRKCTRL_ADD, 0x00000000U ); // add 250611
        
        ucie_hwemu_write( ucie_axi_base + MMPL_ZCALCTRL1_ADD, 0x09470000U ); // add 250611
        ucie_hwemu_write( ucie_axi_base + MMPL_ZCALCTRL4_ADD, 0x00051D47U ); // add 250611
        
        //ucie_hwemu_write( ucie_axi_base + DWORD_0_DWTXZCALSB_ADD      , 0x0000282BU );
        reg_val = ucie_hwemu_read(ucie_axi_base + DWORD_0_DWTXZCALSB_ADD);
        reg_val = reg_val &        ~( DWORD_0_DWTXZCALSB_DWTXZCALPSB_MASK  << DWORD_0_DWTXZCALSB_DWTXZCALPSB_SHIFT);
        reg_val = reg_val &        ~( DWORD_0_DWTXZCALSB_DWTXZCALNSB_MASK  << DWORD_0_DWTXZCALSB_DWTXZCALNSB_SHIFT);
        reg_val = reg_val | ((0x2BU & DWORD_0_DWTXZCALSB_DWTXZCALPSB_MASK) << DWORD_0_DWTXZCALSB_DWTXZCALPSB_SHIFT);
        reg_val = reg_val | ((0x28U & DWORD_0_DWTXZCALSB_DWTXZCALNSB_MASK) << DWORD_0_DWTXZCALSB_DWTXZCALNSB_SHIFT);
        ucie_hwemu_write( ucie_axi_base + DWORD_0_DWTXZCALSB_ADD, reg_val );
        
        //ucie_hwemu_write( ucie_axi_base + DWORD_1_DWTXZCALSB_ADD      , 0x0000282BU );
        reg_val = ucie_hwemu_read(ucie_axi_base + DWORD_1_DWTXZCALSB_ADD);
        reg_val = reg_val &        ~( DWORD_1_DWTXZCALSB_DWTXZCALPSB_MASK  << DWORD_1_DWTXZCALSB_DWTXZCALPSB_SHIFT);
        reg_val = reg_val &        ~( DWORD_1_DWTXZCALSB_DWTXZCALNSB_MASK  << DWORD_1_DWTXZCALSB_DWTXZCALNSB_SHIFT);
        reg_val = reg_val | ((0x2BU & DWORD_1_DWTXZCALSB_DWTXZCALPSB_MASK) << DWORD_1_DWTXZCALSB_DWTXZCALPSB_SHIFT);
        reg_val = reg_val | ((0x28U & DWORD_1_DWTXZCALSB_DWTXZCALNSB_MASK) << DWORD_1_DWTXZCALSB_DWTXZCALNSB_SHIFT);
        ucie_hwemu_write( ucie_axi_base + DWORD_1_DWTXZCALSB_ADD, reg_val );
        
        // sw_less mode = 1
        ucie_hwemu_write( ucie_apb_base+0xE21014U, 0x00000001U );
        
        ucie_hwemu_write( ucie_axi_base + MMPL_MMMODECTRL_ADD         , 0x00000012U );
        ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMISCCTRL1_ADD     , 0x03240800U );
        ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMISCCTRL1_ADD     , 0x03240800U );
        
        //ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMODECTRL0_ADD     , 0x00800000U );
        reg_val = ucie_hwemu_read(ucie_axi_base + DWORD_0_DWMODECTRL0_ADD);
        reg_val = reg_val &       ~( DWORD_0_DWMODECTRL0_DWTXSCRAMBLEEN_MASK  << DWORD_0_DWMODECTRL0_DWTXSCRAMBLEEN_SHIFT);
        reg_val = reg_val &       ~( DWORD_0_DWMODECTRL0_DWRXSCRAMBLEEN_MASK  << DWORD_0_DWMODECTRL0_DWRXSCRAMBLEEN_SHIFT);
        reg_val = reg_val &       ~( DWORD_0_DWMODECTRL0_DWRXCTLCLKSEL_MASK   << DWORD_0_DWMODECTRL0_DWRXCTLCLKSEL_SHIFT);
        //reg_val = reg_val &       ~( DWORD_0_DWMODECTRL0_DWRXLATALIGN_MASK    << DWORD_0_DWMODECTRL0_DWRXLATALIGN_SHIFT);
        reg_val = reg_val | ((0x0U & DWORD_0_DWMODECTRL0_DWTXSCRAMBLEEN_MASK) << DWORD_0_DWMODECTRL0_DWTXSCRAMBLEEN_SHIFT);
        reg_val = reg_val | ((0x0U & DWORD_0_DWMODECTRL0_DWRXSCRAMBLEEN_MASK) << DWORD_0_DWMODECTRL0_DWRXSCRAMBLEEN_SHIFT);
        reg_val = reg_val | ((0x1U & DWORD_0_DWMODECTRL0_DWRXCTLCLKSEL_MASK)  << DWORD_0_DWMODECTRL0_DWRXCTLCLKSEL_SHIFT);
        //reg_val = reg_val | ((0x1U & DWORD_0_DWMODECTRL0_DWRXLATALIGN_MASK)   << DWORD_0_DWMODECTRL0_DWRXLATALIGN_SHIFT);
        ucie_hwemu_write( ucie_axi_base +  DWORD_0_DWMODECTRL0_ADD, reg_val );
        
        //ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMODECTRL0_ADD     , 0x00800000U );
        reg_val = ucie_hwemu_read(ucie_axi_base + DWORD_1_DWMODECTRL0_ADD);
        reg_val = reg_val &       ~( DWORD_1_DWMODECTRL0_DWTXSCRAMBLEEN_MASK  << DWORD_1_DWMODECTRL0_DWTXSCRAMBLEEN_SHIFT);
        reg_val = reg_val &       ~( DWORD_1_DWMODECTRL0_DWRXSCRAMBLEEN_MASK  << DWORD_1_DWMODECTRL0_DWRXSCRAMBLEEN_SHIFT);
        reg_val = reg_val &       ~( DWORD_1_DWMODECTRL0_DWRXCTLCLKSEL_MASK   << DWORD_1_DWMODECTRL0_DWRXCTLCLKSEL_SHIFT);
        //reg_val = reg_val &       ~( DWORD_1_DWMODECTRL0_DWRXLATALIGN_MASK    << DWORD_1_DWMODECTRL0_DWRXLATALIGN_SHIFT);
        reg_val = reg_val | ((0x0U & DWORD_1_DWMODECTRL0_DWTXSCRAMBLEEN_MASK) << DWORD_1_DWMODECTRL0_DWTXSCRAMBLEEN_SHIFT);
        reg_val = reg_val | ((0x0U & DWORD_1_DWMODECTRL0_DWRXSCRAMBLEEN_MASK) << DWORD_1_DWMODECTRL0_DWRXSCRAMBLEEN_SHIFT);
        reg_val = reg_val | ((0x1U & DWORD_1_DWMODECTRL0_DWRXCTLCLKSEL_MASK)  << DWORD_1_DWMODECTRL0_DWRXCTLCLKSEL_SHIFT);
        //reg_val = reg_val | ((0x1U & DWORD_1_DWMODECTRL0_DWRXLATALIGN_MASK)   << DWORD_1_DWMODECTRL0_DWRXLATALIGN_SHIFT);
        ucie_hwemu_write( ucie_axi_base +  DWORD_1_DWMODECTRL0_ADD, reg_val );
        
        ucie_hwemu_write( ucie_axi_base + DWORD_0_DWVREFVAR_ADD       , 0x0080007EU );
        ucie_hwemu_write( ucie_axi_base + DWORD_1_DWVREFVAR_ADD       , 0x0080007EU );
        ucie_hwemu_write( ucie_axi_base + ACSMIM_ACSMINSTRREG25_ADD   , 0x83000061U );
#if 0 /* <ANH001> from ucie_0624_1: not use any more */
        ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMISCCTRL0_ADD     , 0x3d00a001U );
        ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMISCCTRL0_ADD     , 0x3d00a001U );
#endif
        reg_val = ucie_hwemu_read(ucie_axi_base + DWORD_0_DWMISCCTRL0_ADD);
        reg_val = reg_val & DWORD_0_DWMISCCTRL0_MASK;
        reg_val = reg_val &      ~( DWORD_0_DWMISCCTRL0_DWTXCKPARKLEVEL_MASK  << DWORD_0_DWMISCCTRL0_DWTXCKPARKLEVEL_SHIFT);
        reg_val = reg_val | ((0x1U & DWORD_0_DWMISCCTRL0_DWTXCKPARKLEVEL_MASK) << DWORD_0_DWMISCCTRL0_DWTXCKPARKLEVEL_SHIFT);
        ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMISCCTRL0_ADD, (reg_val|(1U)) );
    
        reg_val = ucie_hwemu_read(ucie_axi_base + DWORD_1_DWMISCCTRL0_ADD);
        reg_val = reg_val & DWORD_1_DWMISCCTRL0_MASK;
        reg_val = reg_val &      ~( DWORD_1_DWMISCCTRL0_DWTXCKPARKLEVEL_MASK  << DWORD_1_DWMISCCTRL0_DWTXCKPARKLEVEL_SHIFT);
        reg_val = reg_val | ((0x1U & DWORD_1_DWMISCCTRL0_DWTXCKPARKLEVEL_MASK) << DWORD_1_DWMISCCTRL0_DWTXCKPARKLEVEL_SHIFT);
        ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMISCCTRL0_ADD, (reg_val|(1U)) );
    }
#endif /* #if defined(RCAR_UCIE_V101) || defined(RCAR_UCIE_V102) */

#ifdef UCIE_SETUP_MODE_TIMEOUTOFF
    // 8ms Timeout off
    printf("TIMEOUTOFF\n");
    ucie_hwemu_write( ucie_axi_base + ACSM_ACSMTIMEOUTCTRL1_ADD, 0x00000000U );
#endif /* ifdef UCIE_SETUP_MODE_TIMEOUTOFF */

#ifdef UCIE_SETUP_MODE_HOLDLTSSM
    printf("RELEASE HOLDLTSSM\n");
    //UCIERSTCTRL1.app_ltssm_enable=0 : enable ltssm state transition
    ucie_hwemu_write( ucie_apb_base+0xE00014, 0x00000000U );
#endif /* ifdef UCIE_SETUP_MODE_HOLDLTSSM */
}
//---<ANH001>
/* -----------------------------------------------------------------------------
 * End of function Ucie_Setup_Pre
 * -------------------------------------------------------------------------- */

/*******************************************************
 * NAME        : Ucie_Setup_Start_Linkup
 * FUNCTION    : Start UCIE link-up
 * INPUT       : ucie_ch: ucie0/1
 *               device_type: rc/ep
 * OUTPUT      : none
 * RETURN      : none
 ******************************************************/
static void Ucie_Setup_Start_Linkup(uint32_t ucie_ch, uint32_t device_type)
{
    uint32_t ucie_axi_base;
    uint32_t ucie_apb_base;
    uint32_t reg_val;

    // if ( UCIE_CH0 == ucie_ch )
    // {
    //     ucie_axi_base=0xD8000000U;
    //     ucie_apb_base=0xDC000000U;
    // }
    // else
    // {
    //     ucie_axi_base=0xD9000000U;
    //     ucie_apb_base=0xDD000000U;
    // }
    ucie_apb_base = UCIE_APB_BASE(ucie_ch); 
    ucie_axi_base = UCIE_AXI_BASE(ucie_ch);

#ifndef DIRECT_ADR
    /* axi0 addres OFF */
	ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00000000U );
#endif /* DIRECT_ADR */

    ucie_hwemu_write( ucie_axi_base + DVSEC_UNIT_DSP_DVSEC_UCIE_LINK_CONTROL_ADD, 0x000040C0U );

    if ( MODE_RC == device_type )
    {
        ucie_hwemu_write( ucie_axi_base + DVSEC_UNIT_DSP_DVSEC_UCIE_LINK_CONTROL_ADD, 0x000044C0U );
    }
    else
    {
        /* EP: setup start from SBINIT is on*/
#ifndef DIRECT_ADR
        ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00004141U );
#endif /* DIRECT_ADR */

        reg_val = ucie_hwemu_read(ucie_axi_base + ACSM_ACSMMISCCTRL_ADD);
        reg_val = reg_val & ACSM_ACSMMISCCTRL_MASK;
        reg_val = reg_val | ((0x7U & ACSM_ACSMMISCCTRL_ACSMLTSMTRIGGEREN_MASK) << ACSM_ACSMMISCCTRL_ACSMLTSMTRIGGEREN_SHIFT);
        ucie_hwemu_write( ucie_axi_base + ACSM_ACSMMISCCTRL_ADD, reg_val );

#ifndef DIRECT_ADR
        ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00000000U );
#endif /* DIRECT_ADR */
    }

}
/* -----------------------------------------------------------------------------
 * End of function Ucie_Setup_Start_Linkup
 * -------------------------------------------------------------------------- */

/*******************************************************
 * NAME        : Ucie_Setup_Wait_FreqChange_Req
 * FUNCTION    : Wait UCIE Freq Change Request
 * INPUT       : ucie_ch
 * OUTPUT      : none
 * RETURN      : NORMAL_END    Pass
 *             : ERROR_END     Fail
 ******************************************************/
//+++<ANHH001>
#if 0
static uint32_t Ucie_Setup_Wait_FreqChange_Req(uint32_t ucie_ch)
{
    uint32_t ret = NORMAL_END;
    uint32_t mask;
    uint32_t expect;
    uint32_t timeout;

    uint32_t ucie_axi_base;
    uint32_t ucie_apb_base;

    // if ( UCIE_CH0 == ucie_ch )
    // {
    //     ucie_axi_base=0xD8000000U;
    //     ucie_apb_base=0xDC000000U;
    // }
    // else
    // {
    //     ucie_axi_base=0xD9000000U;
    //     ucie_apb_base=0xDD000000U;
    // }
    ucie_apb_base = UCIE_APB_BASE(ucie_ch); 
    ucie_axi_base = UCIE_AXI_BASE(ucie_ch);

#ifndef DIRECT_ADR
    /* ;  axi0 addres ON */
    ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00004141U );
#endif /* DIRECT_ADR */

    printf("Wait_freqchange_req_end\n");
    mask    = 0x000002U;
    expect  = 0x000002U;
    timeout = 10000;
    while ((ucie_hwemu_read(ucie_apb_base + 0xE1003CU) & mask) != expect)
    {
        ucie_hwemu_read(ucie_axi_base + ACSM_ACSMLTSMSTATUS_ADD); /*  for debug */
#ifndef UCIE_SETUP_MODE_TIMEOUTOFF
        timeout--;
        if (0U == timeout)
        {
            printf("** ch%d TIMEOUT\n",ucie_ch);
            ret = ERROR_END;
            break;
        }
#endif
    }

    return ret;
}
#else
static uint32_t Ucie_Setup_Wait_FreqChange_Req(uint32_t ucie_ch, uint32_t device_type)
{
	uint32_t ret = NORMAL_END;
	uint32_t mask = 0x000002U;
	uint32_t expect = 0x000002U;
	uint32_t timeout = 0;

	uint32_t prev_state;
	uint32_t curr_state;

#ifndef DIRECT_ADR
	//; axi adr = 41
	ucie_hwemu_write(UCIE_APB_BASE(ucie_ch) + 0xE005E8, 0x00004141 );
#endif /* DIRECT_ADR */

	prev_state=0xffffffff;

	while ((ucie_hwemu_read(UCIE_APB_BASE(ucie_ch) + 0xE1003C) & mask) != expect)
	{
		// ucie_hwemu_read(UCIE_AXI_BASE(ucie_ch) + ACSM_ACSMLTSMSTATUS_ADD); // for debug
		curr_state = ucie_hwemu_read(UCIE_AXI_BASE(ucie_ch) + ACSM_ACSMLTSMSTATUS_ADD); // for debug
		if (prev_state != curr_state)
		{
			printf("curr_state %08x\n", curr_state);
		}
		if ((curr_state & 0x1F) == 0x1C)
		{
			printf("** ch%d TRAIN_ERROR\n", ucie_ch);
			ret = ERROR_END;
			return ret;
		}

		prev_state = curr_state;

//+++<ANHH001>
        if (MODE_RC == device_type)
        {
            if (timeout >= TIMEOUT_EXECUTE_LOOP_COUNT) 
            {
                printf("** Ch%d TIMEOUT **\n", ucie_ch);
                ret = ERROR_END;
                break;
            }
        }
        if ((timeout % 10000U) == 0) 
        {  // Print dot every 10000 iterations
            printf(". ");
        }
		timeout++;
//---<ANHH001>
	}

	return ret;
}
#endif
//---<ANHH001>
/* -----------------------------------------------------------------------------
 * End of function Ucie_Setup_Wait_FreqChange_Req
 * -------------------------------------------------------------------------- */

/*******************************************************
 * NAME        : Ucie_Setup_Ack_FreqChange
 * FUNCTION    : UCIE Freq Change Acknowledge
 * INPUT       : ucie_ch
 * OUTPUT      : none
 * RETURN      : none
 ******************************************************/
static void Ucie_Setup_Ack_FreqChange(uint32_t ucie_ch)
{
    // uint32_t ucie_apb_base;

    // if ( UCIE_CH0 == ucie_ch )
    // {
    //     ucie_apb_base=0xDC000000U;
    // }
    // else
    // {
    //     ucie_apb_base=0xDD000000U;
    // }

#ifdef START_INPUT_CLK_500MHZ
#ifdef CHANGED_INPUT_CLK_2000MHZ
    Ucie_clkcnt(ucie_ch, UCIE_CLK_FAST);
#endif
#endif

    ucie_hwemu_write( UCIE_APB_BASE(ucie_ch) + 0xE21004U, 0x00000001U ); /*  ack=1 -> req will negate after 1clk cycle */
    ucie_hwemu_write( UCIE_APB_BASE(ucie_ch) + 0xE21004U, 0x00000000U ); /*  ack=0 */

}
/* -----------------------------------------------------------------------------
 * End of function Ucie_Setup_Ack_FreqChange
 * -------------------------------------------------------------------------- */

/*******************************************************
 * NAME        : Ucie_Setup_Wait_Linkup
 * FUNCTION    : Wait UCIE linkup confirmation
 * INPUT       : ucie_ch
 * OUTPUT      : none
 * RETURN      : NORMAL_END    Pass
 *             : ERROR_END        Fail
 ******************************************************/
static uint32_t Ucie_Setup_Wait_Linkup(uint32_t ucie_ch, uint32_t device_type)
{
    uint32_t ret = NORMAL_END;
    uint32_t mask = 0x0000001FU;
    uint32_t state = 0x00000000U;
	uint32_t timeout = 0U;

    // uint32_t ucie_axi_base;
    // uint32_t ucie_apb_base;

    // if ( UCIE_CH0 == ucie_ch )
    // {
    //     ucie_axi_base=0xD8000000U;
    //     ucie_apb_base=0xDC000000U;
    // }
    // else
    // {
    //     ucie_axi_base=0xD9000000U;
    //     ucie_apb_base=0xDD000000U;
    // }

#ifndef DIRECT_ADR
    /* ;  axi0 addres ON */
    ucie_hwemu_write( UCIE_APB_BASE(ucie_ch) + 0xE005E8U, 0x00004141U );
#endif /* DIRECT_ADR */

    printf("Wait_linkup_end\n");

    while ( ( UCIE_STATE_ACTIVE     != state )
         && ( UCIE_STATE_TRAINERROR != state ) )
    {
        state = (ucie_hwemu_read(UCIE_AXI_BASE(ucie_ch) + ACSM_ACSMLTSMSTATUS_ADD) & mask);
        printf("state = %08x\n", state);

//+++<ANHH001>
        if (MODE_RC == device_type)
        {
            if (timeout >= TIMEOUT_EXECUTE_LOOP_COUNT) 
            {
                printf("** Ch%d TIMEOUT **\n", ucie_ch);
                ret = ERROR_END;
                break;
            }
        }
        if ((timeout % 10000U) == 0) 
		{  // Print dot every 10000 iterations
            printf(". ");
        }
		timeout++;
//---<ANHH001>
    }

    if (UCIE_STATE_TRAINERROR == state)
    {
        ret = ERROR_END;
    }

#ifdef UCIE_SETUP_MODE_TIMEOUTON_L_UP
    // 8ms Timeout is enabled as default value
    printf("RE-ENABLE TIMEOUT\n");
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_ch) + ACSM_ACSMTIMEOUTCTRL1_ADD, ACSM_ACSMTIMEOUTCTRL1_RESET );
#endif /* UCIE_SETUP_MODE_TIMEOUTON_L_UP */

    printf("!\n");        /*  need wait */
    return ret;
}
/* -----------------------------------------------------------------------------
 * End of function Ucie_Setup_Wait_Linkup
 * -------------------------------------------------------------------------- */

/*******************************************************
 * NAME        : Ucie_Setup_PCIE_Pre
 * FUNCTION    : Setup PCIE pre-linkup
 * INPUT       : ucie_ch, device_type
 * OUTPUT      : none
 * RETURN      : none
 ******************************************************/
static void Ucie_Setup_PCIE_Pre(uint32_t ucie_ch, uint32_t device_type)
{
    uint32_t ucie_axi_base;
    uint32_t ucie_apb_base;

    // if ( UCIE_CH0 == ucie_ch )
    // {
    //     ucie_axi_base=0xD8000000U;
    //     ucie_apb_base=0xDC000000U;
    // }
    // else
    // {
    //     ucie_axi_base=0xD9000000U;
    //     ucie_apb_base=0xDD000000U;
    // }
    ucie_apb_base = UCIE_APB_BASE(ucie_ch); 
    ucie_axi_base = UCIE_AXI_BASE(ucie_ch);

    /* ; After linkup */
#ifndef DIRECT_ADR
    /* ;  axi0 addres ON */
    ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00004141U );
#endif /* DIRECT_ADR */

    /* ; ULT_DWPUBMODECTRL */
    ucie_hwemu_write( ucie_axi_base + DWORD_0_DWPUBMODECTRL_ADD, 0x00000000U );

#if 0 /* <ANH001> from ucie_0624_1: not use any more */
    /* ; ULT_DWTXSBINIT */
    /* ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMISCCTRL0_ADD, 0x3D00A400U ); */
    ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMISCCTRL0_ADD, 0x3D002400U ); /*  [15]=0 */

    /* ; ULT_DWRXSBINIT */
    /* # ifndef RCAR_UCIE_V101 */
    /* #if 0 */
    /* RCAR_UCIE_V100 */
    /* ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMISCCTRL0_ADD, 0x3D04A400U ); */
    /* #else */
    /* RCAR_UCIE_V101 */
    /* ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMISCCTRL0_ADD, 0x3D00A400U ); */
    /* #endif */
    ucie_hwemu_write( ucie_axi_base + DWORD_0_DWMISCCTRL0_ADD, 0x3D002400U ); /*  [18:17]=0 */
#endif
    /* ; ULT_DWPUBMODECTRL_DW1 */
    ucie_hwemu_write( ucie_axi_base + DWORD_1_DWPUBMODECTRL_ADD, 0x00000000U );

#if 0 /* <ANH001> from ucie_0624_1: not use any more */
    /* ; ULT_DWTXSBINIT_DW1 */
    /* ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMISCCTRL0_ADD, 0x3D00A400U ); */
    ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMISCCTRL0_ADD, 0x3D002400U ); /*  [15]=0 */

    /* ; ULT_DWRXSBINIT_DW1 */
    /* # ifndef RCAR_UCIE_V101 */
    /* #if 0 */
    /* RCAR_UCIE_V100 */
    /* ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMISCCTRL0_ADD, 0x3D04A400U ); */
    /* #else */
    /* RCAR_UCIE_V101 */
    /* ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMISCCTRL0_ADD, 0x3D00A400U ); */
    /* #endif */
    ucie_hwemu_write( ucie_axi_base + DWORD_1_DWMISCCTRL0_ADD, 0x3D002400U ); /*  [18:17]=0 */
#endif

    /* ; UCIEDBIADR */
#ifndef DIRECT_ADR
    ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00000000U );
#endif /* DIRECT_ADR */

    /* delete GEN2 GEN3

    if ( UCIE_CH0 == device_type )
    {
        //; GEN2_CTRL_OFF
        ucie_hwemu_write( ucie_axi_base+0x00080CU, 0xB012080FU );
    }
    else
    {
        //; GEN2_CTRL_OFF
        ucie_hwemu_write( ucie_axi_base+0x00080CU, 0xB010080FU );
    }

    //; GEN3_RELATED_OFF
    ucie_hwemu_write( ucie_axi_base+0x000890U, 0x00002000U );

    //; GEN3_RELATED_OFF
    ucie_hwemu_write( ucie_axi_base+0x000890U, 0x0000A800U );
    */

    if ( MODE_RC == device_type )
    {
        /* ; SPCIE_CAP_OFF_0CH_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_SPCIE_CAP_SPCIE_CAP_OFF_0CH_REG_ADD, 0x37300127U );
        /* ; SPCIE_CAP_OFF_10H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_SPCIE_CAP_SPCIE_CAP_OFF_10H_REG_ADD, 0x69144272U );
        /* ; SPCIE_CAP_OFF_14H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_SPCIE_CAP_SPCIE_CAP_OFF_14H_REG_ADD, 0x12474905U );
        /* ; SPCIE_CAP_OFF_18H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_SPCIE_CAP_SPCIE_CAP_OFF_18H_REG_ADD, 0x5A055370U );
    }
    else
    {
        /* ; SPCIE_CAP_OFF_0CH_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_SPCIE_CAP_SPCIE_CAP_OFF_0CH_REG_ADD, 0x2228174AU );
        /* ; SPCIE_CAP_OFF_10H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_SPCIE_CAP_SPCIE_CAP_OFF_10H_REG_ADD, 0x78540719U );
        /* ; SPCIE_CAP_OFF_14H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_SPCIE_CAP_SPCIE_CAP_OFF_14H_REG_ADD, 0x03630375U );
        /* ; SPCIE_CAP_OFF_18H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_SPCIE_CAP_SPCIE_CAP_OFF_18H_REG_ADD, 0x31522410U );
    }


    /* delete GEN3

    //; GEN3_EQ_LOCAL_FS_LF_OFF
    ucie_hwemu_write( ucie_axi_base+0x000894U, 0x00000C10U );

    //; GEN3_EQ_PSET_INDEX_OFF
    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000000U );

    //; GEN3_EQ_PSET_COEF_MAP__0
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x0000C900U );

    //; GEN3_EQ_PSET_INDEX_OFF
    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000001U );

    //; GEN3_EQ_PSET_COEF_MAP__0
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00008A00U );

    //; GEN3_EQ_PSET_INDEX_OFF
    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000002U );

    //; GEN3_EQ_PSET_COEF_MAP__0
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x0000A980U );

    //; GEN3_EQ_PSET_INDEX_OFF
    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000003U );

    //; GEN3_EQ_PSET_COEF_MAP__0
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00006A80U );

    //; GEN3_EQ_PSET_INDEX_OFF
    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000004U );

    //; GEN3_EQ_PSET_COEF_MAP__0
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00000C00U );

    //; GEN3_EQ_PSET_INDEX_OFF
    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000005U );

    //; GEN3_EQ_PSET_COEF_MAP__0
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00000AC5U );

    //; GEN3_EQ_PSET_INDEX_OFF
    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000006U );

    //; GEN3_EQ_PSET_COEF_MAP__0
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00000A86U );

    //; GEN3_EQ_PSET_INDEX_OFF
    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000007U );

    //; GEN3_EQ_PSET_COEF_MAP__0
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x0000A845U );

    //; GEN3_EQ_PSET_INDEX_OFF
    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000008U );

    //; GEN3_EQ_PSET_COEF_MAP__0
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00006906U );

    //; GEN3_EQ_PSET_INDEX_OFF
    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000009U );

    //; GEN3_EQ_PSET_COEF_MAP__0
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00000A08U );


    //; GEN3_EQ_PSET_INDEX_OFF
    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x0000000AU );

    //; GEN3_EQ_PSET_COEF_MAP__0
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00010800U );

    //; GEN3_EQ_FB_MODE_DIR_CHANGE_OFF
    ucie_hwemu_write( ucie_axi_base+0x0008ACU, 0x00000000U );

    if ( UCIE_CH0 == device_type )
    {
        //; GEN3_EQ_CONTROL_OFF
        ucie_hwemu_write( ucie_axi_base+0x0008A8U, 0x0C018A61U );
    }
    else
    {
        //; GEN3_EQ_CONTROL_OFF
        ucie_hwemu_write( ucie_axi_base+0x0008A8U, 0x0D050061U );
    }

    //; GEN3_RELATED_OFF
    ucie_hwemu_write( ucie_axi_base+0x000890U, 0x0100A800U );

    //; GEN3_RELATED_OFF
    ucie_hwemu_write( ucie_axi_base+0x000890U, 0x0100A800U );

    //;
    ucie_hwemu_write( ucie_axi_base+0x000890U, 0x0140A800U );

    //;
    ucie_hwemu_write( ucie_axi_base+0x000890U, 0x0140A800U );

    */

    if ( MODE_RC == device_type )
    {
        /* ;PL16G_CAP_OFF_20H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_PL16G_CAP_PL16G_CAP_OFF_20H_REG_ADD, 0x8584443AU );
        /* ;PL16G_CAP_OFF_24H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_PL16G_CAP_PL16G_CAP_OFF_24H_REG_ADD, 0x6631482AU );
    }
    else
    {
        /* ;PL16G_CAP_OFF_20H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_PL16G_CAP_PL16G_CAP_OFF_20H_REG_ADD, 0x390A0826U );
        /* ;PL16G_CAP_OFF_24H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_PL16G_CAP_PL16G_CAP_OFF_24H_REG_ADD, 0x76145138U );
    }

    /* delete GEN3

    ucie_hwemu_write( ucie_axi_base+0x000894U, 0x00000C10U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000000U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x0000C900U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000001U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00008A00U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000002U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x0000A980U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000003U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00006A80U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000004U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00000C00U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000005U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00000AC5U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000006U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00000A86U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000007U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x0000A845U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000008U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00006906U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000009U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00000A08U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x0000000AU );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00010800U );

    ucie_hwemu_write( ucie_axi_base+0x0008ACU, 0x00000000U );

    if ( UCIE_CH0 == device_type )
    {
        ucie_hwemu_write( ucie_axi_base+0x0008A8U, 0x04008521U );
    }
    else
    {
        ucie_hwemu_write( ucie_axi_base+0x0008A8U, 0x04018921U );
    }

    ucie_hwemu_write( ucie_axi_base+0x000890U, 0x0240A800U );

    ucie_hwemu_write( ucie_axi_base+0x000890U, 0x0200A800U );

    */

    if ( MODE_RC == device_type )
    {
        /* ;    PL32G_CAP_OFF_20H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_PL32G_CAP_PL32G_CAP_OFF_20H_REG_ADD, 0x915A4145U );
        /* ;PL32G_CAP_OFF_24H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_PL32G_CAP_PL32G_CAP_OFF_24H_REG_ADD, 0x81817220U );
    }
    else
    {
        /* ; */
        ucie_hwemu_write( ucie_axi_base + PF0_PL32G_CAP_PL32G_CAP_OFF_20H_REG_ADD, 0x00860582U );
        /* ;PL32G_CAP_OFF_24H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_PL32G_CAP_PL32G_CAP_OFF_24H_REG_ADD, 0x96558A61U );
    }

    /* delete GEN3

    ucie_hwemu_write( ucie_axi_base+0x000894U, 0x00000C10U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000000U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x0000C900U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000001U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00008A00U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000002U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x0000A980U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000003U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00006A80U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000004U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00000C00U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000005U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00000AC5U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000006U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00000A86U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000007U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x0000A845U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000008U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00006906U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000009U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00000A08U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x0000000AU );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00010800U );

    ucie_hwemu_write( ucie_axi_base+0x0008ACU, 0x00000000U );

    if ( UCIE_CH0 == device_type )
    {
        ucie_hwemu_write( ucie_axi_base+0x0008A8U, 0x04008421U );
    }
    else
    {
        ucie_hwemu_write( ucie_axi_base+0x0008A8U, 0x05009321U );
    }

    ucie_hwemu_write( ucie_axi_base+0x000890U, 0x0300A800U );

    ucie_hwemu_write( ucie_axi_base+0x000890U, 0x0300A800U );

    */

    if ( MODE_RC == device_type )
    {
        /* ;PL64G_LANE_EQ_10H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_PL64G_CAP_PL64G_LANE_EQ_10H_REG_ADD, 0xA3057527U );
        /* ;PL64G_LANE_EQ_14H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_PL64G_CAP_PL64G_LANE_EQ_14H_REG_ADD, 0x09548A39U );
    }
    else
    {
        /* ;PL64G_LANE_EQ_10H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_PL64G_CAP_PL64G_LANE_EQ_10H_REG_ADD, 0x1A226372U );
        /* ;PL64G_LANE_EQ_14H_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_PL64G_CAP_PL64G_LANE_EQ_14H_REG_ADD, 0x01674946U );
    }

    /* delete GEN3

    ucie_hwemu_write( ucie_axi_base+0x000894U, 0x00000C10U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000000U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00000C00U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000001U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00000B04U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000002U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00000A08U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000003U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00004B00U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000004U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00008A00U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000005U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x0008090AU );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000006U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00086886U );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000007U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x0010088AU );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000008U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x0010080CU );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x00000009U );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x0010278CU );

    ucie_hwemu_write( ucie_axi_base+0x00089CU, 0x0000000AU );
    ucie_hwemu_write( ucie_axi_base+0x000898U, 0x00010800U );

    ucie_hwemu_write( ucie_axi_base+0x0008ACU, 0x00000000U );

    if ( UCIE_CH0 == device_type )
    {
        ucie_hwemu_write( ucie_axi_base+0x0008A8U, 0x05009221U );
    }
    else
    {
        ucie_hwemu_write( ucie_axi_base+0x0008A8U, 0x04048E21U );
    }

    */
}
/* -----------------------------------------------------------------------------
 * End of function Ucie_Setup_PCIE_Pre
 * -------------------------------------------------------------------------- */

/*******************************************************
 * NAME        : Ucie_Setup_PCIE_Start_LinkUp
 * FUNCTION    : Start PCIE link up
 * INPUT       : ucie_ch, device_type
 * OUTPUT      : none
 * RETURN      : none
 ******************************************************/
static void Ucie_Setup_PCIE_Start_LinkUp(uint32_t ucie_ch, uint32_t device_type)
{
    // uint32_t ucie_apb_base;

    // if ( UCIE_CH0 == ucie_ch )
    // {
    //     ucie_apb_base=0xDC000000U;
    // }
    // else
    // {
    //     ucie_apb_base=0xDD000000U;
    // }

#ifdef RCAR_UCIE_V100
    printf("PCIE start link up on X5H 0x%x = 0x%08x \n", ucie_hwemu_read(UCIE_APB_BASE(ucie_ch)+0xE10004U));/* temporal for debug */
#else /* RCAR_UCIE_V102 */
    printf("PCIE start link up on AI_ACC 0x%x = 0x%08x \n", ucie_hwemu_read(UCIE_APB_BASE(ucie_ch)+0xE10010U));/* temporal for debug */
#endif /* RCAR_UCIE_V100 */

    /* ;UCIERSTCTRL1 */
    ucie_hwemu_write( UCIE_APB_BASE(ucie_ch)+0xE00014U, 0x00010001U );

    if ( MODE_RC == device_type )
    {
        /* ;UCIEPCR00 */
        ucie_hwemu_write( UCIE_APB_BASE(ucie_ch)+0xE21000U, 0x00000011U );
    }
    else
    {
        /* ;UCIEPCR00 */
        ucie_hwemu_write( UCIE_APB_BASE(ucie_ch)+0xE21000U, 0x00000012U );
    }

}
/* -----------------------------------------------------------------------------
 * End of function Ucie_Setup_PCIE_Start_LinkUp
 * -------------------------------------------------------------------------- */

/*******************************************************
 * NAME        : Ucie_Setup_PCIE_Wait_LinkUp
 * FUNCTION    : WAit PCIE link up confirmation
 * INPUT       : ucie_ch
 * OUTPUT      : none
 * RETURN      : none
 ******************************************************/
static uint32_t Ucie_Setup_PCIE_Wait_LinkUp(uint32_t ucie_ch, uint32_t device_type)
{
    uint32_t ret = NORMAL_END;
    // uint32_t ucie_apb_base;
    uint32_t val = 0U;
    uint32_t prev = 0xffffffffU; /* temporal for debug */
    uint32_t mask = 0x000010U;
    uint32_t expect = 0x000010U;
    uint32_t timeout = 0U;

    // if ( UCIE_CH0 == ucie_ch )
    // {
    //     ucie_apb_base=0xDC000000U;
    // }
    // else
    // {
    //     ucie_apb_base=0xDD000000U;
    // }

    /*
    PCIE link up:wait rdlh_link bit ([4])
    */
    printf("Wait rdlh_link_up\n");
    while ((val & mask) != expect)
    {
        /* ;UCIEICR00b */
#ifdef RCAR_UCIE_V100
        val = ucie_hwemu_read(UCIE_APB_BASE(ucie_ch)+0xE10004U);
#else /* RCAR_UCIE_V102 */
        val = ucie_hwemu_read(UCIE_APB_BASE(ucie_ch)+0xE10010U);
#endif /* RCAR_UCIE_V100 */
        if (val != prev)
        {
            printf("%08x\n",val);/* temporal for debug */
            prev = val;
        }

//+++<ANHH001>
        if (MODE_RC == device_type)
        {
            if (timeout >= TIMEOUT_EXECUTE_LOOP_COUNT) 
            {
                printf("** Ch%d TIMEOUT **\n", ucie_ch);
                ret = ERROR_END;
                break;
            }
        }
        if ((timeout % 10000U) == 0) 
		{  // Print dot every 10000 iterations
            printf(". ");
        }
		timeout++;
//---<ANHH001>

    }
    printf("!\n");        /*  need wait */
    return ret;
}
/* -----------------------------------------------------------------------------
 * End of function Ucie_Setup_PCIE_Wait_LinkUp
 * -------------------------------------------------------------------------- */

/*******************************************************
 * NAME        : Ucie_Setup_PCIE_Post
 * FUNCTION    : Setup PCIE post link-up
 * INPUT    : none
 * OUTPUT    : none
 * RETURN    : NORMAL_END    Pass
 *            : ERROR_END        Fail
 ******************************************************/
static void Ucie_Setup_PCIE_Post(uint32_t ucie_ch, uint32_t device_type)
{
    uint32_t ucie_axi_base;
    uint32_t ucie_apb_base;

    // if ( UCIE_CH0 == ucie_ch )
    // {
    //     ucie_axi_base=0xD8000000U;
    //     ucie_apb_base=0xDC000000U;
    // }
    // else
    // {
    //     ucie_axi_base=0xD9000000U;
    //     ucie_apb_base=0xDD000000U;
    // }
    ucie_apb_base = UCIE_APB_BASE(ucie_ch); 
    ucie_axi_base = UCIE_AXI_BASE(ucie_ch);

#ifndef DIRECT_ADR
    /* ;  axi0 addres ON */
    ucie_hwemu_write( ucie_apb_base+0xE005E8U, 0x00000000U );
#endif /* DIRECT_ADR */

    /* ;DEVICE_CONTROL_DEVICE_STATUS */
    ucie_hwemu_write( ucie_axi_base + PF0_PCIE_CAP_DEVICE_CONTROL_DEVICE_STATUS_ADD, 0x00102150U );

    if ( MODE_RC != device_type )
    {
        ucie_hwemu_write( ucie_axi_base + PF0_PCIE_CAP_DEVICE_CONTROL_DEVICE_STATUS_ADD + RCAR_UCIE_FN_OFS(1), 0x00102150U );
    }

    if ( MODE_RC == device_type )
    {
        /* ;BAR0_MASK_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_BAR0_REG_ADD + RCAR_UCIE_FN_OFS(1), 0x00000000U );
        /* ;BAR1_MASK_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_BAR1_REG_ADD + RCAR_UCIE_FN_OFS(1), 0x00000000U );

        /* ;BAR0_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_BAR0_REG_ADD, 0x09100004U );
        /* ;BAR1_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_BAR1_REG_ADD, 0x00000000U );
        /* ;TRGT_MAP_CTRL_OFF */
        ucie_hwemu_write( ucie_axi_base + PF0_PORT_LOGIC_TRGT_MAP_CTRL_OFF_ADD, 0x00000040U );
        /* ;SEC_STAT_IO_LIMIT_IO_BASE_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_SEC_STAT_IO_LIMIT_IO_BASE_REG_ADD, 0x00000101U );
        /* ;IO_LIMIT_UPPER_IO_BASE_UPPER_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_ADD, 0x00000000U );
        /* ;SEC_STAT_IO_LIMIT_IO_BASE_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_SEC_STAT_IO_LIMIT_IO_BASE_REG_ADD, 0x00004F40U );
        /* ;MEM_LIMIT_MEM_BASE_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_MEM_LIMIT_MEM_BASE_REG_ADD, 0x08FF08C0U );
        /* ;PREF_MEM_LIMIT_PREF_MEM_BASE_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_ADD, 0x00010001U );
        /* ;PREF_BASE_UPPER_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_PREF_BASE_UPPER_REG_ADD, 0x00000000U );
        /* ;PREF_LIMIT_UPPER_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_PREF_LIMIT_UPPER_REG_ADD, 0x00000000U );
        /* ;PREF_MEM_LIMIT_PREF_MEM_BASE_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_ADD, 0xB00FB000U );
        /* :DATA_LINK_FEATURE_EXT_HDR_OFF -> PF0_CXL_2_0_EXT_CAP_CXL_2_0_ALT_MEM_BASE_LIMIT_OFF_ADD */
        ucie_hwemu_write( ucie_axi_base + PF0_CXL_2_0_EXT_CAP_CXL_2_0_ALT_MEM_BASE_LIMIT_OFF_ADD, 0xAFEF0930U );
        /* ;DATA_LINK_FEATURE_CAP_OFF -> PF0_CXL_2_0_EXT_CAP_CXL_2_0_ALT_PREFETCH_MEM_BASE_LIMIT_OFF_ADD */
        ucie_hwemu_write( ucie_axi_base + PF0_CXL_2_0_EXT_CAP_CXL_2_0_ALT_PREFETCH_MEM_BASE_LIMIT_OFF_ADD, 0x00000000U );

        /* ;RASDP_ERROR_TRACER_INJ_CTRL_OFF -> PF0_CXL_2_0_EXT_CAP_CXL_2_0_CTRL_ALT_BUS_BASE_LIMIT_OFF_ADD */
        ucie_hwemu_write( ucie_axi_base + PF0_CXL_2_0_EXT_CAP_CXL_2_0_CTRL_ALT_BUS_BASE_LIMIT_OFF_ADD, 0x00000000U );
        /* ;RASDP_ERROR_TRACER_INJ_CTRL_OFF -> PF0_CXL_2_0_EXT_CAP_CXL_2_0_CTRL_ALT_BUS_BASE_LIMIT_OFF_ADD */
        ucie_hwemu_write( ucie_axi_base + PF0_CXL_2_0_EXT_CAP_CXL_2_0_CTRL_ALT_BUS_BASE_LIMIT_OFF_ADD, 0x00000000U );
        /* ;RASDP_ERROR_TRACER_INJ_CTRL_OFF -> PF0_CXL_2_0_EXT_CAP_CXL_2_0_CTRL_ALT_BUS_BASE_LIMIT_OFF_ADD */
        ucie_hwemu_write( ucie_axi_base + PF0_CXL_2_0_EXT_CAP_CXL_2_0_CTRL_ALT_BUS_BASE_LIMIT_OFF_ADD, 0x00050000U );
        /* ;RASDP_ERROR_TRACER_INJ_CTRL_OFF -> PF0_CXL_2_0_EXT_CAP_CXL_2_0_CTRL_ALT_BUS_BASE_LIMIT_OFF_ADD */
        ucie_hwemu_write( ucie_axi_base + PF0_CXL_2_0_EXT_CAP_CXL_2_0_CTRL_ALT_BUS_BASE_LIMIT_OFF_ADD, 0x6D050000U );
    }
    else
    {
        /* ;BAR0_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_BAR0_REG_ADD, 0x08C00004U );
        /* ;BAR1_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_BAR1_REG_ADD, 0x00000000U );
        /* ;BAR0_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_BAR0_REG_ADD + RCAR_UCIE_FN_OFS(1), 0x08E00004U );
        /* ;BAR1_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_BAR1_REG_ADD + RCAR_UCIE_FN_OFS(1), 0x00000000U );
        /* ;SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_ADD, 0x08D00000U );
        /* ; */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_ADD + RCAR_UCIE_FN_OFS(1), 0x08F00000U );
        /* ;MEM_LIMIT_MEM_BASE_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_MEM_LIMIT_MEM_BASE_REG_ADD, 0x00004001U );
        /* ; */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_MEM_LIMIT_MEM_BASE_REG_ADD + RCAR_UCIE_FN_OFS(1), 0x00004101U );
        /* ;IO_LIMIT_UPPER_IO_BASE_UPPER_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_ADD, 0xB0000001U );
        /* ;EXP_ROM_BASE_ADDR_REG */
        ucie_hwemu_write( ucie_axi_base + PF0_TYPE1_HDR_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_ADD + RCAR_UCIE_FN_OFS(1), 0xB0010001U );
        /* ;TRGT_MAP_CTRL_OFF */
        ucie_hwemu_write( ucie_axi_base + PF0_PORT_LOGIC_TRGT_MAP_CTRL_OFF_ADD, 0x00000054U );
        /* ;TRGT_MAP_CTRL_OFF */
        ucie_hwemu_write( ucie_axi_base + PF0_PORT_LOGIC_TRGT_MAP_CTRL_OFF_ADD + RCAR_UCIE_FN_OFS(1), 0x00010054U );
    }

}
/* -----------------------------------------------------------------------------
 * End of function Ucie_Setup_PCIE_Post
 * -------------------------------------------------------------------------- */

//+++<ANH001>
/*******************************************************
 * NAME		: wait_time
 * FUNCTION	: Delay function
 * INPUT	: none
 * OUTPUT	: none
 * RETURN	: none
 ******************************************************/
void wait_time(uint32_t count)
{
	if (count > 0)
	{
		printf("W:%d\n", count);
		vTaskDelay(pdMS_TO_TICKS(count));
		printf("!\n");
	}
}

/*******************************************************
 * NAME		: ucie_hwemu_hw_init_rc
 * FUNCTION	: Setup UCIE RC
 * INPUT	: uint32_t ucie_ch UCIE_CH0 / UCIE_CH1
 *            uint32_t device_type UCIE_RC / UCIE_EP
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
uint32_t ucie_hwemu_hw_init_rc(uint32_t ucie_ch, uint32_t device_type, uint32_t conn_ver)
{
	// ===== EP-specific initialization =====
	uint32_t ret = NORMAL_END;
	uint32_t timeout = 0;
	uint32_t *w;
	
	printf("[RC] HW_INIT (Ch%d) \n", ucie_ch);

	// w = (uint32_t *)(0xbffff100);
    // if (0 != w[0])
	// {
	// 	UCIE_SETUP_MODE = w[0];
	// }

	w = (uint32_t *)(0xbffff000);
	wait_time(w[0]); // delay 0ms

LABEL_RETRY:
// -----------------------------------------------------------------------------

	// Ucie_PwrOn(ucie_ch);
    Ucie_Setup_Reset(ucie_ch);

	// Step 1: UCIE Init Pre
	Ucie_Setup_Pre(ucie_ch, device_type, conn_ver);
	printf("[Step1] Ucie_Setup_Pre(%d, %d, %d)\n", ucie_ch, device_type, conn_ver);
	wait_time(w[1]); // delay 2ms
    wait_time(w[2]); // slot
    wait_time(w[3]); // slot
	wait_time(w[4]);

	// -----------------------------------------------------------------------------
	// Step 2: Start UCIE Link
	// ret = ucie_hwemu_start_link(ucie_ch, true);
    Ucie_Setup_Start_Linkup(ucie_ch, device_type);
    wait_time(w[5]);
	printf("[Step2] start_link(%d, %d)\n", ucie_ch, device_type);

	// -----------------------------------------------------------------------------
	// Step 3: Wait for frequency change request
	// ret = ucie_hwemu_setup_wait_freqchange_req(ucie_ch, true);
    ret = Ucie_Setup_Wait_FreqChange_Req(ucie_ch, device_type);
	printf("[Step3] freqchange_req(%d) returned(%d)\n", ucie_ch, ret);
	if (NORMAL_END != ret)
	{
		goto LABEL_ERROR;
	}

	// -----------------------------------------------------------------------------
	// Step 4: Acknowledge freq change
	// ret = ucie_hwemu_setup_ack_freqchange(ucie_ch, true);
    Ucie_Setup_Ack_FreqChange(ucie_ch);
	printf("[Step4] ack_freqchange(%d)\n", ucie_ch);

	// -----------------------------------------------------------------------------
	// Step 5: Wait for link-up
	// ret = ucie_hwemu_wait_linkup(ucie_ch, true);
    ret = Ucie_Setup_Wait_Linkup(ucie_ch, device_type);
    wait_time(w[6]);
	printf("[Step5] wait_linkup(%d) returned(%d)\n", ucie_ch, ret);
	if (NORMAL_END != ret)
	{
		goto LABEL_ERROR;
	}

	// -----------------------------------------------------------------------------
	// Step 6: PCIe initialization
	// ucie_hwemu_pcie_init_pre(ucie_ch, true);
    Ucie_Setup_PCIE_Pre(ucie_ch, device_type);
	wait_time(w[7]);
	printf("[Step6] pcie_init_pre(%d, %d)\n", ucie_ch, device_type);

	// -----------------------------------------------------------------------------
	// Step 7: Start PCIe link
	// ucie_hwemu_pcie_start_link(ucie_ch, true);
    Ucie_Setup_PCIE_Start_LinkUp(ucie_ch, device_type);
	// wait_time(w[7]);
	printf("[Step7] pcie_start_link(%d, %d)\n", ucie_ch, device_type);

	// -----------------------------------------------------------------------------
	// Step 8: Wait for PCIe link-up
	// ret = ucie_hwemu_pcie_wait_linkup(ucie_ch, true);
    ret = Ucie_Setup_PCIE_Wait_LinkUp(ucie_ch, device_type);
	printf("[Step8] pcie_wait_linkup(%d) returned(%d)\n", ucie_ch, ret);
	if (NORMAL_END != ret)
	{
		goto LABEL_ERROR;
	}

	// -----------------------------------------------------------------------------
	// Step 9: PCIe post init
	// ucie_hwemu_pcie_init_post(ucie_ch, true);
    Ucie_Setup_PCIE_Post(ucie_ch, device_type);
	printf("[Step9] pcie_init_post(%d, %d)\n", ucie_ch, device_type);
	wait_time(1);

	return NORMAL_END;

// -----------------------------------------------------------------------------
LABEL_ERROR:
	// if (UCIE_SETUP_MODE & UCIE_SETUP_MODE_RETRY)
#ifdef UCIE_SETUP_MODE_RETRY
	{
		// Ucie_PwrOff(ucie_ch);
		goto LABEL_RETRY;
	}
#endif

	return ERROR_END;
}

/*******************************************************
 * NAME		: ucie_hwemu_hw_init_ep
 * FUNCTION	: Setup UCIE EP
 * INPUT	: uint32_t ucie_ch UCIE_CH0 / UCIE_CH1
 *            uint32_t device_type UCIE_RC / UCIE_EP
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
uint32_t ucie_hwemu_hw_init_ep(uint32_t ucie_ch, uint32_t device_type, uint32_t conn_ver)
{
	// ===== EP-specific initialization =====
	uint32_t ret = NORMAL_END;
	uint32_t timeout = 0;
	uint32_t *w;

	printf("[EP] HW_INIT (Ch%d) \n", ucie_ch);

	// w = (uint32_t *)(0xbffff100);
    // if (0 != w[0])
	// {
	// 	UCIE_SETUP_MODE = w[0];
	// }

	w = (uint32_t *)(0xbffff000);
	// Wait and notify RC to proceed
	wait_time(w[0]); // delay 10ms
    wait_time(w[1]); // delay 2ms
    wait_time(w[2]); // slot
    wait_time(w[3]); // slot
    // wait_time(w[4]);  // delay 0ms

LABEL_RETRY:
// -----------------------------------------------------------------------------

	// Ucie_PwrOn(ucie_ch);    
    Ucie_Setup_Reset(ucie_ch);
	
	// Step 1: UCIE Init Pre
	// ret = ucie_hwemu_init_pre(ucie_ch, false);
	Ucie_Setup_Pre(ucie_ch, device_type, conn_ver);
    wait_time(w[4]);
	printf("[Step1] Ucie_Setup_Pre(%d, %d, %d)\n", ucie_ch, device_type, conn_ver);
	// vTaskDelay(MS_TO_TICKS(1));
	// wait_time(WAIT_REG[1]);	// 0 ms

	// -----------------------------------------------------------------------------
	// Step 2: Start UCIE Link
	// ret = ucie_hwemu_start_link(ucie_ch, false);
    Ucie_Setup_Start_Linkup(ucie_ch, device_type);
	wait_time(w[5]);
	printf("[Step2] start_link(%d, %d)\n", ucie_ch, device_type);

	// -----------------------------------------------------------------------------
	// Step 3: Wait for frequency change request
	// ret = ucie_hwemu_setup_wait_freqchange_req(ucie_ch, false);
    ret = Ucie_Setup_Wait_FreqChange_Req(ucie_ch, device_type);
	printf("[Step3] freqchange_req(%d) returned(%d)\n", ucie_ch, ret);
	if (NORMAL_END != ret)
	{
		goto LABEL_ERROR;
	}

	// -----------------------------------------------------------------------------
	// Step 4: Acknowledge freq change
	// ret = ucie_hwemu_setup_ack_freqchange(ucie_ch, false);
    Ucie_Setup_Ack_FreqChange(ucie_ch);
	printf("[Step4] ack_freqchange(%d)\n", ucie_ch);

	// -----------------------------------------------------------------------------
	// Step 5: Wait for link-up
	// ret = ucie_hwemu_wait_linkup(ucie_ch, false);
    ret = Ucie_Setup_Wait_Linkup(ucie_ch, device_type);
	wait_time(w[6]);
	printf("[Step5] wait_linkup(%d) returned(%d)\n", ucie_ch, ret);
	if (NORMAL_END != ret)
	{
		goto LABEL_ERROR;
	}

	// -----------------------------------------------------------------------------
	// Step 6: PCIe initialization
	// ucie_hwemu_pcie_init_pre(ucie_ch, false);
    Ucie_Setup_PCIE_Pre(ucie_ch, device_type);
	wait_time(w[7]);
	printf("[Step6] pcie_init_pre(%d, %d)\n", ucie_ch, device_type);

	// -----------------------------------------------------------------------------
	// Step 7: Start PCIe link
	// ucie_hwemu_pcie_start_link(ucie_ch, false);
    Ucie_Setup_PCIE_Start_LinkUp(ucie_ch, device_type);
	// wait_time(w[7]);
	printf("[Step7] pcie_start_link(%d, %d)\n", ucie_ch, device_type);

	// -----------------------------------------------------------------------------
	// Step 8: Wait for PCIe link-up
	// ret = ucie_hwemu_pcie_wait_linkup(ucie_ch, false);
    ret = Ucie_Setup_PCIE_Wait_LinkUp(ucie_ch, device_type);
	printf("[Step8] pcie_wait_linkup(%d) returned(%d)\n", ucie_ch, ret);
	if (NORMAL_END != ret)
	{
		goto LABEL_ERROR;
	}

	// -----------------------------------------------------------------------------
	// Step 9: PCIe post init
	// ucie_hwemu_pcie_init_post(ucie_ch, false);
    Ucie_Setup_PCIE_Post(ucie_ch, device_type);
	printf("[Step9] pcie_init_post(%d, %d)\n", ucie_ch, device_type);
	wait_time(1);

	return NORMAL_END;

// -----------------------------------------------------------------------------
LABEL_ERROR:
	// if (UCIE_SETUP_MODE & UCIE_SETUP_MODE_RETRY)
#ifdef UCIE_SETUP_MODE_RETRY
	{
		// Ucie_PwrOff(ucie_ch);
		goto LABEL_RETRY;
	}
#endif

	return ERROR_END;
}
//---<ANH001>