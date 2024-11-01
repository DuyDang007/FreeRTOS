/*
 * FreeRTOS Kernel V11.1.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://github.com/FreeRTOS
 *
 */

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "interrupts.h"
#include "stdio.h"
#include "i2c/r_i2c.h"
#define main_I2C_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )
#include "pfc/r_pfc_api.h"
#include "device_tree_x5h.h"

/*-----------------------------------------------------------*/

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void prvI2CTask( void *pvParameters );

/*-----------------------------------------------------------*/

int main( void )
{
	/* Configure the hardware ready to run the demo. */
	prvSetupHardware();


	xTaskCreate( prvI2CTask, "prvI2CTask", configMINIMAL_STACK_SIZE, NULL, main_I2C_TASK_PRIORITY, NULL);
	/* Start the tasks and timer running. */
	vTaskStartScheduler();
	for( ;; )
	{
	}
	/* Don't expect to reach here. */
	return 0;
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Ensure no interrupts execute while the scheduler is in an inconsistent
	state.  Interrupts are automatically enabled when the scheduler is
	started. */
	portDISABLE_INTERRUPTS();

	Irq_Setup();
	(void)pfcInitModules(getModuleConfigs());
}

static void prvI2CTask( void *pvParameters )
{
	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;

	uint8_t send_data[] = { 0x03, 0x02, 0x06};
	uint8_t result[2] = {0};
	uint8_t default_value[2] = {0};

	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;

	/* Device driver part */
	i2c_instance_ctrl_t g_i2c_device_ctrl_1;
	i2c_master_cfg_t        g_i2c_device_cfg_1 =
	{
	    .channel       = 1,
	    .rate          = I2C_MASTER_RATE_FAST,
	    .slave         = 0x77,
	    .addr_mode     = I2C_MASTER_ADDR_MODE_7BIT,
	    .p_callback    = NULL,     // Callback
	    .p_context     = &g_i2c_device_ctrl_1,
	};
	    printf("PROGRAM START\r\n");
	    R_I2C_Open(&g_i2c_device_ctrl_1, &g_i2c_device_cfg_1);

	    R_I2C_ReadRegMap(&g_i2c_device_ctrl_1, 0x03, default_value, sizeof(default_value));
	    printf("DEFAULT VALUE: ");
	    for (uint8_t i = 0; i < sizeof(default_value); i++)
		printf("0x%x\t", default_value[i]);
	    printf("\r\n");

	    printf("WRITE DATA \r\n");
	    R_I2C_Write(&g_i2c_device_ctrl_1, send_data, sizeof(send_data), 0);
	    printf("WRITE DONE\r\n");

	    R_I2C_ReadRegMap(&g_i2c_device_ctrl_1, 0x03, (uint8_t *)&result, sizeof(result));
	    printf("READ DATA: ");
	    for(uint8_t i = 0;i < sizeof(result); i++)
		printf("0x%x\t", result[i]);
	    printf("\r\n");

	    if (result[0] == send_data[1] && result[1] == send_data[2]) {
		printf("TEST OK\r\n");
	    } else {
		printf("TEST FAILED\r\n");
	    }

	    /* Restore original value */
	    uint8_t restore_data[] = { 0x03, default_value[0], default_value[1] };
	    R_I2C_Write(&g_i2c_device_ctrl_1, restore_data, sizeof(restore_data), 0);
	    R_I2C_Close(&g_i2c_device_ctrl_1);
	    printf("PROGRAM END\r\n");

	for( ;; )
	{
	}

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
