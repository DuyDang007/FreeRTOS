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

#include "FreeRTOS.h"
#include "virtio/r_virtio.h"
#include "smmu-virtio-backend/r_smmu_virtio_backend.h"
#include "platform_rcar.h"
#include "rsc_table.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define VIRTIO_SERVICE_NAME         "virtio-smmu"
#define SHUTDOWN_MSG    0xEF56A55A

#define hello_msg "Hello world from CR52/Free-RTOS!"
#define goodbye_msg "Good bye!"
/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static int rpmsg_endpoint_cb(struct rpmsg_endpoint *ept, void *data, size_t len,
                 uint32_t src, void *priv);
static void rpmsg_service_unbind(struct rpmsg_endpoint *ept);
/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
smmu_virtio_instance_ctrl_t * R_SMMU_VIRTIO_BackEnd_Init(e_mfis_channel_t ch)
{
    smmu_virtio_instance_ctrl_t *result = NULL;
    st_virtio_instance_ctrl_t *virtio_inst = NULL;
    st_virtio_endpoint_t *lept = ( struct rpmsg_endpoint * ) pvPortMalloc( sizeof( struct rpmsg_endpoint ) );
    
    virtio_inst = R_VIRTIO_Create(ch);
    if(virtio_inst != NULL)
    {
        int ret = R_VIRTIO_CreateEP(virtio_inst, lept, VIRTIO_SERVICE_NAME, rpmsg_endpoint_cb, rpmsg_service_unbind, NULL);
        if( ret == 0)
        {
            result = (smmu_virtio_instance_ctrl_t *)virtio_inst;
        }
    }

    return result;
}

uint8_t R_SMMU_VIRTIO_BackEnd_DeInit(smmu_virtio_instance_ctrl_t * p_inst)
{
    (void)p_inst;
    return 0;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
static int rpmsg_endpoint_cb(struct rpmsg_endpoint *ept, void *data, size_t len,
                 uint32_t src, void *priv)
{
    (void)ept;
    (void)priv;
    (void)src;
    (void)data;
    (void)len;

    /* On reception of a shutdown we signal the application to terminate */

    return RPMSG_SUCCESS;
}

static void rpmsg_service_unbind(struct rpmsg_endpoint *ept)
{
    (void)ept;
}