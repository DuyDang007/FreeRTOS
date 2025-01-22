/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SCMI_MAILBOX_H_
#define _SCMI_MAILBOX_H_

#include "transport.h"
#include "util.h"
#include "shmem.h"
#include "mbox.h"

/* get the name of mailbox channel's private data */
#define _SCMI_MBOX_CHAN_NAME(proto, idx) \
	CONCAT3(SCMI_TRANSPORT_CHAN_NAME(proto, idx), _, priv)

struct scmi_mbox_channel {
	/* SHMEM area bound to the channel */
	const struct scmi_dev *shmem;
	/* TX dbell */
	struct mbox_spec tx;
	/* TX reply dbell */
	struct mbox_spec tx_reply;
};

int scmi_mbox_init(struct scmi_dev *transport);

#endif /* _SCMI_MAILBOX_H_ */
