/*************************************************************************************************************
* dmac_common
* Copyright (c) 2025 Renesas Electronics Corporation
* This software is released under the MIT License
* http://opensource.org/licenses/mit-license.php
*************************************************************************************************************/

#ifndef RTDMAC_COMMON_H
#define RTDMAC_COMMON_H


#include <stdbool.h>
// Enum representing DMACs
typedef enum {
    RT_DMAC0,
    RT_DMAC1,
    RT_DMAC2,
    RT_DMAC3,
    RT_DMAC_MAX
} DMAC_t;

// Enum representing DMACs Channels
typedef enum {
    RT_DMAC_CH0,
    RT_DMAC_CH1,
    RT_DMAC_CH2,
    RT_DMAC_CH3,
    RT_DMAC_CH4,
    RT_DMAC_CH5,
    RT_DMAC_CH6,
    RT_DMAC_CH7,
    RT_DMAC_CH8,
    RT_DMAC_CH9,
    RT_DMAC_CH10,
    RT_DMAC_CH11,
    RT_DMAC_CH12,
    RT_DMAC_CH13,
    RT_DMAC_CH14,
    RT_DMAC_CH15,
    RT_DMAC_CH_MAX
} DMAC_ch_t;


/***********************************************************
Macro definitions
***********************************************************/
#define DRV_RTDMAC_EXT_NUM_DESC    128   /* The number of external descriptor memory */
#define DRV_RTDMAC_INT1_NUM_DESC   128   /* The number of internal descriptor memory */
#define DRV_RTDMAC_INT2_NUM_DESC   128   /* The number of internal descriptor memory */

#define rDmacCH_VALID   (0x000000FF00FF0000U)

/***********************************************************
Typedef definitions
***********************************************************/
typedef enum e_rDmacPriorityMode {
    DRV_RTDMAC_PRIO_FIX,
    DRV_RTDMAC_PRIO_RR,
    DRV_RTDMAC_PRIO_MAX
} rDmacPriorityMode_t;

typedef enum e_rDmacDmaMode {
    DRV_DMAC_DMA_NO_DESCRIPTOR,
    DRV_DMAC_DMA_DESC_NORMAL,
    DRV_DMAC_DMA_DESC_REPEAT,
    DRV_DMAC_DMA_DESC_READOUT,
    DRV_DMAC_DMA_DESC_INFINITE,
    DRV_DMAC_DMA_MODE_MAX
} rDmacDmaMode_t;

typedef enum e_rDmacAddrMode {
    DRV_RTDMAC_ADDR_FIXED,
    DRV_RTDMAC_ADDR_INCREMENTED,
    DRV_RTDMAC_ADDR_DECCREMENTED,
    DRV_RTDMAC_ADDR_MAX
} rDmacAddrMode_t;

typedef enum e_rDmacTransferUnit {
    DRV_RTDMAC_TRANS_UNIT_1BYTE,
    DRV_RTDMAC_TRANS_UNIT_2BYTE,
    DRV_RTDMAC_TRANS_UNIT_4BYTE,
    DRV_RTDMAC_TRANS_UNIT_8BYTE,
    DRV_RTDMAC_TRANS_UNIT_16BYTE,
    DRV_RTDMAC_TRANS_UNIT_32BYTE,
    DRV_RTDMAC_TRANS_UNIT_64BYTE,
    DRV_RTDMAC_TRANS_UNIT_MAX
} rDmacTransferUnit_t;

typedef enum e_rDmacResource {
    DRV_RTDMAC_MEMORY,
    DRV_RTDMAC_RESOUCE_MAX
} rDmacResource_t;

typedef enum e_rDmacLowSpeedMode {
    DRV_RTDMAC_SPEED_NORMAL,
    DRV_RTDMAC_SPEED_MAX
} rDmacLowSpeedMode_t;

typedef enum e_rDmacReason {
    DRV_RTDMAC_STAGE_END,
    DRV_RTDMAC_TRANSFER_END,
    DRV_RTDMAC_ADDRESS_ERROR
} rDmacReason_t;

/*!
 *  @brief         Config to set the RT-DMAC
 *  @details
 *  @param[in]      mSrcAddr  : Source address
 *  @param[in]      mDestAddr  : Destination address
 *  @param[in]      mTransferCount : Transfer count
 *  @param[in]      mDMAMode : DMA mode
 *  @param[in]      mSrcAddrMode : Source address mode 
 *  @param[in]      mDestAddrMode : Destination address mode
 *  @param[in]      mTransferUnit : Transfer unit
 *  @param[in]      mResource : Resource
 *  @param[in]      mLowSpeed : Low speed mode
 *  @param[in]      mPrioLevel : Priority level
*/
typedef struct st_rDmacCfg {
    uintptr_t mSrcAddr;
    uintptr_t mDestAddr;
    uint32_t mTransferCount;
    rDmacDmaMode_t mDMAMode;
    rDmacAddrMode_t mSrcAddrMode;
    rDmacAddrMode_t mDestAddrMode;
    rDmacTransferUnit_t mTransferUnit;
    rDmacResource_t mResource;
    rDmacLowSpeedMode_t mLowSpeed;
    uint8_t mPrioLevel;
} rDmacCfg_t;

/*!
 *  @brief         Update DMA descriptor
 *  @details
 *  @param[in]      mSrcAddrUpdate  : Update source address
 *  @param[in]      mDestAddrUpdate  : Update destination address
 *  @param[in]      mTransCountUpdate : Update transfer count
 */
typedef struct st_rDmacDescUpdate {
    bool mSrcAddrUpdate;
    bool mDestAddrUpdate;
    bool mTransCountUpdate;
} rDmacDescUpdate_t;

/*!
 *  @brief         Config DMA descriptor
 *  @details
 *  @param[in]      mDescBaseAddr  : Descriptor base address
 *  @param[in]      mDescUpdate  : Update descriptor
 *  @param[in]      mDescRead1st : Descriptor read 1st
 *  @param[in]      mStateEndEnable : State end enable
 *  @param[in]      mDescCount : Descriptor count
 *  @param[in]      mDescIndex : Descriptor index
 */
typedef struct st_rDmacDescCfg {
    uintptr_t mDescBaseAddr;
    rDmacDescUpdate_t mDescUpdate;
    bool mDescRead1st;
    bool mStateEndEnable;
    uint8_t mDescCount;
    uint8_t mDescIndex;
} rDmacDescCfg_t;

#endif  /* RTDMAC_COMMON_H */

