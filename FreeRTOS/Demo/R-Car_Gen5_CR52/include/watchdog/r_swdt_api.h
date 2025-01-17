/*************************************************************************************************************
* Copyright (c) [2025] Renesas Electronics Corporation
* This software is released under the MIT License
* http://opensource.org/licenses/mit-license.php
* SPDX-License-Identifier: MIT
*************************************************************************************************************/
#ifndef _R_SWDT_API_H
#define _R_SWDT_API_H_

#include <stdint.h>

/**
 * Initialize the Watchdog Timer (WDT) with a specified timeout.
 *
 * @param[in] timeout_sec - Timeout value in seconds. This value will be used to set the
 *                          timeout period for the watchdog timer.
 *
 * @retval 0 if successful, or a non-zero value if an error occurs.
 *
 * This function initializes the watchdog timer with a timeout value in seconds,
 * converting it to the corresponding register value for the WDT.
 */
uint8_t R_SWDT_Init(uint8_t timeout_sec);

/**
 * Reset the watchdog timer with a new timeout and specified number of pings.
 *
 * @param[in] timeout_new_sec - Timeout value in seconds.
 *
 * @retval 0 if successful.
 *
 * This function resets the watchdog timer with the specified timeout and
 * triggers the reset operation the specified number of times.
 */

uint8_t R_SWDT_Ping(uint8_t timeout_sec);

/**
 * Start the watchdog timer.
 *
 * @retval 0 if successful.
 *
 * This function starts the watchdog timer, enabling it to monitor the system
 * and trigger a reset if the timeout expires.
 */
uint32_t R_SWDT_Start();

/**
 * Stop the watchdog timer.
 *
 * @retval 0 if successful.
 *
 * This function stops the watchdog timer, disabling it from monitoring the
 * system and preventing it from triggering a reset.
 */
uint32_t R_SWDT_Stop();

#endif /* R_SWDT_API_H_ */
