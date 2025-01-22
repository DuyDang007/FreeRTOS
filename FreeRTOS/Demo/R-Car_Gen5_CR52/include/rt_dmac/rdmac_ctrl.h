/*************************************************************************************************************
* dmac_ctrl_h
* Copyright (c) 2025 Renesas Electronics Corporation
* This software is released under the MIT License
* http://opensource.org/licenses/mit-license.php
*************************************************************************************************************/

#ifndef _RDMAC_CTRL_H_
#define _RDMAC_CTRL_H_

/**
 * @defgroup RTDMAC_Module RT-DMAC Module
 * @{
 * @brief This module provides functions to configure and control the operation of RT-DMAC.
 *
 * The RT-DMAC module allows for the configuration and control the operation of RT-DMAC.
 * It provides functions to open, close, configure, read, and write of RT-DMAC.
 */

#include "rt_dmac/rdmac_common.h"

/**
 *  @brief          DMA intialize
 *  @details
 *  @param[in]      mode
 *  @return         drv_OK
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
uint16_t R_RTDMAC_RcarDmacCtrlInit(DMAC_t dev, rDmacPriorityMode_t mode);

/**
 *  @brief          DMA Execute
 *  @details
 *  @param[in]      cfg
 *  @param[in]      descCfg
 *  @param[in]      cb
 *  @return         drv_OK
 *  @return         drv_ERR_NOT_INITIALIZED
 *  @return         drv_RTDMAC_CH_BUSY
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
uint16_t R_RTDMAC_RcarDmacExec(DMAC_t dev, uint8_t ch, rDmacCfg_t *cfg, rDmacDescCfg_t *descCfg);

/**
 *  @brief          DMA Stop
 *  @details
 *  @return         drv_OK
 *  @return         drv_ERR_NOT_INITIALIZED
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
uint16_t R_RTDMAC_RcarDmacStop(DMAC_t dev, uint8_t ch);

/** @} */ // end of RTDMAC_Module

#endif  /* _RDMAC_CTRL_H_ */
