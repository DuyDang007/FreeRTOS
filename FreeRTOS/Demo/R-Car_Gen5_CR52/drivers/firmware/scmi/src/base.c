/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 * Copyright (C) 2018-2021 ARM Ltd.
 *
 * System Control and Management Interface (SCMI) Base Protocol
 *
 * SPDX-License-Identifier: GPL-2.0
 */

#include <string.h>
#include "protocol.h"
#include "util.h"
#include "scmi/r_scmi_protocol_base.h"

DT_SCMI_PROTOCOL_DEFINE_NODEV(SCMI_PROTOCOL_BASE, NULL);

#define BASE_ATTRIBUTES_NUM_AGENT(x)    (uint32_t)(((x) >> 8) & 0xFF)
#define BASE_ATTRIBUTES_NUM_PROTOCOL(x) (uint32_t)(((x) >> 0) & 0xFF)

enum scmi_base_protocol_cmd {
    BASE_DISCOVER_VENDOR = 0x3,
    BASE_DISCOVER_SUB_VENDOR = 0x4,
    BASE_DISCOVER_IMPLEMENT_VERSION = 0x5,
    BASE_DISCOVER_LIST_PROTOCOLS = 0x6,
    BASE_DISCOVER_AGENT = 0x7,
    BASE_NOTIFY_ERRORS = 0x8,
    BASE_SET_DEVICE_PERMISSIONS = 0x9,
    BASE_SET_PROTOCOL_PERMISSIONS = 0xa,
    BASE_RESET_AGENT_CONFIGURATION = 0xb,
};

struct scmi_msg_resp_base_attributes {
    int32_t  status;
    uint32_t attributes;
};

struct scmi_msg_resp_discover_vendor {
	int32_t status;
	char 	vendor_id[SCMI_SHORT_NAME_MAX_SIZE];
};

struct scmi_msg_resp_base_discover_impl_version {
    int32_t  status;
    uint32_t version;
};

struct scmi_msg_resp_base_discover_list_proto {
    int32_t  status;
    uint32_t num_protocols;
	uint8_t protocols[4];
};

struct scmi_msg_resp_base_discover_agent {
	int32_t     status;
	uint32_t    agent_id;
	uint8_t     name[SCMI_SHORT_NAME_MAX_SIZE];
};

int R_SCMI_BaseVersionGet(uint32_t *version)
{
	struct scmi_protocol *proto = &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_BASE);
	struct scmi_msg_resp_version reply_buffer;
	struct scmi_message msg, reply;
	int ret;

	/* sanity checks */
	if (!version) {
		return -EINVAL;
	}

	if (proto->id != SCMI_PROTOCOL_BASE) {
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

/**
 * scmi_base_attributes_get() - gets the implementation details
 *  that are associated with the base protocol.
 *
 * @num_protocols: number of protocols
 * @num_agents: number of agents
 *
 * Return: 0 on success, else appropriate SCMI error.
 */
int R_SCMI_BaseAttributesGet(uint8_t *num_protocols, uint8_t *num_agents)
{
	struct scmi_protocol *proto = &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_BASE);
	struct scmi_msg_resp_base_attributes reply_buffer;
	struct scmi_message msg, reply;
	int ret;

	/* sanity checks */
	if (!proto || !num_protocols || !num_agents) {
		return -EINVAL;
	}

	if (proto->id != SCMI_PROTOCOL_BASE) {
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

	*num_agents = (uint8_t)BASE_ATTRIBUTES_NUM_AGENT(reply_buffer.attributes);
    *num_protocols = (uint8_t)BASE_ATTRIBUTES_NUM_PROTOCOL(reply_buffer.attributes);

	return 0;
}

int R_SCMI_BaseVendorIdGet(bool sub_vendor, char *vendor_id)
{
	struct scmi_protocol *proto = &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_BASE);
	struct scmi_msg_resp_discover_vendor reply_buffer;
	struct scmi_message msg, reply;
	int ret;
	int msg_id = sub_vendor ? BASE_DISCOVER_SUB_VENDOR : BASE_DISCOVER_VENDOR;

	/* sanity checks */
	if (!vendor_id) {
		return -EINVAL;
	}

	if (proto->id != SCMI_PROTOCOL_BASE) {
		return -EINVAL;
	}

	msg.hdr = SCMI_MESSAGE_HDR_MAKE(msg_id, SCMI_COMMAND, proto->id, 0x0);
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
	
	strncpy(vendor_id, reply_buffer.vendor_id, SCMI_SHORT_NAME_MAX_SIZE);

	return 0;
}

int R_SCMI_BaseImplementationVersionGet(uint32_t *impl_version)
{
	struct scmi_protocol *proto = &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_BASE);
	struct scmi_msg_resp_base_discover_impl_version reply_buffer;
	struct scmi_message msg, reply;
	int ret;

	/* sanity checks */
	if (!impl_version) {
		return -EINVAL;
	}

	if (proto->id != SCMI_PROTOCOL_BASE) {
		return -EINVAL;
	}

	msg.hdr = SCMI_MESSAGE_HDR_MAKE(BASE_DISCOVER_IMPLEMENT_VERSION,
									SCMI_COMMAND, proto->id, 0x0);
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
	
	*impl_version = reply_buffer.version;

	return 0;
}

int R_SCMI_BaseDiscoverListProtocols(uint32_t *num_protocols,
                                      uint8_t *protocols)
{
	struct scmi_protocol *proto = &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_BASE);
	struct scmi_msg_resp_base_discover_list_proto reply_buffer;
	struct scmi_message msg, reply;
	int ret;
	uint32_t skip = 0;

	/* sanity checks */
	if (!num_protocols || !protocols) {
		return -EINVAL;
	}

	if (proto->id != SCMI_PROTOCOL_BASE) {
		return -EINVAL;
	}

	msg.hdr = SCMI_MESSAGE_HDR_MAKE(BASE_DISCOVER_LIST_PROTOCOLS,
									SCMI_COMMAND, proto->id, 0x0);
	msg.len = sizeof(skip);
	msg.content = &skip;

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
	
	*num_protocols = reply_buffer.num_protocols;
	memcpy(protocols, reply_buffer.protocols, 4);

	return 0;
}

int R_SCMI_BaseDiscoverAgentGet(uint32_t request_agent_id,
								 uint32_t *agent_id, uint8_t *name)
{
	struct scmi_protocol *proto = &SCMI_PROTOCOL_NAME(SCMI_PROTOCOL_BASE);
	struct scmi_msg_resp_base_discover_agent reply_buffer;
	struct scmi_message msg, reply;
	int ret;

	/* sanity checks */
	if (!agent_id || !name) {
		return -EINVAL;
	}

	if (proto->id != SCMI_PROTOCOL_BASE) {
		return -EINVAL;
	}

	msg.hdr = SCMI_MESSAGE_HDR_MAKE(BASE_DISCOVER_AGENT,
									SCMI_COMMAND, proto->id, 0x0);
	msg.len = sizeof(request_agent_id);
	msg.content = &request_agent_id;

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
	
	*agent_id = reply_buffer.agent_id;
	strncpy(name, reply_buffer.name, SCMI_SHORT_NAME_MAX_SIZE);

	return 0;
}

