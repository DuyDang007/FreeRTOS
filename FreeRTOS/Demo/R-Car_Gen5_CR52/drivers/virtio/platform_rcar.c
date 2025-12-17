/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <stdint.h>
#include <errno.h>
#include "FreeRTOS.h"
#include "platform_rcar.h"
#include "rsc_table.h"
#include "mfis.h"


/* Define shared DRAM area for each channel. */
#define SHARED_CH_RAM_BASE (0x40000000UL)
#define SHARED_CH_RAM_SIZE (0x80000000UL)

#define ATTR_NORM_NSHARED   NORM_NSHARED_NCACHE
#define ATTR_PRIV_RW        PRIV_RW_USER_RW
#define ATTR_NORM_NSHARED_PRIV_RW       (ATTR_NORM_NSHARED | ATTR_PRIV_RW)


/* Remote processor operations from r52 to a720. It defines
 * notification operation and remote processor managementi operations. */
extern const struct remoteproc_ops x5h_r_a_proc_ops;

/* RPMsg virtio shared buffer pool */

static struct remoteproc * platform_create_proc(int mfis_ch, int rsc_index);

/*----------------------------- RPMSG Platform implementation ----------------------------*/
/* Create platform
- Init remoteproc instance
- Map shared memory and resource table to the instance
- Set resource table
*/
static struct remoteproc * platform_create_proc(int mfis_ch, int rsc_index)
{
    struct remoteproc *rproc_inst = NULL;
    struct mfis_channel *mfis_inst = NULL;
    struct remoteproc *ret_rproc = NULL;

    void *rsc_table = NULL;
    int rsc_size = 0;
    int ret;
    metal_phys_addr_t pa;

    rproc_inst = (struct remoteproc *)pvPortMalloc(sizeof(struct remoteproc));
    mfis_inst  = (struct mfis_channel *)pvPortMalloc(sizeof(struct mfis_channel));

    if ((rproc_inst != NULL) && (mfis_inst != NULL))
    {
        /* init mfis instance */
        mfis_inst->ch = (unsigned int)mfis_ch;
        mfis_inst->int_source = 0U;
        mfis_inst->recv_message = 0U;
        mfis_inst->cb_function = NULL;

        /* Initialize the resource table on shared memory */
        init_resource_table();

        rsc_table = get_resource_table(rsc_index, &rsc_size);

        if ((rsc_table != NULL) && (rsc_size > 0))
        {
            if (remoteproc_init(rproc_inst,
                                &x5h_r_a_proc_ops,
                                (void *)mfis_inst) != NULL)
            {
                /* mmap resource table */
                pa = (metal_phys_addr_t)rsc_table;
                (void)remoteproc_mmap(rproc_inst,
                                      &pa,
                                      NULL,
                                      rsc_size,
                                      ATTR_NORM_NSHARED_PRIV_RW,
                                      &(rproc_inst->rsc_io));

                /* mmap shared memory */
                pa = SHARED_CH_RAM_BASE;
                (void)remoteproc_mmap(rproc_inst,
                                      &pa,
                                      NULL,
                                      SHARED_CH_RAM_SIZE,
                                      ATTR_NORM_NSHARED_PRIV_RW,
                                      NULL);

                /* parse resource table */
                ret = remoteproc_set_rsc_table(rproc_inst,
                                               rproc_inst->rsc_io->virt,
                                               rsc_size);
                if (ret == 0)
                {
                    ret_rproc = rproc_inst;
                }
            }
        }
    }

    /* cleanup on failure */
    if (ret_rproc == NULL)
    {
        if (rproc_inst != NULL)
        {
            remoteproc_remove(rproc_inst);
            vPortFree(rproc_inst);
        }
        if (mfis_inst != NULL)
        {
            vPortFree(mfis_inst);
        }
    }

    return ret_rproc;
}


