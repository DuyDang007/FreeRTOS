/**********************************************************/
/* Sample program : Virtio MMU Function                   */
/* File Name      : virtio_mmu.c                          */
/* Copyright (C) Renesas Electronics Corp. 2025.          */
/**********************************************************/

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

