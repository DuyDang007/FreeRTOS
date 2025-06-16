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

#define TCAL9539	0x77
#define PTN3222 	0x43
#define MAX96726A_1	0x31
#define MAX96726A_2	0x33
#define RC21214		0x09

#define I2C_APP_SIZE (configMINIMAL_STACK_SIZE * 2)
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


	xTaskCreate( prvI2CTask, "prvI2CTask", I2C_APP_SIZE, NULL, main_I2C_TASK_PRIORITY, NULL);
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

	uint8_t send_data[] = { 0x03, 0x02};
	uint8_t result[1] = {0};
	uint8_t default_value[1] = {0}, restore_data[2];

	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;

	/* Device driver part for ch1 */
	i2c_instance_ctrl_t g_i2c_device_ctrl_1;
	i2c_master_cfg_t        g_i2c_device_cfg_1 =
	{
	    .channel       = 1,
	    .rate          = I2C_MASTER_RATE_FAST,
	    .slave         = TCAL9539,
	    .addr_mode     = I2C_MASTER_ADDR_MODE_7BIT,
	    .p_callback    = NULL,     // Callback
	    .p_context     = &g_i2c_device_ctrl_1,
	};

	printf("------------- Start I2C Test -------------\r\n");
	printf("------------- START TEST I2C CHANNEL %d -------------\r\n", g_i2c_device_cfg_1.channel);
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

	if (result[0] == send_data[1]) {
	    printf("TEST OK \r\n");
	} else {
	    printf("TEST FAILED\r\n");
	}

	/* Restore original value */
	//restore_data[] = { 0x03, default_value[0], default_value[1] };
	restore_data[0] = 0x03;
	restore_data[1] = default_value[0];
	restore_data[2] = default_value[1];

	R_I2C_Write(&g_i2c_device_ctrl_1, restore_data, sizeof(restore_data), 0);
	R_I2C_ReadRegMap(&g_i2c_device_ctrl_1, 0x03, default_value, sizeof(default_value));
	printf("DEFAULT VALUE: ");
	for (uint8_t i = 0; i < sizeof(default_value); i++)
		printf("0x%x\t", default_value[i]);
	printf("\r\n");
	R_I2C_Close(&g_i2c_device_ctrl_1);
	printf("------------- END TEST I2C CHANNEL %d -------------\r\n", g_i2c_device_cfg_1.channel);



	/* Device driver part for channel 2*/
	i2c_instance_ctrl_t g_i2c_device_ctrl_2;
	i2c_master_cfg_t        g_i2c_device_cfg_2 =
	{
	    .channel       = 2,
	    .rate          = I2C_MASTER_RATE_FAST,
	    .slave         = TCAL9539,
	    .addr_mode     = I2C_MASTER_ADDR_MODE_7BIT,
	    .p_callback    = NULL,     // Callback
	    .p_context     = &g_i2c_device_ctrl_2,
	};

	printf("------------- START TEST I2C CHANNEL %d -------------\r\n", g_i2c_device_cfg_2.channel);
	R_I2C_Open(&g_i2c_device_ctrl_2, &g_i2c_device_cfg_2);

	R_I2C_ReadRegMap(&g_i2c_device_ctrl_2, 0x03, default_value, sizeof(default_value));
	printf("DEFAULT VALUE: ");
	for (uint8_t i = 0; i < sizeof(default_value); i++)
		printf("0x%x\t", default_value[i]);
	printf("\r\n");

	printf("WRITE DATA \r\n");
	R_I2C_Write(&g_i2c_device_ctrl_2, send_data, sizeof(send_data), 0);
	printf("WRITE DONE\r\n");

	R_I2C_ReadRegMap(&g_i2c_device_ctrl_2, 0x03, (uint8_t *)&result, sizeof(result));
	printf("READ DATA: ");
	for(uint8_t i = 0;i < sizeof(result); i++)
		printf("0x%x\t", result[i]);
	printf("\r\n");

	if (result[0] == send_data[1]) {
	    printf("TEST OK \r\n");
	} else {
	    printf("TEST FAILED\r\n");
	}

	/* Restore original value */
	//restore_data[] = { 0x03, default_value[0], default_value[1] };
	restore_data[0] = 0x03;
	restore_data[1] = default_value[0];
	restore_data[2] = default_value[1];

	R_I2C_Write(&g_i2c_device_ctrl_2, restore_data, sizeof(restore_data), 0);
	R_I2C_ReadRegMap(&g_i2c_device_ctrl_2, 0x03, default_value, sizeof(default_value));
	printf("DEFAULT VALUE: ");
	for (uint8_t i = 0; i < sizeof(default_value); i++)
		printf("0x%x\t", default_value[i]);
	printf("\r\n");
	R_I2C_Close(&g_i2c_device_ctrl_2);
	printf("------------- END TEST I2C CHANNEL %d -------------\r\n", g_i2c_device_cfg_2.channel);


	/* Device driver part for channel 3*/
	i2c_instance_ctrl_t g_i2c_device_ctrl_3;
	i2c_master_cfg_t        g_i2c_device_cfg_3 =
	{
	    .channel       = 3,
	    .rate          = I2C_MASTER_RATE_FAST,
	    .slave         = MAX96726A_1,
	    .addr_mode     = I2C_MASTER_ADDR_MODE_7BIT,
	    .p_callback    = NULL,     // Callback
	    .p_context     = &g_i2c_device_ctrl_3,
	};

	printf("------------- START TEST I2C CHANNEL %d -------------\r\n", g_i2c_device_cfg_3.channel);
	R_I2C_Open(&g_i2c_device_ctrl_3, &g_i2c_device_cfg_3);

	uint16_t Addr = 0x73;
	int8_t   no_bytes = 2;	// number of bytes to send for the register address (16-bit → 2 bytes)
	uint8_t  send_byte[no_bytes];
	uint8_t write_data[3];

	send_byte[0] = Addr >> 8;
	send_byte[1] = Addr & 0xff;
	R_I2C_Write(&g_i2c_device_ctrl_3, &send_byte[0], no_bytes, false);
	R_I2C_Read(&g_i2c_device_ctrl_3, default_value, sizeof(default_value), false);
	printf("DEFAULT VALUE: ");
	for (uint8_t i = 0; i < sizeof(default_value); i++)
		printf("0x%x\t", default_value[i]);
	printf("\r\n");

	printf("WRITE DATA \r\n");
	write_data[0] = send_byte[0];   // MSB of address
	write_data[1] = send_byte[1];   // LSB of address
	write_data[2] = 0x2;            // Data to write
	R_I2C_Write(&g_i2c_device_ctrl_3, write_data, sizeof(write_data), 0);
	printf("WRITE DONE\r\n");

	R_I2C_Write(&g_i2c_device_ctrl_3, &send_byte[0], no_bytes, false);
	R_I2C_Read(&g_i2c_device_ctrl_3, result, sizeof(result), false);
	printf("READ DATA AFTER WRITE: 0x%x\t", result[0]);
	printf("\r\n");

	if (result[0] == write_data[2]) {
		printf("TEST OK \r\n");
	} else {
		printf("TEST FAILED\r\n");
	}

	/* Restore */
	//restore_data[0] = 0x03;
	//restore_data[1] = default_value[0];
	//restore_data[2] = default_value[1];
	write_data[2] = default_value[0];
	R_I2C_Write(&g_i2c_device_ctrl_3, write_data, sizeof(write_data), 0);


	R_I2C_Write(&g_i2c_device_ctrl_3, send_byte, no_bytes, false);
	R_I2C_Read(&g_i2c_device_ctrl_3, default_value, sizeof(default_value), false);
	printf("DEFAULT VALUE: 0x%x\t", default_value[0]);
	printf("\r\n");

	R_I2C_Close(&g_i2c_device_ctrl_3);
	printf("------------- END TEST I2C CHANNEL %d -------------\r\n", g_i2c_device_cfg_3.channel);


	/* Coding for channel 4*/
	i2c_instance_ctrl_t g_i2c_device_ctrl_4;
	i2c_master_cfg_t        g_i2c_device_cfg_4 =
	{
	    .channel       = 4,
	    .rate          = I2C_MASTER_RATE_FAST,
	    .slave         = MAX96726A_2,
	    .addr_mode     = I2C_MASTER_ADDR_MODE_7BIT,
	    .p_callback    = NULL,     // Callback
	    .p_context     = &g_i2c_device_ctrl_4,
	};

	printf("------------- START TEST I2C CHANNEL %d -------------\r\n", g_i2c_device_cfg_4.channel);
	R_I2C_Open(&g_i2c_device_ctrl_4, &g_i2c_device_cfg_4);

	Addr = 0x31;
	no_bytes = 2;	// number of bytes to send for the register address (16-bit → 2 bytes)
	send_byte[no_bytes];
	write_data[3];
	//uint8_t result[1];

	send_byte[0] = Addr >> 8;
	send_byte[1] = Addr & 0xff;
	R_I2C_Write(&g_i2c_device_ctrl_4, &send_byte[0], no_bytes, false);
	R_I2C_Read(&g_i2c_device_ctrl_4, default_value, sizeof(default_value), false);
	printf("DEFAULT VALUE: ");
	for (uint8_t i = 0; i < sizeof(default_value); i++)
		printf("0x%x\t", default_value[i]);
	printf("\r\n");

	printf("WRITE DATA \r\n");
	write_data[0] = send_byte[0];   // MSB of address
	write_data[1] = send_byte[1];   // LSB of address
	write_data[2] = 0xa2;            // Data to write
	R_I2C_Write(&g_i2c_device_ctrl_4, write_data, sizeof(write_data), 0);
	printf("WRITE DONE\r\n");

	/* Read back to verify */
	R_I2C_Write(&g_i2c_device_ctrl_4, &send_byte[0], no_bytes, false);
	R_I2C_Read(&g_i2c_device_ctrl_4, result, sizeof(result), false);
	printf("READ DATA AFTER WRITE: 0x%x\t", result[0]);
	printf("\r\n");

	if (result[0] == write_data[2]) {
		printf("TEST OK \r\n");
	} else {
		printf("TEST FAILED\r\n");
	}


	write_data[2] = default_value[0];
	R_I2C_Write(&g_i2c_device_ctrl_4, write_data, sizeof(write_data), 0);

	/* Read again to confirm restore */
	R_I2C_Write(&g_i2c_device_ctrl_4, send_byte, no_bytes, false);
	R_I2C_Read(&g_i2c_device_ctrl_4, default_value, sizeof(default_value), false);
	printf("DEFAULT VALUE: 0x%x\t", default_value[0]);
	printf("\r\n");

	R_I2C_Close(&g_i2c_device_ctrl_4);
	printf("------------- END TEST I2C CHANNEL %d -------------\r\n", g_i2c_device_cfg_4.channel);

	/* Device driver part for channel 5 */
	i2c_instance_ctrl_t g_i2c_device_ctrl_5;
	i2c_master_cfg_t        g_i2c_device_cfg_5 =
	{
	    .channel       = 5,
	    .rate          = I2C_MASTER_RATE_FAST,
	    .slave         = PTN3222,
	    .addr_mode     = I2C_MASTER_ADDR_MODE_7BIT,
	    .p_callback    = NULL,     // Callback
	    .p_context     = &g_i2c_device_ctrl_5,
	};
	send_data[0] = 0x02;
	send_data[1] = 0x40;

	printf("------------- START TEST I2C CHANNEL %d -------------\r\n", g_i2c_device_cfg_5.channel);
	R_I2C_Open(&g_i2c_device_ctrl_5, &g_i2c_device_cfg_5);

	R_I2C_ReadRegMap(&g_i2c_device_ctrl_5, 0x02, default_value, sizeof(default_value));
	printf("DEFAULT VALUE: ");
	for (uint8_t i = 0; i < sizeof(default_value); i++)
		printf("0x%x\t", default_value[i]);
	printf("\r\n");

	printf("WRITE DATA \r\n");
	R_I2C_Write(&g_i2c_device_ctrl_5, send_data, sizeof(send_data), 0);
	printf("WRITE DONE\r\n");

	R_I2C_ReadRegMap(&g_i2c_device_ctrl_5, 0x02, (uint8_t *)&result, sizeof(result));
	printf("READ DATA: ");
	for (uint8_t i = 0; i < sizeof(result); i++)
		printf("0x%x\t", result[i]);
	printf("\r\n");

	if (result[0] == send_data[1]) {
		printf("TEST OK \r\n");
	} else {
		printf("TEST FAILED\r\n");
	}

	restore_data[0] = 0x02;
	restore_data[1] = default_value[0];
	restore_data[2] = default_value[1];
	R_I2C_Write(&g_i2c_device_ctrl_5, restore_data, sizeof(restore_data), 0);
	R_I2C_ReadRegMap(&g_i2c_device_ctrl_5, 0x02, default_value, sizeof(default_value));
	printf("DEFAULT VALUE: ");
	for (uint8_t i = 0; i < sizeof(default_value); i++)
		printf("0x%x\t", default_value[i]);
	printf("\r\n");

	R_I2C_Close(&g_i2c_device_ctrl_5);
	printf("------------- END TEST I2C CHANNEL %d -------------\r\n", g_i2c_device_cfg_5.channel);


	/* Device driver part for channel 6 */
	i2c_instance_ctrl_t g_i2c_device_ctrl_6;
	i2c_master_cfg_t        g_i2c_device_cfg_6 =
	{
	    .channel       = 6,
	    .rate          = I2C_MASTER_RATE_FAST,
	    .slave         = RC21214,
	    .addr_mode     = I2C_MASTER_ADDR_MODE_7BIT,
	    .p_callback    = NULL,     // Callback
	    .p_context     = &g_i2c_device_ctrl_6,
	};
	printf("------------- START TEST I2C CHANNEL %d -------------\r\n", g_i2c_device_cfg_6.channel);
	R_I2C_Open(&g_i2c_device_ctrl_6, &g_i2c_device_cfg_6);

	R_I2C_ReadRegMap(&g_i2c_device_ctrl_6, 0xfd, default_value, sizeof(default_value));
	printf("DEFAULT VALUE: ");
	for (uint8_t i = 0; i < sizeof(default_value); i++)
		printf("0x%x\t", default_value[i]);
	printf("\r\n");

	printf("WRITE DATA \r\n");
	send_data[0] = 0xfd;
	send_data[1] = 0x12;
	R_I2C_Write(&g_i2c_device_ctrl_6, send_data, sizeof(send_data), 0);
	printf("WRITE DONE\r\n");

	R_I2C_ReadRegMap(&g_i2c_device_ctrl_6, 0xfd, (uint8_t *)&result, sizeof(result));
	printf("READ DATA: ");
	for (uint8_t i = 0; i < sizeof(result); i++)
		printf("0x%x\t", result[i]);
	printf("\r\n");

	if (result[0] == send_data[1]) {
		printf("TEST OK \r\n");
	} else {
		printf("TEST FAILED\r\n");
	}

	restore_data[0] = 0xfd;
	restore_data[1] = default_value[0];
	restore_data[2] = default_value[1];
	R_I2C_Write(&g_i2c_device_ctrl_6, restore_data, sizeof(restore_data), 0);
	R_I2C_ReadRegMap(&g_i2c_device_ctrl_6, 0xfd, default_value, sizeof(default_value));
	printf("DEFAULT VALUE: ");
	for (uint8_t i = 0; i < sizeof(default_value); i++)
		printf("0x%x\t", default_value[i]);
	printf("\r\n");

	R_I2C_Close(&g_i2c_device_ctrl_6);
	printf("------------- END TEST I2C CHANNEL %d -------------\r\n", g_i2c_device_cfg_6.channel);

	printf("------------- End -------------\r\n");
	for( ;; )
	{
	};
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
