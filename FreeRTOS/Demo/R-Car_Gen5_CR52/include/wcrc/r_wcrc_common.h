/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef R_WCRC_COMMON_H
#define R_WCRC_COMMON_H

/**
 * @defgroup CRC_Module_Common CRC Module Common typedef
 * @{
 * @brief This module provides typedef to configure CRC.
 *
 * The CRC module contains 11 units (0 to 10):
 *      Each unit is equal 1 CRC wrapper (WCRC). 
 *      1 WCRC contains 1 CRC sub-module and 1 KCRC sub-module.
 */

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** 
 * @brief Superset list of all possible WCRC.
 */
typedef enum e_wcrc_unit
{
    WCRC_00 = 0x00,                             ///< WCRC unit 00
    WCRC_01 = 0x01,                             ///< WCRC unit 01
    WCRC_02 = 0x02,                             ///< WCRC unit 02
    WCRC_03 = 0x03,                             ///< WCRC unit 03
    WCRC_04 = 0x04,                             ///< WCRC unit 04
    WCRC_05 = 0x05,                             ///< WCRC unit 05
    WCRC_06 = 0x06,                             ///< WCRC unit 06
    WCRC_07 = 0x07,                             ///< WCRC unit 07
    WCRC_08 = 0x08,                             ///< WCRC unit 08
    WCRC_09 = 0x09,                             ///< WCRC unit 09
    WCRC_10 = 0x0A,                             ///< WCRC unit 10
} wcrc_unit_t;

/** 
 * @brief Superset list of all RT-DMA instance uses with WCRC.
 */
typedef enum e_rtdma_inst
{
    RTDMA0_CH0  = 0x01,                          ///< RTDMA unit 0 - channel 0
    RTDMA0_CH1  = 0x02,                          ///< RTDMA unit 0 - channel 1
    RTDMA0_CH2  = 0x03,                          ///< RTDMA unit 0 - channel 2
    RTDMA0_CH3  = 0x04,                          ///< RTDMA unit 0 - channel 3
    RTDMA0_CH4  = 0x05,                          ///< RTDMA unit 0 - channel 4
    RTDMA0_CH5  = 0x06,                          ///< RTDMA unit 0 - channel 5
    RTDMA0_CH6  = 0x07,                          ///< RTDMA unit 0 - channel 6
    RTDMA0_CH7  = 0x08,                          ///< RTDMA unit 0 - channel 7
    RTDMA0_CH8  = 0x09,                          ///< RTDMA unit 0 - channel 8
    RTDMA0_CH9  = 0x0a,                          ///< RTDMA unit 0 - channel 9
    RTDMA0_CH10 = 0x0b,                          ///< RTDMA unit 0 - channel 10
    RTDMA0_CH11 = 0x0c,                          ///< RTDMA unit 0 - channel 11
    RTDMA0_CH12 = 0x0d,                          ///< RTDMA unit 0 - channel 12
    RTDMA0_CH13 = 0x0e,                          ///< RTDMA unit 0 - channel 13
    RTDMA0_CH14 = 0x0f,                          ///< RTDMA unit 0 - channel 14
    RTDMA0_CH15 = 0x10,                          ///< RTDMA unit 0 - channel 15
    RTDMA1_CH0  = 0x11,                          ///< RTDMA unit 1 - channel 0
    RTDMA1_CH1  = 0x12,                          ///< RTDMA unit 1 - channel 1
    RTDMA1_CH2  = 0x13,                          ///< RTDMA unit 1 - channel 2
    RTDMA1_CH3  = 0x14,                          ///< RTDMA unit 1 - channel 3
    RTDMA1_CH4  = 0x15,                          ///< RTDMA unit 1 - channel 4
    RTDMA1_CH5  = 0x16,                          ///< RTDMA unit 1 - channel 5
    RTDMA1_CH6  = 0x17,                          ///< RTDMA unit 1 - channel 6
    RTDMA1_CH7  = 0x18,                          ///< RTDMA unit 1 - channel 7
    RTDMA1_CH8  = 0x19,                          ///< RTDMA unit 1 - channel 8
    RTDMA1_CH9  = 0x1a,                          ///< RTDMA unit 1 - channel 9
    RTDMA1_CH10 = 0x1b,                          ///< RTDMA unit 1 - channel 10
    RTDMA1_CH11 = 0x1c,                          ///< RTDMA unit 1 - channel 11
    RTDMA1_CH12 = 0x1d,                          ///< RTDMA unit 1 - channel 12
    RTDMA1_CH13 = 0x1e,                          ///< RTDMA unit 1 - channel 13
    RTDMA1_CH14 = 0x1f,                          ///< RTDMA unit 1 - channel 14
    RTDMA1_CH15 = 0x20,                          ///< RTDMA unit 1 - channel 15
    RTDMA2_CH0  = 0x21,                          ///< RTDMA unit 2 - channel 0
    RTDMA2_CH1  = 0x22,                          ///< RTDMA unit 2 - channel 1
    RTDMA2_CH2  = 0x23,                          ///< RTDMA unit 2 - channel 2
    RTDMA2_CH3  = 0x24,                          ///< RTDMA unit 2 - channel 3
    RTDMA2_CH4  = 0x25,                          ///< RTDMA unit 2 - channel 4
    RTDMA2_CH5  = 0x26,                          ///< RTDMA unit 2 - channel 5
    RTDMA2_CH6  = 0x27,                          ///< RTDMA unit 2 - channel 6
    RTDMA2_CH7  = 0x28,                          ///< RTDMA unit 2 - channel 7
    RTDMA2_CH8  = 0x29,                          ///< RTDMA unit 2 - channel 8
    RTDMA2_CH9  = 0x2a,                          ///< RTDMA unit 2 - channel 9
    RTDMA2_CH10 = 0x2b,                          ///< RTDMA unit 2 - channel 10
    RTDMA2_CH11 = 0x2c,                          ///< RTDMA unit 2 - channel 11
    RTDMA2_CH12 = 0x2d,                          ///< RTDMA unit 2 - channel 12
    RTDMA2_CH13 = 0x2e,                          ///< RTDMA unit 2 - channel 13
    RTDMA2_CH14 = 0x2f,                          ///< RTDMA unit 2 - channel 14
    RTDMA2_CH15 = 0x30,                          ///< RTDMA unit 2 - channel 15
    RTDMA3_CH0  = 0x31,                          ///< RTDMA unit 3 - channel 0
    RTDMA3_CH1  = 0x32,                          ///< RTDMA unit 3 - channel 1
    RTDMA3_CH2  = 0x33,                          ///< RTDMA unit 3 - channel 2
    RTDMA3_CH3  = 0x34,                          ///< RTDMA unit 3 - channel 3
    RTDMA3_CH4  = 0x35,                          ///< RTDMA unit 3 - channel 4
    RTDMA3_CH5  = 0x36,                          ///< RTDMA unit 3 - channel 5
    RTDMA3_CH6  = 0x37,                          ///< RTDMA unit 3 - channel 6
    RTDMA3_CH7  = 0x38,                          ///< RTDMA unit 3 - channel 7
    RTDMA3_CH8  = 0x39,                          ///< RTDMA unit 3 - channel 8
    RTDMA3_CH9  = 0x3a,                          ///< RTDMA unit 3 - channel 9
    RTDMA3_CH10 = 0x3b,                          ///< RTDMA unit 3 - channel 10
    RTDMA3_CH11 = 0x3c,                          ///< RTDMA unit 3 - channel 11
    RTDMA3_CH12 = 0x3d,                          ///< RTDMA unit 3 - channel 12
    RTDMA3_CH13 = 0x3e,                          ///< RTDMA unit 3 - channel 13
    RTDMA3_CH14 = 0x3f,                          ///< RTDMA unit 3 - channel 14
    RTDMA3_CH15 = 0x40,                          ///< RTDMA unit 3 - channel 15
} rtdma_inst_t;

