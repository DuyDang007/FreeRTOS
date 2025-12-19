/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

/**
 * @file r_smmu_virtio_backend.h
 * @brief SMMU Virtio backend interface.
 *
 * This header provides public APIs for initializing and deinitializing
 * the SMMU Virtio backend used in inter-processor communication.
 */

#ifndef R_SMMU_VIRTIO_BACKEND_H
#define R_SMMU_VIRTIO_BACKEND_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "virtio/r_virtio.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/* None */

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef void smmu_virtio_instance_ctrl_t;

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/

/**
 * @brief Initialize the SMMU Virtio backend.
 *
 * This function sets up the backend resources required to support
 * Virtio-based communication through the SMMU.
 *
 * @return 0 on success.
 * @return Non-zero value on failure.
 */
smmu_virtio_instance_ctrl_t * R_SMMU_VIRTIO_BackEnd_Init(e_mfis_channel_t ch);

/**
 * @brief Deinitialize the SMMU Virtio backend.
 *
 * This function releases all resources allocated by the Virtio backend
 * and disables SMMU-related configurations.
 *
 * @return 0 on success.
 * @return Non-zero value on failure.
 */
uint8_t R_SMMU_VIRTIO_BackEnd_DeInit(smmu_virtio_instance_ctrl_t * p_inst);

#endif /* R_SMMU_VIRTIO_BACKEND_H */
