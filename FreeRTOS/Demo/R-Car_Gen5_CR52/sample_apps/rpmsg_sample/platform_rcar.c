#include <stdint.h>
#include "platform_info.h"

/* Define shared RT-VRAM area for each channel */
#define CHANNEL_RAM_SIZE (0x100000) // 1MB
#define CHANNEL_BASE(ch) (0x17C00000 + ch*CHANNEL_RAM_SIZE) // ch=[0-3]

/* Remote processor operations from r52 to a720. It defines
 * notification operation and remote processor managementi operations. */
extern const struct remoteproc_ops x5h_r_a_proc_ops;

/*----------------------------- RPMSG Platform implementation ----------------------------*/
/* Init platform: This function does:
- Init related HW module if required
- Create a `struct remoteproc` and return it to `platform` pointer

*/
int platform_init(int argc, char *argv[], void **platform)
{
    return 0;
}


/* Create a RPMsg VirtIO device
- Allocate shared memory
- Create a virtio device with remoteproc
- Init rpmsg virtio device and return it.
*/
struct rpmsg_device *
platform_create_rpmsg_vdev(void *platform, unsigned int vdev_index,
                        unsigned int role,
                        void (*rst_cb)(struct virtio_device *vdev),
                        rpmsg_ns_bind_cb ns_bind_cb)
{
    return NULL;
}

/* Wait for notification from driver
Return 0 if got noti
Otherwise return negative value
*/
int platform_poll(void *platform)
{
    return 0;
}

/* Deinit RPMsg device, call 2 functions:
- `rpmsg_deinit_vdev`
- `remoteproc_remove_virtio`
*/
void platform_release_rpmsg_vdev(struct rpmsg_device *rpdev, void *platform)
{

}

/* Remove platform resource of remote proc
- Remove `remote_proc` device
- Free memory, deinit HW
*/
void platform_cleanup(void *platform)
{

}
