/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <errno.h>
#include "virtio_mmu.h"

static int r_iommu_is_domain_supported(domain_t domain)
{
    if (domain > 0 && domain < SMMU_MAX)
        return 1;

    return 0;
}

int R_IOMMU_AttachDev(domain_t domain, struct st_device)
{
    int ret;

    ret = r_iommu_is_domain_supported(domain);
    if (ret)
        return -EINVAL;

    // To Do: send to virtio backend through RPMsg

    return ret;
}

int R_IOMMU_DetachDev(domain_t domain, struct st_device)
{
    int ret;

    // To Do: send to virtio backend through RPMsg

    return ret;
}

