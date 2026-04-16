/*
 * Copyright (c) 2026 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>
#include <openamp/open_amp.h>
#include <openamp/version.h>
#include <metal/alloc.h>
#include <metal/version.h>
#include "FreeRTOS.h"
#include "interrupts.h"
#include "platform_info.h"
#include "rsc_table.h"
#include "pfc/r_pfc_api.h"

int printf_raw(const char *format, ...);

#define RPMSG_SERVICE_NAME  "rpmsg-perf-sample"
#define LPRINTF(format, ...) printf_raw(format, ##__VA_ARGS__); vTaskDelay(10);
#define LPERROR(format, ...) LPRINTF("ERROR: " format, ##__VA_ARGS__)

#define HELLO_MSG "Hello world from CR52/FreeRTOS!"
#define BYE_MSG   "Goodbye!"

static int shutdown_req = 0;

void echoTask(void *pvParameters);
void vMainAssertCalled(const char *pcFileName, uint32_t ulLineNumber);
void vDeleteCallingTask(void);

static int rpmsg_endpoint_cb(struct rpmsg_endpoint *ept, void *data, size_t len,
                              uint32_t src, void *priv)
{
    int32_t ret = RPMSG_SUCCESS;
    /* cppcheck-suppress misra-c2012-11.5 */
    const char *msg = (const char *)data;

    (void)priv;
    (void)src;

    if ((len == strlen(BYE_MSG)) &&
        (strncmp(msg, BYE_MSG, strlen(BYE_MSG)) == 0)) {
        shutdown_req = 1;
        (void)rpmsg_send(ept, BYE_MSG, strlen(BYE_MSG));
    } else {
        (void)rpmsg_send(ept, HELLO_MSG, strlen(HELLO_MSG));
    }

    return ret;
}

static void rpmsg_service_unbind(struct rpmsg_endpoint *ept)
{
    (void)ept;
    shutdown_req = 1;
}

static void prvSetupHardware(void)
{
    portDISABLE_INTERRUPTS();
    Irq_Setup();
    (void)pfcInitModules(getModuleConfigs());
}

void echoTask(void *pvParameters)
{
    struct rpmsg_endpoint lept;
    int32_t ret;
    void *platform;
    struct rpmsg_device *rpdev;

    (void)pvParameters;

    (void)LPRINTF("openamp lib version: %s (", openamp_version());
    (void)LPRINTF("Major: %d, ", openamp_version_major());
    (void)LPRINTF("Minor: %d, ", openamp_version_minor());
    (void)LPRINTF("Patch: %d)\r\n", openamp_version_patch());
    (void)LPRINTF("libmetal lib version: %s (", metal_ver());
    (void)LPRINTF("Major: %d, ", metal_ver_major());
    (void)LPRINTF("Minor: %d, ", metal_ver_minor());
    (void)LPRINTF("Patch: %d)\r\n", metal_ver_patch());
    (void)LPRINTF("Starting application...\r\n");

    ret = platform_init(MFIS_CHAN, &platform);
    if (ret != 0) {
        (void)LPERROR("Failed to initialize platform.\r\n");
    } else {
        rpdev = platform_create_rpmsg_vdev(platform, 0,
                                           VIRTIO_DEV_DEVICE,
                                           NULL, NULL);
        if (rpdev == NULL) {
            (void)LPERROR("Failed to create rpmsg virtio device.\r\n");
        } else {
            (void)LPRINTF("Try to create rpmsg endpoint.\r\n");

            ret = rpmsg_create_ept(&lept, rpdev, RPMSG_SERVICE_NAME,
                                   RPMSG_ADDR_ANY, RPMSG_ADDR_ANY,
                                   rpmsg_endpoint_cb,
                                   rpmsg_service_unbind);
            if (ret != 0) {
                (void)LPERROR("Failed to create endpoint.\r\n");
            } else {
                (void)LPRINTF("Successfully created rpmsg endpoint.\r\n");
                (void)LPRINTF("RPMsg device TX buffer size: %#x\r\n",
                              rpmsg_get_tx_buffer_size(&lept));
                (void)LPRINTF("RPMsg device RX buffer size: %#x\r\n",
                              rpmsg_get_rx_buffer_size(&lept));

                while (1) {
                    (void)platform_poll(platform);
                    if (shutdown_req != 0) {
                        break;
                    }
                }

                (void)LPRINTF("Goodbye received, terminating...\r\n");

                rpmsg_destroy_ept(&lept);
                platform_release_rpmsg_vdev(rpdev, platform);
                platform_cleanup(platform);
            }
        }
    }

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void)
{
    prvSetupHardware();
    xTaskCreate(echoTask, "echoTask",
                configMINIMAL_STACK_SIZE, NULL,
                (tskIDLE_PRIORITY + 1), NULL);
    vTaskStartScheduler();

    for (;;) {
    }

    return 0;
}

void vMainAssertCalled(const char *pcFileName, uint32_t ulLineNumber)
{
    (void)printf_raw("ASSERT!  Line %d of file %s\n", ulLineNumber, pcFileName);
    taskENTER_CRITICAL();
    for (;;) {
    }
}

void vDeleteCallingTask(void)
{
    vTaskDelete(NULL);
}
