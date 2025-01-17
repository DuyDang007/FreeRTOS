/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include "scmi/r_scmi_api.h"
#include "scmi/include/protocol.h"

int scmi_driver_init(void) {
    return SCMI_SUCCESS;
}

int scmi_protocol_version_get(uint8_t protocol, uint32_t *version) {
    return SCMI_SUCCESS;
}
int scmi_base_attributes_get(uint8_t *num_protocols, uint8_t *num_agents) {
    return SCMI_SUCCESS;
}

int scmi_base_vendor_id_get(bool sub_vendor, char *vendor_id) {
    return SCMI_SUCCESS;
}

int scmi_base_implementation_version_get(uint32_t *impl_version) {
    return SCMI_SUCCESS;
}

int scmi_base_implementation_list_get(uint32_t *num_protocols,
                                      uint8_t *protocols_imp) {
    return SCMI_SUCCESS;
}

int scmi_base_discover_agent_get(uint32_t *agent_id, uint8_t *name) {
    return SCMI_SUCCESS;
}

int scmi_system_message_attributes(bool *warm_reset_support,
        bool *system_suspend_support) {
    return SCMI_SUCCESS;
}

int scmi_system_power_state_get(uint32_t *state) {
    return SCMI_SUCCESS;
}

int scmi_system_power_state_set(uint32_t flags, uint32_t state) {
    return SCMI_SUCCESS;
}

int scmi_power_attributes_get(uint8_t *pd_count) {
    return SCMI_SUCCESS;
}

int scmi_power_domain_attributes_get(uint32_t domain_id,
                                    uint32_t *flags, uint8_t *name) {
    return SCMI_SUCCESS;
}

int scmi_power_domain_state_get(uint32_t domain, uint32_t *state) {
    return SCMI_SUCCESS;
}

int scmi_power_domain_state_set(uint32_t flags, uint32_t domain,
                                    uint32_t *state) {
    return SCMI_SUCCESS;
}

