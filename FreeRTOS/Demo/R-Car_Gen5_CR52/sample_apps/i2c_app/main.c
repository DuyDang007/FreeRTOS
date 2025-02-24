/*
 * FreeRTOS Kernel V11.1.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
extern int printf_delay(const char *format, ...);
/*-----------------------------------------------------------*/

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void prvI2CTask( void *pvParameters );

static void i2cUserCallback(void *data);
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
}

static void prvI2CTask( void *pvParameters )
{
	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;

	uint8_t send_data[] = { 0x1, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
	uint8_t result[24];
	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;

	/* Device driver part */
	i2c_instance_ctrl_t g_i2c_device_ctrl_1;
	i2c_master_cfg_t        g_i2c_device_cfg_1 =
	{
	    .channel       = 1,
	    .rate          = I2C_MASTER_RATE_FAST,
	    .slave         = 0x6d,
	    .addr_mode     = I2C_MASTER_ADDR_MODE_7BIT,
	    .dma_single    = false,
	    .p_context     = &g_i2c_device_ctrl_1,
	};
	    printf_delay("PROGRAM START\r\n");
	    R_I2C_Open(&g_i2c_device_ctrl_1, &g_i2c_device_cfg_1);

	    R_I2C_CallbackSet(&g_i2c_device_ctrl_1, (void *)i2cUserCallback, &g_i2c_device_ctrl_1, NULL);

	    printf_delay("WRITE DATA \r\n");
	    R_I2C_Write(&g_i2c_device_ctrl_1, send_data, sizeof(send_data), 0);
	    printf_delay("WRITE DONE\r\n");

	    R_I2C_Read(&g_i2c_device_ctrl_1, (uint8_t *)&result, sizeof(result), 0);
	    printf_delay("READ DATA: ");
	    for (uint8_t i = 0; i < sizeof(result); i++)
	        printf_delay("0x%x\t", result[i]);
	    printf_delay("\r\n");

	    R_I2C_Close(&g_i2c_device_ctrl_1);
	    printf_delay("PROGRAM END\r\n");

	for( ;; )
	{
	}

}

/*-----------------------------------------------------------*/
void i2cUserCallback(void *data) {
	i2c_instance_ctrl_t * p_instance_ctrl = (i2c_instance_ctrl_t *) data;
	/* Todo: */
}
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
