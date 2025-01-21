/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef R_GPIO_H
#define R_GPIO_H

#include <stdint.h>
/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**
 * @brief Enumeration for GPIO pin direction.
 */
typedef enum e_gpio_direction
{
    GPIO_DIRECTION_INPUT = 0x0,
    GPIO_DIRECTION_OUTPUT,
} gpio_direction_t;

/** Input event edge detection
 *
 */
typedef enum e_gpio_interrupt_input
{
    GPIO_INTERRUPT_INPUT_RISING_EDGE = 10,        ///< Rising Edge
    GPIO_INTERRUPT_INPUT_FALLING_EDGE,		///< Falling Edge
    GPIO_INTERRUPT_INPUT_BOTH_EDGE		///< Both Edge
} gpio_interrupt_input_t;

/** Levels that can be set and read for individual pins */
typedef enum e_gpio_level
{
    GPIO_LEVEL_LOW = 0,              ///< Low
    GPIO_LEVEL_HIGH                  ///< High
} gpio_level_t;

/** Superset list of all possible IO pins. */
typedef enum e_gpio_port
{
    GPIO_PORT_00 = 0x00,           ///< GPIO Port 00
    GPIO_PORT_01 = 0x01,           ///< GPIO Port 01
    GPIO_PORT_02 = 0x02,           ///< GPIO Port 02
    GPIO_PORT_03 = 0x03,           ///< GPIO Port 03
    GPIO_PORT_04 = 0x04,           ///< GPIO Port 04
    GPIO_PORT_05 = 0x05,           ///< GPIO Port 05
    GPIO_PORT_06 = 0x06,           ///< GPIO Port 06
    GPIO_PORT_07 = 0x07,           ///< GPIO Port 07
    GPIO_PORT_08 = 0x08,           ///< GPIO Port 08
    GPIO_PORT_09 = 0x09,           ///< GPIO Port 09
    GPIO_PORT_10 = 0x0A,           ///< GPIO Port 10
} gpio_port_t;

/** Superset list of all possible IO pins. */
typedef enum e_gpio_pin
{
    GPIO_PIN_00 = 0x00,           ///< GPIO Pin  00
    GPIO_PIN_01 = 0x01,           ///< GPIO Pin  01
    GPIO_PIN_02 = 0x02,           ///< GPIO Pin  02
    GPIO_PIN_03 = 0x03,           ///< GPIO Pin  03
    GPIO_PIN_04 = 0x04,           ///< GPIO Pin  04
    GPIO_PIN_05 = 0x05,           ///< GPIO Pin  05
    GPIO_PIN_06 = 0x06,           ///< GPIO Pin  06
    GPIO_PIN_07 = 0x07,           ///< GPIO Pin  07
    GPIO_PIN_08 = 0x08,           ///< GPIO Pin  08
    GPIO_PIN_09 = 0x09,           ///< GPIO Pin  09
    GPIO_PIN_10 = 0x0A,           ///< GPIO Pin  10
    GPIO_PIN_11 = 0x0B,           ///< GPIO Pin  11
    GPIO_PIN_12 = 0x0C,           ///< GPIO Pin  12
    GPIO_PIN_13 = 0x0D,           ///< GPIO Pin  13
    GPIO_PIN_14 = 0x0E,           ///< GPIO Pin  14
    GPIO_PIN_15 = 0x0F,           ///< GPIO Pin  15
    GPIO_PIN_16 = 0x10,           ///< GPIO Pin  16
    GPIO_PIN_17 = 0x11,           ///< GPIO Pin  17
    GPIO_PIN_18 = 0x12,           ///< GPIO Pin  18
    GPIO_PIN_19 = 0x13,           ///< GPIO Pin  19
    GPIO_PIN_20 = 0x14,           ///< GPIO Pin  20
    GPIO_PIN_21 = 0x15,           ///< GPIO Pin  21
    GPIO_PIN_22 = 0x16,           ///< GPIO Pin  22
    GPIO_PIN_23 = 0x17,           ///< GPIO Pin  23
    GPIO_PIN_24 = 0x18,           ///< GPIO Pin  24
    GPIO_PIN_25 = 0x19,           ///< GPIO Pin  25
    GPIO_PIN_26 = 0x1A,           ///< GPIO Pin  26
    GPIO_PIN_27 = 0x1B,           ///< GPIO Pin  27
    GPIO_PIN_28 = 0x1C,           ///< GPIO Pin  28
    GPIO_PIN_29 = 0x1D,           ///< GPIO Pin  29
    GPIO_PIN_30 = 0x1E,           ///< GPIO Pin  30
    GPIO_PIN_31 = 0x1F,           ///< GPIO Pin  31
} gpio_pin_t;

/**
 * @brief Mask for GPIO port bits.
 */
#define GPIO_PRV_PORT_BITS       (0xFF00U)

/**
 * @brief Mask for GPIO pin bits.
 */
#define GPIO_PRV_PIN_BITS        (0x00FFU)

/**
 * @brief Offset for GPIO port bits.
 */
#define GPIO_PRV_PORT_OFFSET     (8U)

