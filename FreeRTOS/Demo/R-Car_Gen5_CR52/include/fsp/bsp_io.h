/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @defgroup BSP_IO BSP I/O access
 * @ingroup RENESAS_COMMON
 * @brief This module provides basic read/write/toggle access to port pins and read/write access to port.
 *
 * @{
 **********************************************************************************************************************/

#ifndef BSP_IO_H
#define BSP_IO_H

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define __STATIC_INLINE static inline

/* Private definition to set enumeration values. */
#define BSP_IO_PRV_8BIT_MASK       (0xFF)
#define BSP_IO_PM_OUTPUT           (3U)

/* Key code for writing PRCR register. */
#define BSP_IO_PRV_PRCR_KEY        (0xA500U)
#define BSP_IO_REG_PROTECT_GPIO    (0x0004U)

/* Shift to get port in bsp_io_port_t and bsp_io_port_pin_t enums. */
#define BSP_IO_PRV_PORT_OFFSET     (8U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Levels that can be set and read for individual pins */
typedef enum e_bsp_io_level
{
    BSP_IO_LEVEL_LOW = 0,              ///< Low
    BSP_IO_LEVEL_HIGH                  ///< High
} bsp_io_level_t;

/** Direction of individual pins */
typedef enum e_bsp_io_dir
{
    BSP_IO_DIRECTION_INPUT = 0,        ///< Input
    BSP_IO_DIRECTION_OUTPUT            ///< Output
} bsp_io_direction_t;

/** Interrupt Input Mode*/
typedef enum e_bsp_interrupt_input
{
    BSP_INTERRUPT_INPUT_RISING_EDGE = 10,        ///< Rising Edge
    BSP_INTERRUPT_INPUT_FALLING_EDGE,		///< Falling Edge
    BSP_INTERRUPT_INPUT_BOTH_EDGE		///< Both Edge
} bsp_interrupt_input_t;

/** Superset list of all possible IO ports. */
typedef enum e_bsp_io_port
{
    BSP_IO_PORT_00 = 0x0000,           ///< IO port 0
    BSP_IO_PORT_01 = 0x0100,           ///< IO port 1
    BSP_IO_PORT_02 = 0x0200,           ///< IO port 2
    BSP_IO_PORT_03 = 0x0300,           ///< IO port 3
    BSP_IO_PORT_04 = 0x0400,           ///< IO port 4
    BSP_IO_PORT_05 = 0x0500,           ///< IO port 5
    BSP_IO_PORT_06 = 0x0600,           ///< IO port 6
    BSP_IO_PORT_07 = 0x0700,           ///< IO port 7
    BSP_IO_PORT_08 = 0x0800,           ///< IO port 8
    BSP_IO_PORT_09 = 0x0900,           ///< IO port 9
    BSP_IO_PORT_10 = 0x0A00,           ///< IO port 10
    //BSP_IO_PORT_11 = 0x0B00,           ///< IO port 11
    //BSP_IO_PORT_12 = 0x0C00,           ///< IO port 12
    //BSP_IO_PORT_13 = 0x0D00,           ///< IO port 13
    //BSP_IO_PORT_14 = 0x0E00,           ///< IO port 14
    //BSP_IO_PORT_15 = 0x0F00,           ///< IO port 15
    //BSP_IO_PORT_16 = 0x1000,           ///< IO port 16
    //BSP_IO_PORT_17 = 0x1100,           ///< IO port 17
    //BSP_IO_PORT_18 = 0x1200,           ///< IO port 18
    //BSP_IO_PORT_19 = 0x1300,           ///< IO port 19
    //BSP_IO_PORT_20 = 0x1400,           ///< IO port 20
    //BSP_IO_PORT_21 = 0x1500,           ///< IO port 21
    //BSP_IO_PORT_22 = 0x1600,           ///< IO port 22
    //BSP_IO_PORT_23 = 0x1700,           ///< IO port 23
    //BSP_IO_PORT_24 = 0x1800,           ///< IO port 24
    //BSP_IO_PORT_25 = 0x1900,           ///< IO port 25
    //BSP_IO_PORT_26 = 0x1A00,           ///< IO port 26
    //BSP_IO_PORT_27 = 0x1B00,           ///< IO port 27
    //BSP_IO_PORT_28 = 0x1C00,           ///< IO port 28
    //BSP_IO_PORT_29 = 0x1D00,           ///< IO port 29
    //BSP_IO_PORT_30 = 0x1E00,           ///< IO port 30
    //BSP_IO_PORT_31 = 0x1F00,           ///< IO port 31
    //BSP_IO_PORT_32 = 0x2000,           ///< IO port 32
    //BSP_IO_PORT_33 = 0x2100,           ///< IO port 33
    //BSP_IO_PORT_34 = 0x2200,           ///< IO port 34
    //BSP_IO_PORT_35 = 0x2300,           ///< IO port 35
} bsp_io_port_t;

/** Superset list of all possible IO port pins. */
typedef enum e_bsp_io_port_pin
{
    BSP_IO_PORT_00_PIN_0  = 0x0000,     ///< IO port 0 pin 0
    BSP_IO_PORT_00_PIN_1  = 0x0001,     ///< IO port 0 pin 1
    BSP_IO_PORT_00_PIN_2  = 0x0002,     ///< IO port 0 pin 2
    BSP_IO_PORT_00_PIN_3  = 0x0003,     ///< IO port 0 pin 3
    BSP_IO_PORT_00_PIN_4  = 0x0004,     ///< IO port 0 pin 4
    BSP_IO_PORT_00_PIN_5  = 0x0005,     ///< IO port 0 pin 5
    BSP_IO_PORT_00_PIN_6  = 0x0006,     ///< IO port 0 pin 6
    BSP_IO_PORT_00_PIN_7  = 0x0007,     ///< IO port 0 pin 7
    BSP_IO_PORT_00_PIN_8  = 0x0008,     ///< IO port 0 pin 8
    BSP_IO_PORT_00_PIN_9  = 0x0009,     ///< IO port 0 pin 9
    BSP_IO_PORT_00_PIN_10 = 0x000A,     ///< IO port 0 pin 10
    BSP_IO_PORT_00_PIN_11 = 0x000B,     ///< IO port 0 pin 11
    BSP_IO_PORT_00_PIN_12 = 0x000C,     ///< IO port 0 pin 12
    BSP_IO_PORT_00_PIN_13 = 0x000D,     ///< IO port 0 pin 13
    BSP_IO_PORT_00_PIN_14 = 0x000E,     ///< IO port 0 pin 14
    BSP_IO_PORT_00_PIN_15 = 0x000F,     ///< IO port 0 pin 15
    BSP_IO_PORT_00_PIN_16 = 0x0010,     ///< IO port 0 pin 16
    BSP_IO_PORT_00_PIN_17 = 0x0011,     ///< IO port 0 pin 17
    BSP_IO_PORT_00_PIN_18 = 0x0012,     ///< IO port 0 pin 18
    BSP_IO_PORT_00_PIN_19 = 0x0013,     ///< IO port 0 pin 19
    BSP_IO_PORT_00_PIN_20 = 0x0014,     ///< IO port 0 pin 20
    BSP_IO_PORT_00_PIN_21 = 0x0015,     ///< IO port 0 pin 21
    BSP_IO_PORT_00_PIN_22 = 0x0016,     ///< IO port 0 pin 22
    BSP_IO_PORT_00_PIN_23 = 0x0017,     ///< IO port 0 pin 23
    BSP_IO_PORT_00_PIN_24 = 0x0018,     ///< IO port 0 pin 24
    BSP_IO_PORT_00_PIN_25 = 0x0019,     ///< IO port 0 pin 25
    BSP_IO_PORT_00_PIN_26 = 0x001A,     ///< IO port 0 pin 26
    BSP_IO_PORT_00_PIN_27 = 0x001B,     ///< IO port 0 pin 27
    BSP_IO_PORT_00_PIN_28 = 0x001C,     ///< IO port 0 pin 28
    BSP_IO_PORT_00_PIN_29 = 0x001D,     ///< IO port 0 pin 29
    BSP_IO_PORT_00_PIN_30 = 0x001E,     ///< IO port 0 pin 30
    BSP_IO_PORT_00_PIN_31 = 0x001F,     ///< IO port 0 pin 31

    BSP_IO_PORT_01_PIN_0  = 0x0100,     ///< IO port 1 pin 0
    BSP_IO_PORT_01_PIN_1  = 0x0101,     ///< IO port 1 pin 1
    BSP_IO_PORT_01_PIN_2  = 0x0102,     ///< IO port 1 pin 2
    BSP_IO_PORT_01_PIN_3  = 0x0103,     ///< IO port 1 pin 3
    BSP_IO_PORT_01_PIN_4  = 0x0104,     ///< IO port 1 pin 4
    BSP_IO_PORT_01_PIN_5  = 0x0105,     ///< IO port 1 pin 5
    BSP_IO_PORT_01_PIN_6  = 0x0106,     ///< IO port 1 pin 6
    BSP_IO_PORT_01_PIN_7  = 0x0107,     ///< IO port 1 pin 7
    BSP_IO_PORT_01_PIN_8  = 0x0108,     ///< IO port 1 pin 8
    BSP_IO_PORT_01_PIN_9  = 0x0109,     ///< IO port 1 pin 9
    BSP_IO_PORT_01_PIN_10 = 0x010A,     ///< IO port 1 pin 10
    BSP_IO_PORT_01_PIN_11 = 0x010B,     ///< IO port 1 pin 11
    BSP_IO_PORT_01_PIN_12 = 0x010C,     ///< IO port 1 pin 12
    BSP_IO_PORT_01_PIN_13 = 0x010D,     ///< IO port 1 pin 13
    BSP_IO_PORT_01_PIN_14 = 0x010E,     ///< IO port 1 pin 14
    BSP_IO_PORT_01_PIN_15 = 0x010F,     ///< IO port 1 pin 15
    BSP_IO_PORT_01_PIN_16 = 0x0110,     ///< IO port 1 pin 16
    BSP_IO_PORT_01_PIN_17 = 0x0111,     ///< IO port 1 pin 17
    BSP_IO_PORT_01_PIN_18 = 0x0112,     ///< IO port 1 pin 18
    BSP_IO_PORT_01_PIN_19 = 0x0113,     ///< IO port 1 pin 19
    BSP_IO_PORT_01_PIN_20 = 0x0114,     ///< IO port 1 pin 20
    BSP_IO_PORT_01_PIN_21 = 0x0115,     ///< IO port 1 pin 21
    BSP_IO_PORT_01_PIN_22 = 0x0116,     ///< IO port 1 pin 22
    BSP_IO_PORT_01_PIN_23 = 0x0117,     ///< IO port 1 pin 23
    BSP_IO_PORT_01_PIN_24 = 0x0118,     ///< IO port 1 pin 24
    BSP_IO_PORT_01_PIN_25 = 0x0119,     ///< IO port 1 pin 25
    BSP_IO_PORT_01_PIN_26 = 0x011A,     ///< IO port 1 pin 26
    BSP_IO_PORT_01_PIN_27 = 0x011B,     ///< IO port 1 pin 27
    BSP_IO_PORT_01_PIN_28 = 0x011C,     ///< IO port 1 pin 28
    BSP_IO_PORT_01_PIN_29 = 0x011D,     ///< IO port 1 pin 29
    BSP_IO_PORT_01_PIN_30 = 0x011E,     ///< IO port 1 pin 30
    BSP_IO_PORT_01_PIN_31 = 0x011F,     ///< IO port 1 pin 31

    BSP_IO_PORT_02_PIN_0  = 0x0200,     ///< IO port 2 pin 0
    BSP_IO_PORT_02_PIN_1  = 0x0201,     ///< IO port 2 pin 1
    BSP_IO_PORT_02_PIN_2  = 0x0202,     ///< IO port 2 pin 2
    BSP_IO_PORT_02_PIN_3  = 0x0203,     ///< IO port 2 pin 3
    BSP_IO_PORT_02_PIN_4  = 0x0204,     ///< IO port 2 pin 4
    BSP_IO_PORT_02_PIN_5  = 0x0205,     ///< IO port 2 pin 5
    BSP_IO_PORT_02_PIN_6  = 0x0206,     ///< IO port 2 pin 6
    BSP_IO_PORT_02_PIN_7  = 0x0207,     ///< IO port 2 pin 7
    BSP_IO_PORT_02_PIN_8  = 0x0208,     ///< IO port 2 pin 8
    BSP_IO_PORT_02_PIN_9  = 0x0209,     ///< IO port 2 pin 9
    BSP_IO_PORT_02_PIN_10 = 0x020A,     ///< IO port 2 pin 10
    BSP_IO_PORT_02_PIN_11 = 0x020B,     ///< IO port 2 pin 11
    BSP_IO_PORT_02_PIN_12 = 0x020C,     ///< IO port 2 pin 12
    BSP_IO_PORT_02_PIN_13 = 0x020D,     ///< IO port 2 pin 13
    BSP_IO_PORT_02_PIN_14 = 0x020E,     ///< IO port 2 pin 14
    BSP_IO_PORT_02_PIN_15 = 0x020F,     ///< IO port 2 pin 15
    BSP_IO_PORT_02_PIN_16 = 0x0210,     ///< IO port 2 pin 16
    BSP_IO_PORT_02_PIN_17 = 0x0211,     ///< IO port 2 pin 17
    BSP_IO_PORT_02_PIN_18 = 0x0212,     ///< IO port 2 pin 18
    BSP_IO_PORT_02_PIN_19 = 0x0213,     ///< IO port 2 pin 19
    BSP_IO_PORT_02_PIN_20 = 0x0214,     ///< IO port 2 pin 20
    BSP_IO_PORT_02_PIN_21 = 0x0215,     ///< IO port 2 pin 21
    BSP_IO_PORT_02_PIN_22 = 0x0216,     ///< IO port 2 pin 22
    BSP_IO_PORT_02_PIN_23 = 0x0217,     ///< IO port 2 pin 23
    BSP_IO_PORT_02_PIN_24 = 0x0218,     ///< IO port 2 pin 24
    BSP_IO_PORT_02_PIN_25 = 0x0219,     ///< IO port 2 pin 25
    BSP_IO_PORT_02_PIN_26 = 0x021A,     ///< IO port 2 pin 26
    BSP_IO_PORT_02_PIN_27 = 0x021B,     ///< IO port 2 pin 27
    BSP_IO_PORT_02_PIN_28 = 0x021C,     ///< IO port 2 pin 28
    BSP_IO_PORT_02_PIN_29 = 0x021D,     ///< IO port 2 pin 29
    BSP_IO_PORT_02_PIN_30 = 0x021E,     ///< IO port 2 pin 30
    BSP_IO_PORT_02_PIN_31 = 0x021F,     ///< IO port 2 pin 31

    BSP_IO_PORT_03_PIN_0  = 0x0300,     ///< IO port 3 pin 0
    BSP_IO_PORT_03_PIN_1  = 0x0301,     ///< IO port 3 pin 1
    BSP_IO_PORT_03_PIN_2  = 0x0302,     ///< IO port 3 pin 2
    BSP_IO_PORT_03_PIN_3  = 0x0303,     ///< IO port 3 pin 3
    BSP_IO_PORT_03_PIN_4  = 0x0304,     ///< IO port 3 pin 4
    BSP_IO_PORT_03_PIN_5  = 0x0305,     ///< IO port 3 pin 5
    BSP_IO_PORT_03_PIN_6  = 0x0306,     ///< IO port 3 pin 6
    BSP_IO_PORT_03_PIN_7  = 0x0307,     ///< IO port 3 pin 7
    BSP_IO_PORT_03_PIN_8  = 0x0308,     ///< IO port 3 pin 8
    BSP_IO_PORT_03_PIN_9  = 0x0309,     ///< IO port 3 pin 9
    BSP_IO_PORT_03_PIN_10 = 0x030A,     ///< IO port 3 pin 10
    BSP_IO_PORT_03_PIN_11 = 0x030B,     ///< IO port 3 pin 11
    BSP_IO_PORT_03_PIN_12 = 0x030C,     ///< IO port 3 pin 12
    BSP_IO_PORT_03_PIN_13 = 0x030D,     ///< IO port 3 pin 13
    BSP_IO_PORT_03_PIN_14 = 0x030E,     ///< IO port 3 pin 14
    BSP_IO_PORT_03_PIN_15 = 0x030F,     ///< IO port 3 pin 15
    BSP_IO_PORT_03_PIN_16 = 0x0310,     ///< IO port 3 pin 16
    BSP_IO_PORT_03_PIN_17 = 0x0311,     ///< IO port 3 pin 17
    BSP_IO_PORT_03_PIN_18 = 0x0312,     ///< IO port 3 pin 18
    BSP_IO_PORT_03_PIN_19 = 0x0313,     ///< IO port 3 pin 19
    BSP_IO_PORT_03_PIN_20 = 0x0314,     ///< IO port 3 pin 20
    BSP_IO_PORT_03_PIN_21 = 0x0315,     ///< IO port 3 pin 21
    BSP_IO_PORT_03_PIN_22 = 0x0316,     ///< IO port 3 pin 22
    BSP_IO_PORT_03_PIN_23 = 0x0317,     ///< IO port 3 pin 23
    BSP_IO_PORT_03_PIN_24 = 0x0318,     ///< IO port 3 pin 24
    BSP_IO_PORT_03_PIN_25 = 0x0319,     ///< IO port 3 pin 25
    BSP_IO_PORT_03_PIN_26 = 0x031A,     ///< IO port 3 pin 26
    BSP_IO_PORT_03_PIN_27 = 0x031B,     ///< IO port 3 pin 27
    BSP_IO_PORT_03_PIN_28 = 0x031C,     ///< IO port 3 pin 28
    BSP_IO_PORT_03_PIN_29 = 0x031D,     ///< IO port 3 pin 29
    BSP_IO_PORT_03_PIN_30 = 0x031E,     ///< IO port 3 pin 30
    BSP_IO_PORT_03_PIN_31 = 0x031F,     ///< IO port 3 pin 31

    BSP_IO_PORT_04_PIN_0  = 0x0400,     ///< IO port 4 pin 0
    BSP_IO_PORT_04_PIN_1  = 0x0401,     ///< IO port 4 pin 1
    BSP_IO_PORT_04_PIN_2  = 0x0402,     ///< IO port 4 pin 2
    BSP_IO_PORT_04_PIN_3  = 0x0403,     ///< IO port 4 pin 3
    BSP_IO_PORT_04_PIN_4  = 0x0404,     ///< IO port 4 pin 4
    BSP_IO_PORT_04_PIN_5  = 0x0405,     ///< IO port 4 pin 5
    BSP_IO_PORT_04_PIN_6  = 0x0406,     ///< IO port 4 pin 6
    BSP_IO_PORT_04_PIN_7  = 0x0407,     ///< IO port 4 pin 7
    BSP_IO_PORT_04_PIN_8  = 0x0408,     ///< IO port 4 pin 8
    BSP_IO_PORT_04_PIN_9  = 0x0409,     ///< IO port 4 pin 9
    BSP_IO_PORT_04_PIN_10 = 0x040A,     ///< IO port 4 pin 10
    BSP_IO_PORT_04_PIN_11 = 0x040B,     ///< IO port 4 pin 11
    BSP_IO_PORT_04_PIN_12 = 0x040C,     ///< IO port 4 pin 12
    BSP_IO_PORT_04_PIN_13 = 0x040D,     ///< IO port 4 pin 13
    BSP_IO_PORT_04_PIN_14 = 0x040E,     ///< IO port 4 pin 14
    BSP_IO_PORT_04_PIN_15 = 0x040F,     ///< IO port 4 pin 15
    BSP_IO_PORT_04_PIN_16 = 0x0410,     ///< IO port 4 pin 16
    BSP_IO_PORT_04_PIN_17 = 0x0411,     ///< IO port 4 pin 17
    BSP_IO_PORT_04_PIN_18 = 0x0412,     ///< IO port 4 pin 18
    BSP_IO_PORT_04_PIN_19 = 0x0413,     ///< IO port 4 pin 19
    BSP_IO_PORT_04_PIN_20 = 0x0414,     ///< IO port 4 pin 20
    BSP_IO_PORT_04_PIN_21 = 0x0415,     ///< IO port 4 pin 21
    BSP_IO_PORT_04_PIN_22 = 0x0416,     ///< IO port 4 pin 22
    BSP_IO_PORT_04_PIN_23 = 0x0417,     ///< IO port 4 pin 23
    BSP_IO_PORT_04_PIN_24 = 0x0418,     ///< IO port 4 pin 24
    BSP_IO_PORT_04_PIN_25 = 0x0419,     ///< IO port 4 pin 25
    BSP_IO_PORT_04_PIN_26 = 0x041A,     ///< IO port 4 pin 26
    BSP_IO_PORT_04_PIN_27 = 0x041B,     ///< IO port 4 pin 27
    BSP_IO_PORT_04_PIN_28 = 0x041C,     ///< IO port 4 pin 28
    BSP_IO_PORT_04_PIN_29 = 0x041D,     ///< IO port 4 pin 29
    BSP_IO_PORT_04_PIN_30 = 0x041E,     ///< IO port 4 pin 30
    BSP_IO_PORT_04_PIN_31 = 0x041F,     ///< IO port 4 pin 31

    BSP_IO_PORT_05_PIN_0  = 0x0500,     ///< IO port 5 pin 0
    BSP_IO_PORT_05_PIN_1  = 0x0501,     ///< IO port 5 pin 1
    BSP_IO_PORT_05_PIN_2  = 0x0502,     ///< IO port 5 pin 2
    BSP_IO_PORT_05_PIN_3  = 0x0503,     ///< IO port 5 pin 3
    BSP_IO_PORT_05_PIN_4  = 0x0504,     ///< IO port 5 pin 4
    BSP_IO_PORT_05_PIN_5  = 0x0505,     ///< IO port 5 pin 5
    BSP_IO_PORT_05_PIN_6  = 0x0506,     ///< IO port 5 pin 6
    BSP_IO_PORT_05_PIN_7  = 0x0507,     ///< IO port 5 pin 7
    BSP_IO_PORT_05_PIN_8  = 0x0508,     ///< IO port 5 pin 8
    BSP_IO_PORT_05_PIN_9  = 0x0509,     ///< IO port 5 pin 9
    BSP_IO_PORT_05_PIN_10 = 0x050A,     ///< IO port 5 pin 10
    BSP_IO_PORT_05_PIN_11 = 0x050B,     ///< IO port 5 pin 11
    BSP_IO_PORT_05_PIN_12 = 0x050C,     ///< IO port 5 pin 12
    BSP_IO_PORT_05_PIN_13 = 0x050D,     ///< IO port 5 pin 13
    BSP_IO_PORT_05_PIN_14 = 0x050E,     ///< IO port 5 pin 14
    BSP_IO_PORT_05_PIN_15 = 0x050F,     ///< IO port 5 pin 15
    BSP_IO_PORT_05_PIN_16 = 0x0510,     ///< IO port 5 pin 16
    BSP_IO_PORT_05_PIN_17 = 0x0511,     ///< IO port 5 pin 17
    BSP_IO_PORT_05_PIN_18 = 0x0512,     ///< IO port 5 pin 18
    BSP_IO_PORT_05_PIN_19 = 0x0513,     ///< IO port 5 pin 19
    BSP_IO_PORT_05_PIN_20 = 0x0514,     ///< IO port 5 pin 20
    BSP_IO_PORT_05_PIN_21 = 0x0515,     ///< IO port 5 pin 21
    BSP_IO_PORT_05_PIN_22 = 0x0516,     ///< IO port 5 pin 22
    BSP_IO_PORT_05_PIN_23 = 0x0517,     ///< IO port 5 pin 23
    BSP_IO_PORT_05_PIN_24 = 0x0518,     ///< IO port 5 pin 24
    BSP_IO_PORT_05_PIN_25 = 0x0519,     ///< IO port 5 pin 25
    BSP_IO_PORT_05_PIN_26 = 0x051A,     ///< IO port 5 pin 26
    BSP_IO_PORT_05_PIN_27 = 0x051B,     ///< IO port 5 pin 27
    BSP_IO_PORT_05_PIN_28 = 0x051C,     ///< IO port 5 pin 28
    BSP_IO_PORT_05_PIN_29 = 0x051D,     ///< IO port 5 pin 29
    BSP_IO_PORT_05_PIN_30 = 0x051E,     ///< IO port 5 pin 30
    BSP_IO_PORT_05_PIN_31 = 0x051F,     ///< IO port 5 pin 31

    BSP_IO_PORT_06_PIN_0  = 0x0600,     ///< IO port 6 pin 0
    BSP_IO_PORT_06_PIN_1  = 0x0601,     ///< IO port 6 pin 1
    BSP_IO_PORT_06_PIN_2  = 0x0602,     ///< IO port 6 pin 2
    BSP_IO_PORT_06_PIN_3  = 0x0603,     ///< IO port 6 pin 3
    BSP_IO_PORT_06_PIN_4  = 0x0604,     ///< IO port 6 pin 4
    BSP_IO_PORT_06_PIN_5  = 0x0605,     ///< IO port 6 pin 5
    BSP_IO_PORT_06_PIN_6  = 0x0606,     ///< IO port 6 pin 6
    BSP_IO_PORT_06_PIN_7  = 0x0607,     ///< IO port 6 pin 7
    BSP_IO_PORT_06_PIN_8  = 0x0608,     ///< IO port 6 pin 8
    BSP_IO_PORT_06_PIN_9  = 0x0609,     ///< IO port 6 pin 9
    BSP_IO_PORT_06_PIN_10 = 0x060A,     ///< IO port 6 pin 10
    BSP_IO_PORT_06_PIN_11 = 0x060B,     ///< IO port 6 pin 11
    BSP_IO_PORT_06_PIN_12 = 0x060C,     ///< IO port 6 pin 12
    BSP_IO_PORT_06_PIN_13 = 0x060D,     ///< IO port 6 pin 13
    BSP_IO_PORT_06_PIN_14 = 0x060E,     ///< IO port 6 pin 14
    BSP_IO_PORT_06_PIN_15 = 0x060F,     ///< IO port 6 pin 15
    BSP_IO_PORT_06_PIN_16 = 0x0610,     ///< IO port 6 pin 16
    BSP_IO_PORT_06_PIN_17 = 0x0611,     ///< IO port 6 pin 17
    BSP_IO_PORT_06_PIN_18 = 0x0612,     ///< IO port 6 pin 18
    BSP_IO_PORT_06_PIN_19 = 0x0613,     ///< IO port 6 pin 19
    BSP_IO_PORT_06_PIN_20 = 0x0614,     ///< IO port 6 pin 20
    BSP_IO_PORT_06_PIN_21 = 0x0615,     ///< IO port 6 pin 21
    BSP_IO_PORT_06_PIN_22 = 0x0616,     ///< IO port 6 pin 22
    BSP_IO_PORT_06_PIN_23 = 0x0617,     ///< IO port 6 pin 23
    BSP_IO_PORT_06_PIN_24 = 0x0618,     ///< IO port 6 pin 24
    BSP_IO_PORT_06_PIN_25 = 0x0619,     ///< IO port 6 pin 25
    BSP_IO_PORT_06_PIN_26 = 0x061A,     ///< IO port 6 pin 26
    BSP_IO_PORT_06_PIN_27 = 0x061B,     ///< IO port 6 pin 27
    BSP_IO_PORT_06_PIN_28 = 0x061C,     ///< IO port 6 pin 28
    BSP_IO_PORT_06_PIN_29 = 0x061D,     ///< IO port 6 pin 29
    BSP_IO_PORT_06_PIN_30 = 0x061E,     ///< IO port 6 pin 30
    BSP_IO_PORT_06_PIN_31 = 0x061F,     ///< IO port 6 pin 31

    BSP_IO_PORT_07_PIN_0  = 0x0700,     ///< IO port 7 pin 0
    BSP_IO_PORT_07_PIN_1  = 0x0701,     ///< IO port 7 pin 1
    BSP_IO_PORT_07_PIN_2  = 0x0702,     ///< IO port 7 pin 2
    BSP_IO_PORT_07_PIN_3  = 0x0703,     ///< IO port 7 pin 3
    BSP_IO_PORT_07_PIN_4  = 0x0704,     ///< IO port 7 pin 4
    BSP_IO_PORT_07_PIN_5  = 0x0705,     ///< IO port 7 pin 5
    BSP_IO_PORT_07_PIN_6  = 0x0706,     ///< IO port 7 pin 6
    BSP_IO_PORT_07_PIN_7  = 0x0707,     ///< IO port 7 pin 7
    BSP_IO_PORT_07_PIN_8  = 0x0708,     ///< IO port 7 pin 8
    BSP_IO_PORT_07_PIN_9  = 0x0709,     ///< IO port 7 pin 9
    BSP_IO_PORT_07_PIN_10 = 0x070A,     ///< IO port 7 pin 10
    BSP_IO_PORT_07_PIN_11 = 0x070B,     ///< IO port 7 pin 11
    BSP_IO_PORT_07_PIN_12 = 0x070C,     ///< IO port 7 pin 12
    BSP_IO_PORT_07_PIN_13 = 0x070D,     ///< IO port 7 pin 13
    BSP_IO_PORT_07_PIN_14 = 0x070E,     ///< IO port 7 pin 14
    BSP_IO_PORT_07_PIN_15 = 0x070F,     ///< IO port 7 pin 15
    BSP_IO_PORT_07_PIN_16 = 0x0710,     ///< IO port 7 pin 16
    BSP_IO_PORT_07_PIN_17 = 0x0711,     ///< IO port 7 pin 17
    BSP_IO_PORT_07_PIN_18 = 0x0712,     ///< IO port 7 pin 18
    BSP_IO_PORT_07_PIN_19 = 0x0713,     ///< IO port 7 pin 19
    BSP_IO_PORT_07_PIN_20 = 0x0714,     ///< IO port 7 pin 20
    BSP_IO_PORT_07_PIN_21 = 0x0715,     ///< IO port 7 pin 21
    BSP_IO_PORT_07_PIN_22 = 0x0716,     ///< IO port 7 pin 22
    BSP_IO_PORT_07_PIN_23 = 0x0717,     ///< IO port 7 pin 23
    BSP_IO_PORT_07_PIN_24 = 0x0718,     ///< IO port 7 pin 24
    BSP_IO_PORT_07_PIN_25 = 0x0719,     ///< IO port 7 pin 25
    BSP_IO_PORT_07_PIN_26 = 0x071A,     ///< IO port 7 pin 26
    BSP_IO_PORT_07_PIN_27 = 0x071B,     ///< IO port 7 pin 27
    BSP_IO_PORT_07_PIN_28 = 0x071C,     ///< IO port 7 pin 28
    BSP_IO_PORT_07_PIN_29 = 0x071D,     ///< IO port 7 pin 29
    BSP_IO_PORT_07_PIN_30 = 0x071E,     ///< IO port 7 pin 30
    BSP_IO_PORT_07_PIN_31 = 0x071F,     ///< IO port 7 pin 31

    BSP_IO_PORT_08_PIN_0  = 0x0800,     ///< IO port 8 pin 0
    BSP_IO_PORT_08_PIN_1  = 0x0801,     ///< IO port 8 pin 1
    BSP_IO_PORT_08_PIN_2  = 0x0802,     ///< IO port 8 pin 2
    BSP_IO_PORT_08_PIN_3  = 0x0803,     ///< IO port 8 pin 3
    BSP_IO_PORT_08_PIN_4  = 0x0804,     ///< IO port 8 pin 4
    BSP_IO_PORT_08_PIN_5  = 0x0805,     ///< IO port 8 pin 5
    BSP_IO_PORT_08_PIN_6  = 0x0806,     ///< IO port 8 pin 6
    BSP_IO_PORT_08_PIN_7  = 0x0807,     ///< IO port 8 pin 7
    BSP_IO_PORT_08_PIN_8  = 0x0808,     ///< IO port 8 pin 8
    BSP_IO_PORT_08_PIN_9  = 0x0809,     ///< IO port 8 pin 9
    BSP_IO_PORT_08_PIN_10 = 0x080A,     ///< IO port 8 pin 10
    BSP_IO_PORT_08_PIN_11 = 0x080B,     ///< IO port 8 pin 11
    BSP_IO_PORT_08_PIN_12 = 0x080C,     ///< IO port 8 pin 12
    BSP_IO_PORT_08_PIN_13 = 0x080D,     ///< IO port 8 pin 13
    BSP_IO_PORT_08_PIN_14 = 0x080E,     ///< IO port 8 pin 14
    BSP_IO_PORT_08_PIN_15 = 0x080F,     ///< IO port 8 pin 15
    BSP_IO_PORT_08_PIN_16 = 0x0810,     ///< IO port 8 pin 16
    BSP_IO_PORT_08_PIN_17 = 0x0811,     ///< IO port 8 pin 17
    BSP_IO_PORT_08_PIN_18 = 0x0812,     ///< IO port 8 pin 18
    BSP_IO_PORT_08_PIN_19 = 0x0813,     ///< IO port 8 pin 19
    BSP_IO_PORT_08_PIN_20 = 0x0814,     ///< IO port 8 pin 20
    BSP_IO_PORT_08_PIN_21 = 0x0815,     ///< IO port 8 pin 21
    BSP_IO_PORT_08_PIN_22 = 0x0816,     ///< IO port 8 pin 22
    BSP_IO_PORT_08_PIN_23 = 0x0817,     ///< IO port 8 pin 23
    BSP_IO_PORT_08_PIN_24 = 0x0818,     ///< IO port 8 pin 24
    BSP_IO_PORT_08_PIN_25 = 0x0819,     ///< IO port 8 pin 25
    BSP_IO_PORT_08_PIN_26 = 0x081A,     ///< IO port 8 pin 26
    BSP_IO_PORT_08_PIN_27 = 0x081B,     ///< IO port 8 pin 27
    BSP_IO_PORT_08_PIN_28 = 0x081C,     ///< IO port 8 pin 28
    BSP_IO_PORT_08_PIN_29 = 0x081D,     ///< IO port 8 pin 29
    BSP_IO_PORT_08_PIN_30 = 0x081E,     ///< IO port 8 pin 30
    BSP_IO_PORT_08_PIN_31 = 0x081F,     ///< IO port 8 pin 31

    BSP_IO_PORT_09_PIN_0  = 0x0900,     ///< IO port 9 pin 0
    BSP_IO_PORT_09_PIN_1  = 0x0901,     ///< IO port 9 pin 1
    BSP_IO_PORT_09_PIN_2  = 0x0902,     ///< IO port 9 pin 2
    BSP_IO_PORT_09_PIN_3  = 0x0903,     ///< IO port 9 pin 3
    BSP_IO_PORT_09_PIN_4  = 0x0904,     ///< IO port 9 pin 4
    BSP_IO_PORT_09_PIN_5  = 0x0905,     ///< IO port 9 pin 5
    BSP_IO_PORT_09_PIN_6  = 0x0906,     ///< IO port 9 pin 6
    BSP_IO_PORT_09_PIN_7  = 0x0907,     ///< IO port 9 pin 7
    BSP_IO_PORT_09_PIN_8  = 0x0908,     ///< IO port 9 pin 8
    BSP_IO_PORT_09_PIN_9  = 0x0909,     ///< IO port 9 pin 9
    BSP_IO_PORT_09_PIN_10 = 0x090A,     ///< IO port 9 pin 10
    BSP_IO_PORT_09_PIN_11 = 0x090B,     ///< IO port 9 pin 11
    BSP_IO_PORT_09_PIN_12 = 0x090C,     ///< IO port 9 pin 12
    BSP_IO_PORT_09_PIN_13 = 0x090D,     ///< IO port 9 pin 13
    BSP_IO_PORT_09_PIN_14 = 0x090E,     ///< IO port 9 pin 14
    BSP_IO_PORT_09_PIN_15 = 0x090F,     ///< IO port 9 pin 15
    BSP_IO_PORT_09_PIN_16 = 0x0910,     ///< IO port 9 pin 16
    BSP_IO_PORT_09_PIN_17 = 0x0911,     ///< IO port 9 pin 17
    BSP_IO_PORT_09_PIN_18 = 0x0912,     ///< IO port 9 pin 18
    BSP_IO_PORT_09_PIN_19 = 0x0913,     ///< IO port 9 pin 19
    BSP_IO_PORT_09_PIN_20 = 0x0914,     ///< IO port 9 pin 20
    BSP_IO_PORT_09_PIN_21 = 0x0915,     ///< IO port 9 pin 21
    BSP_IO_PORT_09_PIN_22 = 0x0916,     ///< IO port 9 pin 22
    BSP_IO_PORT_09_PIN_23 = 0x0917,     ///< IO port 9 pin 23
    BSP_IO_PORT_09_PIN_24 = 0x0918,     ///< IO port 9 pin 24
    BSP_IO_PORT_09_PIN_25 = 0x0919,     ///< IO port 9 pin 25
    BSP_IO_PORT_09_PIN_26 = 0x091A,     ///< IO port 9 pin 26
    BSP_IO_PORT_09_PIN_27 = 0x091B,     ///< IO port 9 pin 27
    BSP_IO_PORT_09_PIN_28 = 0x091C,     ///< IO port 9 pin 28
    BSP_IO_PORT_09_PIN_29 = 0x091D,     ///< IO port 9 pin 29
    BSP_IO_PORT_09_PIN_30 = 0x091E,     ///< IO port 9 pin 30
    BSP_IO_PORT_09_PIN_31 = 0x091F,     ///< IO port 9 pin 31

    BSP_IO_PORT_10_PIN_0  = 0x0A00,     ///< IO port 10 pin 0
    BSP_IO_PORT_10_PIN_1  = 0x0A01,     ///< IO port 10 pin 1
    BSP_IO_PORT_10_PIN_2  = 0x0A02,     ///< IO port 10 pin 2
    BSP_IO_PORT_10_PIN_3  = 0x0A03,     ///< IO port 10 pin 3
    BSP_IO_PORT_10_PIN_4  = 0x0A04,     ///< IO port 10 pin 4
    BSP_IO_PORT_10_PIN_5  = 0x0A05,     ///< IO port 10 pin 5
    BSP_IO_PORT_10_PIN_6  = 0x0A06,     ///< IO port 10 pin 6
    BSP_IO_PORT_10_PIN_7  = 0x0A07,     ///< IO port 10 pin 7
    BSP_IO_PORT_10_PIN_8  = 0x0A08,     ///< IO port 10 pin 8
    BSP_IO_PORT_10_PIN_9  = 0x0A09,     ///< IO port 10 pin 9
    BSP_IO_PORT_10_PIN_10 = 0x0A0A,     ///< IO port 10 pin 10
    BSP_IO_PORT_10_PIN_11 = 0x0A0B,     ///< IO port 10 pin 11
    BSP_IO_PORT_10_PIN_12 = 0x0A0C,     ///< IO port 10 pin 12
    BSP_IO_PORT_10_PIN_13 = 0x0A0D,     ///< IO port 10 pin 13
    BSP_IO_PORT_10_PIN_14 = 0x0A0E,     ///< IO port 10 pin 14
    BSP_IO_PORT_10_PIN_15 = 0x0A0F,     ///< IO port 10 pin 15
    BSP_IO_PORT_10_PIN_16 = 0x0A10,     ///< IO port 10 pin 16
    BSP_IO_PORT_10_PIN_17 = 0x0A11,     ///< IO port 10 pin 17
    BSP_IO_PORT_10_PIN_18 = 0x0A12,     ///< IO port 10 pin 18
    BSP_IO_PORT_10_PIN_19 = 0x0A13,     ///< IO port 10 pin 19
    BSP_IO_PORT_10_PIN_20 = 0x0A14,     ///< IO port 10 pin 20
    BSP_IO_PORT_10_PIN_21 = 0x0A15,     ///< IO port 10 pin 21
    BSP_IO_PORT_10_PIN_22 = 0x0A16,     ///< IO port 10 pin 22
    BSP_IO_PORT_10_PIN_23 = 0x0A17,     ///< IO port 10 pin 23
    BSP_IO_PORT_10_PIN_24 = 0x0A18,     ///< IO port 10 pin 24
    BSP_IO_PORT_10_PIN_25 = 0x0A19,     ///< IO port 10 pin 25
    BSP_IO_PORT_10_PIN_26 = 0x0A1A,     ///< IO port 10 pin 26
    BSP_IO_PORT_10_PIN_27 = 0x0A1B,     ///< IO port 10 pin 27
    BSP_IO_PORT_10_PIN_28 = 0x0A1C,     ///< IO port 10 pin 28
    BSP_IO_PORT_10_PIN_29 = 0x0A1D,     ///< IO port 10 pin 29
    BSP_IO_PORT_10_PIN_30 = 0x0A1E,     ///< IO port 10 pin 30
    BSP_IO_PORT_10_PIN_31 = 0x0A1F,     ///< IO port 10 pin 31
} bsp_io_port_pin_t;

/** Offset for pin safety region access */
typedef enum e_bsp_io_region
{
    BSP_IO_REGION_NOT_SAFE           = 0,                                                 ///< Non safety region
    //BSP_IO_REGION_SAFE               = BSP_FEATURE_BSP_IO_REGION_ADDRESS_DIFF_SELECTABLE, ///< Safety region
    //BSP_IO_REGION_NOT_SAFE_DEDICATED = BSP_FEATURE_BSP_IO_REGION_ADDRESS_DIFF_DEDICATED,  ///< Non safety dedicated region
    BSP_IO_REGION_SAFE               = 1, ///< Safety region
    BSP_IO_REGION_NOT_SAFE_DEDICATED = 2,  ///< Non safety dedicated region
} bsp_io_region_t;

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
extern volatile uint32_t g_protect_port_counter;

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Inline Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Set the output level of the pin in the specified region.
 *
 * @param[in]  region          The target IO region
 * @param[in]  pin             The pin
 **********************************************************************************************************************/
//__STATIC_INLINE void R_BSP_PinSet (bsp_io_region_t region, bsp_io_port_pin_t pin)
//{
//#if BSP_FEATURE_IOPORT_HAS_NONSAFETY_DEDICATED_PORT
//    if (region == BSP_IO_REGION_NOT_SAFE_DEDICATED)
//    {
//        /* Casting to a uint8_t type is valid because only the lower 8 bits of pin(uint16_t) are extracted by masking on
//         * the right side. */
//        R_PORT_NSR->P[pin >> 8] |=
//            (uint8_t) (1UL << (pin & BSP_IO_PRV_8BIT_MASK));
//    }
//    else
//#endif
//    {
//        /* Casting to a uint8_t type is valid because only the lower 8 bits of pin(uint16_t) are extracted by masking on
//         * the right side. */
//        ((R_PORT_COMMON_Type *) (BSP_FEATURE_BSP_IO_SELECTABLE_NON_SAFETY_BASE + region))->P[pin >>
//                                                                                             BSP_IO_PRV_PORT_OFFSET] |=
//            (uint8_t) (1UL << (pin & BSP_IO_PRV_8BIT_MASK));
//    }
//}

/*******************************************************************************************************************//**
 * Clear the output level of the pin in the specified region.
 *
 * @param[in]  region          The target IO region
 * @param[in]  pin             The pin
 **********************************************************************************************************************/
//__STATIC_INLINE void R_BSP_PinClear (bsp_io_region_t region, bsp_io_port_pin_t pin)
//{
//#if BSP_FEATURE_IOPORT_HAS_NONSAFETY_DEDICATED_PORT
//    if (region == BSP_IO_REGION_NOT_SAFE_DEDICATED)
//    {
//        /* Casting to a uint8_t type is valid because only the lower 8 bits of pin(uint16_t) are extracted by masking on
//         * the right side. */
//        R_PORT_NSR->P[pin >> 8] &=
//            (uint8_t) (~(1UL << (pin & BSP_IO_PRV_8BIT_MASK)));
//    }
//    else
//#endif
//    {
//        /* Casting to a uint8_t type is valid because only the lower 8 bits of pin(uint16_t) are extracted by masking on
//         * the right side. */
//        ((R_PORT_COMMON_Type *) (BSP_FEATURE_BSP_IO_SELECTABLE_NON_SAFETY_BASE + region))->P[pin >>
//                                                                                             BSP_IO_PRV_PORT_OFFSET] &=
//            (uint8_t) (~(1UL << (pin & BSP_IO_PRV_8BIT_MASK)));
//    }
//}

/*******************************************************************************************************************//**
 * Toggle the output level of the pin in the specified region.
 *
 * @param[in]  region          The target IO region
 * @param[in]  pin             The pin
 **********************************************************************************************************************/
//__STATIC_INLINE void R_BSP_PinToggle (bsp_io_region_t region, bsp_io_port_pin_t pin)
//{
//#if BSP_FEATURE_IOPORT_HAS_NONSAFETY_DEDICATED_PORT
//    if (region == BSP_IO_REGION_NOT_SAFE_DEDICATED)
//    {
//        /* Casting to a uint8_t type is valid because only the lower 8 bits of pin(uint16_t) are extracted by masking on
//         * the right side. */
//        R_PORT_NSR->P[pin >> 8] ^=
//            (uint8_t) (1UL << (pin & BSP_IO_PRV_8BIT_MASK));
//    }
//    else
//#endif
//    {
//        /* Casting to a uint8_t type is valid because only the lower 8 bits of pin(uint16_t) are extracted by masking on
//         * the right side. */
//        ((R_PORT_COMMON_Type *) (BSP_FEATURE_BSP_IO_SELECTABLE_NON_SAFETY_BASE + region))->P[pin >>
//                                                                                             BSP_IO_PRV_PORT_OFFSET] ^=
//            (uint8_t) (1UL << (pin & BSP_IO_PRV_8BIT_MASK));
//    }
//}

/*******************************************************************************************************************//**
 * Read the input level of the pin in the specified region.
 *
 * @param[in]  region          The target IO region
 * @param[in]  pin             The pin
 *
 * @retval     Current input level
 **********************************************************************************************************************/
//__STATIC_INLINE uint32_t R_BSP_FastPinRead (bsp_io_region_t region, bsp_io_port_pin_t pin)
//{
//#if BSP_FEATURE_IOPORT_HAS_NONSAFETY_DEDICATED_PORT
//    if (region == BSP_IO_REGION_NOT_SAFE_DEDICATED)
//    {
//        return (uint32_t) ((R_PORT_NSR->PIN[pin >> 8]) >>
//                           (pin & BSP_IO_PRV_8BIT_MASK)) & 0x00000001UL;
//    }
//#endif
//
//    return (uint32_t) ((((R_PORT_COMMON_Type *) (BSP_FEATURE_BSP_IO_SELECTABLE_NON_SAFETY_BASE + region))->PIN[pin >>
//                                                                                                               BSP_IO_PRV_PORT_OFFSET
//                        ]) >>
//                       (pin & BSP_IO_PRV_8BIT_MASK)) & 0x00000001UL;
//}

/*******************************************************************************************************************//**
 * Set the output value of the port in the specified region. All pins in the port must be set to the same IO region to
 * use this function.
 *
 * @param[in]  region           The target IO region
 * @param[in]  port             The port
 * @param[in]  set_value        The setting value
 **********************************************************************************************************************/
//__STATIC_INLINE void R_BSP_PortWrite (bsp_io_region_t region, bsp_io_port_t port, uint8_t set_value)
//{
//#if BSP_FEATURE_IOPORT_HAS_NONSAFETY_DEDICATED_PORT
//    if (region == BSP_IO_REGION_NOT_SAFE_DEDICATED)
//    {
//        R_PORT_NSR->P[port >> 8] = set_value;
//    }
//    else
//#endif
//    {
//        ((R_PORT_COMMON_Type *) (BSP_FEATURE_BSP_IO_SELECTABLE_NON_SAFETY_BASE + region))->P[port >>
//                                                                                             BSP_IO_PRV_PORT_OFFSET] =
//            set_value;
//    }
//}

/*******************************************************************************************************************//**
 * Read the input value of the port in the specified region. All pins in the port must be set to the same IO region to
 * use this function.
 *
 * @param[in]  region           The target IO region
 * @param[in]  port             The port
 *
 * @retval     Current input value
 **********************************************************************************************************************/
//__STATIC_INLINE uint32_t R_BSP_PortRead (bsp_io_region_t region, bsp_io_port_t port)
//{
//#if BSP_FEATURE_IOPORT_HAS_NONSAFETY_DEDICATED_PORT
//    if (region == BSP_IO_REGION_NOT_SAFE_DEDICATED)
//    {
//        return (uint32_t) (R_PORT_NSR->PIN[port >> 8]);
//    }
//#endif
//
//    return (uint32_t) (((R_PORT_COMMON_Type *) (BSP_FEATURE_BSP_IO_SELECTABLE_NON_SAFETY_BASE + region))->PIN[port >>
//                                                                                                              BSP_IO_PRV_PORT_OFFSET
//                       ]);
//}
//
///*******************************************************************************************************************//**
// * Enable access to the PFS registers. Uses a reference counter to protect against interrupts that could occur
// * via multiple threads or an ISR re-entering this code.
// **********************************************************************************************************************/
//__STATIC_INLINE void R_BSP_PinAccessEnable (void)
//{
//#if BSP_CFG_PORT_PROTECT
//
//    /** Get the current state of interrupts */
//    FSP_CRITICAL_SECTION_DEFINE;
//    FSP_CRITICAL_SECTION_ENTER;
//
//    /** If this is first entry then allow writing of PFS. */
//    if (0 == g_protect_port_counter)
//    {
//        /** Disable protection using PRCR register. */
//
//        /** When writing to the PRCR register the upper 8-bits must be the correct key. Set lower bits to 0 to
//         * disable writes. */
//        R_RWP_NS->PRCRN = ((R_RWP_NS->PRCRN | BSP_IO_PRV_PRCR_KEY) | BSP_IO_REG_PROTECT_GPIO);
//        R_RWP_S->PRCRS  = ((R_RWP_S->PRCRS | BSP_IO_PRV_PRCR_KEY) | BSP_IO_REG_PROTECT_GPIO);
//    }
//
//    /** Increment the protect counter */
//    g_protect_port_counter++;
//
//    /** Restore the interrupt state */
//    FSP_CRITICAL_SECTION_EXIT;
//#endif
//}
//
///*******************************************************************************************************************//**
// * Disable access to the PFS registers. Uses a reference counter to protect against interrupts that could occur via
// * multiple threads or an ISR re-entering this code.
// **********************************************************************************************************************/
//__STATIC_INLINE void R_BSP_PinAccessDisable (void)
//{
//#if BSP_CFG_PORT_PROTECT
//
//    /** Get the current state of interrupts */
//    FSP_CRITICAL_SECTION_DEFINE;
//    FSP_CRITICAL_SECTION_ENTER;
//
//    /** Is it safe to disable PFS register? */
//    if (0 != g_protect_port_counter)
//    {
//        /* Decrement the protect counter */
//        g_protect_port_counter--;
//    }
//
//    /** Is it safe to disable writing of PFS? */
//    if (0 == g_protect_port_counter)
//    {
//        /** Enable protection using PRCR register. */
//
//        /** When writing to the PRCR register the upper 8-bits must be the correct key. Set lower bits to 0 to
//         * disable writes. */
//        R_RWP_NS->PRCRN = ((R_RWP_NS->PRCRN | BSP_IO_PRV_PRCR_KEY) & (uint16_t) (~BSP_IO_REG_PROTECT_GPIO));
//        R_RWP_S->PRCRS  = ((R_RWP_S->PRCRS | BSP_IO_PRV_PRCR_KEY) & (uint16_t) (~BSP_IO_REG_PROTECT_GPIO));
//    }
//
//    /** Restore the interrupt state */
//    FSP_CRITICAL_SECTION_EXIT;
//#endif
//}
//
/*******************************************************************************************************************//**
 * Read IO region of the pin.
 *
 * @param[in]  pin                   The pin
 *
 * @retval BSP_IO_REGION_SAFE                   IO region of the pin is safety
 * @retval BSP_IO_REGION_NOT_SAFE               IO region of the pin is non-safety
 * @retval BSP_IO_REGION_NOT_SAFE_DEDICATED     IO region of the pin is non-safety (dedicated)
 *
 * This function can be given as an argument to pin/port access functions described below. When used in a function
 * starting with R_BSP_Port, any one pin in the port should be given as an argument to this function.
 * R_BSP_PinSet(), R_BSP_PinClear(), R_BSP_PinToggle(), R_BSP_FastPinRead(), R_BSP_PortWrite(), R_BSP_PortRead()
 *
 * @note This function can be used to get the region of a specified pin, but the overhead should be considered if this
 *       function is executed each time the pin is accessed. When accessing the same pin repeatedly, it is recommended
 *       that the value obtained by this function be held in a variable beforehand, and the value of the variable be
 *       used as the region argument of the pin access function.
 **********************************************************************************************************************/
//__STATIC_INLINE bsp_io_region_t R_BSP_IoRegionGet (bsp_io_port_pin_t pin)
//{
//    uint32_t aselp = 0;
//
//    /* Casting to a uint32_t type is valid because the range of values represented by uint32_t is not over in the
//     * calculation process of the right-hand side. */
//#if BSP_FEATURE_IOPORT_HAS_NONSAFETY_DEDICATED_PORT
//    if (BSP_FEATURE_BSP_IO_SELECTABLE_NON_SAFETY_PORT > (uint32_t) (pin >> BSP_IO_PRV_PORT_OFFSET))
//    {
//        aselp =
//            ((uint32_t) ((R_PORT_SRS->RSELP[pin >> BSP_IO_PRV_PORT_OFFSET]) >> (pin & BSP_IO_PRV_8BIT_MASK)) &
//             0x00000001UL);
//    }
//    else
//    {
//        aselp = (uint32_t) BSP_IO_REGION_NOT_SAFE_DEDICATED;
//    }
//#else
//    aselp =
//        ((uint32_t) ((R_PTADR->RSELP[pin >> BSP_IO_PRV_PORT_OFFSET]) >> (pin & BSP_IO_PRV_8BIT_MASK)) & 0x00000001UL);
//#endif
//
//    if (0U == aselp)
//    {
//        return BSP_IO_REGION_SAFE;
//    }
//    else if (1U == aselp)
//    {
//        return BSP_IO_REGION_NOT_SAFE;
//    }
//    else
//    {
//        return BSP_IO_REGION_NOT_SAFE_DEDICATED;
//    }
//}

/** @} (end addtogroup BSP_IO) */

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
