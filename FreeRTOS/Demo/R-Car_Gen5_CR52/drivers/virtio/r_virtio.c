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
#include "platform_rcar.h"
#include "rsc_table.h"
#include "interrupts.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define hello_msg "Hello world from CR52/Free-RTOS!"
#define goodbye_msg "Good bye!"

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void Virtio_Task( void *pvParameters );
/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
struct st_virtio_instance_ctrl
{
    struct remoteproc             * platform;
    struct rpmsg_device           * rp_dev;
    e_mfis_channel_t                mfis_ch;
    uint8_t                         is_initialized;

    st_virtio_endpoint_t          * p_endpoit_list;
    uint16_t                        ept_num;
};

static st_virtio_instance_ctrl_t *virtio_inst[MFIS_CH_MAX] =
{
    [0 ... (MFIS_CH_MAX - 1)] = NULL
};

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
st_virtio_instance_ctrl_t * R_VIRTIO_Create(e_mfis_channel_t mfis_ch)
{
    st_virtio_instance_ctrl_t *result;
    int ret;
    static int create_task = 1;
    if(virtio_inst[mfis_ch] != NULL)
    {
        result = virtio_inst[mfis_ch];
    }
    else
    {
        result = ( st_virtio_instance_ctrl_t * ) pvPortMalloc( sizeof( st_virtio_instance_ctrl_t ) );
        
        ret = platform_init(mfis_ch, &(result->platform));
        if (ret != 0) {
            vPortFree(result);
            result = NULL;
        } else {
            result->rp_dev = platform_create_rpmsg_vdev(result->platform, 0, VIRTIO_DEV_DEVICE, NULL, NULL);
            if (!(result->rp_dev))
            {
                vPortFree(result);
                result = NULL;
            }
            else
            {
                result->is_initialized = 1;
                virtio_inst[mfis_ch] = result;
            }
        }
    }

    if(create_task == 1)
    {
        xTaskCreate( Virtio_Task, "Virtio_Task", configMINIMAL_STACK_SIZE, NULL, ( tskIDLE_PRIORITY + 3 ), NULL );
        create_task = 0;
    }

    return result;
}

uint8_t R_VIRTIO_Release(st_virtio_instance_ctrl_t * p_ctrl)
{
    platform_release_rpmsg_vdev(p_ctrl->rp_dev, p_ctrl->platform);
    platform_cleanup(p_ctrl->platform);

    return 0;
}

uint8_t R_VIRTIO_CreateEP(st_virtio_instance_ctrl_t *p_vdev_ctrl, st_virtio_endpoint_t * p_ept,
		    const char *name, virtio_ept_cb ept_cb, virtio_ns_unbind_cb unbind_cb, void *priv)
{
    int ret;
    p_ept->priv = priv;
    ret = rpmsg_create_ept(p_ept, p_vdev_ctrl->rp_dev, name,
                   RPMSG_ADDR_ANY, RPMSG_ADDR_ANY,
                   ept_cb,
                   unbind_cb);
    return ret;
}

uint8_t R_VIRTIO_ReleaseEP(st_virtio_endpoint_t * p_ept)
{
    rpmsg_destroy_ept(p_ept);
    return 0;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
static void Virtio_Task( void *pvParameters )
{
    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    for( ;; )
    {
        for (int ch = 0; ch < (int)MFIS_CH_MAX; ch++)
        {
            if (virtio_inst[ch] != NULL)
            {
                (void)platform_poll(virtio_inst[ch]->platform);
            }
        }
        
        vTaskDelay(1);
    }
}