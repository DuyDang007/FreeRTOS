/**********************************************************/
/* Sample program : R-Car Register Header                 */
/* File Name      : reg_rcar.h                            */
/* Copyright (C) Renesas Electronics Corp. 2024.          */
/**********************************************************/

#ifndef _REG_RCAR_H_
#define _REG_RCAR_H_

#ifdef SYSTEM_DEF_X5H				/* X5H */

//#include "reg_x5h_canfd.h"
//#include "reg_x5h_csi.h"
//#include "reg_x5h_hscif.h"
//#include "reg_x5h_intc.h"
//#include "reg_x5h_isp.h"
//#include "reg_x5h_pfc.h"
//#include "reg_x5h_rst.h"
//#include "reg_x5h_rsw3.h"
//#include "reg_x5h_scif.h"
//#include "reg_x5h_tmu.h"
//#include "reg_x5h_vin.h"
//#include "reg_x5h_vsp2.h"
//#include "reg_x5h_vcon.h"
#include "drivers/pcie/reg_x5h_pcie.h"
//#include "reg_x5h_ecm.h"

#else

#error

#endif

#endif /* _REG_RCAR_H_ */
