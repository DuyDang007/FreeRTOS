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
#include "ucie_0621_1/ucie_hwemu.h"
#include "mdlcdrv.h"
#include "state-manager/r_clock_domain_id.h"
#include "state-manager/r_power_domain_id.h"
#include "state-manager/r_reset_domain_id.h"
#include "state-manager/r_state_manager.h"

// -----------------------------------------------------------------------------
// PLL Definitions
// -----------------------------------------------------------------------------
#define PLL9_0      (19U)
#define PLL9_1      (20U)

// -----------------------------------------------------------------------------
// Bit Manipulation Macros
// -----------------------------------------------------------------------------
#define CLEAR_BIT_MASK (~(1 << 4))
#define SET_BIT_MASK    (1 << 6)

// -----------------------------------------------------------------------------
// UCIE Power Management Control Addresses
// -----------------------------------------------------------------------------
#define UCIE_APB0_UCIEPWRMNGCTRL	(0xDCE00070)
#define UCIE_APB1_UCIEPWRMNGCTRL	(0xDDE00070)

// -----------------------------------------------------------------------------
// HDMA Base Address Calculation
// -----------------------------------------------------------------------------
#define HDMA_BASE_ADDR(ucie_chan, dma_chan, rw) \
	(0xD8380000 + (ucie_chan * 0x01000000) + (dma_chan * 0x800) +( rw * 0x400))

// -----------------------------------------------------------------------------
// MSI Definitions
// -----------------------------------------------------------------------------
#define MSI_STOP_BASE			(0x7FFF0000)
#define MSI_ABORT_BASE			(0x7FFF0200)
#define MSI_WATERMARK_BASE		(0x7FFF0400)
#define MSI_DATA				(0xA5A5A5A5)

// -----------------------------------------------------------------------------
// UCIe setup modes
// -----------------------------------------------------------------------------
#define UCIE_SETUP_RETRY_COUNT 			(5U)

#define UCIE_SETUP_MODE_TIMEOUTOFF 		(1<<0)
#define UCIE_SETUP_MODE_HOLDLTSSM 		(1<<1)
#define UCIE_SETUP_MODE_RETRY 			(1<<2)
#define UCIE_SETUP_MODE_TIMEOUTON_F_ACK (1<<14)
#define UCIE_SETUP_MODE_TIMEOUTON_L_UP 	(1<<15)
//uint32_t UCIE_SETUP_MODE = UCIE_SETUP_MODE_TIMEOUTOFF | UCIE_SETUP_MODE_HOLDLTSSM | UCIE_SETUP_MODE_RETRY;
uint32_t UCIE_SETUP_MODE = 0;
// -----------------------------------------------------------------------------
// Timeout Configuration
// -----------------------------------------------------------------------------
#define	TIMEOUT_EXECUTE_LOOP_COUNT (3000000U)	/**< Timeout loop count */

// -----------------------------------------------------------------------------
// Memory Access Functions
// -----------------------------------------------------------------------------

/**
 * @brief Read a 16-bit value from the specified memory-mapped register.
 *
 * @param addr The register address
 * @return uint16_t The value read
 */
uint16_t ucie_hwemu_read16(const volatile uintptr_t addr)
{
	return *((volatile uint16_t*)(addr));
}

/**
 * @brief Write a 16-bit value to the specified memory-mapped register.
 *
 * @param addr The register address
 * @param val  The value to write
 */
void ucie_hwemu_write16(volatile uintptr_t addr, uint16_t val)
{
	*((volatile uint16_t*)(addr)) = val;
}

/**
 * @brief Read a 32-bit value from the specified memory-mapped register.
 *
 * @param addr The register address
 * @return uint32_t The value read
 */
uint32_t ucie_hwemu_read(uint32_t addr)
{
    return *((volatile uint32_t *) addr);
}

/**
 * @brief Write a 32-bit value to the specified memory-mapped register.
 *
 * @param addr The register address
 * @param val  The value to write
 */
void ucie_hwemu_write(uint32_t addr, uint32_t val)
{
    *((volatile uint32_t *) addr) = val;
}

/*******************************************************************************
 * NAME     : ucie_hwemu_hdma_start
 * FUNCTION : Start transfer of UCIE HDMA
 * INPUT    : ucie_chan	UCIE ch (0-1)
 *          : dma_chan	HDMA ch (0-?)
 *          : rw		Use RDCH(1), WRCH(0)
 * OUTPUT   : none
 * RETURN   : none
*******************************************************************************/
void ucie_hwemu_hdma_start(uint32_t ucie_chan, uint32_t dma_chan, uint32_t rw)
{
	uint32_t base;

	base = 0xD8380000 + (ucie_chan * 0x01000000) + (dma_chan * 0x800) + (rw * 0x400);
	ucie_hwemu_read(base + 0x0004);
	ucie_hwemu_write(base + 0x0004, 0x00000001);						// HDMA_DOORBELL_OFF
	ucie_hwemu_read(base + 0x0004);
}

/*******************************************************************************
 * NAME     : ucie_hwemu_hdma_set_regsLL
 * FUNCTION : Set UCIE HDMA registers (Linked List mode)
 * INPUT    : info		HDMA parameter
 * OUTPUT   : none
 * RETURN   : none
*******************************************************************************/
void ucie_hwemu_hdma_set_regsLL(const struct hdma_info_ex *info, uint32_t custom_size)
{
	uint32_t base_addr;
	uint32_t val;
	
	base_addr = HDMA_BASE_ADDR(info->ucieCh, info->dmaCh, info->rw);
	uint32_t xfer_size = (custom_size != 0) ? custom_size : info->size;

	// for debug
    printf(" CH BaseAddr=0x%x, xfer_size=0x%x\n", base_addr, xfer_size);

    /*** Create Linked List ***/
	ucie_hwemu_write((info->llp + 0x00), 0x00000001);		// control
	ucie_hwemu_write((info->llp + 0x04), xfer_size);		// transfer size
	ucie_hwemu_write((info->llp + 0x08), (uint32_t)(info->sar & 0xFFFFFFFF));	// SAR (lower)
	ucie_hwemu_write((info->llp + 0x0C), (uint32_t)(info->sar >> 32));			// SAR (upper)
	ucie_hwemu_write((info->llp + 0x10), (uint32_t)(info->dar & 0xFFFFFFFF));	// DAR (lower)
	ucie_hwemu_write((info->llp + 0x14), (uint32_t)(info->dar >> 32));			// DAR (upper)

	ucie_hwemu_write((info->llp + 0x18), 0x00000006);		// control
	ucie_hwemu_write((info->llp + 0x1C), 0x00000000);
	ucie_hwemu_write((info->llp + 0x20), (uint32_t)(info->llp & 0xFFFFFFFF));	// LLP (lower)
	ucie_hwemu_write((info->llp + 0x24), (uint32_t)(info->llp >> 32));			// LLP (upper)
	ucie_hwemu_write((info->llp + 0x28), 0x00000000);
	ucie_hwemu_write((info->llp + 0x2C), 0x00000000);

	/*** Set HDMA registers ***/
	ucie_hwemu_write(base_addr + 0x0000, 0x00000001); // HDMA_EN_OFF

	val  = 0x30000;
	val |= ((info->weight & 0x1F) << 3) | (info->tc & 0x7);
    	ucie_hwemu_write(base_addr + 0x003C, val); // HDMA_QOS_OFF

	ucie_hwemu_write(base_addr + 0x001C, xfer_size);								// HDMA Transfer Size
	ucie_hwemu_write(base_addr + 0x0020, 0x00000000);
	ucie_hwemu_write(base_addr + 0x0024, 0x00000000);
	ucie_hwemu_write(base_addr + 0x0028, 0x00000000);
	ucie_hwemu_write(base_addr + 0x002C, 0x00000000);

	ucie_hwemu_write(base_addr + 0x0010, (uint32_t)(info->llp & 0xFFFFFFFF));		// HDMA_LLP_LOW_OFF
	ucie_hwemu_write(base_addr + 0x0014, (uint32_t)(info->llp >> 32));				// HDMA_LLP_HIGH_OFF

	ucie_hwemu_write(base_addr + 0x0034, 0x00000001);						// HDMA_CONTROL1_OFF
													//   [0] LLEN=1: Linked List mode Enable

	ucie_hwemu_write(base_addr + 0x0038, 0x00000000);						// HDMA_FUNC_NUM_OFF :Function Number=0

	ucie_hwemu_write(base_addr + 0x0030, 0x00000000);						// HDMA_WATERMARK_EN_OFF
	ucie_hwemu_write(base_addr + 0x0088, 0x00000078);						// HDMA_INT_SETUP_OFF
																			//   [6] Local Abort Interrupt Enable
																			//   [5] Remote Abort Interrupt Enable
																			//   [4] Local Stop Interrupt Enable
																			//   [3] Remote Stop Interrupt Enable
																			//   [2] ABORT_MASK=0
																			//   [1] WATERMARK_MASK=0
																			//   [0] STOP_MASK=0

	ucie_hwemu_write(base_addr + 0x0090, MSI_STOP_BASE);					// HDMA_MSI_STOP_LOW_OFF
	ucie_hwemu_write(base_addr + 0x0094, 0x00000000);						// HDMA_MSI_STOP_HIGH_OFF

	ucie_hwemu_write(base_addr + 0x0098, MSI_WATERMARK_BASE);				// HDMA_MSI_WATERMARK_LOW_OFF
	ucie_hwemu_write(base_addr + 0x009C, 0x00000000);						// HDMA_MSI_WATERMARK_HIGH_OFF

	ucie_hwemu_write(base_addr + 0x00A0, MSI_ABORT_BASE);					// HDMA_MSI_ABORT_LOW_OFF
	ucie_hwemu_write(base_addr + 0x00A4, 0x00000000);						// HDMA_MSI_ABORT_HIGH_OFF

	ucie_hwemu_write(base_addr + 0x00A8, MSI_DATA);							// HDMA_MSI_MSGD_OFF

	ucie_hwemu_write(base_addr + 0x0008, 0x00000000);						// HDMA_ELEM_PF_OFF
	ucie_hwemu_write(base_addr + 0x0018, 0x00000002);						// HDMA_CYCLE_OFF
}

/*******************************************************************************
 * NAME     : ucie_hwemu_hdma_stop
 * FUNCTION : Stop UCIE HDMA
 * INPUT    : ucieCh	UCIE ch (0-1)
 *          : dmaCh		HDMA ch (0-?)
 *          : rw		Use RDCH(1), WRCH(0)
 * OUTPUT   : none
 * RETURN   : none
*******************************************************************************/
void ucie_hwemu_hdma_stop(uint32_t ucie_chan, uint32_t dma_chan, uint32_t rw)
{
	uint32_t base_addr;
	
	base_addr = 0xD8380000 + (ucie_chan * 0x01000000) + (dma_chan * 0x800) + (rw * 0x400);
	ucie_hwemu_read(base_addr + 0x0004);
	ucie_hwemu_write(base_addr + 0x0004, 0x00000002);
	ucie_hwemu_read(base_addr + 0x0004);
	ucie_hwemu_write(base_addr + 0x0004, 0x00000000);
	ucie_hwemu_read(base_addr + 0x0004);
	ucie_hwemu_write(base_addr + 0x0000, 0x00000000);
	ucie_hwemu_read(base_addr + 0x0000);
}