/* Init platform: This function does:
- Init related HW module if required
- Create a `struct remoteproc` and return it to `platform` pointer
*/
int platform_init(int channel, struct remoteproc **platform)
{
    int ret = -EINVAL;
    unsigned long mfis_ch = (unsigned long)channel;
    unsigned long rsc_id = 0UL;
    struct remoteproc *rproc = NULL;

    if (platform != NULL)
    {
        rproc = platform_create_proc(mfis_ch, rsc_id);
        if (rproc != NULL)
        {
            *platform = rproc;
            ret = 0;
        }
    }

    return ret;
}



/* Create a RPMsg VirtIO device
- Create VirtIO device of remoteproc instance
- (Driver only) Initialize the shared buffer pool
- Init rpmsg virtio device with remoteproc device
- 
*/
struct rpmsg_device *
platform_create_rpmsg_vdev(struct remoteproc *platform,
                           unsigned int vdev_index,
                           unsigned int role,
                           void (*rst_cb)(struct virtio_device *vdev),
                           rpmsg_ns_bind_cb ns_bind_cb)
{
    struct rpmsg_device *ret_rpdev = NULL;
    struct rpmsg_virtio_device *rpmsg_vdev = NULL;
    struct virtio_device *vdev = NULL;
    /* MISRA-C:2012 Rule 11.5 deviation
    * Reason:
    *  - platform is an opaque context pointer defined by OpenAMP API
    *  - Cast is required to restore the original object type
    *  - platform always points to struct remoteproc created internally
    *  - No pointer arithmetic is performed
    */
    struct remoteproc *rproc = platform;
    struct metal_io_region *shbuf_io = NULL;
    void *shbuf;
    int ret = 1;

    if (rproc != NULL)
    {
        rpmsg_vdev = metal_allocate_memory(sizeof(*rpmsg_vdev));
    }

    if (rpmsg_vdev != NULL)
    {
        shbuf_io = remoteproc_get_io_with_pa(rproc, SHARED_CH_RAM_BASE);
    }

    if (shbuf_io != NULL)
    {
        shbuf = metal_io_phys_to_virt(shbuf_io, SHARED_CH_RAM_BASE);

        vdev = remoteproc_create_virtio(rproc,
                                        vdev_index,
                                        role,
                                        rst_cb);
    }

    if (vdev != NULL)
    {
        ret = rpmsg_init_vdev(rpmsg_vdev,
                                vdev,
                                ns_bind_cb,
                                shbuf_io,
                                NULL);
    }

    if (ret == 0)
    {
        ret_rpdev =
            rpmsg_virtio_get_rpmsg_device(rpmsg_vdev);
    }

    /* cleanup on failure */
    if ((ret_rpdev == NULL) && (rpmsg_vdev != NULL))
    {
        if (vdev != NULL)
        {
            remoteproc_remove_virtio(rproc, vdev);
        }
        metal_free_memory(rpmsg_vdev);
    }

    return ret_rpdev;
}


/* Wait for notification from driver
Return 0 if got noti
Otherwise return negative value
*/
int platform_poll(struct remoteproc *platform)
{
    struct remoteproc *rproc = platform;
    struct mfis_channel* mfis = (struct mfis_channel*)rproc->priv;
    int ret = -1;

    if (0 != mfis->int_source)
    {
	remoteproc_get_notification(rproc, RSC_NOTIFY_ID_ANY);
        mfis->int_source = 0; // Reset int source to 0
        ret = 0;
    }

    return ret;
}

/* Deinit RPMsg device, call 2 functions:
- `rpmsg_deinit_vdev`
- `remoteproc_remove_virtio`
*/
void platform_release_rpmsg_vdev(struct rpmsg_device *rpdev, struct remoteproc *platform)
{
    (void)platform;
    (void)rpdev;
}

/* Remove platform resource of remote proc
- Remove `remote_proc` device
- Free memory, deinit HW
*/
void platform_cleanup(struct remoteproc *platform)
{
    (void)platform;
}
