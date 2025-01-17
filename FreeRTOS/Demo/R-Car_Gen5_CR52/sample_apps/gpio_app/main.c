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
#include "gpio/r_gpio_api.h"
#define main_GPIO_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

extern int printf_delay(const char *format, ...);

/*-----------------------------------------------------------*/

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void prvGPIOTask( void *pvParameters );

static void gpioUserCallback(void *data);

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
    int i;
    uint8_t lv = 0;

    uint8_t readLevel;

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    printf_delay("\n********** TEST: Output/Input General Mode **********\n");

    // Configure GPIO group 0 pin 0 as input
    R_GPIO_PinConfigMode(RCAR_GPIO_GROUP_00, RCAR_PIN_00, RCAR_IO_DIRECTION_INPUT);

    // Configure GPIO group 0 pin 1 as ouput
    R_GPIO_PinConfigMode(RCAR_GPIO_GROUP_00, RCAR_PIN_01, RCAR_IO_DIRECTION_OUTPUT);

    readLevel = R_GPIO_PinReadInput(RCAR_GPIO_GROUP_00, RCAR_PIN_01);
    printf_delay("PinRead: Before PIN_1=%d\n", readLevel);
    readLevel = R_GPIO_PinReadInput(RCAR_GPIO_GROUP_00, RCAR_PIN_00);
    printf_delay("PinRead: Before PIN_0=%d\n", readLevel);
    printf_delay("--- PAUSE VDK ---\n");
    vTaskDelay(1000);

    for (i = 0; i < 2; i++)
    {
        lv = !lv;
        if(lv == 0)
            printf("PinWrite: LOW\n");
        else
            printf("PinWrite: HIGH\n");
        R_GPIO_PinWriteOutput(RCAR_GPIO_GROUP_00, RCAR_PIN_01, lv);

        readLevel = R_GPIO_PinReadInput(RCAR_GPIO_GROUP_00, RCAR_PIN_01);
        printf_delay("PinRead: After PIN_1=%d\n", readLevel);
        readLevel = R_GPIO_PinReadInput(RCAR_GPIO_GROUP_00, RCAR_PIN_00);
        printf_delay("PinRead: After PIN_0=%d\n", readLevel);
        printf_delay("--- PAUSE VDK ---\n");
        vTaskDelay(1000);
    }

    printf_delay("\n********** TEST: Interrupt Input Mode **********\n");


    rcar_gpio_data_t info = {
        .group = RCAR_GPIO_GROUP_00,
        .pin   = RCAR_PIN_09
    };
    R_GPIO_PinConfigInterruptMode(info.group, info.pin, RCAR_INTERRUPT_INPUT_BOTH_EDGE);
    R_GPIO_SetInterruptCallback(info.group, gpioUserCallback, &info);
    printf_delay("CallbackSet\n");

    R_GPIO_PinConfigMode(RCAR_GPIO_GROUP_00, RCAR_PIN_17, RCAR_IO_DIRECTION_OUTPUT);
    printf_delay("PinCfg\n");

    /* Create signal to test interrupt */
    for(i = 0; i < 4; i++)
    {
        lv = !lv;
        printf("Out: %d\n", lv);
        R_GPIO_PinWriteOutput(RCAR_GPIO_GROUP_00, RCAR_PIN_17, lv);
        vTaskDelay(500);
    }


    for( ;; )
    {
    }
}

/*-----------------------------------------------------------*/

void gpioUserCallback(void *data) {
    
    rcar_gpio_data_t *info = (rcar_gpio_data_t* )data;
    R_GPIO_ClearInterrupt(info->group, info->pin);
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