/*******************************************************************************
 * NAME     : ucie_hwemu_hdma_wait_stop
 * FUNCTION : Wait transfer end of UCIE HDMA
 * INPUT    : ucieCh	UCIE ch (0-1)
 *          : dmaCh		HDMA ch (0-?)
 *          : rw		Use RDCH(1), WRCH(0)
 * OUTPUT   : none
 * RETURN   : 0			Success
 *          : 1			Timeout
*******************************************************************************/
uint32_t ucie_hwemu_hdma_wait_stop(uint32_t ucie_chan, uint32_t dma_chan, uint32_t rw)
{
	uint32_t base_addr;
	uint32_t ret = NORMAL_END;
	uint32_t timeout = 0;
	uint32_t val;

	// Calculate the base address for HDMA registers
	base_addr = 0xD8380000 + (ucie_chan * 0x01000000) + (dma_chan * 0x800) + (rw * 0x400);

	// Wait until HDMA enters STOP state (0x3) or timeout occurs
	while ((ucie_hwemu_read(base_addr + 0x0080) != 0x3) && (timeout < TIMEOUT_EXECUTE_LOOP_COUNT))
	{
		//wait_time(1);    // Wait for data transfer
		timeout++;
	}
	if (timeout >= TIMEOUT_EXECUTE_LOOP_COUNT)
    {
        printf("ERROR: %d Timeout waiting for UCIe execution \n", __LINE__);
		ret = ERROR_END;
    }
    else
    {
        printf("UCIe Execution completed \n");
    }

	// Dump final register values for debugging
	printf("Address 0x%x = 0x%08x \n",(base_addr + 0x80),ucie_hwemu_read(base_addr + 0x80));
	printf("Address 0x%x = 0x%08x \n",(base_addr + 0x84),ucie_hwemu_read(base_addr + 0x84));
	printf("Finish Wait STOP\n");

	val = ucie_hwemu_read(base_addr + 0x0084);

	ucie_hwemu_read(base_addr + 0x008C);
	ucie_hwemu_write(base_addr + 0x008C, (val & 0x7));
	ucie_hwemu_read(base_addr + 0x008C);	

	return ret;
}

/*******************************************************
 * NAME		: ucie_hwemu_init_pre
 * FUNCTION	: Setup UCIE pre-linkup
 * INPUT	: uint32_t ucie_chan
 * 			  bool is_rc
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
static uint32_t ucie_hwemu_init_pre(uint32_t ucie_chan, bool is_rc)
{
	uint32_t ret = NORMAL_END;

	/*====================================*/
	/* CT pattern                         */
	/*====================================*/
	//;--- 6.0 bypass mode ---
	//;EMUL mode
	//; axi adr = 41
	ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE005E8, 0x00000000 );
	ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE005E8, 0x00004141 );
	//;MmInitCtrl
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_MMINITCTRL_ADD, 0x08010000 );
	// state skip off
  ////; AcsmLtmlndex0Var1
	//ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMINDEX0VAR1_ADD, 0x0003BF85 );
	////; AcsmLtmlndex0Var5
	//ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMINDEX0VAR5_ADD, 0x00043FED );
	////; AcsmLtmlndex0Var13
	//ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMINDEX0VAR13_ADD, 0x000CF816 );
	//; axi adr = 0
	ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE005E8, 0x00000000 );
	//; PLLBypen=1, phy_lclk_sel=1
	ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE22000, 0x00000005 );
    ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE22000, 0x00000005 );

	//; UCIe0/1 apb setting
	if (is_rc)
	{
		//; UCIEFMIS
		ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE00308, 0x019A0000 );	//UCIEFMIS
		ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE00000, 0x00000010 );	// UCIE0 -> RC
		//; UCIEPCR00
		ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE21000, 0x00000001 );	//set UCIe device RootPort enable
		//; UCIECSR00
		ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE20000, 0x00000001 );	//select CXL mode
	}
	else
	{
		//; UCIEFMIS
		ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE00308, 0x015C0000 );	//UCIEFMIS


		ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE00000, 0x00000000 );

		//; UCIEPCR00
		ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE21000, 0x00000002 );	//set UCIe device Endpoint enable

		//; UCIECSR00
		ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE20000, 0x00000001 );	//select CXL mode
	}
	//; UCIEICR27
	ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE1007C, 0x00000002 );	//interrupt output enable freq_change_req

	//; UCIe0/1 axi setting 00
	if (is_rc)
	{
		//; LINK_CONTROL2_LINK_STATUS2_REG
		ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + PF0_PCIE_CAP_LINK_CONTROL2_LINK_STATUS2_REG_ADD, 0x00000004 );
	}
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + RCAR_UCIE_BASE_ADD(0x38C), 0x00000000 );
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + RCAR_UCIE_BASE_ADD(0x38C), 0x00000000 );
	//; PL32G_CONTROL_REG
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + PF0_PL32G_CAP_PL32G_CONTROL_REG_ADD, 0x00000200 );	//PL32G_CONTROL_REG. Modified TS Usage Mode Selected.Alternate Protocol Negtiation.
	//; CXL_VLSM_CSR_REG_OFF
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + PF0_PORT_LOGIC_CXL_VLSM_CSR_REG_OFF_ADD, 0x00800000 );	//CXL VLSM CSR REGISTER

	//; RASDP_ERROR_TRACER_CAPABILITY_OFF
	// VTB_adr : 0x00000420, C code adr : 0xd8b003bc
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + RCAR_UCIE_BASE_ADD(0x420), 0x00000027 );
	
	if (!is_rc)
	{
		//; CXL_RCIEP_FLEXBUS_CNTRL_STATUS_OFF
		ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + TMP_CXL_RCIEP_FLEXBUS_CNTRL_STATUS_OFF, 0x00000007 );	//CXL_RCIEP_FLEXBUS_CNTRL_STATUS_OFF
		//; PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG
		ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + PF0_PCIE_CAP_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_ADD, 0x8002D010 ); // PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG
		//; MISC_CONTROL_1_OFF
		ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + PF0_PORT_LOGIC_MISC_CONTROL_1_OFF_ADD, 0x080BFF48 ); // MISC_CONTROL_1_OFF
	}
	else
	{
		//; PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG
        ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + PF0_PCIE_CAP_PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG_ADD, 0x8042D010 ); // PCIE_CAP_ID_PCIE_NEXT_CAP_PTR_PCIE_CAP_REG
		//; MISC_CONTROL_1_OFF
		ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + PF0_PORT_LOGIC_MISC_CONTROL_1_OFF_ADD, 0x040BFF48 ); // MISC_CONTROL_1_OFF
	}
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + RCAR_UCIE_BASE_ADD(0x38c), 0x00000000 );
	
	//; ULT_PLLCTRL4 -> PF0_MEMBAR0_RAS_CAP_MEMBAR0_RAS_UNCOR_ERROR_MASK_REG_OFF_ADD
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + PF0_MEMBAR0_RAS_CAP_MEMBAR0_RAS_UNCOR_ERROR_MASK_REG_OFF_ADD, 0x00000000 ); //elbi
	//; MEMBAR0_RAS_CORR_ERROR_MASK_REG_OFF
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + PF0_MEMBAR0_RAS_CAP_MEMBAR0_RAS_CORR_ERROR_MASK_REG_OFF_ADD, 0x00000000 ); //elbi

	//; UCIe0 axi setting 01
	//;  axi0 addres ON
	ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE005E8, 0x00004141 );

	//;  ULT_CTRL_IMP_SB_CONFIG3
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + IMP_SPECIFIC_SB_UNIT_IMP_SB_CONFIG3_ADD, 0x00018001 );

	//;  ULT_CTRL_IMP_SB_CONFIG0
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + IMP_SPECIFIC_SB_UNIT_IMP_SB_CONFIG0_ADD, 0x000a0190 );

	//;  ULT_CTRL_IMP_SB_CONFIG2
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + IMP_SPECIFIC_SB_UNIT_IMP_SB_CONFIG2_ADD, 0x000a0190 );

	//;  ULT_CTRL_IMP_SB_CONFIG4
	if (is_rc)
	{
		ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + IMP_SPECIFIC_SB_UNIT_IMP_SB_CONFIG4_ADD, 0x002000AE ); //DSP
	}
	else
	{
		ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + IMP_SPECIFIC_SB_UNIT_IMP_SB_CONFIG4_ADD, 0x004000AE ); //USP
	}
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + IMP_SPECIFIC_SB_UNIT_IMP_SB_CONFIG5_ADD, 0x00000000 );

	//;  axi0 addres OFF
	ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE005E8, 0x00000000 );

	//;  ULT_CTRL_DVSEC_UCIE_LINK_CONTROL
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DVSEC_UNIT_DSP_DVSEC_UCIE_LINK_CONTROL_ADD, 0x00004000 );

	//;  ULT_CTRL_CXL_DVSEC_FLEX_CTL_STATUS
#ifdef RCAR_UCIE_V100
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + CXL_DVSEC_UNIT_DSP_CXL_DVSEC_FLEX_CTL_STATUS_ADD, 0x00000027 );
#endif

#if defined(RCAR_UCIE_V101) || defined(RCAR_UCIE_V102)
	// VTB_adr : 0x0000042c, C code adr : 0xd8000420
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + RCAR_UCIE_BASE_ADD(0x42C), 0x00000027 ); // from UT result
#endif
	
	//; UCIe0 axi setting 02
	//;  axi addres ON
	ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE005E8, 0x00004141 );

#if defined(RCAR_UCIE_V101) || defined(RCAR_UCIE_V102)
	//;  ULT_CTRL_IMP_MB_CONFIG11
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + IMP_SPECIFIC_MB_UNIT_IMP_MB_CONFIG11_ADD, 0x00103001 );

	//;  ULT_CTRL_IMP_MB_CONFIG11
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + IMP_SPECIFIC_MB_UNIT_IMP_MB_CONFIG11_ADD, 0x00103001 );
#endif

#ifdef RCAR_UCIE_V100
	//;  ULT_CTRL_IMP_MB_CONFIG11
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + IMP_SPECIFIC_MB_UNIT_IMP_MB_CONFIG11_ADD, 0x00103000 );
	//;  ULT_CTRL_IMP_MB_CONFIG11
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + IMP_SPECIFIC_MB_UNIT_IMP_MB_CONFIG11_ADD, 0x00103000 ); // X5H: bit[0] = 0 
#endif

#ifdef RCAR_UCIE_V100
	//;  ULT_MMTRKEN
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_MMTRKCTRL_ADD, 0x00000002 );

	//;  ULT_MODULEDISABLESTATUS
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_MODULEDEGRADESTATUS_ADD, 0x0000FFFC );

	//;  ULT_ACSMWAITDLY0
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMWAITDLY0_ADD, 0x000007D0 );

	//;  ULT_ACSMWAITDLY1
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMWAITDLY1_ADD, 0x00004A38 );

	//;  ULT_ZCALCOMPSTARTUPTIME
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_ZCALCTRL0_ADD, 0x641F43E8 );

	//;  ULT_ZCALSAMPLETIME
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_ZCALCTRL0_ADD, 0x640FA3E8 );

	//;  ULT_ZCALOFFSETSAMPLETIME
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_ZCALCTRL0_ADD, 0x320FA3E8 );
	
	//;  ULT_TXZCALNOFFSET
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_ZCALCTRL1_ADD, 0x0C219800 );

	//;  ULT_TXZCALPOFFSET
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_ZCALCTRL1_ADD, 0x0C219CC0 );

	//;  ULT_PLLCPINTCTRL
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL0_ADD, 0x2C7C0C03 ); // 7'b111_1111, 7'b000_0011

	//;  ULT_PLLCPPROPCTRL
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL0_ADD, 0x2C7C0B03 ); // 7'b111_1111<<8, 7'b000_1011<<8

	//;  ULT_PLLCPINTGSCTRL
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL0_ADD, 0x2C7F0B03 ); // 7'b111_1111<<16, 7'b111_1111<<16

	//;  ULT_PLLCPPROPGSCTRL
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL0_ADD, 0x287F0B03 ); // 7'b111_1111<<24, 7'b010_1000<<24

	//;  ULT_PLLDIVSEL
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL1_ADD, 0x00002BCE );

	//;  ULT_PLLV2IMODE
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL1_ADD, 0x000033CE );

	//;  ULT_PLLVCOLOWFREQ
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL1_ADD, 0x000013CE );

	//;  ULT_PLLCTRL3
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL3_ADD, 0x50601009 );

	//;  ULT_PLLCTRL4
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL4_ADD, 0x041B6001 );

	//;  ULT_FREQCHANGETYPE
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_MMMODECTRL_ADD, 0x00000012 );
#endif /* #ifdef RCAR_UCIE_V100 */
	
	//;  ULT_DWMODECTRL0
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWMODECTRL0_ADD, 0x0600000C );
	
	//;  ULT_DWDCDSETTLETIME
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWDCCCTRL1_ADD, 0x00C86432 );
	
	//;  ULT_DWDCASETTLETIME
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWDCCCTRL1_ADD, 0x00C81532 );
	
	//;  ULT_DWDCDSAMPLETIME
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWDCCCTRL1_ADD, 0x00141532 );
	
	//;  ULT_DWMODULEDISABLESTATUS
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWMODULEDEGRADESTATUS_ADD, 0x0000FFFC );
	
	//;  ULT_DWVREFVARMAX
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWVREFVAR_ADD, 0x00800001 );
	
	//;  ULT_DWVREFVARMIN
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWVREFVAR_ADD, 0x0080007E );
	
	//;  ULT_DWTXCKPARKLEVEL
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWMISCCTRL0_ADD, 0x3D00A001 );
	
	//;  ULT_DWMODECTRL0
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWMODECTRL0_ADD, 0x0600000C );
	
	//;  ULT_DWDCDSETTLETIME_DW1
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWDCCCTRL1_ADD, 0x00C86432 );
	
	//;  ULT_DWDCASETTLETIME_DW1
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWDCCCTRL1_ADD, 0x00C81532 );
	
	//;  ULT_DWDCDSAMPLETIME_DW1
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWDCCCTRL1_ADD, 0x00141532 );
	
	//;  ULT_DWMODULEDISABLESTATUS_DW1
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWMODULEDEGRADESTATUS_ADD, 0x0000FFFC );
	
	//;  ULT_DWVREFVARMAX_DW1
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWVREFVAR_ADD, 0x00800001 );
	
	//;  ULT_DWVREFVARMIN_DW1
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWVREFVAR_ADD, 0x0080007E );
	
	//;  ULT_DWTXCKPARKLEVEL_DW1
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWMISCCTRL0_ADD, 0x3D00E001 );

	uint32_t reg_val;
