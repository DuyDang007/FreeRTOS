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

#ifndef R_VIRTIO_IOMMU_H
#define R_VIRTIO_IOMMU_H

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
 * @enum virtio_smmu_req_type
 * @brief Virtio IOMMU request types exchanged between frontend and backend.
 *
 * This enumeration defines the types of IOMMU operations that can be
 * requested by the Virtio IOMMU frontend.
 */
typedef enum virtio_smmu_req_type {
    VIRTIO_IOMMU_T_ATTACH,   /**< Attach device (StreamID) to IOMMU domain */
    VIRTIO_IOMMU_T_DETACH,   /**< Detach device from IOMMU domain */
    VIRTIO_IOMMU_T_MAP,      /**< Map IOVA to physical address */
    VIRTIO_IOMMU_T_UNMAP,    /**< Unmap an existing IOVA mapping */
    VIRTIO_IOMMU_T_PROBE     /**< Probe and initialize IOMMU backend */
} virtio_smmu_req_type_t;

/**
 * @struct st_virtio_smmu_payload_req
 * @brief Virtio IOMMU request payload.
 *
 * This structure is sent from the Virtio IOMMU frontend to the backend
 * to request execution of a specific IOMMU operation.
 */
typedef struct st_virtio_smmu_payload_req {
    virtio_smmu_req_type_t           type;   /**< Requested IOMMU operation */
    st_smmu_streamid_instance_ctrl_t p_ctrl; /**< StreamID / domain context */
    uint64_t                         va;     /**< I/O virtual address (IOVA) */
    uint64_t                         pa;     /**< Physical address */
    uint64_t                         size;   /**< Mapping size in bytes */
    uint64_t                         attr;   /**< Mapping attributes */
} st_virtio_smmu_payload_req_t;

/**
 * @struct st_virtio_smmu_payload_resp
 * @brief Virtio IOMMU response payload.
 *
 * This structure is returned by the backend to the frontend to indicate
 * the completion status of a requested IOMMU operation.
 */
typedef struct st_virtio_smmu_payload_resp {
    virtio_smmu_req_type_t type;     /**< IOMMU operation being responded to */
    uint32_t               result;   /**< Result code (0 = success) */
} st_virtio_smmu_payload_resp_t;

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/

#endif /* R_VIRTIO_IOMMU_H */