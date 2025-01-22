/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdbool.h>
#include "protocol.h"
#include "mailbox.h"
#include "transport.h"
#include "common.h"

#define SCMI_CHAN_LOCK_TIMEOUT_USEC 500
#define SCMI_CHAN_SEM_TIMEOUT_USEC 500

struct scmi_dev transport_dev;
DT_SCMI_TRANSPORT_TX_CHAN_DECLARE()

int scmi_status_to_errno(int scmi_status)
{
    switch (scmi_status) {
    case SCMI_SUCCESS:
        return 0;
    case SCMI_NOT_SUPPORTED:
        return -EOPNOTSUPP;
    case SCMI_INVALID_PARAMETERS:
        return -EINVAL;
    case SCMI_DENIED:
        return -EACCES;
    case SCMI_NOT_FOUND:
        return -ENOENT;
    case SCMI_OUT_OF_RANGE:
        return -ERANGE;
    case SCMI_IN_USE:
    case SCMI_BUSY:
        return -EBUSY;
    case SCMI_PROTOCOL_ERROR:
        return -EPROTO;
    case SCMI_COMMS_ERROR:
    case SCMI_GENERIC_ERROR:
    case SCMI_HARDWARE_ERROR:
    default:
        return -EIO;
    }
}

static void scmi_core_reply_cb(struct scmi_channel *chan)
{
	/* Do nothing here */
}

static int scmi_core_setup_chan(const struct scmi_dev *transport,
                struct scmi_channel *chan, bool tx)
{
    int ret;

    if (!chan) {
        return -EINVAL;
    }

    if (chan->ready) {
        return 0;
    }

    /* no support for RX channels ATM */
    if (!tx) {
        return -ENOTSUP;
    }

    chan->cb = scmi_core_reply_cb;

    /* setup transport-related channel data */
    ret = scmi_transport_setup_chan(transport, chan, tx);
    if (ret < 0) {
        SCMI_LOG_ERR("failed to setup channel");
        return ret;
    }

    /* protocols might share a channel. In such cases, this
     * will stop them from being initialized again.
     */
    chan->ready = true;

    return 0;
}

int scmi_send_message(struct scmi_protocol *proto,
                     struct scmi_message *msg,
                     struct scmi_message *reply)
{
    int ret = 0;
	const struct scmi_transport_api *api =
		(const struct scmi_transport_api *)proto->transport->api;

    if (!proto->tx) {
        return -ENODEV;
    }

    ret = scmi_transport_send_message(proto->transport, proto->tx, msg);
    if (ret < 0) {
        SCMI_LOG_ERR("failed to send message");
		return ret;
    }

	/* Wait until the message is read by SCP.
	 * Will add timeout in the future.
	 */
	while (!api->channel_is_free(proto->transport, proto->tx));

    ret = scmi_transport_read_message(proto->transport, proto->tx, reply);
    if (ret < 0) {
        SCMI_LOG_ERR("failed to read reply");
		return ret;
    }

    return ret;
}

static int scmi_core_protocol_setup(const struct scmi_dev *transport)
{
    int ret;

	ret = scmi_core_setup_chan(transport,
			DT_SCMI_TRANSPORT_TX_CHAN(), true);
	if (ret < 0) {
		return ret;
	}

    return 0;
}

static int scmi_core_transport_init(struct scmi_dev *transport)
{
    int ret;

	ret = scmi_mbox_init(transport);
    if (ret < 0) {
        return ret;
    }

    ret = scmi_transport_init(transport);
    if (ret < 0) {
        return ret;
    }

    return scmi_core_protocol_setup(transport);
}

int R_SCMI_DriverInit(void)
{
	scmi_core_transport_init(&transport_dev);
}