#if defined(RCAR_UCIE_V101_W_V100) || defined(RCAR_UCIE_V102_W_V100)
	// Setting CSR for reversalMB
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG25_ADD, 0x83000061);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR4_ADD, 0x48E23803);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR5_ADD, 0x48E23803);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR6_ADD, 0x48E39805);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR7_ADD, 0x4BDFD805);

	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + MMPL_MMMISCCTRL_ADD);
	reg_val = reg_val & MMPL_MMMISCCTRL_MASK;
	reg_val = reg_val &      ~( MMPL_MMMISCCTRL_MMPHYCOMPLIANCE_MASK  << MMPL_MMMISCCTRL_MMPHYCOMPLIANCE_SHIFT);
	reg_val = reg_val | ((0x1 & MMPL_MMMISCCTRL_MMPHYCOMPLIANCE_MASK) << MMPL_MMMISCCTRL_MMPHYCOMPLIANCE_SHIFT);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_MMMISCCTRL_ADD, reg_val );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWMISCCTRL0_ADD);
	reg_val = reg_val & DWORD_0_DWMISCCTRL0_MASK;
	reg_val = reg_val &      ~( DWORD_0_DWMISCCTRL0_DWPHYCOMPLIANCE_MASK  << DWORD_0_DWMISCCTRL0_DWPHYCOMPLIANCE_SHIFT);
	reg_val = reg_val | ((0x1 & DWORD_0_DWMISCCTRL0_DWPHYCOMPLIANCE_MASK) << DWORD_0_DWMISCCTRL0_DWPHYCOMPLIANCE_SHIFT);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWMISCCTRL0_ADD, reg_val );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWMISCCTRL0_ADD);
	reg_val = reg_val & DWORD_1_DWMISCCTRL0_MASK;
	reg_val = reg_val &      ~( DWORD_1_DWMISCCTRL0_DWPHYCOMPLIANCE_MASK  << DWORD_1_DWMISCCTRL0_DWPHYCOMPLIANCE_SHIFT);
	reg_val = reg_val | ((0x1 & DWORD_1_DWMISCCTRL0_DWPHYCOMPLIANCE_MASK) << DWORD_1_DWMISCCTRL0_DWPHYCOMPLIANCE_SHIFT);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWMISCCTRL0_ADD, reg_val );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR0I0_ADD);
	reg_val = reg_val & ACSM_ACSMTRAINVAR0I0_MASK;
	reg_val = reg_val | (0x1 << 16);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR0I0_ADD, reg_val );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR0I1_ADD);
	reg_val = reg_val & ACSM_ACSMTRAINVAR0I1_MASK;
	reg_val = reg_val | (0x1 << 16);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR0I1_ADD, reg_val );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR0I2_ADD);
	reg_val = reg_val & ACSM_ACSMTRAINVAR0I2_MASK;
	reg_val = reg_val | (0x1 << 16);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR0I2_ADD, reg_val );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR1I1_ADD);
	reg_val = reg_val & ACSM_ACSMTRAINVAR1I1_MASK;
	reg_val = reg_val & 0xFFFFFEFF;
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR1I1_ADD, reg_val );
#endif	/* #if defined(RCAR_UCIE_V101_W_V100) || defined(RCAR_UCIE_V102_W_V100) */
	
#if defined(RCAR_UCIE_V101_W_V100) || defined(RCAR_UCIE_V102_W_V100)
	// Setting PLL clock for X5H connection
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL0_P0_ADD  , 0x3F3F100C);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL0_P1_ADD  , 0x3F3F100C);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL0_P2_ADD  , 0x303F0808);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL0_P3_ADD  , 0x3F3F1004);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL1_P0_ADD  , 0x14830C55);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL1_P1_ADD  , 0x14870C04);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL1_P2_ADD  , 0x03870C04);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL1_P3_ADD  , 0x06C70C04);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL3_ADD     , 0x04236B49);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_PLLCTRL4_ADD     , 0x50601009);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMWAITDLY0_ADD , 0x000007D0);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMWAITDLY1_ADD , 0x00004A38);
#endif /*#if defined(RCAR_UCIE_V101_W_V100) || defined(RCAR_UCIE_V102_W_V100)*/

#ifdef RCAR_UCIE_V100
	// Setting CSR for reversalMB
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG95_ADD      , 0x000FFF53); 
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG96_ADD      , 0x000FFFB3); 
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG97_ADD      , 0x00080023); 
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG98_ADD      , 0x0000B00B); 
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG99_ADD      , 0x03970B6A); 

	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMINDEX0VAR14_ADD);
	reg_val = reg_val & ACSM_ACSMLTSMINDEX0VAR14_MASK;
	reg_val = reg_val | (0x1 << 20);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMINDEX0VAR14_ADD, reg_val );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMINDEX0VAR16_ADD);
	reg_val = reg_val & ACSM_ACSMLTSMINDEX0VAR16_MASK;
	reg_val = reg_val | (0x1 << 20);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMINDEX0VAR16_ADD, reg_val );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR0I0_ADD);
	reg_val = reg_val & ACSM_ACSMTRAINVAR0I0_MASK;
	reg_val = reg_val | (0x1 << 16);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR0I0_ADD, reg_val );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR0I1_ADD);
	reg_val = reg_val & ACSM_ACSMTRAINVAR0I1_MASK;
	reg_val = reg_val | (0x1 << 16);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR0I1_ADD, reg_val );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR0I2_ADD);
	reg_val = reg_val & ACSM_ACSMTRAINVAR0I2_MASK;
	reg_val = reg_val | (0x1 << 16);
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR0I2_ADD, reg_val );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR1I1_ADD);
	reg_val = reg_val & ACSM_ACSMTRAINVAR1I1_MASK;
	reg_val = reg_val & 0xFFFFFEFF;
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTRAINVAR1I1_ADD, reg_val );
	
	//setup_highspeed_link_initialization_and_training_steps_x5h(); //tmk+:AIACC:20250407
	//// 2. Setup high-speed link initialization and training steps.
	//// a. Write MmTrkCtrl.MmTrkEn = 1'b0 to disable periodic track run time calibration.
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + RCAR_UCIE_BASE_ADD(0xF01128), 0x00000000 );
	
	//// b. Write DwRxLatCtrl.DwRxVldMargin = 1 to setup VLD lane training variables.
    reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWRXLATCTRL_ADD);
	reg_val = reg_val & DWORD_0_DWRXLATCTRL_MASK;
	reg_val = reg_val &      ~( DWORD_0_DWRXLATCTRL_DWRXVLDMARGIN_MASK  << DWORD_0_DWRXLATCTRL_DWRXVLDMARGIN_SHIFT);
	reg_val = reg_val | ((0x1 & DWORD_0_DWRXLATCTRL_DWRXVLDMARGIN_MASK) << DWORD_0_DWRXLATCTRL_DWRXVLDMARGIN_SHIFT);
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWRXLATCTRL_ADD, reg_val );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWRXLATCTRL_ADD);
	reg_val = reg_val & DWORD_1_DWRXLATCTRL_MASK;
	reg_val = reg_val &      ~( DWORD_1_DWRXLATCTRL_DWRXVLDMARGIN_MASK  << DWORD_1_DWRXLATCTRL_DWRXVLDMARGIN_SHIFT);
	reg_val = reg_val | ((0x1 & DWORD_1_DWRXLATCTRL_DWRXVLDMARGIN_MASK) << DWORD_1_DWRXLATCTRL_DWRXVLDMARGIN_SHIFT);
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWRXLATCTRL_ADD, reg_val );
	
	//// d. Write 00800000 to UcieTrainingSetup1 to program PPGC pattern.
	ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE005E8, 0x00004040 );
	
	// adr 0x4070_1*** is none in .h file
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + RCAR_UCIE_BASE_ADD(0x40701000) + 0x010, 0x00800000 );
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + RCAR_UCIE_BASE_ADD(0x40701000) + 0x014, 0x00800000 );
	
	//// e. Write 00010000 to UcieTrainingSetup2 to program PPGC pattern.
	// adr 0x4070_1*** is none in .h file
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + RCAR_UCIE_BASE_ADD(0x40701000) + 0x020, 0x00010000 );
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + RCAR_UCIE_BASE_ADD(0x40701000) + 0x024, 0x00010000 );
#endif /* #ifdef RCAR_UCIE_V100 */

	/* From Kihara-san (25 June)
	* This is a software workaround due to the current UCIe transfer size limitation of 1MB.
	* Regarding this limitation, could you try the following workaround?
	* - Check the value of DwMiscCtrl0[15].DwTxCkParkLevel.
	* - If it is set to 0, try changing it to 1. This may improve the situation.
	*
	* Note: A similar transfer size limitation was observed in UCIE-TEG on the bare-metal test.
	* After debugging with SNPS, applying this setting adjustment resolved the issue.
	*/

	//// f. Write DwMiscCtrl0.DwTxCkParkLevel=1'b1, clock is parked at its inactive level.
    ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE005E8, 0x00004141 );
	
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWMISCCTRL0_ADD, 0x3D00A001 );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWMISCCTRL0_ADD);
	reg_val = reg_val & DWORD_1_DWMISCCTRL0_MASK;
	reg_val = reg_val &      ~( DWORD_1_DWMISCCTRL0_DWTXCKPARKLEVEL_MASK  << DWORD_1_DWMISCCTRL0_DWTXCKPARKLEVEL_SHIFT);
	reg_val = reg_val | ((0x1 & DWORD_1_DWMISCCTRL0_DWTXCKPARKLEVEL_MASK) << DWORD_1_DWMISCCTRL0_DWTXCKPARKLEVEL_SHIFT);
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWMISCCTRL0_ADD, reg_val );

