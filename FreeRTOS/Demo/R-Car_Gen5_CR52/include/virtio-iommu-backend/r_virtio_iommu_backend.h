/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

/**
 * @file R_VIRTIO_IOMMU_BACKEND_H
 * @brief Virtio IOMMU Backend Interface
 *
 * @details
 * This file provides the public interface for the Virtio IOMMU backend
 * driver. The backend owns and manages the physical IOMMU/SMMU hardware
 * and processes IOMMU requests received from a Virtio IOMMU frontend
 * running on a remote core.
 *
 * The backend communicates with the frontend using Virtio/RPMsg over
 * MFIS channels and executes IOMMU operations such as device attach,
 * detach, IOVA mapping, and unmapping on behalf of the frontend.
 */

#ifndef R_VIRTIO_IOMMU_BACKEND_H
#define R_VIRTIO_IOMMU_BACKEND_H

/**
 * @defgroup VIRTIO_IOMMU_BACKEND Virtio IOMMU Backend
 * @{
 *
 * @brief Virtio IOMMU backend driver interface
 *
 * This module implements the backend side of a Virtio-based IOMMU
 * service. It receives IOMMU operation requests from a Virtio IOMMU
 * frontend, performs the corresponding hardware IOMMU/SMMU operations
 * locally, and returns execution results back to the frontend.
 */

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "virtio/r_virtio.h"
#include "smmu/smmu.h"

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 * @typedef virtio_iommu_instance_ctrl_t
 * @brief Opaque control structure for the Virtio IOMMU backend instance.
 *
 * This type represents an internal Virtio IOMMU backend context.
 * The actual structure definition is private to the backend
 * implementation and hidden from the user.
 */
typedef void virtio_iommu_instance_ctrl_t;

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
/**
 * @brief Initialize the Virtio IOMMU backend.
 *
 * This function initializes a Virtio IOMMU backend instance, sets up
 * Virtio/RPMsg communication over the specified MFIS channel, and
 * prepares the backend to process IOMMU requests from the frontend.
 *
 * @param[in] ch MFIS channel used for Virtio communication.
 *
 * @return Pointer to Virtio IOMMU backend instance control structure.
 * @return NULL if initialization fails.
 */
virtio_iommu_instance_ctrl_t *
R_VIRTIO_IOMMU_Backend_Init(e_mfis_channel_t ch);

/**
 * @brief Deinitialize the Virtio IOMMU backend.
 *
 * This function stops the Virtio IOMMU backend instance, releases all
 * allocated resources, and disables any IOMMU/SMMU configurations
 * associated with the backend.
 *
 * @param[in] p_inst Pointer to Virtio IOMMU backend instance control.
 *
 * @return 0 on success.
 * @return Non-zero value on failure.
 */
uint8_t
R_VIRTIO_IOMMU_Backend_DeInit(virtio_iommu_instance_ctrl_t *p_inst);

/** @} */ /* end of VIRTIO_IOMMU_BACKEND */

#endif /* R_VIRTIO_IOMMU_BACKEND_H */
