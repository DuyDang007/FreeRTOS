/**************************************************************************//**
 * @file     cmsis_rcar_gen3.h
 * @brief    CMSIS for Cortex-R7 on Renesas R-Car Gen3 devices
 ******************************************************************************/
/*
 * Copyright (c) 2019 Renesas Electronics Europe Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * ARM don't currently provide CMSIS for Cortex-R cores.
 * We want the code for CP15 access, instructions and memory barriers so we can
 * setup the caches, etc.
 * We also want to program the MPU, whihc is a bit similar to Cortex-M devices,
 * so we have modified CMSIS code to do this.
 */
#ifndef __CMSIS_RCAR_GEN3_H
#define __CMSIS_RCAR_GEN3_H

/* FPU */
#define __FPU_PRESENT           1

/* MPU */
#define __MPU_PRESENT           1

/* GIC */
#define __GIC_PRESENT           1
#define GIC_DISTRIBUTOR_BASE	0xF1110000U
#define GIC_INTERFACE_BASE	0xF1120000U
#define IRQ_GIC_LINE_COUNT	511
/* Not going to list all the interrupts */
typedef	unsigned int IRQn_Type;

#include "core_ca.h"
#include "core_cr7.h"

#endif /* __CMSIS_RCAR_GEN3_H */