#ifdef RCAR_UCIE_V100
	//// g. Program DwModeCtrl0 with the following values.
	//// i. Write DwModeCtrl0.DwRxCtlClkSel=0x1 to select the receive clock.
	//// ii. Write DwModeCtrl0.DwRxLatAlign=0x1 to select the read latency alignment type.
    reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWMODECTRL0_ADD);
	reg_val = reg_val & DWORD_0_DWMODECTRL0_MASK;
	reg_val = reg_val &      ~( DWORD_0_DWMODECTRL0_DWRXCTLCLKSEL_MASK  << DWORD_0_DWMODECTRL0_DWRXCTLCLKSEL_SHIFT);
	reg_val = reg_val | ((0x1 & DWORD_0_DWMODECTRL0_DWRXCTLCLKSEL_MASK) << DWORD_0_DWMODECTRL0_DWRXCTLCLKSEL_SHIFT);
	reg_val = reg_val &      ~( DWORD_0_DWMODECTRL0_DWRXLATALIGN_MASK  << DWORD_0_DWMODECTRL0_DWRXLATALIGN_SHIFT);
	reg_val = reg_val | ((0x1 & DWORD_0_DWMODECTRL0_DWRXLATALIGN_MASK) << DWORD_0_DWMODECTRL0_DWRXLATALIGN_SHIFT);
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWMODECTRL0_ADD, reg_val );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWMODECTRL0_ADD);
	reg_val = reg_val & DWORD_1_DWMODECTRL0_MASK;
	reg_val = reg_val &      ~( DWORD_1_DWMODECTRL0_DWRXCTLCLKSEL_MASK  << DWORD_1_DWMODECTRL0_DWRXCTLCLKSEL_SHIFT);
	reg_val = reg_val | ((0x1 & DWORD_1_DWMODECTRL0_DWRXCTLCLKSEL_MASK) << DWORD_1_DWMODECTRL0_DWRXCTLCLKSEL_SHIFT);
	reg_val = reg_val &      ~( DWORD_1_DWMODECTRL0_DWRXLATALIGN_MASK  << DWORD_1_DWMODECTRL0_DWRXLATALIGN_SHIFT);
	reg_val = reg_val | ((0x1 & DWORD_1_DWMODECTRL0_DWRXLATALIGN_MASK) << DWORD_1_DWMODECTRL0_DWRXLATALIGN_SHIFT);
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWMODECTRL0_ADD, reg_val );
	
	//// h. Write AcsmLtsmIndex0Var14=0x14efef to enable 1D Vref training.
    reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMINDEX0VAR14_ADD);
	reg_val = reg_val & ACSM_ACSMLTSMINDEX0VAR14_MASK;
	reg_val = reg_val &           ~( ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_MASK  << ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_SHIFT);
	reg_val = reg_val | ((0x14EFEF & ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_MASK) << ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_SHIFT);
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMINDEX0VAR14_ADD, reg_val );
	
	// adr symbol is none
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + (RCAR_UCIE_BASE_ADD(0x41F12000)+0x150) );
	reg_val = reg_val & ACSM_ACSMLTSMINDEX0VAR14_MASK;
	reg_val = reg_val &           ~( ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_MASK  << ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_SHIFT);
	reg_val = reg_val | ((0x14EFEF & ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_MASK) << ACSM_ACSMLTSMINDEX0VAR14_ACSMLTSMINDEX0VAR14_SHIFT);
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + (RCAR_UCIE_BASE_ADD(0x41F12000)+0x150), reg_val );
	
	//// i. Write AcsmLtsmIndex0Var16=0x10f7f1 to enable 1D Vref training.
    reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMINDEX0VAR16_ADD );
	reg_val = reg_val & ACSM_ACSMLTSMINDEX0VAR16_MASK;
	reg_val = reg_val &           ~( ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_MASK  << ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_SHIFT);
	reg_val = reg_val | ((0x10F7F1 & ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_MASK) << ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_SHIFT);
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMINDEX0VAR16_ADD, reg_val );
	
	// adr symbol is none
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + (RCAR_UCIE_BASE_ADD(0x41F12000)+0x158) );
	reg_val = reg_val & ACSM_ACSMLTSMINDEX0VAR16_MASK;
	reg_val = reg_val &           ~( ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_MASK  << ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_SHIFT);
	reg_val = reg_val | ((0x10F7F1 & ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_MASK) << ACSM_ACSMLTSMINDEX0VAR16_ACSMLTSMINDEX0VAR16_SHIFT);
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + (RCAR_UCIE_BASE_ADD(0x41F12000)+0x158), reg_val );
	
	//// j. Write 00001600 to AcsmLoopVar1 so that LTSM will go from RESET to LINKINIT
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLOOPVAR1_ADD, 0x0001600 );
	
	
	//update_instr_code_x5h();
	////Instruction registers
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG5_ADD,   0x03910133 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG6_ADD,   0x022403A3 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG7_ADD,   0x03910E33 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG8_ADD,   0x022404A3 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG9_ADD,   0x02210143 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG10_ADD,  0x02200000 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG11_ADD,  0x83100011 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG12_ADD,  0x0110C007 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG13_ADD,  0x02711003 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG14_ADD,  0xA3100801 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG15_ADD,  0xA3100011 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG16_ADD,  0xA3100021 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG17_ADD,  0x63970181 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG18_ADD,  0xA3100031 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG19_ADD,  0xA3100041 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG20_ADD,  0xA3100851 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG21_ADD,  0x22712003 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG22_ADD,  0x000122C9 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG23_ADD,  0x000C4125 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG24_ADD,  0x83045062 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG25_ADD,  0x000100C6 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG26_ADD,  0x83200091 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG27_ADD,  0x02500000 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG28_ADD,  0x83100041 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG29_ADD,  0x02714003 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG30_ADD,  0x02210243 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG31_ADD,  0x00042EC9 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG32_ADD,  0x000300A6 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG33_ADD,  0x02210443 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG34_ADD,  0x8300D0C1 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG35_ADD,  0x832000D1 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG36_ADD,  0x830001D1 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG37_ADD,  0x83102071 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG38_ADD,  0x00034125 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG39_ADD,  0x000F7155 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG40_ADD,  0x00021006 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG68_ADD,  0x00080423 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG69_ADD,  0x0004FE53 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG70_ADD,  0x0001CFB3 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG71_ADD,  0x03973C6A );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG72_ADD,  0x834090F1 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG73_ADD,  0x80006081 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG74_ADD,  0x80000281 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG75_ADD,  0x83209081 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG76_ADD,  0x83000281 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG77_ADD,  0x80004081 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG78_ADD,  0x80006481 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG79_ADD,  0x8000A181 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG80_ADD,  0x83000001 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG81_ADD,  0x0397606A );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG82_ADD,  0x83200001 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG83_ADD,  0x83000101 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG84_ADD,  0x83005181 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG85_ADD,  0x80004381 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG86_ADD,  0x000F5135 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG87_ADD,  0x00016145 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG88_ADD,  0x03979B6A );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG89_ADD,  0x03978D6A );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG91_ADD,  0x0397406A );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG92_ADD,  0x01180873 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG93_ADD,  0x02780863 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG94_ADD,  0x01180073 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG95_ADD,  0x000FFF53 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG96_ADD,  0x000FFFB3 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG97_ADD,  0x00080023 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG98_ADD,  0x0000B00B );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG99_ADD,  0x03970B6A );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSMIM_ACSMINSTRREG100_ADD, 0x00020173 );
	
	////Mask registers
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR0_ADD,     0x1f9      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR1_ADD,     0x2623001  );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR2_ADD,     0x5        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR3_ADD,     0x1e1      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR4_ADD,     0x3727003  );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR5_ADD,     0x3727003  );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR6_ADD,     0x2733005  );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR7_ADD,     0x36fb005  );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR8_ADD,     0xde673e05 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR9_ADD,     0xde673e05 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR10_ADD,    0xb        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR11_ADD,    0x1e3      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR12_ADD,    0xc0000607 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR13_ADD,    0x32673805 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR14_ADD,    0xde673e05 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR15_ADD,    0x32673805 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR16_ADD,    0xde673e05 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR17_ADD,    0x5        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR18_ADD,    0x32673805 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR19_ADD,    0x26fb007  );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR20_ADD,    0x1603005  );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR21_ADD,    0xcec203   );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR22_ADD,    0xc208985  );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR23_ADD,    0x1020605  );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR24_ADD,    0x4030005  );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR25_ADD,    0xf8100673 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR26_ADD,    0xf8100673 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR27_ADD,    0x8800c277 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR28_ADD,    0x8800c275 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR29_ADD,    0x8800c277 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR30_ADD,    0x7300000d );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR0_ADD,     0x140      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR1_ADD,     0x120      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR2_ADD,     0x100      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR3_ADD,     0x120      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR4_ADD,     0x120      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR5_ADD,     0x120      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR6_ADD,     0x120      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR7_ADD,     0x120      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR8_ADD,     0x123      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR9_ADD,     0x123      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR10_ADD,    0x120      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR11_ADD,    0x120      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR12_ADD,    0x123      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR13_ADD,    0x120      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR14_ADD,    0x123      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR15_ADD,    0x120      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR16_ADD,    0x123      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR17_ADD,    0x120      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR18_ADD,    0x120      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR19_ADD,    0x1dc      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR20_ADD,    0x120      );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR21_ADD,    0x4        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR22_ADD,    0x18       );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR23_ADD,    0x4        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR24_ADD,    0x8        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR25_ADD,    0x3        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR26_ADD,    0x3        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR27_ADD,    0xb        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR28_ADD,    0xb        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR29_ADD,    0xb        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR30_ADD,    0x4        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR21ALT_ADD, 0xcc800b   );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR22ALT_ADD, 0xc208985  );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR23ALT_ADD, 0x100210d  );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR24ALT_ADD, 0x4020005  );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR25ALT_ADD, 0xf820a173 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR26ALT_ADD, 0xf820a173 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR27ALT_ADD, 0x88008077 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR28ALT_ADD, 0x88008075 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR29ALT_ADD, 0x88008077 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK0VAR30ALT_ADD, 0x7300000d );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR21ALT_ADD, 0x4        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR22ALT_ADD, 0x18       );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR23ALT_ADD, 0x4        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR24ALT_ADD, 0x8        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR25ALT_ADD, 0x3        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR26ALT_ADD, 0x3        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR27ALT_ADD, 0xb        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR28ALT_ADD, 0xb        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR29ALT_ADD, 0xb        );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMMSK1VAR30ALT_ADD, 0x4        );
	
	////Address registers
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_CSRADDR2_ADD,  0x304004 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_CSRADDR5_ADD,  0x304018 );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_CSRADDR10_ADD, 0x3042BC );
	ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + MMPL_CSRADDR11_ADD, 0x304020 );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMSEQ0CTRL_ADD);
	reg_val = reg_val & ACSM_ACSMSEQ0CTRL_MASK;
	reg_val = reg_val &       ~( ACSM_ACSMSEQ0CTRL_ACSMSEQ0STOPADDR_MASK  << ACSM_ACSMSEQ0CTRL_ACSMSEQ0STOPADDR_SHIFT);
	reg_val = reg_val | ((0x28 & ACSM_ACSMSEQ0CTRL_ACSMSEQ0STOPADDR_MASK) << ACSM_ACSMSEQ0CTRL_ACSMSEQ0STOPADDR_SHIFT);
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMSEQ0CTRL_ADD, reg_val );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMSEQ1CTRL_ADD);
	reg_val = reg_val & ACSM_ACSMSEQ1CTRL_MASK;
	reg_val = reg_val &       ~( ACSM_ACSMSEQ1CTRL_ACSMSEQ1STOPADDR_MASK  << ACSM_ACSMSEQ1CTRL_ACSMSEQ1STOPADDR_SHIFT);
	reg_val = reg_val | ((0x64 & ACSM_ACSMSEQ1CTRL_ACSMSEQ1STOPADDR_MASK) << ACSM_ACSMSEQ1CTRL_ACSMSEQ1STOPADDR_SHIFT);
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMSEQ1CTRL_ADD, reg_val );
	
	reg_val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMCTRL_ADD);
	reg_val = reg_val & ACSM_ACSMCTRL_MASK;
	reg_val = reg_val &       ~( ACSM_ACSMCTRL_ACSMSTOPADDR_MASK  << ACSM_ACSMCTRL_ACSMSTOPADDR_SHIFT);
	reg_val = reg_val | ((0x28 & ACSM_ACSMCTRL_ACSMSTOPADDR_MASK) << ACSM_ACSMCTRL_ACSMSTOPADDR_SHIFT);
    ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMCTRL_ADD, reg_val );
