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
#include "semphr.h"

#include "interrupts.h"
#include "wcrc/r_wcrc_common.h"
#include "wcrc/r_wcrc.h"
#include <stdio.h>

#define main_CRC_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

extern int printf_delay(const char *format, ...);

/*-----------------------------------------------------------*/

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

static void prvCRCTask( void *pvParameters );

void crcUserCallback(void *data);
void kcrcUserCallback(void *data);
/*-----------------------------------------------------------*/

/*
 * Declare some structs used for CRC API.
 */
static uint32_t crc_input[4] = {0x12345678, 0x12345678, 0x12345678, 0x12345678};

/**** Config CRC Independent mode ****/
wcrc_cfg_t  g_wcrc_cfg0 =
{
    .unit       = WCRC_00,
    .mode       = INDEPENDENT_CRC_MODE,
    .sub_module = CRC_KCRC_SUB_MODULE,

    .crc_cfg    =
    {
        .input_cfg      =
        {
            .p_input_buffer = &crc_input,
            .num_data       = sizeof(crc_input)/sizeof(crc_input[0]),
            .crc_seed       = 0xFFFFFFFF,
            .bit_width      = WIDTH_32_BIT
        },

        .poly           = POLY_32_ETHERNET,
        .is_out_exor    = false,
        .is_out_bitswap = false,
        .out_byteswap   = BYTE_SWAP_00,
        .is_in_exor     = false,
        .is_in_bitswap  = false,
        .in_byteswap    = BYTE_SWAP_00
    },

    .kcrc_cfg   =
    {
        .input_cfg      =
        {
            .p_input_buffer = &crc_input,
            .num_data       = sizeof(crc_input)/sizeof(crc_input[0]),
            .crc_seed       = 0xFFFFFFFF,
            .bit_width      = WIDTH_32_BIT
        },

        .poly           = POLY_32_ETHERNET,
        .poly_size      = POLY_SIZE_32_BIT,
        .is_out_reflect = true,
        .is_in_reflect  = true,
        .shift_mode     = MSB_SHIFT, 
        .xor_mask_out   = 0xFFFFFFFF
    }
};

wcrc_instance_ctrl_t g_wcrc_inst_ctrl_indepe;

/**** Config E2E CRC mode ****/
wcrc_cfg_t  g_wcrc_cfg1 =
{
    .unit       = WCRC_01,
    .mode       = E2E_CRC_MODE,
    .conv_size  = 4,
    .sub_module = CRC_KCRC_SUB_MODULE,

    .crc_cfg    =
    {
        .input_cfg      =
        {
            .p_input_buffer = &crc_input,
            .num_data       = sizeof(crc_input)/sizeof(crc_input[0]),
            .crc_seed       = 0xFFFFFFFF,
            .bit_width      = WIDTH_32_BIT
        },

        .poly           = POLY_32_ETHERNET,
        .is_out_exor    = false,
        .is_out_bitswap = false,
        .out_byteswap   = BYTE_SWAP_00,
        .is_in_exor     = false,
        .is_in_bitswap  = false,
        .in_byteswap    = BYTE_SWAP_00
    },

    .kcrc_cfg   =
    {
        .input_cfg      =
        {
            .p_input_buffer = &crc_input,
            .num_data       = sizeof(crc_input)/sizeof(crc_input[0]),
            .crc_seed       = 0xFFFFFFFF,
            .bit_width      = WIDTH_32_BIT
        },

        .poly           = POLY_32_ETHERNET,
        .poly_size      = POLY_SIZE_32_BIT,
        .is_out_reflect = true,
        .is_in_reflect  = true,
        .shift_mode     = MSB_SHIFT, 
        .xor_mask_out   = 0xFFFFFFFF
    }
};

wcrc_instance_ctrl_t g_wcrc_inst_ctrl_e2e;

/*-----------------------------------------------------------*/
int main( void )
{
    /* Configure the hardware ready to run the demo. */
    prvSetupHardware();

    xTaskCreate( prvCRCTask, "CRC", configMINIMAL_STACK_SIZE, NULL, main_CRC_TASK_PRIORITY, NULL );
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

static void prvCRCTask( void *pvParameters )
{
    uint8_t ret;
    uint32_t num_data;
    uint32_t crc_code = 0x27, kcrc_code = 0x15;

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    printf_delay("\n********** TC1: CRC Independent Mode **********\n");
    ret = R_CRC_Open(&g_wcrc_inst_ctrl_indepe, &g_wcrc_cfg0);
    printf_delay("R_CRC_Open: ret = %d\n", ret);
    vTaskDelay(10);

    ret = R_CRC_Calculate(&g_wcrc_inst_ctrl_indepe);
    printf_delay("\nR_CRC_Calculate: ret = %d\n\n", ret);
    vTaskDelay(10);

    ret = R_CRC_Get_Input_Data(&g_wcrc_inst_ctrl_indepe);
    printf_delay("\nR_CRC_Get_Input_Data: ret = %d\n\n", ret);
    vTaskDelay(10);

    ret = R_CRC_Get_Generated_Value(&g_wcrc_inst_ctrl_indepe);
    printf_delay("\nR_CRC_Get_Generated_Value: ret = %d\n", ret);
    vTaskDelay(10);

    ret = R_CRC_Close(&g_wcrc_inst_ctrl_indepe);
    printf_delay("\nR_CRC_Close: ret = %d\n", ret);
    vTaskDelay(10);

    printf_delay("\n********** TC2: E2E CRC Mode **********\n");
    ret = R_CRC_Open(&g_wcrc_inst_ctrl_e2e, &g_wcrc_cfg1);
    printf_delay("R_CRC_Open: ret = %d\n", ret);
    vTaskDelay(10);

    ret  = R_CRC_Set_Callback(CRC_SUB_MODULE, &g_wcrc_inst_ctrl_e2e,
                             crcUserCallback, &crc_code);
    ret |= R_CRC_Set_Callback(KCRC_SUB_MODULE, &g_wcrc_inst_ctrl_e2e,
                             kcrcUserCallback, &kcrc_code);
    printf_delay("R_CRC_Set_Callback: ret = %d\n", ret);
    vTaskDelay(10);

    ret = R_CRC_Calculate(&g_wcrc_inst_ctrl_e2e);
    printf_delay("\nR_CRC_Calculate: ret = %d\n\n", ret);
    vTaskDelay(10);

    ret = R_CRC_Get_Input_Data(&g_wcrc_inst_ctrl_e2e);
    printf_delay("\nR_CRC_Get_Input_Data: ret = %d\n\n", ret);
    vTaskDelay(10);

    ret = R_CRC_Get_Generated_Value(&g_wcrc_inst_ctrl_e2e);
    printf_delay("\nR_CRC_Get_Generated_Value: ret = %d\n", ret);
    vTaskDelay(10);

    ret = R_CRC_Close(&g_wcrc_inst_ctrl_e2e);
    printf_delay("\nR_CRC_Close: ret = %d\n", ret);
    vTaskDelay(10);

    for( ;; )
    {
    }
}

/*-----------------------------------------------------------*/

void crcUserCallback(void *data) {
    uint32_t * crc_code = (uint32_t *)data;
}

void kcrcUserCallback(void *data) {
    uint32_t * kcrc_code = (uint32_t *)data;
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
