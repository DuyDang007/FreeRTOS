/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief SCMI SHMEM API
 */

#ifndef _SCMI_SHMEM_H_
#define _SCMI_SHMEM_H_

#include "cmsis_rcar_gen5.h"

#define SCMI_SHMEM_CHAN_STATUS_FREE_BIT BIT(0)
#define SCMI_SHMEM_CHAN_FLAG_IRQ_BIT BIT(0)

struct scmi_message;

/**
 * @brief Write a message in the SHMEM area
 *
 * @param shmem pointer to shmem device
 * @param msg message to write
 *
 * @retval 0 if successful
 * @retval negative errno if failure
 */
int scmi_shmem_write_message(const struct scmi_dev *shmem,
			     struct scmi_message *msg);

/**
 * @brief Read a message from a SHMEM area
 *
 * @param shmem pointer to shmem device
 * @param msg message to write the data into
 *
 * @retval 0 if successful
 * @retval negative errno if failure
 */
int scmi_shmem_read_message(const struct scmi_dev *shmem,
			    struct scmi_message *msg);

/**
 * @brief Update the channel flags
 *
 * @param shmem pointer to shmem device
 * @param mask value to negate and bitwise-and the old
 * channel flags value
 * @param val value to bitwise and with the mask and
 * bitwise-or with the masked old value
 */
void scmi_shmem_update_flags(const struct scmi_dev *shmem,
			     uint32_t mask, uint32_t val);

/**
 * @brief Read a channel's status
 *
 * @param shmem pointer to shmem device
 */
uint32_t scmi_shmem_channel_status(const struct scmi_dev *shmem);

int scmi_shmem_init(struct scmi_dev *dev);

#endif /* _SCMI_SHMEM_H_ */