#endif /* #ifdef RCAR_UCIE_V100 */

	if (UCIE_SETUP_MODE & UCIE_SETUP_MODE_TIMEOUTOFF)
	{
		// 8ms Timeout off
		printf("TIMEOUTOFF\n");
		ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTIMEOUTCTRL1_ADD, 0x00000000 );
	}
	if (UCIE_SETUP_MODE & UCIE_SETUP_MODE_HOLDLTSSM)
	{
		printf("RELEASE HOLDLTSSM\n");
		//UCIERSTCTRL1.app_ltssm_enable=0 : enable ltssm state transition
		ucie_hwemu_write( UCIE_APB_BASE(ucie_chan)+0xE00014, 0x00000000 );
	}

	return ret;
}
//---<ANH001>

/*******************************************************
 * NAME		: ucie_hwemu_start_link
 * FUNCTION	: Start UCIE link-up
 * INPUT	: uint32_t ucie_chan
 * 			  bool is_rc
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
static uint32_t ucie_hwemu_start_link(uint32_t ucie_chan, bool is_rc)
{
	uint32_t ret = NORMAL_END;
    ucie_hwemu_write(UCIE_APB_BASE(ucie_chan) + 0xE005E8, 0x00000000);

    // ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + DVSEC_UNIT_DSP_DVSEC_UCIE_LINK_CONTROL_ADD, 0x00004040);
	ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + DVSEC_UNIT_DSP_DVSEC_UCIE_LINK_CONTROL_ADD, 0x000040C0);

    if (is_rc)
	{
        // ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + DVSEC_UNIT_DSP_DVSEC_UCIE_LINK_CONTROL_ADD, 0x00004440);
		ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + DVSEC_UNIT_DSP_DVSEC_UCIE_LINK_CONTROL_ADD, 0x000044C0);
	}

	return ret;
}

/*******************************************************
 * NAME		: ucie_hwemu_setup_wait_freqchange_req
 * FUNCTION	: Wait UCIE Freq Change Request
 * INPUT	: uint32_t ucie_chan
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
static uint32_t ucie_hwemu_setup_wait_freqchange_req(uint32_t ucie_chan, bool is_rc)
{
	uint32_t ret = NORMAL_END;
	uint32_t mask = 0x000002;
	uint32_t expect = 0x000002;
	uint32_t timeout = 0;

//+++<ANHH001>
	uint32_t prev_state;
	uint32_t curr_state;

	//; axi adr = 41
	ucie_hwemu_write(UCIE_APB_BASE(ucie_chan) + 0xE005E8, 0x00004141 );
	prev_state=0xffffffff;
//---<ANHH001>

	while ((ucie_hwemu_read(UCIE_APB_BASE(ucie_chan) + 0xE1003C) & mask) != expect)
	{
//+++<ANHH001>
		// ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMSTATUS_ADD); // for debug
		curr_state = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMSTATUS_ADD); // for debug
		if (prev_state != curr_state)
		{
			printf("curr_state %08x\n", curr_state);
		}
		if ((curr_state & 0x1F) == 0x1C)
		{
			printf("** ch%d TRAIN_ERROR\n", ucie_chan);
			ret = ERROR_END;
			return ret;
		}

		prev_state = curr_state;

//---<ANHH001>

//+++<ANHH001>
		/*
		 * Please imagine a case in which RC is booted too earlier than EP.
		 * RC have to wait until EP is waken up, for this purpose, this software timeout should be enough longer, 
		 * in other words, RC can wait EP within the period of this software loop.
		 * Practically saying,
		 * Timeout never true because this timeout had occurred in a test in which RC is booted about 20ms earlier than EP.
		*/

		if (is_rc) 
		{
			if (timeout >= TIMEOUT_EXECUTE_LOOP_COUNT) 
			{
				printf("** Ch%d TIMEOUT **\n", ucie_chan);
				ret = ERROR_END;
				break;
			}
		}
//---<ANHH001>
        if ((timeout % 10000U) == 0) 
		{  // Print dot every 10000 iterations
            printf(". ");
        }
		timeout++;
	}

	return ret;
}

/*******************************************************
 * NAME		: ucie_hwemu_setup_ack_freqchange
 * FUNCTION	: UCIE Freq Change Acknowledge
 * INPUT	: uint32_t ucie_chan
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
static uint32_t ucie_hwemu_setup_ack_freqchange(uint32_t ucie_chan, bool is_rc)
{
	uint32_t ret = NORMAL_END;

//+++<ANHH001>
	if (UCIE_SETUP_MODE & UCIE_SETUP_MODE_TIMEOUTON_F_ACK)
	{
        //; axi adr = 41
        ucie_hwemu_write( UCIE_APB_BASE(ucie_chan) + 0xE005E8, 0x00004141 );
		// 8ms Timeout is enabled as default value
		printf("RE-ENABLE TIMEOUT\n");
		ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTIMEOUTCTRL1_ADD, ACSM_ACSMTIMEOUTCTRL1_RESET );
	}
//---<ANHH001>

	ucie_hwemu_write(UCIE_APB_BASE(ucie_chan) + 0xE21004, 0x00000001); // ack=1 -> req will negate after 1clk cycle
	ucie_hwemu_write(UCIE_APB_BASE(ucie_chan) + 0xE21004, 0x00000000); // ack=0

	return ret;
}

/*******************************************************
 * NAME		: ucie_hwemu_wait_linkup
 * FUNCTION	: Wait UCIE linkup confirmation
 * INPUT	: uint32_t ucie_chan
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
static uint32_t ucie_hwemu_wait_linkup(uint32_t ucie_chan, bool is_rc)
{
    uint32_t ret = NORMAL_END;
    uint32_t mask = 0x00001F;
	uint32_t expect = 0x000016;
	uint32_t timeout = 0;

//+++<ANHH001>
	uint32_t prev_state;
	uint32_t curr_state;
//---<ANHH001>

	//;  axi0 addres ON
    ucie_hwemu_write(UCIE_APB_BASE(ucie_chan) + 0xE005E8, 0x00004141);

//+++<ANHH001>
	prev_state=0xffffffff;
//---<ANHH001>

    while ((ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMSTATUS_ADD) & mask) != expect)
	{
//+++<ANHH001>
		curr_state=ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMLTSMSTATUS_ADD); // for debug
		if (prev_state != curr_state)
		{
			printf("curr_state %08x\n",curr_state);
		}
		if ((curr_state & 0x1F) == 0x1C)
		{
			printf("** ch%d TRAIN_ERROR\n", ucie_chan);
			ret = ERROR_END;
			return ret;
		}

		prev_state = curr_state;

//---<ANHH001>

//+++<ANHH001>
		/*
		 * Please imagine a case in which RC is booted too earlier than EP.
		 * RC have to wait until EP is waken up, for this purpose, this software timeout should be enough longer, 
		 * in other words, RC can wait EP within the period of this software loop.
		 * Practically saying,
		 * Timeout never true because this timeout had occurred in a test in which RC is booted about 20ms earlier than EP.
		*/
		if (is_rc)
		{
			if (timeout >= TIMEOUT_EXECUTE_LOOP_COUNT) 
			{
				printf("** Ch%d TIMEOUT **\n", ucie_chan);
				ret = ERROR_END;
				break;
			}
		}
//---<ANHH001>
        if ((timeout % 10000U) == 0) 
		{  // Print dot every 10000 iterations
            printf(". ");
        }
		// vTaskDelay(MS_TO_TICKS(1));
		timeout++;
    }

	if (UCIE_SETUP_MODE & UCIE_SETUP_MODE_TIMEOUTON_L_UP)
	{
		// 8ms Timeout is enabled as default value
		printf("RE-ENABLE TIMEOUT\n");
		ucie_hwemu_write( UCIE_AXI_BASE(ucie_chan) + ACSM_ACSMTIMEOUTCTRL1_ADD, ACSM_ACSMTIMEOUTCTRL1_RESET );
	}

    // vTaskDelay(pdMS_TO_TICKS(10));	// TODO: remove it
    return 0;
}

/*******************************************************
 * NAME		: ucie_hwemu_pcie_init_pre
 * FUNCTION	: Setup PCIE pre-linkup
 * INPUT	: uint32_t ucie_chan
 * 			  bool is_rc
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
uint32_t ucie_hwemu_pcie_init_pre(uint32_t ucie_chan, bool is_rc)
{
	uint32_t ret = NORMAL_END;

    //; After linkup
    //axi_adr:H'41
    ucie_hwemu_write(UCIE_APB_BASE(ucie_chan) + 0xE005E8, 0x00004141);

    //; ULT_DWPUBMODECTRL
    ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWPUBMODECTRL_ADD, 0x00000000);

#if 0 /* <ANH001> from ucie_0624_1: not use any more */
    //; ULT_DWTXSBINIT
    //ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWMISCCTRL0_ADD, 0x3D00A400);
    ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWMISCCTRL0_ADD, 0x3D002400); // [15]=0

    //; ULT_DWRXSBINIT
    //ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWMISCCTRL0_ADD, 0x3D04A400);
	ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + DWORD_0_DWMISCCTRL0_ADD, 0x3D002400); // [18:17]=0
#endif /* from ucie_0624_1: not use any more */

	//; ULT_DWPUBMODECTRL_DW1
    ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWPUBMODECTRL_ADD, 0x00000000);

#if 0 /* <ANH001> from ucie_0624_1: not use any more */
    //; ULT_DWTXSBINIT_DW1
    //ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWMISCCTRL0_ADD, 0x3D00A400);
    ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWMISCCTRL0_ADD, 0x3D002400); // [15]=0

	//; ULT_DWRXSBINIT_DW1
    //ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWMISCCTRL0_ADD, 0x3D04A400);
	ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + DWORD_1_DWMISCCTRL0_ADD, 0x3D002400); // [18:17]=0
#endif /* from ucie_0624_1: not use any more */

    //; UCIEDBIADR
    ucie_hwemu_write(UCIE_APB_BASE(ucie_chan) + 0xE005E8, 0x00000000);

	/* 
	 * delete GEN2 GEN3
	 */

    if (is_rc)
	{
        //; SPCIE_CAP_OFF_0CH_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_SPCIE_CAP_SPCIE_CAP_OFF_0CH_REG_ADD, 0x37300127);
        //; SPCIE_CAP_OFF_10H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_SPCIE_CAP_SPCIE_CAP_OFF_10H_REG_ADD, 0x69144272);
        //; SPCIE_CAP_OFF_14H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_SPCIE_CAP_SPCIE_CAP_OFF_14H_REG_ADD, 0x12474905);
        //; SPCIE_CAP_OFF_18H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_SPCIE_CAP_SPCIE_CAP_OFF_18H_REG_ADD, 0x5A055370);
    }
	else
	{
        //; SPCIE_CAP_OFF_0CH_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_SPCIE_CAP_SPCIE_CAP_OFF_0CH_REG_ADD, 0x2228174A);
        //; SPCIE_CAP_OFF_10H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_SPCIE_CAP_SPCIE_CAP_OFF_10H_REG_ADD, 0x78540719);
        //; SPCIE_CAP_OFF_14H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_SPCIE_CAP_SPCIE_CAP_OFF_14H_REG_ADD, 0x03630375);
        //; SPCIE_CAP_OFF_18H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_SPCIE_CAP_SPCIE_CAP_OFF_18H_REG_ADD, 0x31522410);
    }

	/* 
	 * delete GEN3
	 */

    if (is_rc)
	{
        //;PL16G_CAP_OFF_20H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PL16G_CAP_PL16G_CAP_OFF_20H_REG_ADD, 0x8584443A);
        //;PL16G_CAP_OFF_24H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PL16G_CAP_PL16G_CAP_OFF_24H_REG_ADD, 0x6631482A);
    }
	else
	{
        //;PL16G_CAP_OFF_20H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PL16G_CAP_PL16G_CAP_OFF_20H_REG_ADD, 0x390A0826);
        //;PL16G_CAP_OFF_24H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PL16G_CAP_PL16G_CAP_OFF_24H_REG_ADD, 0x76145138);
    }

	/* 
	 * delete GEN3
	 */

    if (is_rc)
	{
        //;	PL32G_CAP_OFF_20H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PL32G_CAP_PL32G_CAP_OFF_20H_REG_ADD, 0x915A4145);
        //;PL32G_CAP_OFF_24H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PL32G_CAP_PL32G_CAP_OFF_24H_REG_ADD, 0x81817220);
    }
	else
	{
        //;
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PL32G_CAP_PL32G_CAP_OFF_20H_REG_ADD, 0x00860582);
        //;PL32G_CAP_OFF_24H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PL32G_CAP_PL32G_CAP_OFF_24H_REG_ADD, 0x96558A61);
    }

	/* 
	 * delete GEN3
	 */

    if (is_rc)
	{
        //;PL64G_LANE_EQ_10H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PL64G_CAP_PL64G_LANE_EQ_10H_REG_ADD, 0xA3057527);
        //;PL64G_LANE_EQ_14H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PL64G_CAP_PL64G_LANE_EQ_14H_REG_ADD, 0x09548A39);
    }
	else
	{
        //;PL64G_LANE_EQ_10H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PL64G_CAP_PL64G_LANE_EQ_10H_REG_ADD, 0x1A226372);
        //;PL64G_LANE_EQ_14H_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PL64G_CAP_PL64G_LANE_EQ_14H_REG_ADD, 0x01674946);
    }

	/* 
	 * delete GEN3
	 */

	return ret;
}

