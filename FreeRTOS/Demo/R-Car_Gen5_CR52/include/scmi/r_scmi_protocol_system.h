/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

/*
 * Base protocol describes the properties of the implementation and provides generic error management. 
 */
#ifndef __R_SCMI_PROTOCOL_SYSTEM_H_
#define __R_SCMI_PROTOCOL_SYSTEM_H_

/**
 * @defgroup SCMI_Protocol_System_Module SCMI Protocol System Module
 * @{
 * @brief Functions to use SCMI system protocol
 */

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief This function returns the version of the system protocol
 * 
 * @param[out] version Return the version
 *
 * @return 0 if all went fine, else return appropriate error. 
 */
int R_SCMI_SystemVersionGet(uint32_t *version);

/**
 * @brief This function returns the attributes of the system protocol
 * 
 * @param[out] warm_reset_support Pointer to store the warm reset support
 * @param[out] system_suspend_support Pointer to store the system suspend support
 *
 * @return 0 if all went fine, else return appropriate error. 
 */
int R_SCMI_SystemMessageAttributes(bool *warm_reset_support,
        bool *system_suspend_support);

/**
 * @brief This function returns the system power state
 * 
 * @param[out] state Pointer to store the system power state
 *
 * @return 0 if all went fine, else return appropriate error. 
 */
int R_SCMI_SystemPowerStateGet(uint32_t *state);

/**
 * @brief This function sets the system power state
 * 
 * @param[in] flags Specify setting gracefully or forcefully
 * @param[in] state Power state to set
 *
 * @return 0 if all went fine, else return appropriate error. 
 */
int R_SCMI_SystemPowerStateSet(uint32_t flags, uint32_t state);

/** @} */ // end of group SCMI_Protocol_System_Module

#endif // __R_SCMI_PROTOCOL_SYSTEM_H_
