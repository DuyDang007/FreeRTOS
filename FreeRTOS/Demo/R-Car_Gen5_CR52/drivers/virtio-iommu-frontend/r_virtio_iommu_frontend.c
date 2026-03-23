/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <openamp/open_amp.h>
#include <openamp/version.h>
#include <metal/alloc.h>
#include <metal/version.h>
#include <openamp/remoteproc.h>
#include <openamp/rpmsg_virtio.h>

#include "FreeRTOS.h"
#include "virtio/r_virtio.h"
#include "platform_rcar.h"
#include "rsc_table.h"
#include "virtio-iommu-frontend/r_virtio_iommu_frontend.h"
#include "r_virtio_iommu.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define RPMSG_SERV_NAME "virtio-iommu"

#define RSC_MEM_PA_OFFSET           0x0000UL
#define RSC_MEM_SIZE                0x1000UL
#define VRING_MEM_PA_OFFSET         0x1000UL
#define VRING_MEM_OFFSET            0x4000UL
#define SHARED_BUF_PA_OFFSET        0x9000UL
#define SHARED_BUF_SIZE             0x40000UL


/* Globals */
st_virtio_endpoint_t *lept;


static int rpmsg_endpoint_cb(struct rpmsg_endpoint *ept, void *data, size_t len,
                 uint32_t src, void *priv);
static void rpmsg_service_unbind(struct rpmsg_endpoint *ept);

virtio_iommu_frontend_instance_ctrl_t * R_VIRTIO_IOMMU_Init(e_mfis_channel_t ch)
{
    virtio_iommu_frontend_instance_ctrl_t *result = NULL;
    uintptr_t rsc_table_address;
    int rsc_size = 0;

    rsc_table_address = (uintptr_t)get_resource_table(ch, &rsc_size);

    st_rsc_table_info_t rsc_table = {
        .rsc_mem_pa         = rsc_table_address + RSC_MEM_PA_OFFSET,
        .rsc_mem_size       = RSC_MEM_SIZE,
        .vring_mem_pa       = rsc_table_address + VRING_MEM_PA_OFFSET,
        .vring_mem_offset   = VRING_MEM_OFFSET,
        .shared_buf_pa      = rsc_table_address + SHARED_BUF_PA_OFFSET,
        .shared_buf_size    = SHARED_BUF_SIZE,
    };

    st_virtio_instance_ctrl_t *virtio_inst = NULL;
    lept = ( struct rpmsg_endpoint * ) pvPortMalloc( sizeof( struct rpmsg_endpoint ) );
    virtio_inst = R_VIRTIO_FE_Create(ch, &rsc_table);
    if(virtio_inst != NULL)
    {
        int ret = R_VIRTIO_CreateEP(virtio_inst, lept, RPMSG_SERV_NAME, rpmsg_endpoint_cb, rpmsg_service_unbind, NULL);
        if( ret == 0)
        {
            result = (virtio_iommu_frontend_instance_ctrl_t *)virtio_inst;
        }
    }

    return result;
}

int R_VIRTIO_IOMMU_DeInit(void)
{
    return 0;
}

int R_VIRTIO_IOMMU_Attach(st_smmu_streamid_instance_ctrl_t *p_ctrl)
{
    st_virtio_msg_t req  = {0};
    st_virtio_msg_t resp = {0};
    virtio_iommu_frontend_instance_ctrl_t *p_virtio_fe = (virtio_iommu_frontend_instance_ctrl_t *)p_ctrl->p_context;
    req.hdr.driver_id = (uint8_t)VIRTIO_SMMU_ID;

    st_virtio_smmu_payload_req_t *smmu = (st_virtio_smmu_payload_req_t *)req.payload;
    smmu->type = VIRTIO_IOMMU_T_ATTACH;
    /* Use memcpy to copy p_ctrl because some members may be const-qualified */
    memcpy(&smmu->p_ctrl, p_ctrl, sizeof(st_smmu_streamid_instance_ctrl_t));
    
    int ret = R_VIRTIO_SendData(lept, &req, sizeof(st_virtio_msg_t));

    return 0;
}