/*******************************************************
 * NAME		: ucie_hwemu_pcie_start_link
 * FUNCTION	: Start UCIE link-up
 * INPUT	: uint32_t ucie_chan
 * 			  bool is_rc
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
static uint32_t ucie_hwemu_pcie_start_link(uint32_t ucie_chan, bool is_rc)
{
	uint32_t ret = NORMAL_END;

#ifdef RCAR_UCIE_V100	/* X5H UCIe v100 */
	printf("PCIE start link up on X5H 0x%x = 0x%08x \n",
			(UCIE_APB_BASE(ucie_chan) + 0xE10004), ucie_hwemu_read(UCIE_APB_BASE(ucie_chan) + 0xE10004));
#else /* AIAAC UCIe v102 */
	printf("PCIE start link up on AI_ACC 0x%x = 0x%08x \n",
			(UCIE_APB_BASE(ucie_chan) + 0xE10010), ucie_hwemu_read(UCIE_APB_BASE(ucie_chan) + 0xE10010));
#endif /* RCAR_UCIE_V100 */
	//;UCIERSTCTRL1
    ucie_hwemu_write(UCIE_APB_BASE(ucie_chan) + 0xE00014, 0x00010001);

    if (is_rc)
	{
		//;UCIEPCR00
        ucie_hwemu_write(UCIE_APB_BASE(ucie_chan) + 0xE21000, 0x00000011);
    }
	else
	{
        //;UCIEPCR00
        ucie_hwemu_write(UCIE_APB_BASE(ucie_chan) + 0xE21000, 0x00000012);
    }

	return ret;
}

/*******************************************************
 * NAME		: ucie_hwemu_pcie_wait_linkup
 * FUNCTION	: Wait PCIE link up confirmation
 * INPUT	: uint32_t ucie_chan
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
static uint32_t ucie_hwemu_pcie_wait_linkup(uint32_t ucie_chan, bool is_rc)
{
    int ret = NORMAL_END;
	uint32_t val;
    uint32_t prev = 0xFFFFFFFF; // For debug logging of value changes
    uint32_t mask = 0x000010;   // Bit 4: rdlh_link
    uint32_t expect = 0x000010;
    uint32_t timeout = 0;

    printf("%s %d START\n", __func__, __LINE__);

	/*
	PCIE link up:wait rdlh_link bit ([4])
	*/

    while (1)
	{
		//;UCIEICR00b
#ifdef RCAR_UCIE_V100	/* X5H UCIe v100 */
		val = ucie_hwemu_read(UCIE_APB_BASE(ucie_chan) + 0xE10004);
#else	/* AIAAC UCIe v102 */
		val = ucie_hwemu_read(UCIE_APB_BASE(ucie_chan) + 0xE10010);
#endif /* RCAR_UCIE_V100 */
		if (val != prev)
		{
			printf("0x%08x\n", val); // temporal for debug
			prev = val;
		}

		if ((val & mask) == expect)
		{
			break;
		}

        if (timeout >= TIMEOUT_EXECUTE_LOOP_COUNT)
        {
			printf("** ch%d TIMEOUT\n", ucie_chan);
            ret = ERROR_END;
            break;
        }
        if ((timeout % 10000U) == 0) 
		{  // Print dot every 10000 iterations
            printf(". ");
        }
        timeout++;
    }

	printf("!\n"); // need wait
    // vTaskDelay(pdMS_TO_TICKS(5));	// TODO: remove it

    return ret;
}

/*******************************************************
 * NAME		: ucie_hwemu_pcie_init_post
 * FUNCTION	: Setup PCIE post link-up
 * INPUT	: uint32_t ucie_chan
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
static uint32_t ucie_hwemu_pcie_init_post(uint32_t ucie_chan, bool is_rc)
{
	uint32_t ret = NORMAL_END;

    //; UCIEDBIADR
    /////////////////////////////////////////////////////////////////////////////////////////////////
    //axi_adr:H'00
    ucie_hwemu_write(UCIE_APB_BASE(ucie_chan) + 0xE005E8, 0x00000000);

    //;DEVICE_CONTROL_DEVICE_STATUS
	ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PCIE_CAP_DEVICE_CONTROL_DEVICE_STATUS_ADD, 0x00102150);

    if (!is_rc)
	{
		ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PCIE_CAP_DEVICE_CONTROL_DEVICE_STATUS_ADD + RCAR_UCIE_FN_OFS(1), 0x00102150);
    }

    if (is_rc)
	{
        //;BAR0_MASK_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_BAR0_REG_ADD + RCAR_UCIE_FN_OFS(1), 0x00000000);
        //;BAR1_MASK_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_BAR1_REG_ADD + RCAR_UCIE_FN_OFS(1), 0x00000000);

        //;BAR0_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_BAR0_REG_ADD, 0x09100004);
        //;BAR1_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_BAR1_REG_ADD, 0x00000000);
        //;TRGT_MAP_CTRL_OFF
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PORT_LOGIC_TRGT_MAP_CTRL_OFF_ADD, 0x00000040);
        //;SEC_STAT_IO_LIMIT_IO_BASE_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_SEC_STAT_IO_LIMIT_IO_BASE_REG_ADD, 0x00000101);
        //;IO_LIMIT_UPPER_IO_BASE_UPPER_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_ADD, 0x00000000);
        //;SEC_STAT_IO_LIMIT_IO_BASE_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_SEC_STAT_IO_LIMIT_IO_BASE_REG_ADD, 0x00004F40);
        //;MEM_LIMIT_MEM_BASE_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_MEM_LIMIT_MEM_BASE_REG_ADD, 0x08FF08C0);
        //;PREF_MEM_LIMIT_PREF_MEM_BASE_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_ADD, 0x00010001);
        //;PREF_BASE_UPPER_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_PREF_BASE_UPPER_REG_ADD, 0x00000000);
        //;PREF_LIMIT_UPPER_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_PREF_LIMIT_UPPER_REG_ADD, 0x00000000);
        //;PREF_MEM_LIMIT_PREF_MEM_BASE_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_PREF_MEM_LIMIT_PREF_MEM_BASE_REG_ADD, 0xB00FB000);
        //;DATA_LINK_FEATURE_EXT_HDR_OFF
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_CXL_2_0_EXT_CAP_CXL_2_0_ALT_MEM_BASE_LIMIT_OFF_ADD, 0xAFEF0930);
        //;DATA_LINK_FEATURE_CAP_OFF
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_CXL_2_0_EXT_CAP_CXL_2_0_ALT_PREFETCH_MEM_BASE_LIMIT_OFF_ADD, 0x00000000);

        //;RASDP_ERROR_TRACER_INJ_CTRL_OFF
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_CXL_2_0_EXT_CAP_CXL_2_0_CTRL_ALT_BUS_BASE_LIMIT_OFF_ADD, 0x00000000);
        //;RASDP_ERROR_TRACER_INJ_CTRL_OFF
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_CXL_2_0_EXT_CAP_CXL_2_0_CTRL_ALT_BUS_BASE_LIMIT_OFF_ADD, 0x00000000);
        //;RASDP_ERROR_TRACER_INJ_CTRL_OFF
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_CXL_2_0_EXT_CAP_CXL_2_0_CTRL_ALT_BUS_BASE_LIMIT_OFF_ADD, 0x00050000);
        //;RASDP_ERROR_TRACER_INJ_CTRL_OFF
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_CXL_2_0_EXT_CAP_CXL_2_0_CTRL_ALT_BUS_BASE_LIMIT_OFF_ADD, 0x6D050000);
    }
	else
	{
        //;BAR0_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_BAR0_REG_ADD, 0x08C00004);
        //;BAR1_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_BAR1_REG_ADD, 0x00000000);
        //;BAR0_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_BAR0_REG_ADD + RCAR_UCIE_FN_OFS(1), 0x08E00004);
        //;BAR1_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_BAR1_REG_ADD + RCAR_UCIE_FN_OFS(1), 0x00000000);
        //;SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_ADD, 0x08D00000);
        //;
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_SEC_LAT_TIMER_SUB_BUS_SEC_BUS_PRI_BUS_REG_ADD + RCAR_UCIE_FN_OFS(1), 0x08F00000);
        //;MEM_LIMIT_MEM_BASE_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_MEM_LIMIT_MEM_BASE_REG_ADD, 0x00004001);
        //;
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_MEM_LIMIT_MEM_BASE_REG_ADD + RCAR_UCIE_FN_OFS(1), 0x00004101);
        //;IO_LIMIT_UPPER_IO_BASE_UPPER_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_ADD, 0xB0000001);
        //;EXP_ROM_BASE_ADDR_REG
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_TYPE1_HDR_IO_LIMIT_UPPER_IO_BASE_UPPER_REG_ADD + RCAR_UCIE_FN_OFS(1), 0xB0010001);
        //;TRGT_MAP_CTRL_OFF
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PORT_LOGIC_TRGT_MAP_CTRL_OFF_ADD, 0x00000054);
        //;TRGT_MAP_CTRL_OFF
        ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + PF0_PORT_LOGIC_TRGT_MAP_CTRL_OFF_ADD + RCAR_UCIE_FN_OFS(1), 0x00010054);
    }

	return ret;
}

//+++<ANH001>
#if 0 /* Should removes because CR52 cannot access more than 32bit PA */
void ucie_hwemu_init_inbound(uint32_t ucie_chan, uint64_t base_addr, uint64_t target_addr)
{
    uint32_t val;

    val = ucie_hwemu_read(UCIE_AXI_BASE(ucie_chan) + 0x300510) | 0x000FFFF0;		// IATU LIMIT_ADDR_OFF_INBOUND_2
    ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + 0x300510, val);

    ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + 0x300508, base_addr & 0xFFFFFFFF);	// IATU LWR_BASE_ADDR_OFF_INBOUND_2
    ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + 0x30050C, base_addr >> 32);		// IATU UPPER_BASE_ADDR_OFF_INBOUND_2
   // ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + 0x300514, target_addr &  0xFFFFFFFF);   // IATU LWR_TARGET_ADDR_OFF_INBOUND_2
    ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + 0x300514, target_addr);   // IATU LWR_TARGET_ADDR_OFF_INBOUND_2
    //ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + 0x300518, target_addr >> 32);		// IATU UPPER_TARGET_ADDR_OFF_INBOUND_2
	ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + 0x300518, 0x00000000);
    ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + 0x300500, 0x00000000);			// IATU REGION_CTRL_1_OFF_INBOUND_2
    ucie_hwemu_write(UCIE_AXI_BASE(ucie_chan) + 0x300504, 0x80000000);			// IATU REGION_CTRL_2_OFF_INBOUND_2: [31] REDION_EN=1, [30] MATCH_MODE=0 (Address Match Mode)

    vTaskDelay(MS_TO_TICKS(1));
}
#endif
//---<ANH001>

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
 * NAME		: ucie_pwrmngctrl_reg_modify
 * FUNCTION	: Setup UCIE
 * INPUT	: uint32_t address
 * OUTPUT	: none
 * RETURN	: none
 ******************************************************/
