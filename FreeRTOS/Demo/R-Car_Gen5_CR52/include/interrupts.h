/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

/*
 * CMSIS GIC wrapper
 *
 * The CMSIS GIC code does most of what we need, with the exception of
 * registering a context that is used in the interrupt callback.
 *
 * This wrapper also allows us to set the default properties for interrupts
 * and do any device specifc initialisation.
 */

#ifndef __INTERRUPTS_H_
#define __INTERRUPTS_H_

#define __FPU_PRESENT             1

#include <stdint.h>

#define R_OS_BSP_GENERIC_ARM_TIMER_IRQNUM 30
#define HSCIF_INT_ID	623
#define RESERVED_PRIORITY_BIT 3
#define MAX_PRIORITY_VALUE 31
#define IPRIORITY(x) (((x <= MAX_PRIORITY_VALUE && x >= 0) ? x : MAX_PRIORITY_VALUE) << RESERVED_PRIORITY_BIT)

#define INTID_GPIO_GRP0                0x0020
#define INTID_GPIO_GRP1                0x0024
#define INTID_GPIO_GRP2                0x0028
#define INTID_GPIO_GRP3                0x002C
#define INTID_GPIO_GRP4                0x0030
#define INTID_GPIO_GRP5                0x0034
#define INTID_GPIO_GRP6                0x0038
#define INTID_GPIO_GRP7                0x003C
#define INTID_GPIO_GRP8                0x0040
#define INTID_GPIO_GRP9                0x0044
#define INTID_GPIO_GRP10       0x0048
#define INTID_GPIO_NO_EXIST    0x0

typedef void (*IrqHandlerFn)(void *data);

typedef struct
{
    void *ctx;
    int channel_info;
} Context_t;

void Irq_Setup(void);
void Irq_SetupEntry(unsigned int id, IrqHandlerFn Handler, Context_t *Context);
void Irq_RemoveEntry(unsigned int id);
void Irq_Enable(unsigned int id);
void Irq_Disable(unsigned int id);
void Irq_SetPriority(unsigned int id, uint8_t priority);
unsigned int FreeRTOS_GetActiveIRQ(void);
void FreeRTOS_EndOfInterrupt(unsigned int id);
int Irq_GetTableId(unsigned int id);
uint32_t Irq_RegRead(uint32_t addr);
void Irq_RegWrite(uint32_t addr, uint32_t val);
int Irq_MergeSetup(unsigned int id);
int Irq_GetMergeStatReg(unsigned int id);

#endif /* __INTERRUPTS_H_*/
