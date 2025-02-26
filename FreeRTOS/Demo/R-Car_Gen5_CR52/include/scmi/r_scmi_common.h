/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _R_SCMI_COMMON_H_
#define _R_SCMI_COMMON_H_

/**
 * @defgroup SCMI_Common_Module SCMI Common Module
 * @{
 * @brief SCMI Common.
 *
 * SCMI Common.
 */

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /** __cplusplus */

/**
 * @brief Graceful flags.
 */
typedef enum {
    FLAGS_FORCEFUL              = 0,    /**< Forceful request */
    FLAGS_GRACEFUL              = 1     /**< Graceful request */
} e_flags_t;

/**
 * @brief SCMI System State.
 */
typedef enum {
    SYSTEM_STATE_SHUTDOWN       = 0,    ///< Shutdown State
    SYSTEM_STATE_COLD_RESET     ,       ///< Cold Reset State
    SYSTEM_STATE_WARM_RESET     ,       ///< Warm Reset State
    SYSTEM_STATE_POWER_UP       ,       ///< Power Up State
    SYSTEM_STATE_SUSPEND        ,       ///< Suspend State
    SYSTEM_STATE_MAX                    ///< Number of System States
} e_system_state_t;

/**
 * @brief Driver Init.
 */
int R_SCMI_DriverInit(void);

/**
 * @brief Irq handler.
 */
void R_SCMI_IsrHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @} */ // end of SCMI_Common_Module

#endif /* _R_SCMI_COMMON_H_ */

