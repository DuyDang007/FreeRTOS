/**************************************************************************//**
 * @file     irq_ctrl_gic.c
 * @brief    Interrupt controller handling implementation for GIC
 * @version  V1.0.1
 * @date     9. April 2018
 ******************************************************************************/
/*
 * Copyright (c) 2017 ARM Limited. All rights reserved.
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

#include <stddef.h>

//#include "RTE_Components.h"
//#include CMSIS_device_header
#include "cmsis_rcar_gen3.h"

#include "irq_ctrl.h"

#if defined(__GIC_PRESENT) && (__GIC_PRESENT == 1U)

/// Number of implemented interrupt lines
#ifndef IRQ_GIC_LINE_COUNT
#define IRQ_GIC_LINE_COUNT      (1020U)
#endif

static uint32_t     IRQ_ID0;

/// Configure interrupt request mode.
__WEAK int32_t IRQ_SetMode (GICDistributor_Type *GICDistributor, IRQn_ID_t irqn, uint32_t mode) {
  uint32_t val;
  uint8_t cfg;
  uint8_t secure;
  uint8_t cpu;
  int32_t status = 0;

  if ((irqn >= 0) && (irqn < (IRQn_ID_t)IRQ_GIC_LINE_COUNT)) {
    // Check triggering mode
    val = (mode & IRQ_MODE_TRIG_Msk);

    if (val == IRQ_MODE_TRIG_LEVEL) {
      cfg = 0x00U;
    } else if (val == IRQ_MODE_TRIG_EDGE) {
      cfg = 0x02U;
    } else {
      cfg = 0x00U;
      status = -1;
    }

    // Check interrupt type
    val = mode & IRQ_MODE_TYPE_Msk;

    if (val != IRQ_MODE_TYPE_IRQ) {
      status = -1;
    }

    // Check interrupt domain
    val = mode & IRQ_MODE_DOMAIN_Msk;

    if (val == IRQ_MODE_DOMAIN_NONSECURE) {
      secure = 0U;
    } else {
      // Check security extensions support
      val = GIC_DistributorInfo(GICDistributor) & (1UL << 10U);

      if (val != 0U) {
        // Security extensions are supported
        secure = 1U;
      } else {
        secure = 0U;
        status = -1;
      }
    }

    // Check interrupt CPU targets
    val = mode & IRQ_MODE_CPU_Msk;

    if (val == IRQ_MODE_CPU_ALL) {
      cpu = 0xFFU;
    } else {
      cpu = val >> IRQ_MODE_CPU_Pos;
    }

    // Apply configuration if no mode error
    if (status == 0) {
      GIC_SetConfiguration(GICDistributor, (IRQn_Type)irqn, cfg);
      GIC_SetTarget       (GICDistributor, (IRQn_Type)irqn, cpu);

      if (secure != 0U) {
        GIC_SetGroup (GICDistributor, (IRQn_Type)irqn, secure);
      }
    }
  }

  return (status);
}


/// Get ID number of current interrupt request (IRQ).
__WEAK IRQn_ID_t IRQ_GetActiveIRQ (GICDistributor_Type *GICDistributor, GICInterface_Type *GICInterface) {
  IRQn_ID_t irqn;
  uint32_t prio;

  /* Dummy read to avoid GIC 390 errata 801120 */
  GIC_GetHighPendingIRQ(GICInterface);

  irqn = GIC_AcknowledgePending(GICInterface);

  __DSB();

  /* Workaround GIC 390 errata 733075 (GIC-390_Errata_Notice_v6.pdf, 09-Jul-2014)  */
  /* The following workaround code is for a single-core system.  It would be       */
  /* different in a multi-core system.                                             */
  /* If the ID is 0 or 0x3FE or 0x3FF, then the GIC CPU interface may be locked-up */
  /* so unlock it, otherwise service the interrupt as normal.                      */
  /* Special IDs 1020=0x3FC and 1021=0x3FD are reserved values in GICv1 and GICv2  */
  /* so will not occur here.                                                       */

  if ((irqn == 0) || (irqn >= 0x3FE)) {
    /* Unlock the CPU interface with a dummy write to Interrupt Priority Register */
    prio = GIC_GetPriority(GICDistributor, (IRQn_Type)0);
    GIC_SetPriority (GICDistributor, (IRQn_Type)0, prio);

    __DSB();

    if ((irqn == 0U) && ((GIC_GetIRQStatus (GICDistributor, (IRQn_Type)irqn) & 1U) != 0U) && (IRQ_ID0 == 0U)) {
      /* If the ID is 0, is active and has not been seen before */
      IRQ_ID0 = 1U;
    }
    /* End of Workaround GIC 390 errata 733075 */
  }

  return (irqn);
}

#endif
