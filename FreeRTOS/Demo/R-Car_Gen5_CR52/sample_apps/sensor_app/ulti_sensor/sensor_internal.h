/*
 *
 * Copyright (c) 2026 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef SENSOR_INTERNAL_H
#define SENSOR_INTERNAL_H

#include <stdint.h>

/*******************************************************************************************************************//**
 * @enum e_scp_retcode_t
 * SCP Return code.
***********************************************************************************************************************/
typedef enum {
    SCP_RET_SUCCESS                 = 0,        /**< Successful completion of the command. */
    SCP_RET_NOT_SUPPORTED           = -1,       /**< The command or feature is not supported, or supported but not within the calling agent's view of the platform. */
    SCP_RET_INVALID_PARAMETERS      = -2,       /**< One or more parameters passed to the command are invalid or beyond legal limits. */
    SCP_RET_DENIED                  = -3,       /**< The caller is not permitted to perform the specific action, such as accessing a resource or feature that it is not allowed to use. */
    SCP_RET_NOT_FOUND               = -4,       /**< The entity that is being accessed does not exist. Examples includes non-existent or invalid commands, resources such as power domains, clocks or sensors. */
    SCP_RET_OUT_OF_RANGE            = -5,       /**< Requested settings are outside the legal range under the current operating state or condition. */
    SCP_RET_BUSY                    = -6,       /**< The platform is out of resources and thus unable to process a command. */
    SCP_RET_COMMS_ERROR             = -7,       /**< The command could not be correctly transmitted to the platform. */
    SCP_RET_GENERIC_ERROR           = -8,       /**< The command failed to be processed owing to an unspecified fault within the platform. */
    SCP_RET_HARDWARE_ERROR          = -9,       /**< A hardware error occurred in a platform component during execution of a command. */
    SCP_RET_PROTOCOL_ERROR          = -10,      /**< Returned when the receiver detects that the caller has violated the protocol specification. */
} e_scp_retcode_t;

e_scp_retcode_t r_scp_ths_set_sensor_enabled(uint32_t tsc_id, uint32_t enable);
e_scp_retcode_t r_scp_ths_get_sensor_enabled(uint32_t tsc_id, uint32_t *enabled);
e_scp_retcode_t r_scp_ths_read_temperature(uint32_t tsc_id, int32_t *temp_mc);

#endif /* SENSOR_INTERNAL_H */