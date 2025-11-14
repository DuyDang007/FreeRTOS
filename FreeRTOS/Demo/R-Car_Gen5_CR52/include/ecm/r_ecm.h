/*
* Copyright (c) 2025 Renesas Electronics Corporation
*
* SPDX-License-Identifier: MIT
*
*/

#ifndef R_ECM_H
#define R_ECM_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>
#include <stddef.h>
#include "r_error_domain_id.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ECM_GET_ERROR_DOMAIN_ID(register_number, bit_positive)  ((register_number * 32) + bit_positive)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 *  @struct STag_EcmIrqCfg
 *  @brief  Handle structure for DMAC.
 *  @details This structure is used to manage the DMAC Irq handle.
 */
typedef void (*IrqErrorHandlerFn)(e_ecm_error_id_t id);

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
uint8_t R_ECM_SetInterruptCallback(IrqErrorHandlerFn irq_handler);
uint8_t R_ECM_SetDetection(e_ecm_error_id_t id, int8_t isEnable);
uint8_t R_ECM_SetPinOut(e_ecm_error_id_t id, int8_t isEnable);
uint8_t R_ECM_SetReset(e_ecm_error_id_t id, int8_t isEnable);
uint8_t R_ECM_SetInterruptNotification(e_ecm_error_id_t id, int8_t isEnable);
uint8_t R_ECM_DisableAll();
uint8_t R_ECM_CheckErrorStatus(e_ecm_error_id_t id);
uint8_t R_ECM_PusedoError(e_ecm_error_id_t id);

#endif /* R_ECM_H */