/** 
 * @brief Enumeration for WCRC modes 
 */
typedef enum e_wcrc_mode
{
    INDEPENDENT_CRC_MODE = 0,                   ///< Independent CRC mode.
    E2E_CRC_MODE,                               ///< E2E CRC mode.
    DATA_THROUGH_MODE,                          ///< Data through mode.
    E2E_DATA_THROUGH_MODE,                      ///< E2E and data through mode.
    REGISTER_ACCESS_BY_CMD_MODE,                ///< Register access by command function.
    COMPARING_CRC_RESULT_MODE,                  ///< Comparing CRC result.
} wcrc_mode_t;

/* Polynomial modes */
typedef enum e_wcrc_poly
{
    POLY_32_ETHERNET = 7,
    POLY_16_CCITT_FALSE_CRC16,
    POLY_8_SAE_J1850,
    POLY_8_0X2F,
    POLY_32_0XF4ACFB13,
    POLY_32_0X1EDC6F41,
    POLY_21_0X102899,
    POLY_17_0X1685B,
    POLY_15_0X4599
} wcrc_poly_t;

/** 
 * @brief Enumeration for WCRC to use sub-modules.
 */
typedef enum e_wcrc_sub_module
{
    CRC_SUB_MODULE = 0,                         ///< CRC sub module.
    KCRC_SUB_MODULE,                            ///< KCRC sub module.
    CRC_KCRC_SUB_MODULE,                        ///< Both: CRC and KCRC sub modules.
} wcrc_sub_module_t;

/** 
 * @brief Control structure for CRC operations.
 */
typedef void wcrc_ctrl_t;

/** 
 * @brief User configures KCRC poly size, used in open function.
 */
typedef enum e_kcrc_poly_size
{
    POLY_SIZE_32_BIT = 0,                       ///< KCRC poly size: 32 Bits.
    POLY_SIZE_16_BIT,                           ///< KCRC poly size: 16 Bits.
    POLY_SIZE_8_BIT                             ///< KCRC poly size:  8 Bits.
} kcrc_poly_size_t;

/** 
 * @brief User configures CRC/KCRC width of data input, used in open function.
 */
