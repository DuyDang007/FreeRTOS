/*******************************************************************************
  Includes
*******************************************************************************/
#include <stdio.h>
#include "mdlcdrv.h"
#include "reg_x5h_mdlc.h"
/*******************************************************************************
  Defines
*******************************************************************************/
uint32_t pdidMaxNums[28] = {
	[MOD_HIER_VIPN] =  1,
	[MOD_HIER_VIPS] =  1,
	[MOD_HIER_VIO]  =  7,
	[MOD_HIER_PERE] =  1,
	[MOD_HIER_PERW] =  0,
	[MOD_HIER_DDR0] =  0,
	[MOD_HIER_DDR1] =  0,
	[MOD_HIER_DDR2] =  0,
	[MOD_HIER_DDR3] =  0,
	[MOD_HIER_DDR4] =  0,
	[MOD_HIER_DDR5] =  0,
	[MOD_HIER_DDR6] =  0,
	[MOD_HIER_DDR7] =  0,
	[MOD_HIER_HSCN] =  6,
	[MOD_HIER_RT]   = 11,
	[MOD_HIER_TOP]  =  0,
	[MOD_HIER_HSCS] =  1,
	[MOD_HIER_IMN]  =  1,
	[MOD_HIER_IMS]  =  1,
	[MOD_HIER_GPC]  =  3,
	[MOD_HIER_DSP]  =  4,
	[MOD_HIER_MM]   =  0,
	[MOD_HIER_NPU0] = 13,
	[MOD_HIER_NPU1] = 13,
	[MOD_HIER_CMNN] = 20,
	[MOD_HIER_CMNS] = 19,
	[MOD_HIER_SCP]  =  0,
	[MOD_HIER_AON]  =  0,
};

/*******************************************************************************
  Internal Functions
*******************************************************************************/
/*******************************************************
 * NAME		: mdlc_mpg_checkStReg
 * FUNCTION	: Check & Wait MPG status
 * INPUT	: mod_hier		Module Hierarchy
 *			: pdid			Power Domain ID
 * OUTPUT	: none
 * RETURN	: NORMAL_END	MPDGS == MPDG
 *			: 1		MPDGS != MPDG
 ******************************************************/

static uint32_t mem_read32(const volatile uintptr_t addr)
{
    return *((volatile uint32_t*)(addr));
}

static void mem_write32(volatile uintptr_t addr, uint32_t data)
{
    *((volatile uint32_t*)(addr)) = data;
}


static uint32_t mdlc_mpg_checkStReg(uintptr_t base, uint32_t pdid)
{
	uint32_t ret = 1;
	uint32_t mpdgs, mpdg;
	uint32_t timeout = 500000;

	while (--timeout) {
		mpdgs = mem_read32(MDLC_MPDGS(base, pdid));
		mpdg  = mem_read32(MDLC_MPDG(base, pdid));
		if (mpdgs == mpdg) {
			ret = 0;
			break;
		}
	}
	if (timeout == 0) {
		printf("** MPG Status and Trigger are unmatched.\n");
		printf("     MPDGS ( 0x%08X )= 0x%08X\n", MDLC_MPDGS(base, pdid), mpdgs);
		printf("     MPDG  ( 0x%08X )= 0x%08X\n", MDLC_MPDG(base, pdid), mpdg);
	}

	return ret;
}
/*******************************************************
 * NAME		: mdlc_ms_checkStReg
 * FUNCTION	: Check & Wait MS status
 * INPUT	: mod_hier		Module Hierarchy
 *			: regNo			Register No
 * OUTPUT	: none
 * RETURN	: 0	MSRESS == MSRES
 *			: 1		MSRESS != MSRES
 ******************************************************/
