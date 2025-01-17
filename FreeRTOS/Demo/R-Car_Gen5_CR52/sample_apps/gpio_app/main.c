/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/******************************************************************************
 * This project provides two demo applications.  A simple blinky style project,
 * and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting (defined in this file) is used to
 * select between the two.  The simply blinky demo is implemented and described
 * in main_blinky.c.  The more comprehensive test and demo application is
 * implemented and described in main_full.c.
 *
 * This file implements the code that is not demo specific, including the
 * hardware setup and standard FreeRTOS hook functions.
 *
 * ENSURE TO READ THE DOCUMENTATION PAGE FOR THIS PORT AND DEMO APPLICATION ON
 * THE http://www.FreeRTOS.org WEB SITE FOR FULL INFORMATION ON USING THIS DEMO
 * APPLICATION, AND ITS ASSOCIATE FreeRTOS ARCHITECTURE PORT!
 */

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "interrupts.h"
#include "stdio.h"
#include "gpio/r_ioport.h"
#define main_GPIO_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

//#define printf_delay(fmt, ...)		\
//	vTaskDelay(10);			\
//        printf(fmt, ##__VA_ARGS__);	\

extern int printf_delay(const char *format, ...);

/*-----------------------------------------------------------*/

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void prvGPIOTask( void *pvParameters );

static void gpioUserCallback(void *data);
/*-----------------------------------------------------------*/

/*
 * Declare some structs used for FSP GPIO API.
 */
/**** Config GPIO output/input general mode ****/
ioport_pin_cfg_t g_gpio_pin_cfg[] =
{
	{	
		.pin_cfg = BSP_IO_DIRECTION_INPUT,
		.pin = BSP_IO_PORT_00_PIN_0
	},

	{	
		.pin_cfg = BSP_IO_DIRECTION_OUTPUT,
		.pin = BSP_IO_PORT_00_PIN_1
	},
};

ioport_cfg_t g_gpio_cfg =
{
	.number_of_pins = sizeof(g_gpio_pin_cfg)/sizeof(g_gpio_pin_cfg[0]),
	.p_pin_cfg_data = &g_gpio_pin_cfg[0],
	.p_extend = NULL
};

ioport_instance_ctrl_t g_gpio_instance_ctrl;

/**** Config GPIO interrupt input mode ****/
ioport_pin_cfg_t g_gpio_pin_cfg_irq =
{

	.pin_cfg = BSP_INTERRUPT_INPUT_BOTH_EDGE,
	.pin = BSP_IO_PORT_00_PIN_9
};

ioport_cfg_t g_gpio_cfg_irq =
{
	.number_of_pins = 1,
	.p_pin_cfg_data = &g_gpio_pin_cfg_irq,
	.p_extend = NULL
};

ioport_instance_ctrl_t g_gpio_instance_ctrl_irq;

/*-----------------------------------------------------------*/
int main( void )
{
	/* Configure the hardware ready to run the demo. */
	prvSetupHardware();

	xTaskCreate( prvGPIOTask, "GPIO", configMINIMAL_STACK_SIZE, NULL, main_GPIO_TASK_PRIORITY, NULL );
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

static void prvGPIOTask( void *pvParameters )
{
	uint8_t ret;
	int i;
	bsp_io_level_t lv = BSP_IO_LEVEL_LOW;

	bsp_io_level_t readLevel;

	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;

	printf_delay("\n********** TEST: Output/Input General Mode **********\n");

	ret = R_IOPORT_Open(&g_gpio_instance_ctrl, &g_gpio_cfg);
	printf_delay("Open : ret = %d\n", ret);

	ret = R_IOPORT_PinRead(&g_gpio_instance_ctrl, BSP_IO_PORT_00_PIN_1, &readLevel);
	printf_delay("PinRead: Before PIN_1=%d\n", readLevel);
	ret |= R_IOPORT_PinRead(&g_gpio_instance_ctrl, BSP_IO_PORT_00_PIN_0, &readLevel);
	printf_delay("PinRead: Before PIN_0=%d\n", readLevel);
	printf_delay("--- PAUSE VDK ---\n");
	vTaskDelay(1000);

	for (i = 0; i < 2; i++)
	{
		lv = !lv;
		if(lv == BSP_IO_LEVEL_LOW)
			printf("PinWrite: LOW\n");
		else
			printf("PinWrite: HIGH\n");
		ret = R_IOPORT_PinWrite(&g_gpio_instance_ctrl, BSP_IO_PORT_00_PIN_1, lv);

		ret = R_IOPORT_PinRead(&g_gpio_instance_ctrl, BSP_IO_PORT_00_PIN_1, &readLevel);
		printf_delay("PinRead: After PIN_1=%d\n", readLevel);
		ret |= R_IOPORT_PinRead(&g_gpio_instance_ctrl, BSP_IO_PORT_00_PIN_0, &readLevel);
		printf_delay("PinRead: After PIN_0=%d\n", readLevel);
		printf_delay("--- PAUSE VDK ---\n");
		vTaskDelay(1000);
	}

	ret = R_IOPORT_Close(&g_gpio_instance_ctrl);
	printf_delay("Close: ret = %d\n", ret);

	printf_delay("\n********** TEST: Interrupt Input Mode **********\n");
	ret = R_IOPORT_Open(&g_gpio_instance_ctrl_irq, &g_gpio_cfg_irq);
	printf_delay("Open : ret = %d\n", ret);

	//printf_delay("Addr: &lv=0x%x\n", &lv);

	//ret = R_IOPORT_CallbackSet(&g_gpio_instance_ctrl_irq, gpioUserCallback, &lv);
	ret = R_IOPORT_CallbackSet(&g_gpio_instance_ctrl_irq, gpioUserCallback, &g_gpio_instance_ctrl_irq);
	printf_delay("CallbackSet: ret = %d\n", ret);

	ret = R_IOPORT_PinCfg(&g_gpio_instance_ctrl_irq, BSP_IO_PORT_00_PIN_17, BSP_IO_DIRECTION_OUTPUT);
	printf_delay("PinCfg: ret = %d\n", ret);

	/* Create signal to test interrupt */
	for(i = 0; i < 4; i++)
	{
		lv = !lv;
		if(lv == BSP_IO_LEVEL_LOW)
			printf("Out: 0\n");
		else
			printf("Out: 1\n");
		ret = R_IOPORT_PinWrite(&g_gpio_instance_ctrl_irq, BSP_IO_PORT_00_PIN_17, lv);
		vTaskDelay(500);
	}

	ret = R_IOPORT_Close(&g_gpio_instance_ctrl_irq);
	printf_delay("Close: ret = %d\n", ret);

	for( ;; )
	{
	}
}

/*-----------------------------------------------------------*/

void gpioUserCallback(void *data) {
    ioport_instance_ctrl_t * p_instance_ctrl = (ioport_instance_ctrl_t *) data;
    printf("Handle GPIO interrupt\n");
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