int R_VIRTIO_IOMMU_Map(st_smmu_streamid_instance_ctrl_t *p_ctrl,
                        uint64_t va, uint64_t pa, uint64_t size, uint64_t attr)
{
    st_virtio_msg_t req  = {0};
    st_virtio_msg_t resp = {0};
    virtio_iommu_frontend_instance_ctrl_t *p_virtio_fe = (virtio_iommu_frontend_instance_ctrl_t *)p_ctrl->p_context;

    req.hdr.driver_id = (uint8_t)VIRTIO_SMMU_ID;

    st_virtio_smmu_payload_req_t *smmu = (st_virtio_smmu_payload_req_t *)req.payload;
    smmu->type = VIRTIO_IOMMU_T_MAP;
    /* Use memcpy to copy p_ctrl because some members may be const-qualified */
    memcpy(&smmu->p_ctrl, p_ctrl, sizeof(st_smmu_streamid_instance_ctrl_t));
    smmu->va   = va;
    smmu->pa   = pa;
    smmu->size = size;
    smmu->attr = attr;
    
    int ret = R_VIRTIO_SendData(lept, &req, sizeof(st_virtio_msg_t));

    return 0;
}

int R_VIRTIO_IOMMU_UnMap(st_smmu_streamid_instance_ctrl_t *p_ctrl,
                          uint64_t va, uint64_t pa, uint64_t size)
{
    st_virtio_msg_t req  = {0};
    st_virtio_msg_t resp = {0};
    virtio_iommu_frontend_instance_ctrl_t *p_virtio_fe = (virtio_iommu_frontend_instance_ctrl_t *)p_ctrl->p_context;

    req.hdr.driver_id = (uint8_t)VIRTIO_SMMU_ID;

    st_virtio_smmu_payload_req_t *smmu = (st_virtio_smmu_payload_req_t *)req.payload;
    smmu->type = VIRTIO_IOMMU_T_UNMAP;
    /* Use memcpy to copy p_ctrl because some members may be const-qualified */
    memcpy(&smmu->p_ctrl, p_ctrl, sizeof(st_smmu_streamid_instance_ctrl_t));
    smmu->va   = va;
    smmu->pa   = pa;
    smmu->size = size;

    int ret = R_VIRTIO_SendData(lept, &req, sizeof(st_virtio_msg_t));

    return 0;
}

int R_VIRTIO_IOMMU_Detach(st_smmu_streamid_instance_ctrl_t *p_ctrl)
{
    st_virtio_msg_t req  = {0};
    st_virtio_msg_t resp = {0};
    virtio_iommu_frontend_instance_ctrl_t *p_virtio_fe = (virtio_iommu_frontend_instance_ctrl_t *)p_ctrl->p_context;

    req.hdr.driver_id = (uint8_t)VIRTIO_SMMU_ID;

    st_virtio_smmu_payload_req_t *smmu = (st_virtio_smmu_payload_req_t *)req.payload;
    smmu->type = VIRTIO_IOMMU_T_DETACH;
    /* Use memcpy to copy p_ctrl because some members may be const-qualified */
    memcpy(&smmu->p_ctrl, p_ctrl, sizeof(st_smmu_streamid_instance_ctrl_t));

    int ret = R_VIRTIO_SendData(lept, &req, sizeof(st_virtio_msg_t));

    return 0;
}

static int rpmsg_endpoint_cb(struct rpmsg_endpoint *ept, void *data, size_t len,
			     uint32_t src, void *priv)
{
	char payload[RPMSG_BUFFER_SIZE];
    (void)priv;
    (void)src;

    memset(payload, 0, RPMSG_BUFFER_SIZE);
    memcpy(payload, data, len);

    return RPMSG_SUCCESS;
}

static void rpmsg_service_unbind(struct rpmsg_endpoint *ept)
{
    (void)ept;
}


