/*************************************************************************
 * LICENSE:
 *    This source code is copyright (c) Renesas Electronics Corporation.
 *    All rights reserved.
 *
 ************************************************************************/
#ifndef UCIE_DRV_PORT_H_
#define UCIE_DRV_PORT_H_
/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

#ifdef EVA_UCIE_ON
// #include "common.h"
// #include "uciedrv.h"
#include "clock_controller.h"
#include "module_controller.h"
#include "ucie_hwemu.h"
#endif

#ifdef BUILD_ROM
// #include "common.h"
#include "result_code.h"
#include "scif_drv.h"
#include "scif_ctrl.h"
#endif

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

#ifdef EVA_UCIE_ON
// -----------------------------------------------------------------------------
// Return Status Codes
// -----------------------------------------------------------------------------
#define	NORMAL_END	(0)
#define	ERROR_END	(1)
// -----------------------------------------------------------------------------
// UCIE Channel Definitions
// -----------------------------------------------------------------------------
#define UCIE_CH0    (0)
#define UCIE_CH1    (1)
#define MODE_EP     (0U)
#define MODE_RC     (1U)

// #define ucie_hwemu_write(addr, data)    mem_write32((addr), (data))
// #define UCIE_READ(addr)	          mem_read32((addr))
// #define UCIE_DEBUG_PRINTF(...)    mprintf(__VA_ARGS__)

#define UCIE_MS_AREA_PERI (0U)
#define UCIE_MS_AREA_CORE (1U)
#define UCIE_MS_ST_RUN (MS_RUN)
#define UCIE_MS_ST_RESET (MS_RESET)
static inline void Ucie_ms_transition(uint32_t ucie_ch, uint32_t area, uint32_t st)
{
    uint32_t ucie_ms_bit;
    if(UCIE_CH0==ucie_ch){
        if(UCIE_MS_AREA_PERI==area) {
            ucie_ms_bit=2;
        } else {
            ucie_ms_bit=0;
        }
    } else {
        if(UCIE_MS_AREA_PERI==area) {
            ucie_ms_bit=6;
        } else {
            ucie_ms_bit=4;
        }
    }
    mdlc_transition_ms(MODULE_NUM_HSCS, REG_NUM_HSCS, ucie_ms_bit, st);
}
#define UCIE_CLK_STBY (0U)
#define UCIE_CLK_SLOW (1U)
#define UCIE_CLK_FAST (2U)
static inline void Ucie_clkcnt(uint32_t ucie_ch, uint32_t mode)
{
    uint32_t ucixcoreclkcr;
    uint32_t pll_num;
    uint32_t val;
    if ( UCIE_CH0 == ucie_ch ){
        ucixcoreclkcr = 0xDE201080U;
        pll_num = PLL9_0;
    } else {
        ucixcoreclkcr = 0xDE201084U;
        pll_num = PLL9_1;
    }

    val = ucie_hwemu_read( ucixcoreclkcr );
    if( UCIE_CLK_STBY == mode ) {
        /* stp */
        ucie_hwemu_write( ucixcoreclkcr, val | (1U<<8) );
        switch_clock_source_pll(pll_num);
    } else if ( UCIE_CLK_SLOW == mode ) {
        /* stp */
        ucie_hwemu_write( ucixcoreclkcr, val | (1U<<8) );
        /*  Freq. = 1/(1+3) = 1/4 */
        ucie_hwemu_write( ucixcoreclkcr, 0x00000103U );
        ucie_hwemu_write( ucixcoreclkcr, 0x00000003U );
    } else if ( UCIE_CLK_FAST == mode ) {
        /* stp */
        ucie_hwemu_write( ucixcoreclkcr, val | (1U<<8) );
        /*  Freq. = 1/(1+0) = 1/1 */
        ucie_hwemu_write( ucixcoreclkcr, 0x00000100U );
        ucie_hwemu_write( ucixcoreclkcr, 0x00000000U );
    }
}
#endif /* EVA_UCIE_ON */

#ifdef BUILD_ROM
// #define UCIE_WRITE(addr, data)      (reg_write_32((addr), (data)))
// #define UCIE_READ(addr)             (reg_read_32((addr)))
// #define UCIE_DEBUG_PRINTF(...)     mprintf(__VA_ARGS__)
static inline void mprintf(const char *format,...)
{
    u8_send_data++;
    (void)scif_putchar(u8_send_data);
    (void)scif_putchar(CR_CODE);
    (void)scif_putchar(LF_CODE);
}
#define UCIE_CH0                    (0U)
#define UCIE_CH1                    (1U)
#define MODE_EP                     (0U)
#define MODE_RC                     (1U)
#define NORMAL_END                  (ROM_OK)
#define ERROR_END                   (ROM_ERR_HW)

#define UCIE_MS_AREA_PERI (0U)
#define UCIE_MS_AREA_CORE (1U)
#define UCIE_MS_ST_RUN (3U)
#define UCIE_MS_ST_RESET (1U)
static inline void Ucie_ms_transition(uint32_t ucie_ch, uint32_t area, uint32_t st){
/* WRITE ME */
}
static inline void Ucie_pll_sel(uint32_t ucie_ch)
{
/* WRITE ME */
}
static inline void Ucie_clkcnt(uint32_t ucie_ch, uint32_t mode)
{
/* WRITE ME */
}
#endif /* BUILD_ROM */

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/
#ifdef BUILD_ROM
static uint8_t u8_send_data;
#endif 

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#endif  /* UCIE_DRV_PORT_H_ */
