/*
* Copyright (c) 2025 Renesas Electronics Corporation
*
* SPDX-License-Identifier: MIT
*
*/

#ifndef R_TAUD_API_H
#define R_TAUD_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
// #define SCPBUSD8_SCP_MAIN       75000000

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef enum{
    TAUD_CH_INTERVAL_TIMER_MODE     = 0B0000,
    TAUD_CH_JUDGE_MODE              = 0B0001,
    TAUD_CH_EVENT_COUNT_MODE        = 0B0011,
    TAUD_CH_ONE_COUNT_MODE          = 0B0100,
    TAUD_CH_COUNT_UP_DOWN_MODE      = 0B1001,
    TAUD_CH_PULSE_ONE_COUNT_MODE    = 0B1010,
} taud_ch_mode_t;

typedef enum{
    TAUD_CLK_CK0    = 0B00,
    TAUD_CLK_CK1    = 0B01,
    TAUD_CLK_CK2    = 0B10,
    TAUD_CLK_CK3    = 0B11,
} taud_clk_src_t;

typedef enum{
    TAUD_SLAVE_CH  = 0B0,
    TAUD_MASTER_CH = 0B1,
} taud_is_master;

typedef struct tau_clock
{
    taud_clk_src_t  clk_src;
    uint32_t        clk_value;
    uint8_t         is_using;
} tau_clock_t;

typedef struct taud_channel_config
{
    uint8_t         unit;
    uint8_t         channel;
    taud_is_master  is_master;
    uint16_t        count;
    taud_clk_src_t  clk_src;
    taud_ch_mode_t  mode;
    uint8_t         output_en;
    uint8_t         output_polarity;
} taud_channel_config_t;


/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
uint32_t taud_set_src_clk_main(uint32_t clock);
tau_clock_t * taud_pwm_set_clk(uint8_t unit, uint32_t freq_hz);
uint32_t taud_pwm_get_clk(taud_clk_src_t clk_src);
uint8_t taud_pwm_channel_setup(taud_channel_config_t * p_ch_cfg);
uint8_t taud_pwm_start(uint8_t unit, uint16_t ch_mask);
uint8_t taud_pwm_stop(uint8_t unit, uint16_t ch_mask);
uint8_t taud_pwm_set_count(uint8_t unit, uint8_t ch, uint16_t count);

#endif /* R_TAUD_API_H */