void ucie_pwrmngctrl_reg_modify(uint32_t address)
{
    uint32_t value = ucie_hwemu_read(address);
    value &= CLEAR_BIT_MASK;                 // Clear sys_aux_pwr_det bit (bit 4 )
    value |= SET_BIT_MASK;                   // Set app_ready_entr_l23 bit (bit 6)
    ucie_hwemu_write(address, value);          // Write back the modified value
}

void Ucie_PwrOn(uint32_t ucie_chan)
{
	uint32_t ucie_core_ms_bit;
	uint32_t ucie_peri_ms_bit;
	volatile uintptr_t uciepwrmngctrl;
	uint32_t pll_num;
	// uint32_t UCIE_APB_BASE;

    if (ucie_chan == UCIE_CH0)
	{
		ucie_peri_ms_bit=2;
		ucie_core_ms_bit=0;
		// UCIE_APB_BASE=0xDC000000;
		pll_num=PLL9_0;
    }
	else
	{
		ucie_peri_ms_bit=6;
		ucie_core_ms_bit=4;
		// UCIE_APB_BASE=0xDD000000;
		pll_num=PLL9_1;
    }

//  uciepwrmngctrl=UCIE_APB0_UCIEPWRMNGCTRL or UCIE_APB1_UCIEPWRMNGCTRL;
	uciepwrmngctrl = UCIE_APB_BASE(ucie_chan) + 0x00E00070;

    printf("uciepwrmngctrl: 0x%X\n", uciepwrmngctrl);
	
    uint32_t ret;
    //ret = mdlc_ctrlModulePowerGating(MOD_HIER_HSCS, 1);
    ret = R_StateManager_PowerOn(X5H_POWER_DOMAIN_ID_UCI);
    if(ret) {
        printf("SCP MPG set fail\n");
    }
    else {
        printf("SCP MPG set done\n");
    }

    //mdlc_ms_module_run_bit(MODULE_NUM_HSCS, REG_NUM_HSCS, ucie_peri_ms_bit); 
    ret = R_StateManager_ClockOn(X5H_CLOCK_ID_MDLC_UCIE12);
    ret = R_StateManager_ResetDeassert(X5H_RESET_DOMAIN_ID_UCIE12);
    if(ret) {
        printf("SCP MS peri set fail\n");
    }
    else {
        printf("SCP MS peri set done\n");
    }

	/*UCIE_APB[0/1]_UCIEPWRMNGCTRL register modify*/
	printf("uciepwrmngctrl val before: 0x%X\n", ucie_hwemu_read(uciepwrmngctrl));
	ucie_pwrmngctrl_reg_modify(uciepwrmngctrl);
	printf("uciepwrmngctrl val after: 0x%X\n", ucie_hwemu_read(uciepwrmngctrl));
    

    if (UCIE_SETUP_MODE & UCIE_SETUP_MODE_HOLDLTSSM)
	{
		//UCIERSTCTRL1.app_ltssm_enable=1 : hold ltssm state transition
		printf("HOLDLTSSM\n");
		ucie_hwemu_write( UCIE_APB_BASE(ucie_chan) + 0xE00014, 0x00000001 );
    }

	switch_clock_source_pll(pll_num);
	
    //mdlc_ms_module_run_bit(MODULE_NUM_HSCS, REG_NUM_HSCS, ucie_core_ms_bit);
    ret = R_StateManager_ClockOn(X5H_CLOCK_ID_MDLC_UCIE11);
    ret = R_StateManager_ResetDeassert(X5H_RESET_DOMAIN_ID_UCIE11);
    if(ret) {
        printf("SCP MS core set fail\n");
    }
    else {
        printf("SCP MS core set done\n");
    }
}

void Ucie_PwrOff(uint32_t ucie_chan)
{
	uint32_t ucie_core_ms_bit;
	uint32_t ucie_peri_ms_bit;
	// uint32_t UCIE_APB_BASE;

	if (ucie_chan == UCIE_CH0)
	{
		ucie_peri_ms_bit=2;
		ucie_core_ms_bit=0;
		// UCIE_APB_BASE=0xDC000000;
	}
	else
	{
		ucie_peri_ms_bit=6;
		ucie_core_ms_bit=4;
		// UCIE_APB_BASE=0xDD000000;
	}
	mdlc_transition_ms(MODULE_NUM_HSCS, REG_NUM_HSCS, ucie_core_ms_bit, MS_RESET);
	mdlc_transition_ms(MODULE_NUM_HSCS, REG_NUM_HSCS, ucie_peri_ms_bit, MS_RESET);
}

static uint32_t ucie_hwemu_hw_init_rc(uint32_t ucie_chan)
{
	// ===== EP-specific initialization =====
	uint32_t ret = NORMAL_END;
	uint32_t timeout = 0;
	uint32_t *w;
	
	printf("[RC] HW_INIT (Ch%d) \n", ucie_chan);

	w = (uint32_t *)(0xbffff100);
    //if (0 != w[0])
	//{
		//UCIE_SETUP_MODE = w[0];
	//}

	w = (uint32_t *)(0xbffff000);
	//wait_time(w[0]); // delay 0ms

LABEL_RETRY:
// -----------------------------------------------------------------------------

    printf("[Step0] power on\n");
	Ucie_PwrOn(ucie_chan);

	// Step 1: UCIE Init Pre
    printf("[Step1] ucie_init_pre\n");	
    ret = ucie_hwemu_init_pre(ucie_chan, true);
	printf("[Step1] ucie_init_pre(%d)\n", ret);
	//wait_time(w[1]); // delay 2ms
    //wait_time(w[2]); // slot
    //wait_time(w[3]); // slot
	//wait_time(w[4]);

	// -----------------------------------------------------------------------------
	// Step 2: Start UCIE Link
	printf("[Step2] start_link\n");
    ret = ucie_hwemu_start_link(ucie_chan, true);
    //wait_time(w[5]);
	printf("[Step2] start_link(%d)\n", ret);

	// -----------------------------------------------------------------------------
	// Step 3: Wait for frequency change request
    printf("[Step3] freqchange_req\n");
	ret = ucie_hwemu_setup_wait_freqchange_req(ucie_chan, true);
	printf("[Step3] freqchange_req(%d)\n", ret);
	//if (NORMAL_END != ret)
	//{
		//goto LABEL_ERROR;
	//}

	// -----------------------------------------------------------------------------
	// Step 4: Acknowledge freq change
    printf("[Step4] ack_freqchange\n");
	ret = ucie_hwemu_setup_ack_freqchange(ucie_chan, true);
	printf("[Step4] ack_freqchange(%d)\n", ret);

	// -----------------------------------------------------------------------------
	// Step 5: Wait for link-up
    printf("[Step5] wait_linkup\n");
	ret = ucie_hwemu_wait_linkup(ucie_chan, true);
    //wait_time(w[6]);
	printf("[Step5] wait_linkup(%d)\n", ret);
	//if (NORMAL_END != ret)
	//{
		//goto LABEL_ERROR;
	//}

	// -----------------------------------------------------------------------------
	// Step 6: PCIe initialization
    printf("[Step6] pcie_init_pre(0)\n");
	ucie_hwemu_pcie_init_pre(ucie_chan, true);
	//wait_time(w[7]);

	// -----------------------------------------------------------------------------
	// Step 7: Start PCIe link
    printf("[Step7] pcie_start_link(0)\n");
    ucie_hwemu_pcie_start_link(ucie_chan, true);
	// //wait_time(w[7]);

	// -----------------------------------------------------------------------------
	// Step 8: Wait for PCIe link-up
	printf("[Step8] pcie_wait_linkup\n");
    ret = ucie_hwemu_pcie_wait_linkup(ucie_chan, true);
	printf("[Step8] pcie_wait_linkup(%d)\n", ret);
	//if (NORMAL_END != ret)
	//{
		//goto LABEL_ERROR;
	//}

	// -----------------------------------------------------------------------------
	// Step 9: PCIe post init
    printf("[Step9] pcie_init_post(0)\n");
	ucie_hwemu_pcie_init_post(ucie_chan, true);
	//wait_time(1);

	return NORMAL_END;

// -----------------------------------------------------------------------------
LABEL_ERROR:
	if (UCIE_SETUP_MODE & UCIE_SETUP_MODE_RETRY)
	{
		Ucie_PwrOff(ucie_chan);
		goto LABEL_RETRY;
	}

	return ERROR_END;
}

static uint32_t ucie_hwemu_hw_init_ep(uint32_t ucie_chan)
{
	// ===== EP-specific initialization =====
	uint32_t ret = NORMAL_END;
	uint32_t timeout = 0;
	uint32_t *w;

	printf("[EP] HW_INIT (Ch%d) \n", ucie_chan);

	w = (uint32_t *)(0xbffff100);
    //if (0 != w[0])
	//{
		//UCIE_SETUP_MODE = w[0];
	//}

	w = (uint32_t *)(0xbffff000);
	// Wait and notify RC to proceed
	//wait_time(w[0]); // delay 10ms
    //wait_time(w[1]); // delay 2ms
    //wait_time(w[2]); // slot
    //wait_time(w[3]); // slot
    // //wait_time(w[4]);  // delay 0ms
    vTaskDelay(10000);
LABEL_RETRY:
// -----------------------------------------------------------------------------

	Ucie_PwrOn(ucie_chan);
	
	// Step 1: UCIE Init Pre
	ret = ucie_hwemu_init_pre(ucie_chan, false);
	//wait_time(w[4]);
	printf("[Step1] ucie_init_pre(%d)\n", ret);
	// vTaskDelay(MS_TO_TICKS(1));
	// //wait_time(WAIT_REG[1]);	// 0 ms

	// -----------------------------------------------------------------------------
	// Step 2: Start UCIE Link
	ret = ucie_hwemu_start_link(ucie_chan, false);
	//wait_time(w[5]);
	printf("[Step2] start_link(%d)\n", ret);

	// -----------------------------------------------------------------------------
	// Step 4: Wait for frequency change request
	ret = ucie_hwemu_setup_wait_freqchange_req(ucie_chan, false);
	printf("[Step4] freqchange_req(%d)\n", ret);
	if (NORMAL_END != ret)
	{
		goto LABEL_ERROR;
	}

	// -----------------------------------------------------------------------------
	// Step 3: Acknowledge freq change
	ret = ucie_hwemu_setup_ack_freqchange(ucie_chan, false);
	printf("[Step3] ack_freqchange(%d)\n", ret);

	// -----------------------------------------------------------------------------
	// Step 5: Wait for link-up
	ret = ucie_hwemu_wait_linkup(ucie_chan, false);
	//wait_time(w[6]);
	printf("[Step5] wait_linkup(%d)\n", ret);
	if (NORMAL_END != ret)
	{
		goto LABEL_ERROR;
	}

	// -----------------------------------------------------------------------------
	// Step 6: PCIe initialization
	ucie_hwemu_pcie_init_pre(ucie_chan, false);
	//wait_time(w[7]);
	printf("[Step6] pcie_init_pre(0)\n");

	// -----------------------------------------------------------------------------
	// Step 7: Start PCIe link
	ucie_hwemu_pcie_start_link(ucie_chan, false);
	// wait_time(w[7]);
	printf("[Step7] pcie_start_link(0)\n");

	// -----------------------------------------------------------------------------
	// Step 8: Wait for PCIe link-up
	ret = ucie_hwemu_pcie_wait_linkup(ucie_chan, false);
	printf("[Step8] pcie_wait_linkup(%d)\n", ret);
	if (NORMAL_END != ret)
	{
		goto LABEL_ERROR;
	}

	// -----------------------------------------------------------------------------
	// Step 9: PCIe post init
	ucie_hwemu_pcie_init_post(ucie_chan, false);
	printf("[Step9] pcie_init_post(0)\n");
	//wait_time(1);

	return NORMAL_END;

// -----------------------------------------------------------------------------
LABEL_ERROR:
	if (UCIE_SETUP_MODE & UCIE_SETUP_MODE_RETRY)
	{
		Ucie_PwrOff(ucie_chan);
		goto LABEL_RETRY;
	}

	return ERROR_END;
}