static uint32_t mdlc_ms_checkStReg(uintptr_t base, uint32_t regNo)
{
	uint32_t ret = 1;
	uint32_t msress, msres;
	uint32_t timeout = 500000;

	while (--timeout) {
		msress = mem_read32(MDLC_MSRESS(base, regNo));
		msres  = mem_read32(MDLC_MSRES(base, regNo));
		if (msress == msres) {
			ret = 0;
			break;
		}
	}
	if (timeout == 0) {
		printf("** MS Status and Trigger are unmatched.\n");
		printf("     MSRESS ( 0x%08X )= 0x%08X\n", MDLC_MSRESS(base, regNo), msress);
		printf("     MSRES  ( 0x%08X )= 0x%08X\n", MDLC_MSRES(base, regNo), msres);
	}

	return ret;
}

/*******************************************************************************
  Functions
*******************************************************************************/
/*******************************************************
 * NAME		: mdlc_returnBase
 * FUNCTION	: return the base address of each hierarchy
 * INPUT	: mod_hier		Module Hierarchy
 * OUTPUT	: none
 * RETURN	: Normal: base address
 * 			  Error : 0x0
 ******************************************************/
uintptr_t mdlc_returnBase(uint32_t mod_hier)
{
	switch (mod_hier) {
		case MOD_HIER_VIPN: return MDLC_BASE_VIPN;
		case MOD_HIER_VIPS: return MDLC_BASE_VIPS;
		case MOD_HIER_VIO : return MDLC_BASE_VIO ;
		case MOD_HIER_PERE: return MDLC_BASE_PERE;
		case MOD_HIER_PERW: return MDLC_BASE_PERW;
		case MOD_HIER_DDR0: return MDLC_BASE_DDR0;
		case MOD_HIER_DDR1: return MDLC_BASE_DDR1;
		case MOD_HIER_DDR2: return MDLC_BASE_DDR2;
		case MOD_HIER_DDR3: return MDLC_BASE_DDR3;
		case MOD_HIER_DDR4: return MDLC_BASE_DDR4;
		case MOD_HIER_DDR5: return MDLC_BASE_DDR5;
		case MOD_HIER_DDR6: return MDLC_BASE_DDR6;
		case MOD_HIER_DDR7: return MDLC_BASE_DDR7;
		case MOD_HIER_HSCN: return MDLC_BASE_HSCN;
		case MOD_HIER_RT  : return MDLC_BASE_RT  ;
		case MOD_HIER_TOP : return MDLC_BASE_TOP ;
		case MOD_HIER_HSCS: return MDLC_BASE_HSCS;
		case MOD_HIER_IMN : return MDLC_BASE_IMN ;
		case MOD_HIER_IMS : return MDLC_BASE_IMS ;
		case MOD_HIER_GPC : return MDLC_BASE_GPC ;
		case MOD_HIER_DSP : return MDLC_BASE_DSP ;
		case MOD_HIER_MM  : return MDLC_BASE_MM  ;
		case MOD_HIER_NPU0: return MDLC_BASE_NPU0;
		case MOD_HIER_NPU1: return MDLC_BASE_NPU1;
		case MOD_HIER_CMNN: return MDLC_BASE_CMNN;
		case MOD_HIER_CMNS: return MDLC_BASE_CMNS;
		case MOD_HIER_SCP : return MDLC_BASE_SCP ;
		case MOD_HIER_AON : return MDLC_BASE_AON ;
		default: printf("** Module Hierarchy %d is illegal.\n", mod_hier);
	}
	return 0;
}
/*******************************************************
 * NAME		: mdlc_mpg_DisableWriteProtect
 * FUNCTION	: Write for ModulePowerGating Register
 * INPUT	: mod_hier		PoerDomain Hierarchy
 * OUTPUT	: none
 ******************************************************/
void mdlc_mpg_DisableWriteProtect(uint32_t mod_hier)
{
	/* Disable Write Protection */
	mem_write32(MDLC_PKCPROT0(mdlc_returnBase(mod_hier)), MDLC_WRITE_EN);
}
/*******************************************************
 * NAME		: mdlc_mpg_DisableWriteProtect
 * FUNCTION	: Write for ModulePowerGating Register
 * INPUT	: mod_hier		PoerDomain Hierarchy
 * OUTPUT	: none
 ******************************************************/
