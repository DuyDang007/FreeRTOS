/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef R_SMMU_VIRTIO_BACKEND_H
#define R_SMMU_VIRTIO_BACKEND_H

/**
 * @defgroup SMMU_VIRTIO_BACKEND SMMU Virtio Backend
 * @{
 * @brief SMMU Virtio backend driver interface.
 *
 * This module provides APIs to initialize and deinitialize the SMMU Virtio
 * backend used for Virtio-based inter-processor communication.
 */

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

/** @} */ // end of SMMU_VIRTIO_BACKEND_Module

#endif /* R_SMMU_VIRTIO_BACKEND_H */
