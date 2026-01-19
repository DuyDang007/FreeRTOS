/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

/**
 * @file R_VIRTIO_IOMMU_FRONTEND_H
 * @brief Virtio IOMMU Frontend Interface
 *
 * @details
 * This file provides the public API definitions for the Virtio IOMMU
 * frontend driver. The frontend communicates with a Virtio IOMMU backend
 * service running on a remote core via Virtio/RPMsg.
 *
 * The frontend exposes APIs for device attach/detach and IOVA mapping,
 * while the actual IOMMU/SMMU hardware is owned and controlled by the
 * backend.
 *
 * This interface is intended to be used by drivers running on the CA
 * core (Linux or RTOS) to manage DMA address translation through a
 * virtualized IOMMU service.
 */

#ifndef R_VIRTIO_IOMMU_FRONTEND_H
#define R_VIRTIO_IOMMU_FRONTEND_H

/**
 * @defgroup VIRTIO_IOMMU_FRONTEND Virtio IOMMU Frontend
 * @ingroup Middleware
 * @{
 *
 * @brief Virtio IOMMU frontend module
 *
 * This module implements the frontend side of a Virtio-based IOMMU
 * service. It forwards IOMMU operation requests (attach, detach, map,
 * unmap) to a backend running on another core, which performs the actual
 * hardware IOMMU/SMMU operations.
 */

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>
#include "smmu/smmu.h"

/***********************************************************************************************************************
 * Public API Functions
 **********************************************************************************************************************/
/**
 * @brief Initialize the Virtio IOMMU frontend.
 *
 * This function initializes the Virtio IOMMU frontend and establishes
 * communication with the Virtio IOMMU backend service.
 *
 * This function must be called before invoking any other Virtio IOMMU
 * frontend APIs.
 *
 * @return 0 on success.
 * @return Negative error code on failure.
 */
int R_VIRTIO_IOMMU_Init(void);

/**
 * @brief Deinitialize the Virtio IOMMU frontend.
 *
 * This function shuts down the Virtio IOMMU frontend and releases all
 * global resources allocated during R_VIRTIO_IOMMU_ProbeDev().
 *
 * @return 0 on success.
 * @return Negative error code on failure.
 */
int R_VIRTIO_IOMMU_DeInit(void);

/**
 * @brief Attach a device to an IOMMU domain.
 *
 * This function sends a request to the Virtio IOMMU backend to attach
 * a device, identified by its Stream ID, to an IOMMU domain.
 *
 * After attachment, DMA transactions issued by the device will be
 * translated according to the domain configuration managed by the
 * backend.
 *
 * @note Memory mappings must be created separately using
 *       R_VIRTIO_IOMMU_MapDev().
 *
 * @param[in] p_ctrl Pointer to the SMMU stream ID control structure
 *                   describing the device and domain context.
 *
 * @return 0 on success.
 * @return Negative error code on failure.
 */
int R_VIRTIO_IOMMU_Attach(st_smmu_streamid_instance_ctrl_t *p_ctrl);

/**
 * @brief Map an IOVA to a physical address for a device.
 *
 * This function requests the Virtio IOMMU backend to create an
 * IOVA-to-physical address translation entry for the specified device.
 *
 * The device must be attached to an IOMMU domain before calling this
 * function.
 *
 * @param[in] p_ctrl Pointer to the SMMU stream ID control structure.
 * @param[in] va     I/O virtual address (IOVA) visible to the device.
 * @param[in] pa     Physical address in system memory.
 * @param[in] size   Size of the mapping in bytes.
 * @param[in] attr   Mapping attributes (read/write, cache, etc.).
 *
 * @return 0 on success.
 * @return Negative error code on failure.
 */
int R_VIRTIO_IOMMU_Map(st_smmu_streamid_instance_ctrl_t *p_ctrl,
                          uint64_t va,
                          uint64_t pa,
                          uint64_t size,
                          uint64_t attr);

/**
 * @brief Unmap an IOVA mapping from an IOMMU domain.
 *
 * This function requests the Virtio IOMMU backend to remove an existing
 * IOVA-to-physical address mapping associated with the specified device.
 *
 * @param[in] p_ctrl Pointer to the SMMU stream ID control structure.
 * @param[in] va     I/O virtual address (IOVA) to be unmapped.
 * @param[in] pa     Physical address previously mapped (if required).
 * @param[in] size   Size of the mapping to remove in bytes.
 *
 * @return 0 on success.
 * @return Negative error code on failure.
 */
int R_VIRTIO_IOMMU_UnMap(st_smmu_streamid_instance_ctrl_t *p_ctrl,
                            uint64_t va,
                            uint64_t pa,
                            uint64_t size);

/**
 * @brief Detach a device from an IOMMU domain.
 *
 * This function sends a request to the Virtio IOMMU backend to detach
 * a device from its associated IOMMU domain.
 *
 * After detachment, the device will no longer be able to perform DMA
 * transactions through the IOMMU.
 *
 * @note All IOVA mappings should be removed before calling this function.
 *
 * @param[in] p_ctrl Pointer to the SMMU stream ID control structure.
 *
 * @return 0 on success.
 * @return Negative error code on failure.
 */
int R_VIRTIO_IOMMU_Detach(st_smmu_streamid_instance_ctrl_t *p_ctrl);

/** @} */ /* end of VIRTIO_IOMMU_FRONTEND */

#endif /* R_VIRTIO_IOMMU_FRONTEND_H */
