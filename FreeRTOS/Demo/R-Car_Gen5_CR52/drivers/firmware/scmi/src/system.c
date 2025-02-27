/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

#include "scmi/inc/protocol.h"
#include "scmi/inc/util.h"
#include "scmi/inc/common.h"
#include "scmi/inc/system.h"

#define SYSTEM_MESSAGE_ATTRIBUTES_WARM_RESET(x) \
	(uint32_t)(((x) >> 31) & 0x1)
#define SYSTEM_MESSAGE_ATTRIBUTES_SUSPEND(x) \
	(uint32_t)(((x) >> 30) & 0x1)

SCMI_PROTOCOL_DEFINE_NODEV(SCMI_PROTOCOL_SYSTEM, NULL);

enum scmi_system_prot_cmd {
    SYSTEM_POWER_STATE_SET = 0x3,
    SYSTEM_POWER_STATE_GET,
    SYSTEM_POWER_STATE_NOTIFY,
};

struct system_info {
    uint32_t version;
    bool graceful_timeout_supported;
};

struct system_attributes {
    int32_t     status;
    uint32_t    attributes;
};

struct system_power_state_get {
    int32_t  status;
    uint32_t state;
};

struct system_power_state_set {
	uint32_t flags;
    uint32_t state;
    int32_t  status;
};

struct scmi_protocol *scmi_system_proto_get(void)
{
	return &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_SYSTEM);
}

int scmi_system_version_get(uint32_t *version)
{
	struct scmi_protocol *proto = &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_SYSTEM);
	struct scmi_msg_resp_version reply_buffer;
	struct scmi_message msg, reply;
	int ret;

	/* sanity checks */
	if (!version) {
		return -EINVAL;
	}

	if (proto->id != SCMI_PROTOCOL_SYSTEM) {
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

int scmi_system_message_attributes(bool *warm_reset_support,
        						   bool *system_suspend_support)
{
	struct scmi_protocol *proto = &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_SYSTEM);
	int message_id = SYSTEM_POWER_STATE_SET;
	struct system_attributes reply_buffer;
	struct scmi_message msg, reply;
	int ret;

	/* sanity checks */
	if (!warm_reset_support || !system_suspend_support) {
		return -EINVAL;
	}

	if (proto->id != SCMI_PROTOCOL_SYSTEM) {
		return -EINVAL;
	}

	msg.hdr = SCMI_MESSAGE_HDR_MAKE(SCMI_PROTOCOL_MESSAGE_ATTRIBUTES,
									SCMI_COMMAND, proto->id, 0x0);
	msg.len = sizeof(message_id);
	msg.content = &message_id;

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

	*warm_reset_support =
		SYSTEM_MESSAGE_ATTRIBUTES_WARM_RESET(reply_buffer.attributes);
	*system_suspend_support =
		SYSTEM_MESSAGE_ATTRIBUTES_SUSPEND(reply_buffer.attributes);

	return ret;
}

int scmi_system_request_notify(bool enable)
{
	struct scmi_protocol *proto = &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_SYSTEM);
	uint32_t reply_status;
	uint32_t notify_enable = (uint32_t)enable;
	struct scmi_message msg, reply;
	int ret;

	if (proto->id != SCMI_PROTOCOL_SYSTEM) {
		return -EINVAL;
	}

	msg.hdr = SCMI_MESSAGE_HDR_MAKE(SYSTEM_POWER_STATE_NOTIFY, SCMI_COMMAND,
					proto->id, 0x0);
	msg.len = sizeof(notify_enable);
	msg.content = &notify_enable;

	reply.hdr = msg.hdr;
	reply.len = sizeof(reply_status);
	reply.content = &reply_status;

	ret = scmi_send_message(proto, &msg, &reply);
	if (ret < 0) {
		return ret;
	}

	if (reply_status != SCMI_SUCCESS) {
		return scmi_status_to_errno(reply_status);
	}

	return 0;
}

int scmi_system_power_state_get(uint32_t *state)
{
	struct scmi_protocol *proto = &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_SYSTEM);
	struct system_power_state_get reply_buffer;
	struct scmi_message msg, reply;
	int ret;

	/* sanity checks */
	if (!state) {
		return -EINVAL;
	}

	if (proto->id != SCMI_PROTOCOL_SYSTEM) {
		return -EINVAL;
	}

	msg.hdr = SCMI_MESSAGE_HDR_MAKE(SYSTEM_POWER_STATE_GET, SCMI_COMMAND,
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

	*state = (uint32_t)reply_buffer.state;

	return 0;
}

int scmi_system_power_state_set(uint32_t flags, uint32_t state)
{
	struct scmi_protocol *proto = &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_SYSTEM);
	uint32_t reply_status = SCMI_GENERIC_ERROR;
	struct scmi_message msg, reply;
	int ret;
	struct system_power_state_set system_power_set = {
		.flags = flags,
		.state = state,
	};

	if (proto->id != SCMI_PROTOCOL_SYSTEM) {
		return -EINVAL;
	}

	msg.hdr = SCMI_MESSAGE_HDR_MAKE(SYSTEM_POWER_STATE_SET, SCMI_COMMAND,
					proto->id, 0x0);
	msg.len = sizeof(system_power_set);
	msg.content = &system_power_set;

	reply.hdr = msg.hdr;
	reply.len = sizeof(reply_status);
	reply.content = &reply_status;

	ret = scmi_send_message(proto, &msg, &reply);
	if (ret < 0) {
		return ret;
	}

	if (reply_status != SCMI_SUCCESS) {
		return scmi_status_to_errno(reply_status);
	}

	return 0;
}