/** Superset list of all possible IO port pins. */
typedef enum e_bsp_io_port_pin
{
    GPIO_PORT_00_PIN_0  = 0x0000,     ///< IO port 0 pin 0
    GPIO_PORT_00_PIN_1  = 0x0001,     ///< IO port 0 pin 1
    GPIO_PORT_00_PIN_2  = 0x0002,     ///< IO port 0 pin 2
    GPIO_PORT_00_PIN_3  = 0x0003,     ///< IO port 0 pin 3
    GPIO_PORT_00_PIN_4  = 0x0004,     ///< IO port 0 pin 4
    GPIO_PORT_00_PIN_5  = 0x0005,     ///< IO port 0 pin 5
    GPIO_PORT_00_PIN_6  = 0x0006,     ///< IO port 0 pin 6
    GPIO_PORT_00_PIN_7  = 0x0007,     ///< IO port 0 pin 7
    GPIO_PORT_00_PIN_8  = 0x0008,     ///< IO port 0 pin 8
    GPIO_PORT_00_PIN_9  = 0x0009,     ///< IO port 0 pin 9
    GPIO_PORT_00_PIN_10 = 0x000A,     ///< IO port 0 pin 10
    GPIO_PORT_00_PIN_11 = 0x000B,     ///< IO port 0 pin 11
    GPIO_PORT_00_PIN_12 = 0x000C,     ///< IO port 0 pin 12
    GPIO_PORT_00_PIN_13 = 0x000D,     ///< IO port 0 pin 13
    GPIO_PORT_00_PIN_14 = 0x000E,     ///< IO port 0 pin 14
    GPIO_PORT_00_PIN_15 = 0x000F,     ///< IO port 0 pin 15
    GPIO_PORT_00_PIN_16 = 0x0010,     ///< IO port 0 pin 16
    GPIO_PORT_00_PIN_17 = 0x0011,     ///< IO port 0 pin 17
    GPIO_PORT_00_PIN_18 = 0x0012,     ///< IO port 0 pin 18
    GPIO_PORT_00_PIN_19 = 0x0013,     ///< IO port 0 pin 19
    GPIO_PORT_00_PIN_20 = 0x0014,     ///< IO port 0 pin 20
    GPIO_PORT_00_PIN_21 = 0x0015,     ///< IO port 0 pin 21
    GPIO_PORT_00_PIN_22 = 0x0016,     ///< IO port 0 pin 22
    GPIO_PORT_00_PIN_23 = 0x0017,     ///< IO port 0 pin 23
    GPIO_PORT_00_PIN_24 = 0x0018,     ///< IO port 0 pin 24
    GPIO_PORT_00_PIN_25 = 0x0019,     ///< IO port 0 pin 25
    GPIO_PORT_00_PIN_26 = 0x001A,     ///< IO port 0 pin 26
    GPIO_PORT_00_PIN_27 = 0x001B,     ///< IO port 0 pin 27
    GPIO_PORT_00_PIN_28 = 0x001C,     ///< IO port 0 pin 28
    GPIO_PORT_00_PIN_29 = 0x001D,     ///< IO port 0 pin 29
    GPIO_PORT_00_PIN_30 = 0x001E,     ///< IO port 0 pin 30
    GPIO_PORT_00_PIN_31 = 0x001F,     ///< IO port 0 pin 31

    GPIO_PORT_01_PIN_0  = 0x0100,     ///< IO port 1 pin 0
    GPIO_PORT_01_PIN_1  = 0x0101,     ///< IO port 1 pin 1
    GPIO_PORT_01_PIN_2  = 0x0102,     ///< IO port 1 pin 2
    GPIO_PORT_01_PIN_3  = 0x0103,     ///< IO port 1 pin 3
    GPIO_PORT_01_PIN_4  = 0x0104,     ///< IO port 1 pin 4
    GPIO_PORT_01_PIN_5  = 0x0105,     ///< IO port 1 pin 5
    GPIO_PORT_01_PIN_6  = 0x0106,     ///< IO port 1 pin 6
    GPIO_PORT_01_PIN_7  = 0x0107,     ///< IO port 1 pin 7
    GPIO_PORT_01_PIN_8  = 0x0108,     ///< IO port 1 pin 8
    GPIO_PORT_01_PIN_9  = 0x0109,     ///< IO port 1 pin 9
    GPIO_PORT_01_PIN_10 = 0x010A,     ///< IO port 1 pin 10
    GPIO_PORT_01_PIN_11 = 0x010B,     ///< IO port 1 pin 11
    GPIO_PORT_01_PIN_12 = 0x010C,     ///< IO port 1 pin 12
    GPIO_PORT_01_PIN_13 = 0x010D,     ///< IO port 1 pin 13
    GPIO_PORT_01_PIN_14 = 0x010E,     ///< IO port 1 pin 14
    GPIO_PORT_01_PIN_15 = 0x010F,     ///< IO port 1 pin 15
    GPIO_PORT_01_PIN_16 = 0x0110,     ///< IO port 1 pin 16
    GPIO_PORT_01_PIN_17 = 0x0111,     ///< IO port 1 pin 17
    GPIO_PORT_01_PIN_18 = 0x0112,     ///< IO port 1 pin 18
    GPIO_PORT_01_PIN_19 = 0x0113,     ///< IO port 1 pin 19
    GPIO_PORT_01_PIN_20 = 0x0114,     ///< IO port 1 pin 20
    GPIO_PORT_01_PIN_21 = 0x0115,     ///< IO port 1 pin 21
    GPIO_PORT_01_PIN_22 = 0x0116,     ///< IO port 1 pin 22
    GPIO_PORT_01_PIN_23 = 0x0117,     ///< IO port 1 pin 23
    GPIO_PORT_01_PIN_24 = 0x0118,     ///< IO port 1 pin 24
    GPIO_PORT_01_PIN_25 = 0x0119,     ///< IO port 1 pin 25
    GPIO_PORT_01_PIN_26 = 0x011A,     ///< IO port 1 pin 26
    GPIO_PORT_01_PIN_27 = 0x011B,     ///< IO port 1 pin 27
    GPIO_PORT_01_PIN_28 = 0x011C,     ///< IO port 1 pin 28
    GPIO_PORT_01_PIN_29 = 0x011D,     ///< IO port 1 pin 29
    GPIO_PORT_01_PIN_30 = 0x011E,     ///< IO port 1 pin 30
    GPIO_PORT_01_PIN_31 = 0x011F,     ///< IO port 1 pin 31

    GPIO_PORT_02_PIN_0  = 0x0200,     ///< IO port 2 pin 0
    GPIO_PORT_02_PIN_1  = 0x0201,     ///< IO port 2 pin 1
    GPIO_PORT_02_PIN_2  = 0x0202,     ///< IO port 2 pin 2
    GPIO_PORT_02_PIN_3  = 0x0203,     ///< IO port 2 pin 3
    GPIO_PORT_02_PIN_4  = 0x0204,     ///< IO port 2 pin 4
    GPIO_PORT_02_PIN_5  = 0x0205,     ///< IO port 2 pin 5
    GPIO_PORT_02_PIN_6  = 0x0206,     ///< IO port 2 pin 6
    GPIO_PORT_02_PIN_7  = 0x0207,     ///< IO port 2 pin 7
    GPIO_PORT_02_PIN_8  = 0x0208,     ///< IO port 2 pin 8
    GPIO_PORT_02_PIN_9  = 0x0209,     ///< IO port 2 pin 9
    GPIO_PORT_02_PIN_10 = 0x020A,     ///< IO port 2 pin 10
    GPIO_PORT_02_PIN_11 = 0x020B,     ///< IO port 2 pin 11
    GPIO_PORT_02_PIN_12 = 0x020C,     ///< IO port 2 pin 12
    GPIO_PORT_02_PIN_13 = 0x020D,     ///< IO port 2 pin 13
    GPIO_PORT_02_PIN_14 = 0x020E,     ///< IO port 2 pin 14
    GPIO_PORT_02_PIN_15 = 0x020F,     ///< IO port 2 pin 15
    GPIO_PORT_02_PIN_16 = 0x0210,     ///< IO port 2 pin 16
    GPIO_PORT_02_PIN_17 = 0x0211,     ///< IO port 2 pin 17
    GPIO_PORT_02_PIN_18 = 0x0212,     ///< IO port 2 pin 18
    GPIO_PORT_02_PIN_19 = 0x0213,     ///< IO port 2 pin 19
    GPIO_PORT_02_PIN_20 = 0x0214,     ///< IO port 2 pin 20
    GPIO_PORT_02_PIN_21 = 0x0215,     ///< IO port 2 pin 21
    GPIO_PORT_02_PIN_22 = 0x0216,     ///< IO port 2 pin 22
    GPIO_PORT_02_PIN_23 = 0x0217,     ///< IO port 2 pin 23
    GPIO_PORT_02_PIN_24 = 0x0218,     ///< IO port 2 pin 24
    GPIO_PORT_02_PIN_25 = 0x0219,     ///< IO port 2 pin 25
    GPIO_PORT_02_PIN_26 = 0x021A,     ///< IO port 2 pin 26
    GPIO_PORT_02_PIN_27 = 0x021B,     ///< IO port 2 pin 27
    GPIO_PORT_02_PIN_28 = 0x021C,     ///< IO port 2 pin 28
    GPIO_PORT_02_PIN_29 = 0x021D,     ///< IO port 2 pin 29
    GPIO_PORT_02_PIN_30 = 0x021E,     ///< IO port 2 pin 30
    GPIO_PORT_02_PIN_31 = 0x021F,     ///< IO port 2 pin 31

    GPIO_PORT_03_PIN_0  = 0x0300,     ///< IO port 3 pin 0
    GPIO_PORT_03_PIN_1  = 0x0301,     ///< IO port 3 pin 1
    GPIO_PORT_03_PIN_2  = 0x0302,     ///< IO port 3 pin 2
    GPIO_PORT_03_PIN_3  = 0x0303,     ///< IO port 3 pin 3
    GPIO_PORT_03_PIN_4  = 0x0304,     ///< IO port 3 pin 4
    GPIO_PORT_03_PIN_5  = 0x0305,     ///< IO port 3 pin 5
    GPIO_PORT_03_PIN_6  = 0x0306,     ///< IO port 3 pin 6
    GPIO_PORT_03_PIN_7  = 0x0307,     ///< IO port 3 pin 7
    GPIO_PORT_03_PIN_8  = 0x0308,     ///< IO port 3 pin 8
    GPIO_PORT_03_PIN_9  = 0x0309,     ///< IO port 3 pin 9
    GPIO_PORT_03_PIN_10 = 0x030A,     ///< IO port 3 pin 10
    GPIO_PORT_03_PIN_11 = 0x030B,     ///< IO port 3 pin 11
    GPIO_PORT_03_PIN_12 = 0x030C,     ///< IO port 3 pin 12
    GPIO_PORT_03_PIN_13 = 0x030D,     ///< IO port 3 pin 13
    GPIO_PORT_03_PIN_14 = 0x030E,     ///< IO port 3 pin 14
    GPIO_PORT_03_PIN_15 = 0x030F,     ///< IO port 3 pin 15
    GPIO_PORT_03_PIN_16 = 0x0310,     ///< IO port 3 pin 16
    GPIO_PORT_03_PIN_17 = 0x0311,     ///< IO port 3 pin 17
    GPIO_PORT_03_PIN_18 = 0x0312,     ///< IO port 3 pin 18
    GPIO_PORT_03_PIN_19 = 0x0313,     ///< IO port 3 pin 19
    GPIO_PORT_03_PIN_20 = 0x0314,     ///< IO port 3 pin 20
    GPIO_PORT_03_PIN_21 = 0x0315,     ///< IO port 3 pin 21
    GPIO_PORT_03_PIN_22 = 0x0316,     ///< IO port 3 pin 22
    GPIO_PORT_03_PIN_23 = 0x0317,     ///< IO port 3 pin 23
    GPIO_PORT_03_PIN_24 = 0x0318,     ///< IO port 3 pin 24
    GPIO_PORT_03_PIN_25 = 0x0319,     ///< IO port 3 pin 25
    GPIO_PORT_03_PIN_26 = 0x031A,     ///< IO port 3 pin 26
    GPIO_PORT_03_PIN_27 = 0x031B,     ///< IO port 3 pin 27
    GPIO_PORT_03_PIN_28 = 0x031C,     ///< IO port 3 pin 28
    GPIO_PORT_03_PIN_29 = 0x031D,     ///< IO port 3 pin 29
    GPIO_PORT_03_PIN_30 = 0x031E,     ///< IO port 3 pin 30
    GPIO_PORT_03_PIN_31 = 0x031F,     ///< IO port 3 pin 31

    GPIO_PORT_04_PIN_0  = 0x0400,     ///< IO port 4 pin 0
    GPIO_PORT_04_PIN_1  = 0x0401,     ///< IO port 4 pin 1
    GPIO_PORT_04_PIN_2  = 0x0402,     ///< IO port 4 pin 2
    GPIO_PORT_04_PIN_3  = 0x0403,     ///< IO port 4 pin 3
    GPIO_PORT_04_PIN_4  = 0x0404,     ///< IO port 4 pin 4
    GPIO_PORT_04_PIN_5  = 0x0405,     ///< IO port 4 pin 5
    GPIO_PORT_04_PIN_6  = 0x0406,     ///< IO port 4 pin 6
    GPIO_PORT_04_PIN_7  = 0x0407,     ///< IO port 4 pin 7
    GPIO_PORT_04_PIN_8  = 0x0408,     ///< IO port 4 pin 8
    GPIO_PORT_04_PIN_9  = 0x0409,     ///< IO port 4 pin 9
    GPIO_PORT_04_PIN_10 = 0x040A,     ///< IO port 4 pin 10
    GPIO_PORT_04_PIN_11 = 0x040B,     ///< IO port 4 pin 11
    GPIO_PORT_04_PIN_12 = 0x040C,     ///< IO port 4 pin 12
    GPIO_PORT_04_PIN_13 = 0x040D,     ///< IO port 4 pin 13
    GPIO_PORT_04_PIN_14 = 0x040E,     ///< IO port 4 pin 14
    GPIO_PORT_04_PIN_15 = 0x040F,     ///< IO port 4 pin 15
    GPIO_PORT_04_PIN_16 = 0x0410,     ///< IO port 4 pin 16
    GPIO_PORT_04_PIN_17 = 0x0411,     ///< IO port 4 pin 17
    GPIO_PORT_04_PIN_18 = 0x0412,     ///< IO port 4 pin 18
    GPIO_PORT_04_PIN_19 = 0x0413,     ///< IO port 4 pin 19
    GPIO_PORT_04_PIN_20 = 0x0414,     ///< IO port 4 pin 20
    GPIO_PORT_04_PIN_21 = 0x0415,     ///< IO port 4 pin 21
    GPIO_PORT_04_PIN_22 = 0x0416,     ///< IO port 4 pin 22
    GPIO_PORT_04_PIN_23 = 0x0417,     ///< IO port 4 pin 23
    GPIO_PORT_04_PIN_24 = 0x0418,     ///< IO port 4 pin 24
    GPIO_PORT_04_PIN_25 = 0x0419,     ///< IO port 4 pin 25
    GPIO_PORT_04_PIN_26 = 0x041A,     ///< IO port 4 pin 26
    GPIO_PORT_04_PIN_27 = 0x041B,     ///< IO port 4 pin 27
    GPIO_PORT_04_PIN_28 = 0x041C,     ///< IO port 4 pin 28
    GPIO_PORT_04_PIN_29 = 0x041D,     ///< IO port 4 pin 29
    GPIO_PORT_04_PIN_30 = 0x041E,     ///< IO port 4 pin 30
    GPIO_PORT_04_PIN_31 = 0x041F,     ///< IO port 4 pin 31

    GPIO_PORT_05_PIN_0  = 0x0500,     ///< IO port 5 pin 0
    GPIO_PORT_05_PIN_1  = 0x0501,     ///< IO port 5 pin 1
    GPIO_PORT_05_PIN_2  = 0x0502,     ///< IO port 5 pin 2
    GPIO_PORT_05_PIN_3  = 0x0503,     ///< IO port 5 pin 3
    GPIO_PORT_05_PIN_4  = 0x0504,     ///< IO port 5 pin 4
    GPIO_PORT_05_PIN_5  = 0x0505,     ///< IO port 5 pin 5
    GPIO_PORT_05_PIN_6  = 0x0506,     ///< IO port 5 pin 6
    GPIO_PORT_05_PIN_7  = 0x0507,     ///< IO port 5 pin 7
    GPIO_PORT_05_PIN_8  = 0x0508,     ///< IO port 5 pin 8
    GPIO_PORT_05_PIN_9  = 0x0509,     ///< IO port 5 pin 9
    GPIO_PORT_05_PIN_10 = 0x050A,     ///< IO port 5 pin 10
    GPIO_PORT_05_PIN_11 = 0x050B,     ///< IO port 5 pin 11
    GPIO_PORT_05_PIN_12 = 0x050C,     ///< IO port 5 pin 12
    GPIO_PORT_05_PIN_13 = 0x050D,     ///< IO port 5 pin 13
    GPIO_PORT_05_PIN_14 = 0x050E,     ///< IO port 5 pin 14
    GPIO_PORT_05_PIN_15 = 0x050F,     ///< IO port 5 pin 15
    GPIO_PORT_05_PIN_16 = 0x0510,     ///< IO port 5 pin 16
    GPIO_PORT_05_PIN_17 = 0x0511,     ///< IO port 5 pin 17
    GPIO_PORT_05_PIN_18 = 0x0512,     ///< IO port 5 pin 18
    GPIO_PORT_05_PIN_19 = 0x0513,     ///< IO port 5 pin 19
    GPIO_PORT_05_PIN_20 = 0x0514,     ///< IO port 5 pin 20
    GPIO_PORT_05_PIN_21 = 0x0515,     ///< IO port 5 pin 21
    GPIO_PORT_05_PIN_22 = 0x0516,     ///< IO port 5 pin 22
    GPIO_PORT_05_PIN_23 = 0x0517,     ///< IO port 5 pin 23
    GPIO_PORT_05_PIN_24 = 0x0518,     ///< IO port 5 pin 24
    GPIO_PORT_05_PIN_25 = 0x0519,     ///< IO port 5 pin 25
    GPIO_PORT_05_PIN_26 = 0x051A,     ///< IO port 5 pin 26
    GPIO_PORT_05_PIN_27 = 0x051B,     ///< IO port 5 pin 27
    GPIO_PORT_05_PIN_28 = 0x051C,     ///< IO port 5 pin 28
    GPIO_PORT_05_PIN_29 = 0x051D,     ///< IO port 5 pin 29
    GPIO_PORT_05_PIN_30 = 0x051E,     ///< IO port 5 pin 30
    GPIO_PORT_05_PIN_31 = 0x051F,     ///< IO port 5 pin 31

    GPIO_PORT_06_PIN_0  = 0x0600,     ///< IO port 6 pin 0
    GPIO_PORT_06_PIN_1  = 0x0601,     ///< IO port 6 pin 1
    GPIO_PORT_06_PIN_2  = 0x0602,     ///< IO port 6 pin 2
    GPIO_PORT_06_PIN_3  = 0x0603,     ///< IO port 6 pin 3
    GPIO_PORT_06_PIN_4  = 0x0604,     ///< IO port 6 pin 4
    GPIO_PORT_06_PIN_5  = 0x0605,     ///< IO port 6 pin 5
    GPIO_PORT_06_PIN_6  = 0x0606,     ///< IO port 6 pin 6
    GPIO_PORT_06_PIN_7  = 0x0607,     ///< IO port 6 pin 7
    GPIO_PORT_06_PIN_8  = 0x0608,     ///< IO port 6 pin 8
    GPIO_PORT_06_PIN_9  = 0x0609,     ///< IO port 6 pin 9
    GPIO_PORT_06_PIN_10 = 0x060A,     ///< IO port 6 pin 10
    GPIO_PORT_06_PIN_11 = 0x060B,     ///< IO port 6 pin 11
    GPIO_PORT_06_PIN_12 = 0x060C,     ///< IO port 6 pin 12
    GPIO_PORT_06_PIN_13 = 0x060D,     ///< IO port 6 pin 13
    GPIO_PORT_06_PIN_14 = 0x060E,     ///< IO port 6 pin 14
    GPIO_PORT_06_PIN_15 = 0x060F,     ///< IO port 6 pin 15
    GPIO_PORT_06_PIN_16 = 0x0610,     ///< IO port 6 pin 16
    GPIO_PORT_06_PIN_17 = 0x0611,     ///< IO port 6 pin 17
    GPIO_PORT_06_PIN_18 = 0x0612,     ///< IO port 6 pin 18
    GPIO_PORT_06_PIN_19 = 0x0613,     ///< IO port 6 pin 19
    GPIO_PORT_06_PIN_20 = 0x0614,     ///< IO port 6 pin 20
    GPIO_PORT_06_PIN_21 = 0x0615,     ///< IO port 6 pin 21
    GPIO_PORT_06_PIN_22 = 0x0616,     ///< IO port 6 pin 22
    GPIO_PORT_06_PIN_23 = 0x0617,     ///< IO port 6 pin 23
    GPIO_PORT_06_PIN_24 = 0x0618,     ///< IO port 6 pin 24
    GPIO_PORT_06_PIN_25 = 0x0619,     ///< IO port 6 pin 25
    GPIO_PORT_06_PIN_26 = 0x061A,     ///< IO port 6 pin 26
    GPIO_PORT_06_PIN_27 = 0x061B,     ///< IO port 6 pin 27
    GPIO_PORT_06_PIN_28 = 0x061C,     ///< IO port 6 pin 28
    GPIO_PORT_06_PIN_29 = 0x061D,     ///< IO port 6 pin 29
    GPIO_PORT_06_PIN_30 = 0x061E,     ///< IO port 6 pin 30
    GPIO_PORT_06_PIN_31 = 0x061F,     ///< IO port 6 pin 31

    GPIO_PORT_07_PIN_0  = 0x0700,     ///< IO port 7 pin 0
    GPIO_PORT_07_PIN_1  = 0x0701,     ///< IO port 7 pin 1
    GPIO_PORT_07_PIN_2  = 0x0702,     ///< IO port 7 pin 2
    GPIO_PORT_07_PIN_3  = 0x0703,     ///< IO port 7 pin 3
    GPIO_PORT_07_PIN_4  = 0x0704,     ///< IO port 7 pin 4
    GPIO_PORT_07_PIN_5  = 0x0705,     ///< IO port 7 pin 5
    GPIO_PORT_07_PIN_6  = 0x0706,     ///< IO port 7 pin 6
    GPIO_PORT_07_PIN_7  = 0x0707,     ///< IO port 7 pin 7
    GPIO_PORT_07_PIN_8  = 0x0708,     ///< IO port 7 pin 8
    GPIO_PORT_07_PIN_9  = 0x0709,     ///< IO port 7 pin 9
    GPIO_PORT_07_PIN_10 = 0x070A,     ///< IO port 7 pin 10
    GPIO_PORT_07_PIN_11 = 0x070B,     ///< IO port 7 pin 11
    GPIO_PORT_07_PIN_12 = 0x070C,     ///< IO port 7 pin 12
    GPIO_PORT_07_PIN_13 = 0x070D,     ///< IO port 7 pin 13
    GPIO_PORT_07_PIN_14 = 0x070E,     ///< IO port 7 pin 14
    GPIO_PORT_07_PIN_15 = 0x070F,     ///< IO port 7 pin 15
    GPIO_PORT_07_PIN_16 = 0x0710,     ///< IO port 7 pin 16
    GPIO_PORT_07_PIN_17 = 0x0711,     ///< IO port 7 pin 17
    GPIO_PORT_07_PIN_18 = 0x0712,     ///< IO port 7 pin 18
    GPIO_PORT_07_PIN_19 = 0x0713,     ///< IO port 7 pin 19
    GPIO_PORT_07_PIN_20 = 0x0714,     ///< IO port 7 pin 20
    GPIO_PORT_07_PIN_21 = 0x0715,     ///< IO port 7 pin 21
    GPIO_PORT_07_PIN_22 = 0x0716,     ///< IO port 7 pin 22
    GPIO_PORT_07_PIN_23 = 0x0717,     ///< IO port 7 pin 23
    GPIO_PORT_07_PIN_24 = 0x0718,     ///< IO port 7 pin 24
    GPIO_PORT_07_PIN_25 = 0x0719,     ///< IO port 7 pin 25
    GPIO_PORT_07_PIN_26 = 0x071A,     ///< IO port 7 pin 26
    GPIO_PORT_07_PIN_27 = 0x071B,     ///< IO port 7 pin 27
    GPIO_PORT_07_PIN_28 = 0x071C,     ///< IO port 7 pin 28
    GPIO_PORT_07_PIN_29 = 0x071D,     ///< IO port 7 pin 29
    GPIO_PORT_07_PIN_30 = 0x071E,     ///< IO port 7 pin 30
    GPIO_PORT_07_PIN_31 = 0x071F,     ///< IO port 7 pin 31

    GPIO_PORT_08_PIN_0  = 0x0800,     ///< IO port 8 pin 0
    GPIO_PORT_08_PIN_1  = 0x0801,     ///< IO port 8 pin 1
    GPIO_PORT_08_PIN_2  = 0x0802,     ///< IO port 8 pin 2
    GPIO_PORT_08_PIN_3  = 0x0803,     ///< IO port 8 pin 3
    GPIO_PORT_08_PIN_4  = 0x0804,     ///< IO port 8 pin 4
    GPIO_PORT_08_PIN_5  = 0x0805,     ///< IO port 8 pin 5
    GPIO_PORT_08_PIN_6  = 0x0806,     ///< IO port 8 pin 6
    GPIO_PORT_08_PIN_7  = 0x0807,     ///< IO port 8 pin 7
    GPIO_PORT_08_PIN_8  = 0x0808,     ///< IO port 8 pin 8
    GPIO_PORT_08_PIN_9  = 0x0809,     ///< IO port 8 pin 9
    GPIO_PORT_08_PIN_10 = 0x080A,     ///< IO port 8 pin 10
    GPIO_PORT_08_PIN_11 = 0x080B,     ///< IO port 8 pin 11
    GPIO_PORT_08_PIN_12 = 0x080C,     ///< IO port 8 pin 12
    GPIO_PORT_08_PIN_13 = 0x080D,     ///< IO port 8 pin 13
    GPIO_PORT_08_PIN_14 = 0x080E,     ///< IO port 8 pin 14
    GPIO_PORT_08_PIN_15 = 0x080F,     ///< IO port 8 pin 15
    GPIO_PORT_08_PIN_16 = 0x0810,     ///< IO port 8 pin 16
    GPIO_PORT_08_PIN_17 = 0x0811,     ///< IO port 8 pin 17
    GPIO_PORT_08_PIN_18 = 0x0812,     ///< IO port 8 pin 18
    GPIO_PORT_08_PIN_19 = 0x0813,     ///< IO port 8 pin 19
    GPIO_PORT_08_PIN_20 = 0x0814,     ///< IO port 8 pin 20
    GPIO_PORT_08_PIN_21 = 0x0815,     ///< IO port 8 pin 21
    GPIO_PORT_08_PIN_22 = 0x0816,     ///< IO port 8 pin 22
    GPIO_PORT_08_PIN_23 = 0x0817,     ///< IO port 8 pin 23
    GPIO_PORT_08_PIN_24 = 0x0818,     ///< IO port 8 pin 24
    GPIO_PORT_08_PIN_25 = 0x0819,     ///< IO port 8 pin 25
    GPIO_PORT_08_PIN_26 = 0x081A,     ///< IO port 8 pin 26
    GPIO_PORT_08_PIN_27 = 0x081B,     ///< IO port 8 pin 27
    GPIO_PORT_08_PIN_28 = 0x081C,     ///< IO port 8 pin 28
    GPIO_PORT_08_PIN_29 = 0x081D,     ///< IO port 8 pin 29
    GPIO_PORT_08_PIN_30 = 0x081E,     ///< IO port 8 pin 30
    GPIO_PORT_08_PIN_31 = 0x081F,     ///< IO port 8 pin 31

    GPIO_PORT_09_PIN_0  = 0x0900,     ///< IO port 9 pin 0
    GPIO_PORT_09_PIN_1  = 0x0901,     ///< IO port 9 pin 1
    GPIO_PORT_09_PIN_2  = 0x0902,     ///< IO port 9 pin 2
    GPIO_PORT_09_PIN_3  = 0x0903,     ///< IO port 9 pin 3
    GPIO_PORT_09_PIN_4  = 0x0904,     ///< IO port 9 pin 4
    GPIO_PORT_09_PIN_5  = 0x0905,     ///< IO port 9 pin 5
    GPIO_PORT_09_PIN_6  = 0x0906,     ///< IO port 9 pin 6
    GPIO_PORT_09_PIN_7  = 0x0907,     ///< IO port 9 pin 7
    GPIO_PORT_09_PIN_8  = 0x0908,     ///< IO port 9 pin 8
    GPIO_PORT_09_PIN_9  = 0x0909,     ///< IO port 9 pin 9
    GPIO_PORT_09_PIN_10 = 0x090A,     ///< IO port 9 pin 10
    GPIO_PORT_09_PIN_11 = 0x090B,     ///< IO port 9 pin 11
    GPIO_PORT_09_PIN_12 = 0x090C,     ///< IO port 9 pin 12
    GPIO_PORT_09_PIN_13 = 0x090D,     ///< IO port 9 pin 13
    GPIO_PORT_09_PIN_14 = 0x090E,     ///< IO port 9 pin 14
    GPIO_PORT_09_PIN_15 = 0x090F,     ///< IO port 9 pin 15
    GPIO_PORT_09_PIN_16 = 0x0910,     ///< IO port 9 pin 16
    GPIO_PORT_09_PIN_17 = 0x0911,     ///< IO port 9 pin 17
    GPIO_PORT_09_PIN_18 = 0x0912,     ///< IO port 9 pin 18
    GPIO_PORT_09_PIN_19 = 0x0913,     ///< IO port 9 pin 19
    GPIO_PORT_09_PIN_20 = 0x0914,     ///< IO port 9 pin 20
    GPIO_PORT_09_PIN_21 = 0x0915,     ///< IO port 9 pin 21
    GPIO_PORT_09_PIN_22 = 0x0916,     ///< IO port 9 pin 22
    GPIO_PORT_09_PIN_23 = 0x0917,     ///< IO port 9 pin 23
    GPIO_PORT_09_PIN_24 = 0x0918,     ///< IO port 9 pin 24
    GPIO_PORT_09_PIN_25 = 0x0919,     ///< IO port 9 pin 25
    GPIO_PORT_09_PIN_26 = 0x091A,     ///< IO port 9 pin 26
    GPIO_PORT_09_PIN_27 = 0x091B,     ///< IO port 9 pin 27
    GPIO_PORT_09_PIN_28 = 0x091C,     ///< IO port 9 pin 28
    GPIO_PORT_09_PIN_29 = 0x091D,     ///< IO port 9 pin 29
    GPIO_PORT_09_PIN_30 = 0x091E,     ///< IO port 9 pin 30
    GPIO_PORT_09_PIN_31 = 0x091F,     ///< IO port 9 pin 31

    GPIO_PORT_10_PIN_0  = 0x0A00,     ///< IO port 10 pin 0
    GPIO_PORT_10_PIN_1  = 0x0A01,     ///< IO port 10 pin 1
    GPIO_PORT_10_PIN_2  = 0x0A02,     ///< IO port 10 pin 2
    GPIO_PORT_10_PIN_3  = 0x0A03,     ///< IO port 10 pin 3
    GPIO_PORT_10_PIN_4  = 0x0A04,     ///< IO port 10 pin 4
    GPIO_PORT_10_PIN_5  = 0x0A05,     ///< IO port 10 pin 5
    GPIO_PORT_10_PIN_6  = 0x0A06,     ///< IO port 10 pin 6
    GPIO_PORT_10_PIN_7  = 0x0A07,     ///< IO port 10 pin 7
    GPIO_PORT_10_PIN_8  = 0x0A08,     ///< IO port 10 pin 8
    GPIO_PORT_10_PIN_9  = 0x0A09,     ///< IO port 10 pin 9
    GPIO_PORT_10_PIN_10 = 0x0A0A,     ///< IO port 10 pin 10
    GPIO_PORT_10_PIN_11 = 0x0A0B,     ///< IO port 10 pin 11
    GPIO_PORT_10_PIN_12 = 0x0A0C,     ///< IO port 10 pin 12
    GPIO_PORT_10_PIN_13 = 0x0A0D,     ///< IO port 10 pin 13
    GPIO_PORT_10_PIN_14 = 0x0A0E,     ///< IO port 10 pin 14
    GPIO_PORT_10_PIN_15 = 0x0A0F,     ///< IO port 10 pin 15
    GPIO_PORT_10_PIN_16 = 0x0A10,     ///< IO port 10 pin 16
    GPIO_PORT_10_PIN_17 = 0x0A11,     ///< IO port 10 pin 17
    GPIO_PORT_10_PIN_18 = 0x0A12,     ///< IO port 10 pin 18
    GPIO_PORT_10_PIN_19 = 0x0A13,     ///< IO port 10 pin 19
    GPIO_PORT_10_PIN_20 = 0x0A14,     ///< IO port 10 pin 20
    GPIO_PORT_10_PIN_21 = 0x0A15,     ///< IO port 10 pin 21
    GPIO_PORT_10_PIN_22 = 0x0A16,     ///< IO port 10 pin 22
    GPIO_PORT_10_PIN_23 = 0x0A17,     ///< IO port 10 pin 23
    GPIO_PORT_10_PIN_24 = 0x0A18,     ///< IO port 10 pin 24
    GPIO_PORT_10_PIN_25 = 0x0A19,     ///< IO port 10 pin 25
    GPIO_PORT_10_PIN_26 = 0x0A1A,     ///< IO port 10 pin 26
    GPIO_PORT_10_PIN_27 = 0x0A1B,     ///< IO port 10 pin 27
    GPIO_PORT_10_PIN_28 = 0x0A1C,     ///< IO port 10 pin 28
    GPIO_PORT_10_PIN_29 = 0x0A1D,     ///< IO port 10 pin 29
    GPIO_PORT_10_PIN_30 = 0x0A1E,     ///< IO port 10 pin 30
    GPIO_PORT_10_PIN_31 = 0x0A1F,     ///< IO port 10 pin 31
} gpio_port_pin_t;