void mdlc_mpg_EnableWriteProtect(uint32_t mod_hier)
{
	/* Enable Write Protection */
	mem_write32(MDLC_PKCPROT0(mdlc_returnBase(mod_hier)), MDLC_WRITE_DIS);
}
/*******************************************************
 * NAME		: mdlc_mpg_ctrlStatus
 * FUNCTION	: Control MPG for one Power Domain
 * INPUT	: mod_hier		Module Hierarchy
 *			: pdid			Power Domain ID
 *			: ctrl			ON (From Module Power Gating To Module Power RUN) :1
 *			:				OFF(From Module Power RUN To Module Power Gating) :0
 * OUTPUT	: none
 * RETURN	: 0	Success
 *			: 1		Fail
******************************************************/
uint32_t mdlc_mpg_ctrlStatus(uint32_t mod_hier, uint32_t pdid, uint32_t ctrl)
{
	uintptr_t base = mdlc_returnBase(mod_hier);
	uint32_t curSt;
	uint32_t mpier, mpimr;

	/* Check current status. and if the transition does not need, skip following processes. */
	curSt = mem_read32(MDLC_MPDGS(base, pdid));
	if ((ctrl == PD_ON)  && (curSt == MPG_RUN))    { return 0; }
	if ((ctrl == PD_OFF) && (curSt == MPG_GATING)) { return 0; }

	/* Check MPDG & MPDGS coherency */
	if (mdlc_mpg_checkStReg(base, pdid) == 1) {
		printf("[NG] Current Status Error! [Hier=%d, PDID=%d]\n", mod_hier, pdid);
		return 1;
	}

	/*------------------------------------------------------------------------*/
	mdlc_mpg_DisableWriteProtect(mod_hier);

	if (ctrl == PD_ON) { mpier = 0x00000000; mpimr = 0xFFFFFFFF; }
	else               { mpier = 0xFFFFFFFF; mpimr = 0x00000000; }
	mem_write32(MDLC_MPIER0(base), mpier);
	mem_write32(MDLC_MPIMR0(base), mpimr);

	/* Transition to "Module Power Reset" */
	mem_write32(MDLC_MPDG(base, pdid), MPG_RESET);
	if (mdlc_mpg_checkStReg(base, pdid) == 1) {
		mdlc_mpg_EnableWriteProtect(mod_hier);
		printf("[NG] Transition to \"Module Power Reset\" is failed [Hier=%d, PDID=%d]\n", mod_hier, pdid);
		return 1;
	}

	mem_write32(MDLC_MPIER0(base), 0x00000000);
	mem_write32(MDLC_MPIMR0(base), 0xFFFFFFFF);

	/* Transition to "Module Power RUN" or "Module Power Gating" */
	mem_write32(MDLC_MPDG(base, pdid), (ctrl==PD_ON)?MPG_RUN:MPG_GATING);

	mdlc_mpg_EnableWriteProtect(mod_hier);
	/*------------------------------------------------------------------------*/

	if (mdlc_mpg_checkStReg(base, pdid) == 1) {
		printf("[NG] Transition to \"Module Power %s\" is failed [Hier=%d, PDID=%d]\n",
				(ctrl==PD_ON)?"RUN":"Gating", mod_hier, pdid);
		return 1;
	}

	return 0;
}
/*******************************************************
 * NAME		: mdlc_ctrlModulePowerGating
 * FUNCTION	: Control Power ON/OFF for All Power Domains in one Module Hierarchy
 * INPUT	: mod_hier		Module Hierarchy
 *			: ctrl			ON:1, OFF:0
 * OUTPUT	: none
 * RETURN	: 0	Success
 *			: 1		Fail
 ******************************************************/
