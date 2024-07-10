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

#include "core_cr52.h"
#include "irq_ctrl.h"

#define R_OS_BSP_GENERIC_ARM_TIMER_IRQNUM 30

typedef void (*IrqHandlerFn)(void *data);

void Irq_Setup(void);
void Irq_SetupEntry(unsigned int id, IrqHandlerFn Handler, void *Context);
void Irq_RemoveEntry(unsigned int id);
void Irq_Enable(unsigned int id);
void Irq_Disable(unsigned int id);
void Irq_SetPriority(unsigned int id, unsigned int priority);
unsigned int FreeRTOS_GetActiveIRQ(void);
void FreeRTOS_EndOfInterrupt(unsigned int id);

#endif /* __INTERRUPTS_H_*/
