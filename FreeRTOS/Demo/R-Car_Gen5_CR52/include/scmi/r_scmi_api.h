/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef R_SCMI_API_H_
#define R_SCMI_API_H_

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /** __cplusplus */

/*******************************************************************************************************************//**
 * @enum e_flags_t
 * Graceful flags.
***********************************************************************************************************************/
typedef enum {
    FLAGS_FORCEFUL              = 0,    /**< Forceful request */
    FLAGS_GRACEFUL              = 1     /**< Graceful request */
} e_flags_t;

/*******************************************************************************************************************//**
 * @enum e_system_state_t
 * SCMI System State.
***********************************************************************************************************************/
typedef enum {
    SYSTEM_STATE_SHUTDOWN       = 0,    /**< Shutdown State */
    SYSTEM_STATE_COLD_RESET     ,       /**< Cold Reset State */
    SYSTEM_STATE_WARM_RESET     ,       /**< Warm Reset State */
    SYSTEM_STATE_POWER_UP       ,       /**< Power Up State */
    SYSTEM_STATE_SUSPEND        ,       /**< Suspend State */
    SYSTEM_STATE_MAX                    /**< Number of System States */
} e_system_state_t;

int scmi_driver_init(void);
int scmi_protocol_version_get(uint8_t protocol, uint32_t *version);
int scmi_base_attributes_get(uint8_t *num_protocols, uint8_t *num_agents);
int scmi_base_vendor_id_get(bool sub_vendor, char *vendor_id);
int scmi_base_implementation_version_get(uint32_t *impl_version);
int scmi_base_implementation_list_get(uint32_t *num_protocols,
                                      uint8_t *protocols_imp);
int scmi_base_discover_agent_get(uint32_t *agent_id, uint8_t *name);
int scmi_system_message_attributes(bool *warm_reset_support,
        bool *system_suspend_support);
int scmi_system_power_state_get(uint32_t *state);
int scmi_system_power_state_set(uint32_t flags, uint32_t state);
int scmi_power_attributes_get(uint8_t *pd_count);
int scmi_power_domain_attributes_get(uint32_t domain_id,
                                    uint32_t *flags, uint8_t *name);
int scmi_power_domain_state_get(uint32_t domain, uint32_t *state);
int scmi_power_domain_state_set(uint32_t flags, uint32_t domain,
                                    uint32_t *state);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* R_SCMI_API_H_ */