uint32_t mdlc_ctrlModulePowerGating(uint32_t mod_hier, uint32_t ctrl)
{
	uint32_t pdid_max;
	int32_t pdid, loop_lim;

	/* Check Module Hierarchy */
	if (mdlc_returnBase(mod_hier) == 0x0) { return 1; }

	/* Get Max value of each Module Hierarchy */
	pdid_max = pdidMaxNums[mod_hier];
	if (pdid_max == 0) {
		printf("** Module Number (%d) controlling is skip.\n", mod_hier);
		return 0;
	}

	/* Controlling for each Power Domain in target module */
	pdid     = (ctrl==PD_ON)?pdid_max:0;		/* PowerON : Control from highest to lowest PDIDs */
	loop_lim = (ctrl==PD_ON)?0:pdid_max;		/* PowerOFF: Control from lowest to highest PDIDs */
	while (1) {
		/* Power Control */
		if (mdlc_mpg_ctrlStatus(mod_hier, pdid, ctrl) == 1) {
			printf("** Module Number (%d) Power-%s is failure.\n", mod_hier, ctrl==PD_ON?"ON":"OFF");
			return 1;
		}

		/* Check if this PDID is the last element */
		if (pdid == loop_lim) { break; }

		if (ctrl == PD_ON) { pdid--; }
		else               { pdid++; }
	}

	return 0;
}
/*******************************************************
 * NAME		: mdlc_transitionMpg
 * FUNCTION	: Primitive Transition Module Power Gating
 * INPUT	: mod_hier		Module Hierarchy
 *			: pdid			Power Domain ID
 *			: status		Target Status (MPG_GATING, MPG_RESET or MPG_RUN)
 * OUTPUT	: none
 * RETURN	: 0	Success
 *			: 1		Fail
 ******************************************************/
uint32_t mdlc_transitionMpg(uint32_t mod_hier, uint32_t pdid, uint32_t status)
{
	uintptr_t base = mdlc_returnBase(mod_hier);

	/* Disable Write Protection */
	mdlc_mpg_DisableWriteProtect(mod_hier);

	/* Transition MPG status */
	mem_write32(MDLC_MPDG(base, pdid), status);

	/* Enable Write Protection */
	mdlc_mpg_EnableWriteProtect(mod_hier);

	return mdlc_mpg_checkStReg(base, pdid);
}

/*******************************************************
 * NAME		: mdlc_ms_DisableWriteProtect
 * FUNCTION	: Write for ModulePowerGating Register
 * INPUT	: mod_hier		PoerDomain Hierarchy
 * OUTPUT	: none
 ******************************************************/
void mdlc_ms_DisableWriteProtect(uint32_t mod_hier)
{
	/* Disable Write Protection */
	mem_write32(MDLC_PKCPROT1(mdlc_returnBase(mod_hier)), MDLC_WRITE_EN);
}
/*******************************************************
 * NAME		: mdlc_ms_EnableWriteProtect
 * FUNCTION	: Write for ModulePowerGating Register
 * INPUT	: mod_hier		PoerDomain Hierarchy
 * OUTPUT	: none
 ******************************************************/
