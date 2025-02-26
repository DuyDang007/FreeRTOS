/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef __VIRTIO_MMU_H__
#define __VIRTIO_MMU_H__

/**
 * @defgroup Virtual_IOMMU_Module Virtual IOMMU Module
 * @{
 * @brief Virtual IOMMU
 *
 * Virtual IOMMU
 */

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/**
 * @brief SMMU_DSP_BASE
 */
#define SMMU_DSP_BASE       0xfa000000
/**
 * @brief SMMU_HCN_BASE
 */
#define SMMU_HCN_BASE       0xf8800000
/**
 * @brief SMMU_HCS0_BASE
 */
#define SMMU_HCS0_BASE      0xfcc00000
/**
 * @brief SMMU_HCS1_BASE
 */
#define SMMU_HCS1_BASE      0xfce00000
/**
 * @brief SMMU_IMN_BASE
 */
#define SMMU_IMN_BASE       0xf8000000
/**
 * @brief SMMU_IMS_BASE
 */
#define SMMU_IMS_BASE       0xfc000000
/**
 * @brief SMMU_MP_BASE
 */
#define SMMU_MP_BASE        0xfe000000
/**
 * @brief SMMU_NPU0_BASE
 */
#define SMMU_NPU0_BASE      0xf8400000
/**
 * @brief SMMU_NPU1_BASE
 */
#define SMMU_NPU1_BASE      0xfc400000
/**
 * @brief SMMU_PERE_BASE
 */
#define SMMU_PERE_BASE      0xfd000000
/**
 * @brief SMMU_PERW_BASE
 */
#define SMMU_PERW_BASE      0xfd800000
/**
 * @brief SMMU_PV_BASE
 */
#define SMMU_PV_BASE        0xf8600000
/**
 * @brief SMMU_RT_BASE
 */
#define SMMU_RT_BASE        0xfb000000
/**
 * @brief SMMU_SCP_BASE
 */
#define SMMU_SCP_BASE       0xfa800000
/**
 * @brief SMMU_VI0_BASE
 */
#define SMMU_VI0_BASE       0xf9000000
/**
 * @brief SMMU_VI1_BASE
 */
#define SMMU_VI1_BASE       0xf9800000
/**
 * @brief SMMU_VIPN_BASE
 */
#define SMMU_VIPN_BASE      0xf8200000
/**
 * @brief SMMU_VIPS_BASE
 */
#define SMMU_VIPS_BASE      0xfc200000
/**
 * @brief SMMU_DSP_FMU_BASE
 */
#define SMMU_DSP_FMU_BASE   0xcbf60000
/**
 * @brief SMMU_HCN_FMU_BASE
 */
#define SMMU_HCN_FMU_BASE   0xc9cb0000
/**
 * @brief SMMU_HCS0_FMU_BASE
 */
#define SMMU_HCS0_FMU_BASE  0xde860000
/**
 * @brief SMMU_HCS1_FMU_BASE
 */
#define SMMU_HCS1_FMU_BASE  0xde870000
/**
 * @brief SMMU_IMN_FMU_BASE
 */
#define SMMU_IMN_FMU_BASE   0xc1980000
/**
 * @brief SMMU_IMS_FMU_BASE
 */
#define SMMU_IMS_FMU_BASE   0xc1d80000
/**
 * @brief SMMU_MP_FMU_BASE
 */
#define SMMU_MP_FMU_BASE    0xed600000
/**
 * @brief SMMU_NPU0_FMU_BASE
 */
#define SMMU_NPU0_FMU_BASE  0xd2f60000
/**
 * @brief SMMU_NPU1_FMU_BASE
 */
#define SMMU_NPU1_FMU_BASE  0xd6f60000
/**
 * @brief SMMU_PERE_FMU_BASE
 */
#define SMMU_PERE_FMU_BASE  0xc09e0000
/**
 * @brief SMMU_PERW_FMU_BASE
 */
#define SMMU_PERW_FMU_BASE  0xc05b0000
/**
 * @brief SMMU_PV_FMU_BASE
 */
#define SMMU_PV_FMU_BASE    0xcb500000
/**
 * @brief SMMU_RT_FMU_BASE
 */
#define SMMU_RT_FMU_BASE    0x19450000
/**
 * @brief SMMU_SCP_FMU_BASE
 */
#define SMMU_SCP_FMU_BASE   0xc1180000
/**
 * @brief SMMU_VI0_FMU_BASE
 */
#define SMMU_VI0_FMU_BASE   0xc5840000
/**
 * @brief SMMU_VI1_FMU_BASE
 */
#define SMMU_VI1_FMU_BASE   0xc5850000
/**
 * @brief SMMU_VIPN_FMU_BASE
 */
#define SMMU_VIPN_FMU_BASE  0xc3130000
/**
 * @brief SMMU_VIPS_FMU_BASE
 */
#define SMMU_VIPS_FMU_BASE  0xc3530000

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 * @brief domain
 */
typedef enum {
    SMMU_DSP = 0,
    SMMU_HCN,
    SMMU_HCS0,
    SMMU_HCS1,
    SMMU_IMN,
    SMMU_IMS,
    SMMU_MP,
    SMMU_NPU0,
    SMMU_NPU1,
    SMMU_PERE,
    SMMU_PERW,
    SMMU_PV,
    SMMU_RT,
    SMMU_SCP,
    SMMU_VI0,
    SMMU_VI1,
    SMMU_VIPN,
    SMMU_VIPS,
    SMMU_DSP_FMU,
    SMMU_HCN_FMU,
    SMMU_HCS0_FMU,
    SMMU_HCS1_FMU,
    SMMU_IMN_FMU,
    SMMU_IMS_FMU,
    SMMU_MP_FMU,
    SMMU_NPU0_FMU,
    SMMU_NPU1_FMU,
    SMMU_PERE_FMU,
    SMMU_PERW_FMU,
    SMMU_PV_FMU,
    SMMU_RT_FMU,
    SMMU_SCP_FMU,
    SMMU_VI0_FMU,
    SMMU_VI1_FMU,
    SMMU_VIPN_FMU,
    SMMU_VIPS_FMU,
    SMMU_MAX,
} domain_t;

/**
 * @brief The basic device structure
 */
typedef struct st_device {
    uint32_t *base_addr;    ///< Base address of SMMU domain.
    uint32_t size;          ///< Size of SMMU domain.
    uint32_t stream_id;     ///< Stream ID.
} device_t;

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
/**
 * @brief Attach devcie to domain
 *
 * @param domain SMMU domain
 * @param st_device Device driver
 */
int R_IOMMU_AttachDev(domain_t domain, struct st_device);

/**
 * @brief Detach devcie from domain
 *
 * @param domain SMMU domain
 * @param st_device Device driver
 */
int R_IOMMU_DetachDev(domain_t domain, struct st_device);

/** @} */ // end of Virtual_IOMMU_Module

#endif /* __VIRTIO_MMU_H__*/