/*******************************************************
 * NAME		: ucie_hwemu_hw_init
 * FUNCTION	: Setup UCIE
 * INPUT	: uint32_t ucie_chan
 * 			  bool is_rc
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
uint32_t ucie_hwemu_hw_init(uint32_t ucie_chan, bool is_rc)
{
	return is_rc ? ucie_hwemu_hw_init_rc(ucie_chan)
	             : ucie_hwemu_hw_init_ep(ucie_chan);

#if 0
	if (!is_rc)
	{
		// AI ACC initialization
		ucie_hwemu_hw_ai_acc_init();
		printf("%s %d ucie_hwemu_hw_ai_acc_init(UCIe Ch%d, is_rc%d)\n",
				__func__, __LINE__, ucie_chan, is_rc);
		// vTaskDelay(MS_TO_TICKS(1));
		wait_time(RELEASE, TIMEOUT_EXECUTE_LOOP_COUNT);		// Init OK, RC can start
	}
	else
	{
		wait_time(NEED_WAIT, TIMEOUT_EXECUTE_LOOP_COUNT);	// RC wait for EP init, maximum 100ms
	}

    ret = ucie_hwemu_init_pre(ucie_chan, is_rc);
	printf("%s %d ucie_hwemu_init_pre(UCIe Ch%d, is_rc%d)\n",
			__func__, __LINE__, ucie_chan, is_rc);
	if (!is_rc)
	{
    	wait_time(NEED_WAIT, 1);	// Add some very short delay for stable hw init
	}
	else
	{
		wait_time(NEED_WAIT, 8192);	// Add some delay for x5h hw init
	}

    ret = ucie_hwemu_start_link(ucie_chan, is_rc);
	printf("%s %d ucie_hwemu_start_link(UCIe Ch%d, is_rc%d)\n",
			__func__, __LINE__, ucie_chan, is_rc);
    // vTaskDelay(MS_TO_TICKS(1));
	wait_time(NEED_WAIT, 1);	// Add some very short delay for stable hw init

	// wait_ep_init(is_rc);
	// wait_time(w[2]);

    ret = ucie_hwemu_setup_wait_freqchange_req(ucie_chan, is_rc); // wait UCIE1
	printf("%s %d ucie_hwemu_setup_wait_freqchange_req(UCIe Ch%d) returned %d\n",
			__func__, __LINE__, ucie_chan, ret);
	if (ret != NORMAL_END)
	{
		return ret;
	}
	// vTaskDelay(MS_TO_TICKS(1));
	wait_time(NEED_WAIT, 1);	// Add some very short delay for stable hw init

	ret = ucie_hwemu_setup_ack_freqchange(ucie_chan, is_rc); // EP
	printf("%s %d ucie_hwemu_setup_ack_freqchange(UCIe Ch%d) returned %d\n",
			__func__, __LINE__, ucie_chan, ret);
    // vTaskDelay(MS_TO_TICKS(1));
	wait_time(NEED_WAIT, 1);	// Add some very short delay for stable hw init

    ret = ucie_hwemu_wait_linkup(ucie_chan, is_rc);
    if (ret != NORMAL_END)
	{
		printf("%s %d ucie_hwemu_wait_linkup timedout!\n", __func__, __LINE__);
		return ret;
	}
	else
	{
		printf("%s %d ucie_hwemu_wait_linkup OK!\n", __func__, __LINE__);
	}
	// vTaskDelay(MS_TO_TICKS(1));
	wait_time(NEED_WAIT, 1);	// Add some very short delay for stable hw init

	// wait_ep_init(is_rc);
	// wait_time(w[3]);

	// PCIE linkup
	if (!is_rc)
	{
		// EP starts first, followed by RC
    	ucie_hwemu_pcie_init_pre(ucie_chan, is_rc);
		wait_time(RELEASE, TIMEOUT_EXECUTE_LOOP_COUNT);		// Init OK, RC can start
	}
	else
	{
		wait_time(NEED_WAIT, TIMEOUT_EXECUTE_LOOP_COUNT);	// RC wait for EP init
		ucie_hwemu_pcie_init_pre(ucie_chan, is_rc);
	}
    // vTaskDelay(MS_TO_TICKS(1));
	wait_time(NEED_WAIT, 1);	// Add some very short delay for stable hw init

	// wait_ep_init(is_rc);
	// wait_time(w[4]);

	// EP starts link up, followed by RC
	if (!is_rc)
	{
    	ucie_hwemu_pcie_start_link(ucie_chan, is_rc);
		wait_time(RELEASE, TIMEOUT_EXECUTE_LOOP_COUNT);		// Init OK, RC can start
	}
	else
	{
		wait_time(NEED_WAIT, TIMEOUT_EXECUTE_LOOP_COUNT);	// RC wait for EP init
		ucie_hwemu_pcie_start_link(ucie_chan, is_rc);
	}
    // vTaskDelay(MS_TO_TICKS(1));
	wait_time(NEED_WAIT, 1);	// Add some very short delay for stable hw init

	// wait_ep_init(is_rc);
	// wait_time(w[5]);

    ret = ucie_hwemu_pcie_wait_linkup(ucie_chan, is_rc);
    if (ret != NORMAL_END)
	{
        printf("%s %d wait PCIe linkup timedout!\n", __func__, __LINE__);
    }
	else
	{
        printf("%s %d PCIe wait linkup OK!\n", __func__, __LINE__);
    }
	wait_time(NEED_WAIT, 1);	// Add some very short delay for stable hw init

    // vTaskDelay(MS_TO_TICKS(1));
	// wait_time(w[6]); // Approximate delay of 20ms

    ucie_hwemu_pcie_init_post(ucie_chan, is_rc);
    // vTaskDelay(MS_TO_TICKS(1));
	wait_time(NEED_WAIT, 1);	// Add some very short delay for stable hw init

    return NORMAL_END;
#endif
}

/*******************************************************
 * NAME		: ucie_hwemu_hw_ai_acc_init
 * FUNCTION	: Setup UCIE
 * INPUT	: none
 * OUTPUT	: none
 * RETURN	: none
 ******************************************************/
void ucie_hwemu_hw_ai_acc_init(void)
{
    uint32_t ucie_core_ms_init_bit_array[] = {0, 4}; //0,1 and 4,5
    uint32_t ucie_peripheral_ms_init_bit_array[] = {2, 6}; // 2,3 and 6,7

    mdlc_ms_init(MODULE_NUM_HSCS, REG_NUM_HSCS, ucie_peripheral_ms_init_bit_array); //uice peripheral ms init

    /*UCIE_APB[0/1]_UCIEPWRMNGCTRL register modify*/
//    ucie_hwemu_read(UCIE_APB0_UCIEPWRMNGCTRL);
    ucie_pwrmngctrl_reg_modify(UCIE_APB0_UCIEPWRMNGCTRL);
//    ucie_hwemu_read(UCIE_APB0_UCIEPWRMNGCTRL);
//    ucie_hwemu_read(UCIE_APB1_UCIEPWRMNGCTRL);
    ucie_pwrmngctrl_reg_modify(UCIE_APB1_UCIEPWRMNGCTRL);
//    ucie_hwemu_read(UCIE_APB1_UCIEPWRMNGCTRL);

    switch_clock_source_pll(PLL9_0);
    switch_clock_source_pll(PLL9_1);

    mdlc_ms_init(MODULE_NUM_HSCS, REG_NUM_HSCS, ucie_core_ms_init_bit_array); //uice ip core ms init

    // vTaskDelay(MS_TO_TICKS(30));
	// vTaskDelay(MS_TO_TICKS(5));
	// wait_time(NEED_WAIT, 62000); // Approximate delay of 30ms
}

// -----------------------------------------------------------------------------
// Function Body: UCIE AI-ACC LOOPBACK
// -----------------------------------------------------------------------------
/*******************************************************
 * NAME		: ucie_hwemu_hw_init_loopback
 * FUNCTION	: Setup UCIE loopback test on AI-ACC
 * INPUT	: none
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
uint32_t ucie_hwemu_hw_init_loopback(void)
{
    uint32_t ret = NORMAL_END;
//#if (BOARD == AI_ACC)
    ucie_hwemu_hw_ai_acc_init();
	vTaskDelay(MS_TO_TICKS(1));
//#endif
////missing 
	/*====================================*/
	/* PCIE memory area                   */
	/*====================================*/
	//ucie0_mem = 0x20000000000; // D2D (UCIe ch0/no coherent (CXL))
	//ucie1_mem = 0x24000000000; // D2D (UCIe ch1/no coherent (CXL))

	//ucie0_tgt = 0x4E000000; // UCIE0 INBOUND: translate from 0x24000000000 to 0x4E000000
	//ucie1_tgt = 0x4F000000; // UCIE1 INBOUND: translate from 0x20000000000 to 0x4F000000
	//tmubase = TMU0_BASE; // use TMU0 ch0
	//tmuch = 0;

	//mem_write16(tmubase + TMU_TCR0_OFFSET, 0x0000); // TPSC=(Input-Clock)/4
	//mem_write32(tmubase + TMU_TCOR0_OFFSET, 0xFFFFFFFF);
	//mem_write32(tmubase + TMU_TCNT0_OFFSET, 0xFFFFFFFF);

	/*====================================*/
	/* DMA                                */
	/*====================================*/
	//dma_size = 0x00100000; // 1MB

//////////////////////////////////////	
    ucie_hwemu_init_pre(0, true);
    printf("%s %d UCIe init\n", __func__, __LINE__);
    ucie_hwemu_init_pre(1, false);
    printf("%s %d UCIe init\n", __func__, __LINE__);
    vTaskDelay(MS_TO_TICKS(1));

    ucie_hwemu_start_link(0, true);
    printf("%s %d UCIe start link\n", __func__, __LINE__);    
    ucie_hwemu_start_link(1, false);
    printf("%s %d UCIe start link\n", __func__, __LINE__);
    vTaskDelay(MS_TO_TICKS(1));
	ret = ucie_hwemu_setup_wait_freqchange_req(0, true); // wait UCIE0
	if (ret != NORMAL_END)
	{
		return ret;
	}
	ucie_hwemu_setup_ack_freqchange(0, true); // RC
    vTaskDelay(MS_TO_TICKS(1));
	ret = ucie_hwemu_setup_wait_freqchange_req(1, false); // wait UCIE1
	if (ret != NORMAL_END)
	{
		return ret;
	}
	ucie_hwemu_setup_ack_freqchange(1, false); // EP
    vTaskDelay(MS_TO_TICKS(1));

	
    /* Do not return error for waiting link-up channel 0,
       give it a try with channel 1  */
    ucie_hwemu_wait_linkup(0, true);
    ret = ucie_hwemu_wait_linkup(1, false);
	if (ret != NORMAL_END)
	{
        printf("%s %d UCIe wait linkup timedout!\n", __func__, __LINE__);
        return ret;
    }

    printf("%s %d UCIe wait linkup OK!\n", __func__, __LINE__);

    ucie_hwemu_pcie_init_pre(0, true);
    ucie_hwemu_pcie_init_pre(1, false);
    vTaskDelay(MS_TO_TICKS(1));
    ucie_hwemu_pcie_start_link(1, false);//EP
    ucie_hwemu_pcie_start_link(0, true);//RC

    vTaskDelay(MS_TO_TICKS(1));

    /* Do not return error for waiting link-up channel 0,
       give it a try with channel 1  */
    ucie_hwemu_pcie_wait_linkup(0, true);
    ret = ucie_hwemu_pcie_wait_linkup(1, false);
	if (ret != NORMAL_END)
	{
        printf("%s %d wait PCIe linkup timedout!\n", __func__, __LINE__);
		return ret;
    }

    vTaskDelay(MS_TO_TICKS(20));

    ucie_hwemu_pcie_init_post(0, true);
	ucie_hwemu_pcie_init_post(1, false);
	ucie_hwemu_write(0xD8000004, 0x00110007); // ???

	ucie_hwemu_write(0xD9000080, 0x00000000); // ???

	ucie_hwemu_write(0xD90008BC, 0x040BFF4A); // ???
	ucie_hwemu_write(0xD90008BC, 0x040BFF4E); // ???
    vTaskDelay(MS_TO_TICKS(1));

    return ret;
}
// -----------------------------------------------------------------------------
// Function Body: UCIE AI-ACC LOOPBACK
// -----------------------------------------------------------------------------
