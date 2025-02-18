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
#include "semphr.h"

#include "interrupts.h"
#include "stdio.h"
#include "gpio/r_gpio.h"
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

/*
 * Declare some structs used for GPIO API.
 */
/**** Config GPIO output/input general mode ****/
gpio_pin_cfg_t g_gpio_pin_cfg[] =
{
    {
        .pin_cfg = GPIO_DIRECTION_INPUT,
        .pin = GPIO_PORT_00_PIN_0
    },

    {
        .pin_cfg = GPIO_DIRECTION_OUTPUT,
        .pin = GPIO_PORT_00_PIN_1
    },
};

gpio_cfg_t g_gpio_cfg =
{
    .number_of_pins = sizeof(g_gpio_pin_cfg)/sizeof(g_gpio_pin_cfg[0]),
    .p_pin_cfg_data = &g_gpio_pin_cfg[0],
    .p_extend = NULL
};

gpio_instance_ctrl_t g_gpio_instance_ctrl;

/**** Config GPIO interrupt input mode ****/
gpio_pin_cfg_t g_gpio_pin_cfg_irq =
{

    .pin_cfg = GPIO_INTERRUPT_INPUT_BOTH_EDGE,
    .pin = GPIO_PORT_00_PIN_9
};

gpio_cfg_t g_gpio_cfg_irq =
{
    .number_of_pins = 1,
    .p_pin_cfg_data = &g_gpio_pin_cfg_irq,
    .p_extend = NULL
};

gpio_instance_ctrl_t g_gpio_instance_ctrl_irq;

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
    gpio_level_t lv = GPIO_LEVEL_LOW;

    gpio_level_t readLevel;

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    printf_delay("\n********** TC1: GPIO Pull test port0-pin0 **********\n");
    ret = R_GPIO_Open(&g_gpio_instance_ctrl, &g_gpio_cfg);
    printf_delay("Open : ret = %d\n", ret);

    ret = R_GPIO_PinSetPull(&g_gpio_instance_ctrl, g_gpio_cfg.p_pin_cfg_data[0].pin, GPIO_REQ_NO_PULL);
    printf_delay("R_GPIO_PinSetPull: No-pull ret = %d\n", ret);
    ret = R_GPIO_PinRead(&g_gpio_instance_ctrl, GPIO_PORT_00_PIN_0, &readLevel);
    printf_delay("PinRead: %d\n", readLevel);

    ret = R_GPIO_PinSetPull(&g_gpio_instance_ctrl, g_gpio_cfg.p_pin_cfg_data[0].pin, GPIO_REQ_PULL_UP);
    printf_delay("R_GPIO_PinSetPull: Up ret = %d\n", ret);
    ret = R_GPIO_PinRead(&g_gpio_instance_ctrl, GPIO_PORT_00_PIN_0, &readLevel);
    printf_delay("PinRead: %d\n", readLevel);

    ret = R_GPIO_PinSetPull(&g_gpio_instance_ctrl, g_gpio_cfg.p_pin_cfg_data[0].pin, GPIO_REQ_PULL_DOWN);
    printf_delay("R_GPIO_PinSetPull: Down ret = %d\n", ret);
    ret = R_GPIO_PinRead(&g_gpio_instance_ctrl, GPIO_PORT_00_PIN_0, &readLevel);
    printf_delay("PinRead: %d\n", readLevel);

    ret = R_GPIO_Close(&g_gpio_instance_ctrl);
    printf_delay("Close: ret = %d\n", ret);
    vTaskDelay(100);

    printf_delay("\n********** TC2: Output/Input General Mode **********\n");

    ret = R_GPIO_Open(&g_gpio_instance_ctrl, &g_gpio_cfg);
    printf_delay("Open : ret = %d\n", ret);

    ret = R_GPIO_PinSetPull(&g_gpio_instance_ctrl, g_gpio_cfg.p_pin_cfg_data[0].pin, GPIO_REQ_PULL_DOWN);
    printf_delay("R_GPIO_PinSetPull: Down Port0-Pin0 ret = %d\n", ret);

    ret = R_GPIO_PinRead(&g_gpio_instance_ctrl, GPIO_PORT_00_PIN_1, &readLevel);
    printf_delay("PinRead: Before PIN_1=%d\n", readLevel);
    ret |= R_GPIO_PinRead(&g_gpio_instance_ctrl, GPIO_PORT_00_PIN_0, &readLevel);
    printf_delay("PinRead: Before PIN_0=%d\n", readLevel);
    printf_delay("--- PAUSE VDK ---\n");
    vTaskDelay(1000);

    for (i = 0; i < 2; i++)
    {
        lv = !lv;
        if(lv == GPIO_LEVEL_LOW)
            printf("PinWrite: LOW\n");
        else
            printf("PinWrite: HIGH\n");
        ret = R_GPIO_PinWrite(&g_gpio_instance_ctrl, GPIO_PORT_00_PIN_1, lv);

        ret = R_GPIO_PinRead(&g_gpio_instance_ctrl, GPIO_PORT_00_PIN_1, &readLevel);
        printf_delay("PinRead: After PIN_1=%d\n", readLevel);
        ret |= R_GPIO_PinRead(&g_gpio_instance_ctrl, GPIO_PORT_00_PIN_0, &readLevel);
        printf_delay("PinRead: After PIN_0=%d\n", readLevel);
        printf_delay("--- PAUSE VDK ---\n");
        vTaskDelay(1000);
    }

    ret = R_GPIO_Close(&g_gpio_instance_ctrl);
    printf_delay("Close: ret = %d\n", ret);

    printf_delay("\n********** TC3: Interrupt Input Mode **********\n");
    ret = R_GPIO_Open(&g_gpio_instance_ctrl_irq, &g_gpio_cfg_irq);
    printf_delay("Open : ret = %d\n", ret);

    ret = R_GPIO_PinSetPull(&g_gpio_instance_ctrl_irq, g_gpio_cfg_irq.p_pin_cfg_data->pin, GPIO_REQ_PULL_DOWN);
    printf_delay("R_GPIO_PinSetPull: Down Port0-Pin9 ret = %d\n", ret);

    ret = R_GPIO_CallbackSet(&g_gpio_instance_ctrl_irq, gpioUserCallback, &g_gpio_instance_ctrl_irq);
    printf_delay("CallbackSet: ret = %d\n", ret);

    ret = R_GPIO_PinCfg(&g_gpio_instance_ctrl_irq, GPIO_PORT_00_PIN_17, GPIO_DIRECTION_OUTPUT);
    printf_delay("PinCfg: ret = %d\n", ret);

    /* Create signal to test interrupt */
    for(i = 0; i < 4; i++)
    {
        lv = !lv;
        if(lv == GPIO_LEVEL_LOW)
            printf("Out: 0\n");
        else
            printf("Out: 1\n");
        ret = R_GPIO_PinWrite(&g_gpio_instance_ctrl_irq, GPIO_PORT_00_PIN_17, lv);
        vTaskDelay(500);
    }

    ret = R_GPIO_Close(&g_gpio_instance_ctrl_irq);
    printf_delay("Close: ret = %d\n", ret);

    for( ;; )
    {
    }
}

/*-----------------------------------------------------------*/

void gpioUserCallback(void *data) {
    gpio_instance_ctrl_t * p_instance_ctrl = (gpio_instance_ctrl_t *) data;
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
