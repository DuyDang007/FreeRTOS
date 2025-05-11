/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include "scmi/inc/protocol.h"
#include "scmi/inc/util.h"
#include "scmi/inc/power.h"

SCMI_PROTOCOL_DEFINE_NODEV(SCMI_PROTOCOL_POWER_DOMAIN, NULL);

struct scmi_power_state_get_reply {
	int32_t status;
	uint32_t power_state;
};

struct scmi_power_protocol_attr {
    int32_t status;
    uint32_t attributes;
    uint32_t statistics_address_low;
    uint32_t statistics_address_high;
    uint32_t statistics_len;
};

int scmi_power_version_get(uint32_t *version)
{
	struct scmi_protocol *proto = &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_POWER_DOMAIN);
	struct scmi_msg_resp_version reply_buffer;
	struct scmi_message msg, reply;
	int ret;

	/* sanity checks */
	if (!version) {
		return -EINVAL;
	}

	if (proto->id != SCMI_PROTOCOL_POWER_DOMAIN) {
		return -EINVAL;
	}

	msg.hdr = SCMI_MESSAGE_HDR_MAKE(SCMI_PROTOCOL_VERSION, SCMI_COMMAND,
					proto->id, 0x0);
	msg.len = 0;
	msg.content = NULL;

	reply.hdr = msg.hdr;
	reply.len = sizeof(reply_buffer);
	reply.content = &reply_buffer;

	ret = scmi_send_message(proto, &msg, &reply);
	if (ret < 0) {
		return ret;
	}

	if (reply_buffer.status != SCMI_SUCCESS) {
		return scmi_status_to_errno(reply_buffer.status);
	}

	*version = (uint32_t)reply_buffer.version;

	return 0;
}

int scmi_power_protocol_attributes(uint32_t *attributes)
{
	struct scmi_protocol *proto = &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_POWER_DOMAIN);
	struct scmi_power_protocol_attr reply_buffer;
	struct scmi_message msg, reply;
	int ret;

	/* sanity checks */
	if (!attributes) {
		return -EINVAL;
	}

	if (proto->id != SCMI_PROTOCOL_POWER_DOMAIN) {
		return -EINVAL;
	}

	msg.hdr = SCMI_MESSAGE_HDR_MAKE(SCMI_PROTOCOL_ATTRIBUTES, SCMI_COMMAND,
					proto->id, 0x0);
	msg.len = 0;
	msg.content = NULL;

	reply.hdr = msg.hdr;
	reply.len = sizeof(reply_buffer);
	reply.content = &reply_buffer;

	ret = scmi_send_message(proto, &msg, &reply);
	if (ret < 0) {
		return ret;
	}

	if (reply_buffer.status != SCMI_SUCCESS) {
		return scmi_status_to_errno(reply_buffer.status);
	}

	*attributes = (uint32_t)reply_buffer.attributes;

	return 0;
}

int scmi_power_state_get(struct scmi_power_state_config *cfg)
{
	struct scmi_protocol *proto = &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_POWER_DOMAIN);
	struct scmi_power_state_get_reply reply_buffer;
	struct scmi_message msg, reply;
	int ret;

	/* sanity checks */
	if (!cfg) {
		return -EINVAL;
	}

	if (proto->id != SCMI_PROTOCOL_POWER_DOMAIN) {
		return -EINVAL;
	}

	msg.hdr = SCMI_MESSAGE_HDR_MAKE(SCMI_POWER_DOMAIN_MSG_POWER_STATE_GET, SCMI_COMMAND,
					proto->id, 0x0);
	msg.len = sizeof(cfg->domain_id);
	msg.content = &cfg->domain_id;

	reply.hdr = msg.hdr;
	reply.len = sizeof(reply_buffer);
	reply.content = &reply_buffer;

	ret = scmi_send_message(proto, &msg, &reply);
	if (ret < 0) {
		return ret;
	}

	if (reply_buffer.status != SCMI_SUCCESS) {
		return scmi_status_to_errno(reply_buffer.status);
	}

	cfg->power_state = reply_buffer.power_state;

	return 0;
}

int scmi_power_state_set(struct scmi_power_state_config *cfg)
{
	struct scmi_protocol *proto = &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_POWER_DOMAIN);
	struct scmi_message msg, reply;
	int status, ret;

	/* sanity checks */
	if (!proto || !cfg) {
		return -EINVAL;
	}

	if (proto->id != SCMI_PROTOCOL_POWER_DOMAIN) {
		return -EINVAL;
	}

	msg.hdr = SCMI_MESSAGE_HDR_MAKE(SCMI_POWER_DOMAIN_MSG_POWER_STATE_SET, SCMI_COMMAND,
					proto->id, 0x0);
	msg.len = sizeof(*cfg);
	msg.content = cfg;

	reply.hdr = msg.hdr;
	reply.len = sizeof(status);
	reply.content = &status;

	ret = scmi_send_message(proto, &msg, &reply);
	if (ret < 0) {
		return ret;
	}

	if (status != SCMI_SUCCESS) {
		return scmi_status_to_errno(status);
	}

	return 0;
}

