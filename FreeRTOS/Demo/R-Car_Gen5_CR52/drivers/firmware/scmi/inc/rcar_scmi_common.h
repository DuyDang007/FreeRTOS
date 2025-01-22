/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef __RCAR_SCMI_COMMON_H_
#define __RCAR_SCMI_COMMON_H_

/* Describe R-Car X5H Specific transport using shared memory
 * and MFIS Mailbox 
 */

/* Shared memory address */
#define X5H_SCMI_SHMEM_BASE_ADDR       (0xC1000000U)
#define X5H_SCMI_SHMEM_PLATFORM_CR52   (X5H_SCMI_SHMEM_BASE_ADDR + (0x60200U))
#define X5H_SCMI_SHMEM_AGENT_CR52      (X5H_SCMI_SHMEM_BASE_ADDR + (0x60500U))

/* Shared memory size */
#define X5H_SCMI_SHMEM_SIZE         (256U)

/* MFIS SCP Base Address */
#define X5H_MFIS_SCP_BASE           (0x18840000U)
/* MFIS SCP COMMON Base Address */
#define X5H_MFIS_SCP_COMMON_BASE    (0x189E1000U)

/* MFIS Register access key */
#define X5H_MFIS_SCP_KEY_CODE           (0xACC00000U)

/* MFIS Register disable write protect */
#define X5H_MFIS_SCP_DISABLE_MFIS_WRITE_PROTECTION \
    (X5H_MFIS_SCP_KEY_CODE | 0x00000001U)

/* MFIS IRQ register source bits for interrupts generated (15-1bit used) */
#define X5H_MFIS_SCP_IRQ_REG_SOURCE(val)    ((uint32_t)(0x00007FFFU & (val)) << 1)

/* MFIS IRQ register internal interrupt request bit (0bit used) */
#define X5H_MFIS_SCP_IRQ_REG_INT(n)         (0x00000001U & (n))

/* Realtime Core[m](m=0-11) for CR52 Agent */
#define X5H_MFIS_SCP_IRQ_RTCORE_CR52        (0U)

/* MFIS IRQ register mask bits (31-16bit unused) */
#define X5H_MFIS_SCP_IRQ_REG_MASK           (0x0000FFFFU)

#endif /* __RCAR_SCMI_COMMON_H_ */