/**
 * @brief Configuration for a single GPIO pin.
 */
typedef struct st_gpio_pin_cfg
{
    uint32_t pin_cfg;
    gpio_port_pin_t pin;
} gpio_pin_cfg_t;

/** Multiple pin configuration data for loading into registers by R_GPIO_Open() */
typedef struct st_gpio_cfg
{
    uint16_t                 number_of_pins; ///< Number of pins for which there is configuration data
    gpio_pin_cfg_t const * p_pin_cfg_data; ///< Pin configuration data
    const void             * p_extend;       ///< Pointer to hardware extend configuration
} gpio_cfg_t;

/**
 * @brief Control structure for GPIO operations.
 */
typedef void gpio_ctrl_t;

/**
 * @brief Instance control structure for GPIO.
 */
typedef struct st_gpio_instance_ctrl
{
    uint32_t open;
    void (* p_callback)(void *);
    void *p_context;
    gpio_cfg_t const * p_cfg;
} gpio_instance_ctrl_t;

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
/**
 * @brief Open the GPIO module.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] p_cfg Pointer to the configuration structure.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_GPIO_Open(gpio_ctrl_t * const p_ctrl, const gpio_cfg_t * p_cfg);

/**
 * @brief Close the GPIO module.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_GPIO_Close(gpio_ctrl_t * const p_ctrl);

/**
 * @brief Configure multiple GPIO pins.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] p_cfg Pointer to the configuration structure.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_GPIO_PinsCfg(gpio_ctrl_t * const p_ctrl, const gpio_cfg_t * p_cfg);

/**
 * @brief Configure a single GPIO pin.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] pin GPIO pin to configure.
 * @param[in] cfg Configuration settings for the pin.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_GPIO_PinCfg(gpio_ctrl_t * const p_ctrl, gpio_port_pin_t pin, uint32_t cfg);

/**
 * @brief Set interrupt input for a GPIO pin.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] pin GPIO pin to configure.
 * @param[in] option Interrupt input option.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_GPIO_PinInterruptInput(gpio_ctrl_t * const p_ctrl, gpio_port_pin_t pin, gpio_interrupt_input_t option);

/**
 * @brief Set a callback function for GPIO events.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] p_callback Pointer to the callback function.
 * @param[in] p_context Pointer to the user context.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_GPIO_CallbackSet(gpio_ctrl_t * const p_ctrl, void (*p_callback)(void *), void * const p_context);

/**
 * @brief Read the value of a GPIO pin.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] pin GPIO pin to read.
 * @param[out] p_pin_value Pointer to store the pin value.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_GPIO_PinRead(gpio_ctrl_t * const p_ctrl, gpio_port_pin_t pin, gpio_level_t * p_pin_value);

/**
 * @brief Write a value to a GPIO pin.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] pin GPIO pin to write.
 * @param[in] level Value to write to the pin.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_GPIO_PinWrite(gpio_ctrl_t * const p_ctrl, gpio_port_pin_t pin, gpio_level_t level);

/**
 * @brief Set the direction of a GPIO port.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] port GPIO port to configure.
 * @param[in] direction_values Direction values for the port.
 * @param[in] mask Mask to apply to the direction values.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_GPIO_PortDirectionSet(gpio_ctrl_t * const p_ctrl, gpio_port_t port, uint32_t direction_values, uint32_t mask);

/**
 * @brief Read the value of a GPIO port.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] port GPIO port to read.
 * @param[out] p_port_value Pointer to store the port value.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_GPIO_PortRead(gpio_ctrl_t * const p_ctrl, gpio_port_t port, uint32_t * p_port_value);

/**
 * @brief Write a value to a GPIO port.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] port GPIO port to write.
 * @param[in] value Value to write to the port.
 * @param[in] mask Mask to apply to the value.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
int R_GPIO_PortWrite(gpio_ctrl_t * const p_ctrl, gpio_port_t port, uint32_t value, uint32_t mask);

#endif // R_GPIO_H
