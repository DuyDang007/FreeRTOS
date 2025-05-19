/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef __SERIAL_H__
#define __SERIAL_H__

/**
 * @defgroup Serial_Module Serial Module
 * @{
 * @brief This module provides functions to configure and control Serial.
 *
 * The Serial module allows for the configuration and control serial devices.
 * It provides functions to init port, put string, get char, put char and close.
 */
/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#ifndef UART_ID
#if (BOARD == X5H_VDK)
#define UART_ID SCIF0
#else
#define UART_ID HSCIF0
#endif // BOARD
#endif // UART_ID

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 * @brief Serial devices.
 */
typedef enum e_serial_devices {
    SCIF0,                 ///< SCIF channel 0.
    SCIF1,                 ///< SCIF channel 1.
    SCIF2_UNSUPPORTED,     ///< SCIF channel 2.
    SCIF3,                 ///< SCIF channel 3.
    SCIF4,                 ///< SCIF channel 4.
    HSCIF0,                ///< HSCIF channel 0.
    HSCIF1,                ///< HSCIF channel 1.
    HSCIF2,                ///< HSCIF channel 2.
    HSCIF3,                ///< HSCIF channel 3.
} e_serial_devices_t;

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
/**
 * @brief Port initialize.
 *
 * @param[in]  device - Serial channel.
 *
 */
void R_SERIAL_PortInit(e_serial_devices_t device);

/**
 * @brief Send string.
 *
 * @param[in]  buffer - Input string.
 * @param[in]  length - Length string.
 *
 */
void R_SERIAL_PutString(const unsigned char * buffer, unsigned short length);

/**
 * @brief Receive char.
 *
 * @param[in]  recv_char - Output char.
 *
 * @retval 0 if successful.
 *
 */
int32_t R_SERIAL_GetChar(unsigned char * recv_char);

/**
 * @brief Send char.
 *
 * @param[in]  send_char - Input char.
 *
 * @retval 0 if successful.
 *
 */
int32_t R_SERIAL_PutChar(unsigned char send_char);

/**
 * @brief Serial close.
 *
 */
void R_SERIAL_Close();

/** @} */ // end of Serial_Module

#endif	/* __SERIAL_H__ */
