/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

#include "protocol.h"
#include "util.h"
#include "common.h"
#include "scmi/r_scmi_protocol_system.h"

DT_SCMI_PROTOCOL_DEFINE_NODEV(SCMI_PROTOCOL_SYSTEM, NULL);

enum scmi_system_prot_cmd {
    SYSTEM_POWER_STATE_SET = 0x3,
    SYSTEM_POWER_STATE_GET,
    SYSTEM_POWER_STATE_NOTIFY,
};

int R_SCMI_SystemVersionGet(uint32_t *version)
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

int R_SCMI_SystemMessageAttributes(bool *warm_reset_support,
        						   bool *system_suspend_support)
{
	int ret;
	return ret;
}

