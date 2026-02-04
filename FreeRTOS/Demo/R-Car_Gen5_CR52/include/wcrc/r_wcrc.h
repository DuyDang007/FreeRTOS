/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef R_WCRC_H
#define R_WCRC_H

/**
 * @defgroup CRC_Module CRC Module
 * @{
 * @brief This module provides functions to configure and control CRC.
 *
 * The CRC module contains 11 units (0 to 10):
 *      Each unit is equal 1 CRC wrapper (WCRC). 
 *      1 WCRC contains 1 CRC sub-module and 1 KCRC sub-module.
 * The CRC module allows for the configuration and generate CRC data.
 * It provides functions to open, close, set mode, run mode.
 */

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>
#include "wcrc/r_wcrc_common.h"


/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/

/**
 * @brief Open the CRC module.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] p_cfg Pointer to the configuration structure.
 *
 * @retval 0 on success.
 * @retval Error code on failure.
 */
int R_CRC_Open(wcrc_ctrl_t * const p_ctrl, const wcrc_cfg_t * const p_cfg);

/**
 * @brief Close the CRC module.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_CRC_Close(wcrc_ctrl_t * const p_ctrl);

/**
 * @brief Perform a CRC calculation.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_CRC_Calculate(wcrc_ctrl_t * const p_ctrl);

/**
 * @brief Get generated CRC value.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_CRC_Get_Generated_Value(wcrc_ctrl_t const * const p_ctrl);

/**
 * @brief Get input data for CRC.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_CRC_Get_Input_Data(wcrc_ctrl_t const * const p_ctrl);

/**
 * @brief Set CRC user callback for all modes (except: CRC Independent Mode) 
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] module Choose WCRC sub-modules: CRC or KCRC.
 * @param[in] p_callback Pointer to user callback.
 * @param[in] p_context  Pointer to user context.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_CRC_Set_Callback(wcrc_sub_module_t module, wcrc_ctrl_t * const p_ctrl,
                       void (* p_callback)(void *), void * const p_context);

/**
 * @brief Set buffer address for storing CRC/KCRC data.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] crc_addr CRC buffer address.
 * @param[in] kcrc_addr KCRC buffer address.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_CRC_Set_BufferAddress(wcrc_ctrl_t * const p_ctrl,
                            uint32_t crc_addr, uint32_t kcrc_addr);

/**
 * @brief Get buffer size for storing CRC/KCRC data.
 *
 * @param[in] module CRC or KCRC sub module.
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[out] buf_size Pointer to a variable where the buffer size will be stored.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
uint32_t R_CRC_Get_BufferSize(wcrc_sub_module_t module, wcrc_ctrl_t * const p_ctrl,
                             uint32_t * buf_size);

/**
 * @brief Check if CRC/KCRC operation is done.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] timeout wait time (ms).
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_CRC_Wait_Operation(wcrc_ctrl_t * p_ctrl, uint32_t timeout);

#ifdef __cplusplus
}
#endif

/** @} */ // end of CRC_Module

#endif // R_WCRC_H
