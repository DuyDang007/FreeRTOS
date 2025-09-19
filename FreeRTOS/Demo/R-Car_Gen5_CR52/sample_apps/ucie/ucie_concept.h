/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef R_UCIE_CONCEPT_H_
#define R_UCIE_CONCEPT_H_

#define UCIE_LOOPCHECK_ADDR       0x90000000

/**
 *  @brief Structure to hold data to detect transfer done. 
 */
typedef struct st_ucie_trigger_sig {
    uint64_t addr;          /**< Destination transfer address >*/
    uint32_t size;          /**< Transfer size >*/
    uint32_t flag;          /**< Transfer flag >*/
} st_ucie_trigger_sig_t;

#endif /* R_UCIE_CONCEPT_H_ */
