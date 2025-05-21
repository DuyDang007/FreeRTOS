/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef _DEVICE_TREE_X5H_H_
#define _DEVICE_TREE_X5H_H_

#include <stdint.h>
#include "board.h"

#define MODULE_CONFIG(module_id, is_enabled)    {module_id, is_enabled}

/**
 * @brief Superset list of all HW IPs.
 */
typedef enum {
    MODULE_INVALID = 0,		///< Invalid module ID.
    MODULE_SCIF0,			///< Module ID: SCIF0.
    MODULE_SCIF1,			///< Module ID: SCIF1.
    MODULE_SCIF2,			///< Module ID: SCIF2.
    MODULE_SCIF3,			///< Module ID: SCIF3.
    MODULE_HSCIF0,			///< Module ID: HSCIF0.
    MODULE_HSCIF1,			///< Module ID: HSCIF1.
    MODULE_HSCIF2,			///< Module ID: HSCIF2.
    MODULE_HSCIF3,			///< Module ID: HSCIF3.
    MODULE_I2C0,			///< Module ID: I2C0.
    MODULE_I2C1,			///< Module ID: I2C1.
    // Add device_id
    MODULE_MAX = 255			///< Max module ID.
} e_module_id_t;

#if (BOARD == X5H_VDK)
#define MODULE_CONFIGS \
    MODULE_CONFIG(MODULE_SCIF0, 1),     \
    MODULE_CONFIG(MODULE_I2C0,  1),     \
    MODULE_CONFIG(MODULE_I2C1,  1)
    // Add module configs
#endif

/**
 * @brief Configuration PFC for module HW IP.
 */
typedef struct {
    e_module_id_t module_id;	///< Module id.
    uint32_t is_enabled;	///< Enable PFC for module or not.
} st_module_config_t;

static inline st_module_config_t* getModuleConfigs() {
    static st_module_config_t MODULE_CONFIG_LIST[] = {
        MODULE_CONFIGS
    };

    return MODULE_CONFIG_LIST;
}

#endif // _DEVICE_TREE_X5H_H_
