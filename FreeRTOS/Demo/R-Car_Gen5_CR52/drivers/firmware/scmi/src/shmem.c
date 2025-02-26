/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include "protocol.h"
#include "shmem.h"
#include "util.h"
#include "common.h"
#include "rcar_scmi_common.h"

struct scmi_shmem_config {
	uintptr_t phys_addr;
	uint32_t size;
};

struct scmi_shmem_data {
	uintptr_t regmap;
};

struct scmi_shmem_layout {
	volatile uint32_t res0;
	volatile uint32_t chan_status;
	volatile uint32_t res1[2];
	volatile uint32_t chan_flags;
	volatile uint32_t len;
	volatile uint32_t msg_hdr;
};

int scmi_shmem_get_channel_status(const struct scmi_dev *dev, uint32_t *status)
{
	struct scmi_shmem_data *data;
	struct scmi_shmem_layout *layout;

	data = dev->data;
	layout = (struct scmi_shmem_layout *)data->regmap;

	*status = layout->chan_status;

	return 0;
}

static void scmi_shmem_memcpy(uintptr_t dst, const uintptr_t src, uint32_t bytes)
{
	int i;

	for (i = 0; i < bytes; i++) {
		((uint8_t*)dst)[i] = ((const uint8_t*)src)[i];
	}
}

int scmi_shmem_read_message(const struct scmi_dev *shmem, struct scmi_message *msg)
{
	struct scmi_shmem_layout *layout;
	struct scmi_shmem_data *data;
	const struct scmi_shmem_config *cfg;

	data = shmem->data;
	cfg = shmem->config;
	layout = (struct scmi_shmem_layout *)data->regmap;

	/* some sanity checks first */
	if (!msg) {
		return -EINVAL;
	}

	if ((!msg->content && msg->len) || (msg->content && !msg->len)) {
		return -EINVAL;
	}

	if (cfg->size < (sizeof(*layout) + msg->len)) {
		SCMI_LOG_ERR("message doesn't fit in shmem area");
		return -EINVAL;
	}

	/* mismatch between expected reply size and actual size? */
	if (msg->len != (layout->len - sizeof(layout->msg_hdr))) {
		SCMI_LOG_ERR("bad message len. Expected 0x%x, got 0x%x",
			msg->len,
			(uint32_t)(layout->len - sizeof(layout->msg_hdr)));
		return -EINVAL;
	}

	/* header match? */
	if (layout->msg_hdr != msg->hdr) {
		SCMI_LOG_ERR("bad message header. Expected 0x%x, got 0x%x",
			msg->hdr, layout->msg_hdr);
		return -EINVAL;
	}

	if (msg->content) {
		scmi_shmem_memcpy(POINTER_TO_UINT(msg->content),
				  data->regmap + sizeof(*layout), msg->len);
	}

	return 0;
}

int scmi_shmem_write_message(const struct scmi_dev *shmem, struct scmi_message *msg)
{
	struct scmi_shmem_layout *layout;
	struct scmi_shmem_data *data;
	const struct scmi_shmem_config *cfg;

	data = shmem->data;
	cfg = shmem->config;
	layout = (struct scmi_shmem_layout *)data->regmap;

	/* some sanity checks first */
	if (!msg) {
		return -EINVAL;
	}

	if ((!msg->content && msg->len) || (msg->content && !msg->len)) {
		return -EINVAL;
	}

	if (cfg->size < (sizeof(*layout) + msg->len)) {
		return -EINVAL;
	}

	if (!(layout->chan_status & SCMI_SHMEM_CHAN_STATUS_FREE_BIT)) {
		return -EBUSY;
	}

	layout->len = sizeof(layout->msg_hdr) + msg->len;
	layout->msg_hdr = msg->hdr;

	if (msg->content) {
		scmi_shmem_memcpy(data->regmap + sizeof(*layout),
				  POINTER_TO_UINT(msg->content), msg->len);
	}

	/* done, mark channel as busy and proceed */
	layout->chan_status &= ~SCMI_SHMEM_CHAN_STATUS_FREE_BIT;

	return 0;
}

uint32_t scmi_shmem_channel_status(const struct scmi_dev *shmem)
{
	struct scmi_shmem_layout *layout;
	struct scmi_shmem_data *data;

	data = shmem->data;
	layout = (struct scmi_shmem_layout *)data->regmap;

	return layout->chan_status;
}

void scmi_shmem_update_flags(const struct scmi_dev *shmem, uint32_t mask, uint32_t val)
{
	struct scmi_shmem_layout *layout;
	struct scmi_shmem_data *data;

	data = shmem->data;
	layout = (struct scmi_shmem_layout *)data->regmap;

	layout->chan_flags = (layout->chan_flags & ~mask) | (val & mask);
}

const struct scmi_shmem_config config = {
	.phys_addr = X5H_SCMI_SHMEM_PLATFORM_CR52,
	.size = X5H_SCMI_SHMEM_SIZE - sizeof(struct scmi_shmem_layout),
};

struct scmi_shmem_data data;

int scmi_shmem_init(struct scmi_dev *dev)
{
	if (!dev)
		return -EINVAL;

	dev->config = &config;
	dev->data = &data;

	if (config.size < sizeof(struct scmi_shmem_layout)) {
		return -EINVAL;
	}

	/* No MMU -> map 1:1 */
	data.regmap = config.phys_addr;

	return 0;
}