void mdlc_ms_EnableWriteProtect(uint32_t mod_hier)
{
	/* Enable Write Protection */
	mem_write32(MDLC_PKCPROT1(mdlc_returnBase(mod_hier)), MDLC_WRITE_DIS);
}
/*******************************************************
 * NAME		: mdlc_ms_ctrlStatus
 * FUNCTION	: Control Module Standby
 * INPUT	: mod_hier		Module Hierarchy
 *			: regNo			Register No
 *			: regBit		Bits to be set
 *			: mask			Bits to be cleared
 * OUTPUT	: none
 * RETURN	: 0	Success
 *			: 1		Fail
 * REMARKS	: Check to PowerDomain status is already ON
******************************************************/
uint32_t mdlc_ms_ctrlStatus(uint32_t mod_hier, uint32_t regNo, uint32_t regBit, uint32_t mask)
{
	uintptr_t base = mdlc_returnBase(mod_hier);
	uint32_t msress;
	uint32_t val, timeout;

	/* Check current status. and if the transition does not need, skip following processes. */
	msress = mem_read32(MDLC_MSRESS(base, regNo));
	if ((msress & mask) == regBit) { return 0; }

	/* Check MSRES & MSRESS coherency */
	if (mdlc_ms_checkStReg(base, regNo) == 1) {
		printf("[NG] Current Status Error! [Hier=%d, RegNo=%d]\n", mod_hier, regNo);
		return 1;
	}

	/*------------------------------------------------------------------------*/
	mdlc_ms_DisableWriteProtect(mod_hier);

	/* Setting Module Standby */
	val  = msress & (~mask);
	val |= regBit;
	mem_write32(MDLC_MSRES(base, regNo), val);

	mdlc_ms_EnableWriteProtect(mod_hier);
	/*------------------------------------------------------------------------*/

	if (mdlc_ms_checkStReg(base, regNo) == 1) {
		printf("[NG] Transition is failed [Hier=%d, RegNo=%d, TargetBit=0x%x]\n",
				mod_hier, regNo, regBit);
		return 1;
	}

	return 0;
}
/*******************************************************
 * NAME		: mdlc_ctrlModuleStandby
 * FUNCTION	: Control Module Standby for one module
 * INPUT	: mod_hier		Module Hierarchy
 *			: regNo			Register No
 *			: bitPos		bit position -> bit shift number
 *			: status		Target Status (MS_STANDBY, MS_RESET, MS_STOP or MS_RUN)
 * OUTPUT	: none
 * RETURN	: 0	Success
 *			: 1		Fail
******************************************************/
uint32_t mdlc_ctrlModuleStandby(uint32_t mod_hier, uint32_t regNo, uint32_t bitPos, uint32_t status)
{
	uintptr_t base = mdlc_returnBase(mod_hier);
	uint32_t val, mask, timeout;
	uint32_t ret = 0;

	/* Check Module Hierarchy */
	if (base == 0x0) { return 1; }

	val  = status << bitPos;
	mask =    0x3 << bitPos;
	if (mdlc_ms_ctrlStatus(mod_hier, regNo, val, mask)) { ret = 1; }

	/* Check Status */
	timeout = 10000;
	while (--timeout) {
		if ((mem_read32(MDLC_MSRESS(base, regNo)) & mask) == val) { break; }
	}
	if (timeout == 0) {
		printf("[NG] Transition is failed [Hier=%d, RegNo=%d, TargetBit=%d:%d]\n",
				mod_hier, regNo, bitPos+1, bitPos);
		printf("     MSRES  ( 0x%08X )= 0x%08X\n", MDLC_MSRES(base, regNo), mem_read32(MDLC_MSRES(base, regNo)));
		printf("              write-value = 0x%08X\n", val);
		ret = 1;
	}

	return ret;
}

/*******************************************************
 * NAME		: mdlc_dumpMpgReg
 * FUNCTION	: for Register dump
 * INPUT	: mod_hier		Module Hierarchy
 * OUTPUT	: none
 * RETURN	: none
******************************************************/
void mdlc_dumpMpgReg(uint32_t mod_hier)
{
	uintptr_t base = mdlc_returnBase(mod_hier);
	uint32_t pdid, pdid_max;

	/* Check Module Hierarchy */
	if (mdlc_returnBase(mod_hier) == 0x0) { return; }

	/* Controlling for each Power Domain in target module */
	pdid_max = pdidMaxNums[mod_hier];
	for (pdid=0; pdid<=pdid_max; pdid++) {
		/* Register Dump */
		printf("  MDLC%dMPDGS%d   ( 0x%08X )= 0x%08X\n", mod_hier, pdid,
				MDLC_MPDGS(base, pdid), mem_read32(MDLC_MPDGS(base, pdid)));
	}
}