typedef enum e_width_input
{
    WIDTH_32_BIT = 0,                           ///< KCRC poly size: 32 Bits.
    WIDTH_16_BIT,                               ///< KCRC poly size: 16 Bits.
    WIDTH_8_BIT                                 ///< KCRC poly size:  8 Bits.
} width_input_t;

/** 
 * @brief Structure for CRC inputs.
 */
typedef struct st_crc_input
{
    const void *    p_input_buffer;             ///< Pointer to input buffer.
    uint32_t        num_data;                   ///< Number data input.
    uint32_t        crc_seed;                   ///< Initial CRC value.
    width_input_t   bit_width;                  ///< The valid bit width of each input data.
} crc_input_t;

/** 
 * @brief Structure for CRC outputs.
 */
typedef struct st_crc_output
{
    bool            is_done;                    ///< CRC generation is done or not.
    uint32_t        num_data;                   ///< Number data output.
    void    *       p_output_buffer;            ///< Pointer to output buffer.
} crc_output_t;

/** 
 * @brief User configures CRC byte swap mode, used in open function.
 */
typedef enum e_byte_swap_mode
{
    BYTE_SWAP_00 = 0,                           ///< Mode byteswapp_00.
    BYTE_SWAP_01,                               ///< Mode byteswapp_01.
    BYTE_SWAP_10,                               ///< Mode byteswapp_10.
    BYTE_SWAP_11,                               ///< Mode byteswapp_11.
} byteswap_mode_t;

/** 
 * @brief User configures CRC sub-module, used in open function.
 */
typedef struct st_crc_module_cfg
{
    crc_input_t     input_cfg;                  ///< CRC input info.
    wcrc_poly_t     poly;                       ///< CRC poly.
    bool            is_out_exor;                ///< EXOR ON of output data: H’FFFF FFFF ^ data_out[31:0].
    bool            is_out_bitswap;             ///< Bit swap of output data.
    byteswap_mode_t out_byteswap;               ///< Byte swap of output data.
    bool            is_in_exor;                 ///< EXOR ON of input data:  H’FFFF FFFF ^ data_int[31:0].
    bool            is_in_bitswap;              ///< Bit swap of input data.
    byteswap_mode_t in_byteswap;                ///< Byte swap of input data.
    uint32_t *      p_rtdma_inst;               ///< Pointer to RT-DMA instance.
    uint8_t         num_rtdma_inst;             ///< Number of RT-DMA instances.
} crc_module_cfg_t;

/** 
 * @brief User configures KCRC shift mode, used in open function.
 */
typedef enum e_kcrc_shift_mode
{
    MSB_SHIFT = 0,                              ///< KCRC CMD2: Mode M.
    LSB_SHIFT                                   ///< KCRC CMD2: Mode L.
} kcrc_shift_mode_t;

/** 
 * @brief User configures KCRC sub-module, used in open function.
 */
typedef struct st_kcrc_module_cfg
{
    crc_input_t         input_cfg;              ///< KCRC input info.
    wcrc_poly_t         poly;                   ///< KCRC poly.
    kcrc_poly_size_t    poly_size;              ///< KCRC poly size.
    bool                is_out_reflect;         ///< Output is flipped bit by bit.
    bool                is_in_reflect;          ///< Input is flipped bit by bit.
    uint8_t             shift_mode;             ///< MSB shift or LSB shift.
    uint32_t            xor_mask_out;           ///< Value XOR mask for Data output.
    uint32_t *          p_rtdma_inst;           ///< Pointer to RT-DMA instance.
    uint8_t             num_rtdma_inst;         ///< Number of RT-DMA instances.
} kcrc_module_cfg_t;

/** 
 * @brief User configures WCRC module, used in open function.
 */
typedef struct st_wcrc_cfg
{
    wcrc_unit_t         unit;                   ///< WCRC unit.
    wcrc_mode_t         mode;                   ///< WCRC mode.
    uint32_t            conv_size[2];           ///< Conversion size for E2E_* mode
                                                ///< conv_size[0] is used for CRC module.
                                                ///< conv_size[1] is used for KCRC module.
    wcrc_sub_module_t   sub_module;             ///< WCRC chooses sub-module.
    crc_module_cfg_t    crc_cfg;                ///< CRC sub-module configuration.
    kcrc_module_cfg_t   kcrc_cfg;               ///< KCRC sub-module configuration.
} wcrc_cfg_t;

/** 
 * @brief Driver instance control structure.
 */
typedef struct st_wcrc_instance_ctrl
{
    uint32_t            open;                   ///< Mark instance is opened.
    wcrc_cfg_t const *  p_cfg;                  ///< Pointer to initial configurations.
    crc_output_t        crc_data[2];            ///< crc_data[0] is used for CRC module.
                                                ///< crc_data[1] is used for KCRC module.
    void            (*  p_callback[2])(void *); ///< Pointer to the CRC/KCRC callback functions.
    void             *  p_context[2];           ///< Pointer to context to be passed into callback.
    void             *  p_extend[2];            ///< Pointer to extend hardware configurations.
} wcrc_instance_ctrl_t;

#ifdef __cplusplus
}
#endif

/** @} */ // end of CRC_Module

#endif // R_WCRC_COMMON_H
