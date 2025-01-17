/*************************************************************************************************************
* i2c_api_h
* Copyright (c) [2021-2022] Renesas Electronics Corporation
* This software is released under the MIT License
* http://opensource.org/licenses/mit-license.php
* SPDX-License-Identifier: MIT
*************************************************************************************************************/

#ifndef R_I2C_API_H_
#define R_I2C_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * \defgroup  I2CMaingroup   I2C related drivers
 * @brief I2C driver interface and related decoder drivers
 */

/**
 * \defgroup I2Cgroup I2C driver API
 * @ingroup  I2CMaingroup
 * @brief   I2C driver interface
 * @{
 */

/**
 * List of I2C channels
 */
typedef enum {
    R_I2C_IF0 = 0,  /**< channel 0 */
    R_I2C_IF1,      /**< channel 1 */
    R_I2C_IF2,      /**< channel 2 */
    R_I2C_IF3,      /**< channel 3 */
    R_I2C_IF4,      /**< channel 4 */
    R_I2C_IF5,      /**< channel 5 */
    R_I2C_IF6,      /**< channel 6 */
    R_I2C_IF7,      /**< channel 7 */
    R_I2C_IF8,      /**< channel 8 */
    R_I2C_LAST      /**< delimiter */
} r_i2c_Unit_t;

/**
 * Transmit data through the given I2C channel
 *
 * @param[in] Unit      - I2C unit number, see @ref r_i2c_Unit_t
 * @param[in] SlaveAddr - Slave address
 * @param[in] Bytes     - Pointer to an array of bytes values to transmitted.(must not be NULL)
 * @param[in] NumBytes  - Number of bytes in the array
 *
 * @retval 0 if successful
 *
 */
uint32_t R_I2C_Write(r_i2c_Unit_t Unit, uint32_t SlaveAddr, const uint8_t *Bytes,
                     uint32_t NumBytes);

/**
 * Read a number of bytes from an address of the given I2C channel
 *
 * @param[in]  Unit      - I2C unit number, see @ref r_i2c_Unit_t
 * @param[in]  SlaveAddr - Slave address
 * @param[out] Bytes     - Pointer to an array of bytes values. Received bytes will be stored there.(must not be NULL)
 * @param[in]  NumBytes  - Number of bytes to be read
 *
 * @retval 0 if successful
 *
 */
uint32_t R_I2C_Read(r_i2c_Unit_t Unit, uint32_t SlaveAddr, uint8_t *Bytes,
                    uint32_t NumBytes);

/**
 * Configure the slave map in read mode and read the number of bytes required
 *
 * @param[in]  Unit      - I2C unit number, see @ref r_i2c_Unit_t
 * @param[in]  SlaveAddr - Slave base address
 * @param[in]  SlaveReg  - Salve register offset
 * @param[out] Bytes     - Pointer to an array of bytes values. Received bytes will be stored there.(must not be NULL)
 * @param[in]  NumBytes  - Number of bytes to be read
 *
 * @retval 0 if successful
 */
uint32_t R_I2C_ReadRegMap(r_i2c_Unit_t Unit, uint32_t SlaveAddr, uint32_t SlaveReg,
                          uint8_t *Bytes, uint32_t NumBytes);

/** @} */ /* end of group I2Cgroup */

#ifdef __cplusplus
}
#endif

#endif /* R_I2C_API_H_ */
