/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

/**
 * @file
 * @brief SCMI vendor protocol helpers
 */

#ifndef SCMI_PROTOCOL_VENDOR_H
#define SCMI_PROTOCOL_VENDOR_H

#include "util.h"
#include "protocol.h"

/**
 * @struct scmi_vendor_request_config
 *
 * @brief Describes the parameters for the VENDOR
 * command
 */
struct scmi_vendor_request_config {
    uint32_t flags;
    uint32_t domain_id;
    uint32_t reset_status;
};

/**
 * @brief Send the getting RESET status command and get its reply
 *
 * @param cfg pointer to structure containing configuration
 * domain to be get
 *
 * @retval 0 if successful
 * @retval negative errno if failure
 */
int scmi_vendor_reset_domain_status_get(struct scmi_vendor_request_config *cfg);

#endif /* SCMI_PROTOCOL_VENDOR_H */
