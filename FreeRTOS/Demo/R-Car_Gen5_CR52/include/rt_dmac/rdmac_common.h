/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef RTDMAC_COMMON_H
#define RTDMAC_COMMON_H

/**
*  @defgroup      RTDMAC_Module Configuration Module of RT-DMAC Module
*  @{
*  @brief         This module provides functions to configure and control RT-DMAC communication.
*
*  The RT-DMAC module allows for the configuration and control of RT-DMAC communication.
*  It provides struct,enum to configure RT-DMAC.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 *  @brief         Enum representing RT_DMAC instances
 *  @details       R-Car Gen5 will have 4 instances: 0, 1, 2, 3
 *  @note          Each instance corresponds to a specific DMA controller in the system.
 */
typedef enum {
    RT_DMAC0, /*!< DMA Controller Instance 0 */
    RT_DMAC1, /*!< DMA Controller Instance 1 */
    RT_DMAC2, /*!< DMA Controller Instance 2 */
    RT_DMAC3, /*!< DMA Controller Instance 3 */
    RT_DMAC_MAX /*!< Maximum number of DMA Controller Instances */
} DMAC_t;

/**
 *  @brief         Enum representing RT_DMAC channels
 *  @details       R-Car Gen5 each instance will have 16 channels from 0 to 15.
 *  @note          Each channel corresponds to a specific DMA channel within a DMA controller instance.
 */
typedef enum {
    RT_DMAC_CH0,  /*!< DMA Channel 0 */
    RT_DMAC_CH1,  /*!< DMA Channel 1 */
    RT_DMAC_CH2,  /*!< DMA Channel 2 */
    RT_DMAC_CH3,  /*!< DMA Channel 3 */
    RT_DMAC_CH4,  /*!< DMA Channel 4 */
    RT_DMAC_CH5,  /*!< DMA Channel 5 */
    RT_DMAC_CH6,  /*!< DMA Channel 6 */
    RT_DMAC_CH7,  /*!< DMA Channel 7 */
    RT_DMAC_CH8,  /*!< DMA Channel 8 */
    RT_DMAC_CH9,  /*!< DMA Channel 9 */
    RT_DMAC_CH10, /*!< DMA Channel 10 */
    RT_DMAC_CH11, /*!< DMA Channel 11 */
    RT_DMAC_CH12, /*!< DMA Channel 12 */
    RT_DMAC_CH13, /*!< DMA Channel 13 */
    RT_DMAC_CH14, /*!< DMA Channel 14 */
    RT_DMAC_CH15, /*!< DMA Channel 15 */
    RT_DMAC_CH_MAX /*!< Maximum number of DMA Channels */
} DMAC_ch_t;

/***********************************************************
Macro definitions
***********************************************************/
/**
 *  @def DRV_RTDMAC_EXT_NUM_DESC
 *  @brief Number of external descriptor memory entries.
 *  @details Specifies the number of entries in the external descriptor memory.
 */
#define DRV_RTDMAC_EXT_NUM_DESC    128   /* The number of external descriptor memory */

/**
 *  @def DRV_RTDMAC_INT1_NUM_DESC
 *  @brief Number of internal descriptor memory entries (Type 1).
 *  @details Specifies the number of entries in the first type of internal descriptor memory.
 */
#define DRV_RTDMAC_INT1_NUM_DESC   128   /* The number of internal descriptor memory */

/**
 *  @def DRV_RTDMAC_INT2_NUM_DESC
 *  @brief Number of internal descriptor memory entries (Type 2).
 *  @details Specifies the number of entries in the second type of internal descriptor memory.
 */
#define DRV_RTDMAC_INT2_NUM_DESC   128   /* The number of internal descriptor memory */

/**
 *  @def rDmacCH_VALID
 *  @brief Valid channel mask.
 *  @details Specifies the bitmask used to validate DMA channels.
 */
#define rDmacCH_VALID   (0x000000FF00FF0000U)


/***********************************************************
Typedef definitions
***********************************************************/

/**
 *  @brief         Enum representing DMA operation priority mode
 *  @details       Specifies the priority mode for DMA operations. There are two modes:
 *                 - Fixed mode: The priority is fixed and does not change.
 *                 - Round-robin mode: The priority rotates among the channels.
 */
typedef enum e_rDmacPriorityMode {
    DRV_RTDMAC_PRIO_FIX, /*!< Fixed priority mode */
    DRV_RTDMAC_PRIO_RR,  /*!< Round-robin priority mode */
    DRV_RTDMAC_PRIO_MAX  /*!< Maximum number of priority modes */
} rDmacPriorityMode_t;

/**
 *  @brief         Enum representing Descriptor operating mode
 *  @details       Specifies whether Descriptor is enabled or disabled, and its
 *                 operational mode.
 */
typedef enum e_rDmacDmaMode {
    DRV_DMAC_DMA_NO_DESCRIPTOR, /*!< No descriptor mode */
    DRV_DMAC_DMA_DESC_NORMAL,   /*!< Normal descriptor mode */
    DRV_DMAC_DMA_DESC_REPEAT,   /*!< Repeat descriptor mode */
    DRV_DMAC_DMA_DESC_READOUT,  /*!< Readout descriptor mode */
    DRV_DMAC_DMA_DESC_INFINITE, /*!< Infinite descriptor mode */
    DRV_DMAC_DMA_MODE_MAX       /*!< Maximum number of descriptor modes */
} rDmacDmaMode_t;

