/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <metal/io.h>
#include <openamp/remoteproc.h>
#include "platform_rcar.h"
#include "mfis.h"
#include "FreeRTOS.h"

/* prototype */
void x5h_proc_interrupt_cb(void *arg);
void *metal_machine_io_mem_map(void *va, metal_phys_addr_t pa,
			       size_t size, unsigned int flags);

void x5h_proc_interrupt_cb(void *arg)
{
    (void)arg;
	return;
}

/* Implementation of io mem mapping function */
void *metal_machine_io_mem_map(void *va, metal_phys_addr_t pa,
			       size_t size, unsigned int flags)
{
	metal_unused(size);
	metal_unused(flags);

	/* Add implementation here */
    /* Since virtual address is not used, just return the physical address */
    va = (void *)pa;

	return va;
}

/* Implementation of remoteproc device for X5H */

/* Initialize the remoteproc instance
- Create a metal device
- Get IO regison accessor: metal_device_io_region
- Register interrupt handler, enable interrupt 
 -> Change to an MFIS instance
*/
static struct remoteproc *
x5h_proc_init(struct remoteproc *rproc,
              const struct remoteproc_ops *ops,
              void *arg)
{
    (void)ops;
    struct remoteproc_priv *rproc_priv = arg;
    struct remoteproc *ret = NULL;
    struct mfis_channel *mfis;

    if ((rproc != NULL) && (arg != NULL))
    {
        /* MISRA-C:2012 Rule 11.5 deviation
         * arg is expected to point to struct mfis_channel by design.
         */
        mfis = rproc_priv->p_mfis_ch;
        mfis->cb_function = x5h_proc_interrupt_cb;
        mfis->arg = mfis;
        mfis_init(mfis);

        ret = rproc;
    }

    return ret;
}


/* Remove the remoteproc instance
- Disable, unregister interrupt
- Close metal device
*/
static void x5h_proc_remove(struct remoteproc *rproc)
{
    mfis_deinit((struct mfis_channel *)rproc->priv);
}

/* Memory map the memory with physical address or destination address as input
- Create and initialize `struct remoteproc_mem` object to contain shared mem info
- Create and initialize `struct metal_io_region` object to assign to rproc device
- Add the `metal_io_region` to rproc device
*/
static void *
x5h_proc_mmap(struct remoteproc *rproc, metal_phys_addr_t *pa,
              metal_phys_addr_t *da, size_t size,
              unsigned int attribute, struct metal_io_region **io)
{
    struct remoteproc_mem *mem = NULL;
    struct metal_io_region *tmpio = NULL;
    metal_phys_addr_t lpa;
    metal_phys_addr_t lda;
    void *ret = NULL;

    /* Skip checking valid address of pa, da */
    lda = *da;
    lpa = *pa;

    if ((lpa != METAL_BAD_PHYS) || (lda != METAL_BAD_PHYS))
    {
        if (lpa == METAL_BAD_PHYS)
        {
            lpa = lda;
        }
        
        if (lda == METAL_BAD_PHYS)
        {
            lda = lpa;
        }

        if (attribute == 0U)
        {
            attribute = ATTR_NORM_SHARED_NCACHE_PRIV_RW_USER_RW;
        }

        mem = metal_allocate_memory(sizeof(*mem));
        if (mem != NULL)
        {
            mem->pa = lpa;

            tmpio = metal_allocate_memory(sizeof(*tmpio));
            if (tmpio != NULL)
            {
                /* MISRA-C:2012 Rule 11.x deviation
                 * Casting physical address to pointer is platform specific.
                 * On this platform, VA == PA mapping is guaranteed by design.
                 */
                metal_io_init(tmpio,
                              (void *)lpa,
                              &mem->pa,
                              size,
                              (sizeof(metal_phys_addr_t) << 3),
                              attribute,
                              NULL);

                /* Init the memory object and assign to rproc */
                remoteproc_init_mem(mem, NULL, lpa, lda, size, tmpio);
                remoteproc_add_mem(rproc, mem);

                if (io != NULL)
                {
                    *io = tmpio;
                }

                *pa = lpa;
                *da = lda;

                ret = metal_io_phys_to_virt(tmpio, mem->pa);
            }
            else
            {
                metal_free_memory(mem);
            }
        }
    }

    return ret;
}


/* Notify the remote
- Trigger interrupt to remote
*/
static int x5h_proc_notify(struct remoteproc *rproc, uint32_t id)
{
    struct remoteproc_priv *priv = rproc->priv;
    uint16_t irq_id = (uint16_t)id;
    irq_id &= (uint16_t)0x7FFF;
    return mfis_trigger_interrupt(priv->p_mfis_ch, irq_id);
}

/* Remote processor operations from r52 to a720. It defines
 * notification operation and remote processor managementi operations. */
const struct remoteproc_ops x5h_r_a_proc_ops = {
    .init   = &x5h_proc_init,
    .remove = &x5h_proc_remove,
    .mmap   = &x5h_proc_mmap,
    .notify = &x5h_proc_notify,
    // Do not implement since CR can boot independently
    .start  = NULL,
    .stop   = NULL,
    .shutdown = NULL,
};
