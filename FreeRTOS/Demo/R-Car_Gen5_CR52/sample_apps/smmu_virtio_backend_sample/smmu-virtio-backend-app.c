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
#include "smmu-virtio-backend/r_smmu_virtio_backend.h"


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
    smmu_virtio_instance_ctrl_t *smmu_virtio_inst;
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
    

    printf("TC1: SMMU Virtio Create. Waiting for connection ...\r\n");
    smmu_virtio_inst = R_SMMU_VIRTIO_BackEnd_Init(mfis_ch);
    if(smmu_virtio_inst == NULL)
    {
        printf("Result: Failed\r\n");
    }
    else
    {
        printf("Result: Passed\r\n");
    };

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
