/*
 * Copyright (c) 2019-2020 Renesas Electronics Europe Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stddef.h>
#include "cmsis_rcar_gen5.h"
#include "interrupts.h"
#include "irq_ctrl.h"
#include "drivers/gic/gicv3_basic.h"

#define MAX_IRQ_NUMBER       1019
#define DEFAULT_ISR_PRIORITY 0

typedef void (*IrqHandlerFn)(void *data);

typedef struct
{
	IrqHandlerFn Handler;
	void *Context;
} IvtEntry;

static IvtEntry HandlerTable[MAX_IRQ_NUMBER];

void Irq_Setup(void)
{
    uint32_t rd, affinity;
    affinity = 0;

    //
    // Configure the interrupt controller
    //
    // Set location of GIC
	setGICAddr(CR52_GICD_ADDR, CR52_GICR_ADDR);

    // Enable GIC
    enableGIC();

    // Get the ID of the Redistributor connected to this PE
    rd = getRedistID(affinity);
    // Mark this core as being active
    wakeUpRedist(rd);

    // Configure the CPU interface
    // This assumes that the SRE bits are already set
    setPriorityMask(0xFF);
    enableGroup0Ints();
    enableGroup1Ints();
}

/* Set up a CR7 or INTC-RT GIC entry */
void Irq_SetupEntry(unsigned int id, IrqHandlerFn Handler, void *Context)
{
	/* Just in case... */
	if (id > MAX_IRQ_NUMBER)
		while (1)
			;

	HandlerTable[id].Handler = Handler;
	HandlerTable[id].Context = Context;

	Irq_SetPriority(id, DEFAULT_ISR_PRIORITY);
}

static void StubHandler(void *data)
{
	while (1)
		;
}

/* Remove a CR7 or INTC-RT GIC entry */
void Irq_RemoveEntry(unsigned int id)
{
	/* Just in case... */
	if (id > MAX_IRQ_NUMBER)
		while (1)
			;

	Irq_Disable(id);

	HandlerTable[id].Handler = &StubHandler;
	HandlerTable[id].Context = NULL;
}

void Irq_Enable(unsigned int id)
{
    uint32_t rd, affinity;

    affinity = 0;
    rd = getRedistID(affinity);
    setIntGroup(id, rd, GICV3_GROUP1_NON_SECURE);
    enableInt(id, rd);
}
/* Legacy: IRQ_Enable should not be used */
int32_t IRQ_Enable (IRQn_ID_t irqn)
{
    Irq_Enable(irqn);
    return 0;
}

void Irq_Disable(unsigned int id)
{
	disableInt(id, CR52_CPU_ID);
}
/* Legacy: IRQ_Disable should not be used */
int32_t IRQ_Disable (IRQn_ID_t irqn)
{
	Irq_Disable(irqn);
	return 0;
}

void Irq_SetPriority(unsigned int id, unsigned int priority)
{
	setIntPriority(id, CR52_CPU_ID, priority);
}


/* CR7 GIC interrupt handler that also checks the INTC-RT GIC */
void vApplicationIRQHandler(uint32_t ulICCIAR)
{
	/*
	 * Interrupts cannot be re-enabled until the source of the interrupt is
	 * cleared. The ID of the interrupt is obtained by bitwise ANDing the ICCIAR
	 * value with 0x3FF
	 */
	uint32_t id = ulICCIAR & 0x3FFU;
	IvtEntry *pEntry;

	if (id > MAX_IRQ_NUMBER) {
        return;
	}

	pEntry = &HandlerTable[id];

	if (!pEntry->Handler) {
		/* No interrupt handler! */
		while (1)
			;
	}

	pEntry->Handler(pEntry->Context);

}
