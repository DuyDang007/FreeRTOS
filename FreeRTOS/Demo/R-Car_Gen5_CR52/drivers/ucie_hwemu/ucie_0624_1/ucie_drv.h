/*************************************************************************
 * LICENSE:
 *    This source code is copyright (c) Renesas Electronics Corporation.
 *    All rights reserved.
 *
 ************************************************************************/
#ifndef UCIE_DRV_H_
#define UCIE_DRV_H_
/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
//#define RCAR_UCIE_V102

//For Palladium
#define USE_EMULATOR
#if defined(RCAR_UCIE_V101) || defined(RCAR_UCIE_V102)
#define DIRECT_ADR
#endif
//#define STATE_SKIP

/* suppress timeout error */
#define UCIE_SETUP_MODE_TIMEOUTOFF
/* hold training state during setup */
#define UCIE_SETUP_MODE_HOLDLTSSM
/* retry from RESET when ERROR */
#define UCIE_SETUP_MODE_RETRY
/* enable timeout error after link up */
#define UCIE_SETUP_MODE_TIMEOUTON_L_UP

#define UCIE_SETUP_SEQ_LAST (8U)

#define UCIE_SETUP_VER_V100 (0U)
#define UCIE_SETUP_VER_V101 (1U)
#define UCIE_SETUP_VER_V102 (2U)

// X5H-AIACC X5H side (RC only)
#if defined(RCAR_UCIE_V100_W_V102)
//   #define UCIE_CH_RC UCIE_CH0
  #define UCIE_CH_RC_WITH UCIE_SETUP_VER_V102
#endif

// X5H-AIACC AIACC side (EP only)
#if defined(RCAR_UCIE_V102_W_V100)
//   #define UCIE_CH_EP UCIE_CH0
  #define UCIE_CH_EP_WITH UCIE_SETUP_VER_V100
#endif

// #if defined(RCAR_UCIE_V101_W_V100) || defined(RCAR_UCIE_V102_W_V100)
//   //X5H-AIACC AIACC side (EP only)
//   #define UCIE_CH_EP UCIE_CH0
//   #define UCIE_CH_EP_WITH UCIE_SETUP_VER_V100
// #elif defined(RCAR_UCIE_V100_W_V101)
//   //X5H-AIACC X5H side (RC only)
//   #define UCIE_CH_RC UCIE_CH0
//   #define UCIE_CH_RC_WITH UCIE_SETUP_VER_V101
// #elif defined(RCAR_UCIE_V100_W_V102)
//   //X5H-AIACC X5H side (RC only)
//   #define UCIE_CH_RC UCIE_CH0
//   #define UCIE_CH_RC_WITH UCIE_SETUP_VER_V102
// #elif defined(RCAR_UCIE_V102_W_V102_RC)
//   #define UCIE_CH_RC UCIE_CH1
//   #define UCIE_CH_RC_WITH UCIE_SETUP_VER_V102
// #elif defined(RCAR_UCIE_V102_W_V102_EP)
//   #define UCIE_CH_EP UCIE_CH1
//   #define UCIE_CH_EP_WITH UCIE_SETUP_VER_V102
// #else
//   #if defined(RCAR_UCIE_V100)
//     //X5H loopback
//     #define UCIE_CH_RC UCIE_CH0
//     #define UCIE_CH_EP UCIE_CH1
//     #define UCIE_CH_RC_WITH UCIE_SETUP_VER_V100
//     #define UCIE_CH_EP_WITH UCIE_SETUP_VER_V100
//   #elif defined(RCAR_UCIE_V101)
//     #define UCIE_CH_RC UCIE_CH0
//     #define UCIE_CH_EP UCIE_CH1
//     #define UCIE_CH_RC_WITH UCIE_SETUP_VER_V101
//     #define UCIE_CH_EP_WITH UCIE_SETUP_VER_V101
//   #elif defined(RCAR_UCIE_V102)
//     #define UCIE_CH_RC UCIE_CH0
//     #define UCIE_CH_EP UCIE_CH1
//     #define UCIE_CH_RC_WITH UCIE_SETUP_VER_V102
//     #define UCIE_CH_EP_WITH UCIE_SETUP_VER_V102
//   #endif
// #endif

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
//uint32_t Ucie_hdma_test_ep(void);
// uint32_t Ucie_Setup_Seq(uint32_t ucie_ch, uint32_t device_type, uint32_t conn_ver, uint32_t seq);
uint32_t ucie_hwemu_hw_init_rc(uint32_t ucie_chan, uint32_t device_type, uint32_t conn_ver);
uint32_t ucie_hwemu_hw_init_ep(uint32_t ucie_chan, uint32_t device_type, uint32_t conn_ver);
// void     wait_time(uint32_t count); // for boot code

#endif  /* UCIE_DRV_H_ */
