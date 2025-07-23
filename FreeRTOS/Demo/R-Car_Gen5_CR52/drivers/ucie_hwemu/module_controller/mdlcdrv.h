#ifndef _MDLCDRV_H_
#define _MDLCDRV_H_

/*******************************************************************************
  Includes
*******************************************************************************/
#include <stdint.h>
#include "module_controller.h"
/*******************************************************************************
  Defines
*******************************************************************************/
/* Module Hierarchy */
enum {						// Module Number
	MOD_HIER_VIPN = 0,		// 00
	MOD_HIER_VIPS,			// 01
	MOD_HIER_VIO ,			// 02
	MOD_HIER_PERE,			// 03
	MOD_HIER_PERW,			// 04
	MOD_HIER_DDR0,			// 05
	MOD_HIER_DDR1,			// 06
	MOD_HIER_DDR2,			// 07
	MOD_HIER_DDR3,			// 08
	MOD_HIER_DDR4,			// 09
	MOD_HIER_DDR5,			// 10
	MOD_HIER_DDR6,			// 11
	MOD_HIER_DDR7,			// 12
	MOD_HIER_HSCN,			// 13
	MOD_HIER_RT  ,			// 14
	MOD_HIER_TOP ,			// 15
	MOD_HIER_HSCS,			// 16
	MOD_HIER_IMN ,			// 17
	MOD_HIER_IMS ,			// 18
	MOD_HIER_GPC ,			// 19
	MOD_HIER_DSP ,			// 20
	MOD_HIER_MM  ,			// 21
	MOD_HIER_NPU0,			// 22
	MOD_HIER_NPU1,			// 23
	MOD_HIER_CMNN,			// 24
	MOD_HIER_CMNS,			// 25
	MOD_HIER_SCP ,			// 26
	MOD_HIER_AON ,			// 27
	MOD_HIER_ILLEGAL,
};

#define MDLC_WRITE_EN		(0xA5A5A501U)
#define MDLC_WRITE_DIS		(0xA5A5A500U)

///* Status for Module Power Gating */
//#define MPG_GATING		(0x0U)
//#define MPG_RESET		(0x1U)
//#define MPG_RUN			(0x3U)

/* Status for Module Standby */
#define MS_STANDBY		(0x0U)
#define MS_RESET		(0x1U)
#define MS_STOP			(0x2U)
#define MS_RUN			(0x3U)

#define PD_ON			(1)
#define PD_OFF			(0)

/*******************************************************************************
  Functions
*******************************************************************************/
uintptr_t mdlc_returnBase(uint32_t mod_hier);

/* MPG */
void mdlc_mpg_DisableWriteProtect(uint32_t mod_hier);
void mdlc_mpg_EnableWriteProtect(uint32_t mod_hier);
uint32_t mdlc_mpg_ctrlStatus(uint32_t mod_hier, uint32_t pdid, uint32_t ctrl);
uint32_t mdlc_ctrlModulePowerGating(uint32_t mod_hier, uint32_t ctrl);
uint32_t mdlc_transitionMpg(uint32_t mod_hier, uint32_t pdid, uint32_t status);
/* MS */
void mdlc_ms_DisableWriteProtect(uint32_t mod_hier);
void mdlc_ms_EnableWriteProtect(uint32_t mod_hier);
uint32_t mdlc_ms_ctrlStatus(uint32_t mod_hier, uint32_t regNo, uint32_t regBit, uint32_t mask);
uint32_t mdlc_ctrlModuleStandby(uint32_t mod_hier, uint32_t regNo, uint32_t bitPos, uint32_t status);
/* Others */
void mdlc_dumpMpgReg(uint32_t mod_hier);

#endif /* _MDLCDRV_H_ */