/**
 *  @brief         Enum representing Source/Destination address mode
 *  @details       Specifies whether the DMA source/destination address is incremented,
 *                 fixed, or decremented, and the amount of increment.
 */
typedef enum e_rDmacAddrMode {
    DRV_RTDMAC_ADDR_FIXED,        /*!< Fixed address mode */
    DRV_RTDMAC_ADDR_INCREMENTED,  /*!< Incremented address mode */
    DRV_RTDMAC_ADDR_DECREMENTED,  /*!< Decremented address mode */
    DRV_RTDMAC_ADDR_MAX           /*!< Maximum number of address modes */
} rDmacAddrMode_t;

/**
 *  @brief         Enum representing Source/Destination transfer unit
 *  @details       Specifies the amount of increment or decrement to the
 *                 source/destination of the DMA.
 */
typedef enum e_rDmacTransferUnit {
    DRV_RTDMAC_TRANS_UNIT_1BYTE,   /*!< Transfer unit of 1 byte */
    DRV_RTDMAC_TRANS_UNIT_2BYTE,   /*!< Transfer unit of 2 bytes */
    DRV_RTDMAC_TRANS_UNIT_4BYTE,   /*!< Transfer unit of 4 bytes */
    DRV_RTDMAC_TRANS_UNIT_8BYTE,   /*!< Transfer unit of 8 bytes */
    DRV_RTDMAC_TRANS_UNIT_16BYTE,  /*!< Transfer unit of 16 bytes */
    DRV_RTDMAC_TRANS_UNIT_32BYTE,  /*!< Transfer unit of 32 bytes */
    DRV_RTDMAC_TRANS_UNIT_64BYTE,  /*!< Transfer unit of 64 bytes */
    DRV_RTDMAC_TRANS_UNIT_MAX      /*!< Maximum number of transfer units */
} rDmacTransferUnit_t;

/**
 *  @brief         Enum representing Resource selection
 *  @details       Specifies resource selection as memory or maximum resource.
 */
typedef enum e_rDmacResource {
    DRV_RTDMAC_MEMORY,        /*!< Memory resource */
    DRV_RTDMAC_RESOUCE_MAX    /*!< Maximum number of resources */
} rDmacResource_t;

/**
 *  @brief         Enum representing DMA Transfer Slow Speed mode
 *  @details       Specifies the number of times by clock (ZS) cycle that DMA
 *                 transfer is kept waiting after single DMA transfer. DMA transfer is
 *                 operated once in multiple clock cycle specified by SLM.
*/
typedef enum e_rDmacLowSpeedMode {
    DRV_RTDMAC_SPEED_NORMAL,    /*!< Normal speed mode */
    DRV_RTDMAC_SPEED_MAX        /*!< Maximum number of speed modes */
} rDmacLowSpeedMode_t;

/**
 *  @struct st_rDmacCfg
 *  @brief         Configuration structure for the DMA controller.
 *  @details       This structure is used to configure the DMA controller.
*/
typedef struct st_rDmacCfg {
    uintptr_t mSrcAddr;                /*!< Source address */
    uintptr_t mDestAddr;               /*!< Destination address */
    uint32_t mTransferCount;           /*!< Transfer count */
    rDmacDmaMode_t mDMAMode;           /*!< DMA mode */
    rDmacAddrMode_t mSrcAddrMode;      /*!< Source address mode */
    rDmacAddrMode_t mDestAddrMode;     /*!< Destination address mode */
    rDmacTransferUnit_t mTransferUnit; /*!< Transfer unit */
    rDmacResource_t mResource;         /*!< Resource */
    rDmacLowSpeedMode_t mLowSpeed;     /*!< Low speed mode */
    uint8_t mPrioLevel;                /*!< Priority level */
} rDmacCfg_t;

/**
 *  @struct st_rDmacDescUpdate
 *  @brief         Update DMA descriptor
 *  @details       This structure is used to update the DMA descriptor.
 */
typedef struct st_rDmacDescUpdate {
    bool mSrcAddrUpdate;    /*!< Update source address */
    bool mDestAddrUpdate;   /*!< Update destination address */
    bool mTransCountUpdate; /*!< Update transfer count */
} rDmacDescUpdate_t;

/**
 *  @struct st_rDmacDescCfg
 *  @brief         Config DMA descriptor
 *  @details    This structure is used to configure the DMA descriptor.
 */
typedef struct st_rDmacDescCfg {
    uintptr_t mDescBaseAddr;    /*!< Descriptor base address */
    rDmacDescUpdate_t mDescUpdate;  /*!< Update descriptor */
    bool mDescRead1st;        /*!< Descriptor read 1st */
    bool mStateEndEnable;   /*!< State end enable */
    uint8_t mDescCount;     /*!< Descriptor count */
    uint8_t mDescIndex;    /*!< Descriptor index */
} rDmacDescCfg_t;

/** @} */ // end of RTDMAC_Module

#endif  /* RTDMAC_COMMON_H */