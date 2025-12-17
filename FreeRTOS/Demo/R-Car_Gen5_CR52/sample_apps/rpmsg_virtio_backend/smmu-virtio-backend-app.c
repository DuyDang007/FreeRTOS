/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * This is a sample demonstration application that showcases usage of rpmsg
 * This application is meant to run on the remote CPU running baremetal code.
 * This application echoes back data that was sent to it by the host core.
 */

#include <stdio.h>
#include <errno.h>
#include <openamp/open_amp.h>
#include <openamp/version.h>
#include <metal/alloc.h>
#include <metal/version.h>
#include "FreeRTOS.h"
#include "interrupts.h"
#include "pfc/r_pfc_api.h"
#include "virtio/r_virtio.h"

#define VIRTIO_SERVICE_NAME         "virtio-smmu"
#define SHUTDOWN_MSG    0xEF56A55A

static st_virtio_endpoint_t lept;
static int shutdown_req = 0;
SemaphoreHandle_t xSemaphore = NULL;

#define hello_msg "Hello world from CR52/Free-RTOS!"
#define goodbye_msg "Good bye!"

/*-----------------------------------------------------------------------------*
 *  RPMSG endpoint callbacks
 *-----------------------------------------------------------------------------*/
static int rpmsg_endpoint_cb(struct rpmsg_endpoint *ept, void *data, size_t len,
                 uint32_t src, void *priv)
{
    char payload[RPMSG_BUFFER_SIZE];
    (void)priv;
    (void)src;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    memset(payload, 0, RPMSG_BUFFER_SIZE);
    memcpy(payload, data, len);

    /* On reception of a shutdown we signal the application to terminate */
    if ((*(unsigned int *)data) == SHUTDOWN_MSG) {
        shutdown_req = 1;
        return RPMSG_SUCCESS;
    }

    rpmsg_send(ept, hello_msg, strlen(hello_msg));

    return RPMSG_SUCCESS;
}

static void rpmsg_service_unbind(struct rpmsg_endpoint *ept)
{
    (void)ept;
    printf("unexpected Remote endpoint destroy\r\n");
    shutdown_req = 1;
}

/*----------------------------------------------------------------------------*/
static void prvSetupHardware( void )
{
    /* Ensure no interrupts execute while the scheduler is in an inconsistent
    state.  Interrupts are automatically enabled when the scheduler is
    started. */
    portDISABLE_INTERRUPTS();

    Irq_Setup();

    (void)pfcInitModules(getModuleConfigs());
}

/*-----------------------------------------------------------------------------*
 *  Application
 *-----------------------------------------------------------------------------*/
void echoTask( void *pvParameters )
{
    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;
    int ret;
    printf("Starting Virtio sample\r\n");
    st_virtio_instance_ctrl_t *virtio_inst;
    e_mfis_channel_t mfis_ch;
    if (MFIS_CHAN == 0)
    {
        mfis_ch = MFIS_CR_TO_CA_CH0;
    }
    else if(MFIS_CHAN == 1)
    {
        mfis_ch = MFIS_CR_TO_CA_CH1;
    }
    else
    {
        printf("MFIS Channel not support\r\n");
    }
    

    printf("TC1: Virtio Create. Waiting for connection ...\r\n");
    virtio_inst = R_VIRTIO_Create(mfis_ch);
    if(virtio_inst == NULL)
    {
        printf("Result: Failed\r\n");
    }
    else
    {
        printf("Result: Passed\r\n");
    }

        /* Initialize RPMSG framework */
    printf("TC2: create smmu-virtio endpoint.\r\n");

    ret = R_VIRTIO_CreateEP(virtio_inst, &lept, VIRTIO_SERVICE_NAME,
                   rpmsg_endpoint_cb,
                   rpmsg_service_unbind, NULL);
    if (ret)
    {
        printf("Result: Failed\r\n");
    }
    else
    {
        printf("Result: Passed\r\n");
    };

    printf("TC3: Endpoint Callback Handler.\r\n");

    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
    {
        printf("Result: Passed\r\n");
    }

    for( ;; )
    {
        vTaskDelay(1);
    }
}


/*-----------------------------------------------------------------------------*
 *  Application entry point
 *-----------------------------------------------------------------------------*/
int main(void)
{

    /* Configure the hardware ready to run the demo. */
    prvSetupHardware();

    xSemaphore = xSemaphoreCreateBinary();

    xTaskCreate( echoTask, "echoTask", configMINIMAL_STACK_SIZE, NULL, ( tskIDLE_PRIORITY + 1 ), NULL );
    /* Start the tasks and timer running. */
    vTaskStartScheduler();
    for( ;; )
    {
    }
    /* Don't expect to reach here. */

    return 0;
}

/*-----------------------------------------------------------*/

int printf_raw(const char *format, ...);

void vMainAssertCalled( const char *pcFileName, uint32_t ulLineNumber )
{
    /* Don't use printf as it uses FreeRTOS resources */
    printf_raw("ASSERT!  Line %d of file %s\n", ulLineNumber, pcFileName);
    taskENTER_CRITICAL();
    for( ;; );
}

void vDeleteCallingTask( void )
{
     vTaskDelete( NULL );
}
