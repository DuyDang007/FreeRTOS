/*
 * Copyright (C) 2019-2020 Renesas Electronics Europe Ltd. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 */

.extern __stack
.extern __irq_stack
.extern __supervisor_stack
.extern __abort_stack
.extern __fiq_stack
.extern __undef_stack
.extern _vector_table
.extern SystemInit

.global _reset
.global software_init_hook	

#include "drivers/timer/arm_generic_timer.h"
.section .boot

#define MODE_MASK	0x1F		/* Bit mask for mode bits in CPSR */
#define ARM_MODE_USR	0x10
#define ARM_MODE_FIQ	0x11
#define ARM_MODE_IRQ	0x12
#define ARM_MODE_SVC	0x13
//#define ARM_MODE_MON	0x16
#define ARM_MODE_ABT	0x17
#define ARM_MODE_HYP	0x1A
#define ARM_MODE_UND	0x1B
#define ARM_MODE_SYS	0x1F

#define A_BIT	(1 << 8)
#define I_BIT	(1 << 7)
#define F_BIT	(1 << 6)
#define T_BIT	(1 << 5)


#define BIT(n)  (1 << (n))

/* Hyp System Control Register */
#define HSCTLR_RES1		(BIT(29) | BIT(28) | BIT(23) | \
				 BIT(22) | BIT(18) | BIT(16) | \
				 BIT(11) | BIT(4)  | BIT(3))
#define SCTLR_M_BIT		BIT(0)
#define SCTLR_A_BIT		BIT(1)
#define SCTLR_C_BIT		BIT(2)
#define SCTLR_I_BIT		BIT(12)

/* Hyp Auxiliary Control Register */
#define HACTLR_CPUACTLR         BIT(0)
#define HACTLR_CDBGDCI          BIT(1)
#define HACTLR_FLASHIFREGIONR   BIT(7)
#define HACTLR_PERIPHPREGIONR   BIT(8)
#define HACTLR_QOSR_BIT         BIT(9)
#define HACTLR_BUSTIMEOUTR_BIT  BIT(10)
#define HACTLR_INTMONR_BIT      BIT(12)
#define HACTLR_ERR_BIT          BIT(13)

#define HACTLR_INIT (HACTLR_ERR_BIT | HACTLR_INTMONR_BIT | \
		     HACTLR_BUSTIMEOUTR_BIT | HACTLR_QOSR_BIT | \
		     HACTLR_PERIPHPREGIONR | HACTLR_FLASHIFREGIONR | \
		     HACTLR_CDBGDCI | HACTLR_CPUACTLR)

#define CPACR_NA	(0U)
#define CPACR_FA	(3U)

#define CPACR_CP10(r)	(r << 20)
#define CPACR_CP11(r)	(r << 22)
#define FPEXC_EN	(1 << 30)


_reset:
	/* Check if we are starting in HYP mode */
	mrs r0, cpsr
	and r0, r0, #MODE_MASK
	cmp r0, #ARM_MODE_HYP
	bne EL1_Reset_Handler
	
	/* Init HSCTLR see Armv8-R AArch32 architecture profile */
	ldr r0, =(HSCTLR_RES1 | SCTLR_I_BIT | SCTLR_C_BIT)
	mcr p15, 4, r0, c1, c0, 0

    /* Configure Arm Genereric Timer frequency */
    ldr r0, =GENERIC_TIMER_CLK
    mcr p15, 0, r0, c14, c0, 0

	/* Init HACTLR: Enable EL1 access to all IMP DEF registers */
	ldr r0, =HACTLR_INIT
	mcr p15, 4, r0, c1, c0, 1


	mrs	r0, cpsr		/* Original PSR value */
	bic	r0 ,r0, #MODE_MASK	/* Clear the mode bits */
	orr	r0 ,r0, #ARM_MODE_SVC	/* Set mode bits */
	msr	spsr_cxsf, r0		/* Change the mode */
	//ldr	sp, =__stack

	ldr r0, =EL1_Reset_Handler
	msr elr_hyp, r0
	dsb
	isb
	eret

EL1_Reset_Handler:
	/* Common and SVC mode registers */
	mov r0,  #0
	mov r1,  #0
	mov r2,  #0
	mov r3,  #0
	mov r4,  #0
	mov r5,  #0
	mov r6,  #0
	mov r7,  #0
	mov r8,  #0
	mov r9,  #0
	mov r10, #0
	mov r11, #0
	mov r12, #0
	mov r13, #0         /* r13_svc */
	mov r14, #0         /* r14_svc */
	mrs r0,  cpsr
	msr spsr_cxsf, r0   /* spsr_svc */
	
	/* FIQ mode registers */
	cps #ARM_MODE_FIQ
	mov r8,  #0         /* r8_fiq */
	mov r9,  #0         /* r9_fiq */
	mov r10, #0         /* r10_fiq */
	mov r11, #0         /* r11_fiq */
	mov r12, #0         /* r12_fiq */
	mov r13, #0         /* r13_fiq */
	mov r14, #0         /* r14_fiq */
	mrs r0,  cpsr
	msr spsr_cxsf, r0   /* spsr_fiq */
	
	/* IRQ mode registers */
	cps #ARM_MODE_IRQ
	mov r13, #0         /* r13_irq */
	mov r14, #0         /* r14_irq */
	mrs r0,  cpsr
	msr spsr_cxsf, r0   /* spsr_irq */
	
	/* ABT mode registers */
	cps #ARM_MODE_ABT
	mov r13, #0         /* r13_abt */
	mov r14, #0         /* r14_abt */
	mrs r0,  cpsr
	msr spsr_cxsf, r0   /* spsr_abt */
	
	/* UND mode registers */
	cps #ARM_MODE_UND
	mov r13, #0         /* r13_und */
	mov r14, #0         /* r14_und */
	mrs r0,  cpsr
	msr spsr_cxsf, r0   /* spsr_und */
	
	/* SYS mode registers */
	cps #ARM_MODE_SYS
	mov r13, #0         /* r13_sys */
	mov r14, #0         /* r14_sys */


#if 1 //defined(CONFIG_FPU) && defined(CONFIG_CPU_HAS_VFP)
	/*
	 * Initialise FPU registers to a defined state.
	 */
	
	/* Allow VFP coprocessor access */
	mrc p15, 0, r0, c1, c0, 2
	orr r0, r0, #(CPACR_CP10(CPACR_FA) | CPACR_CP11(CPACR_FA))
	mcr p15, 0, r0, c1, c0, 2
	
	/* Enable VFP */
	mov  r0, #FPEXC_EN
	vmsr fpexc, r0
	
	/* Initialise VFP registers */
	fmdrr d0,  r1, r1
	fmdrr d1,  r1, r1
	fmdrr d2,  r1, r1
	fmdrr d3,  r1, r1
	fmdrr d4,  r1, r1
	fmdrr d5,  r1, r1
	fmdrr d6,  r1, r1
	fmdrr d7,  r1, r1
	fmdrr d8,  r1, r1
	fmdrr d9,  r1, r1
	fmdrr d10, r1, r1
	fmdrr d11, r1, r1
	fmdrr d12, r1, r1
	fmdrr d13, r1, r1
	fmdrr d14, r1, r1
	fmdrr d15, r1, r1
        vmsr fpscr, r1
    	vmsr fpexc, r1
#endif

	/*
	 * Configure stack.
	 */
	
	/* FIQ mode stack */
	msr CPSR_c, #(ARM_MODE_FIQ | I_BIT | F_BIT)
	ldr sp, =(__fiq_stack)
	
	/* IRQ mode stack */
	msr CPSR_c, #(ARM_MODE_IRQ | I_BIT | F_BIT)
	ldr sp, =(__irq_stack)

	/* ABT mode stack */
	msr CPSR_c, #(ARM_MODE_ABT | I_BIT | F_BIT)
	ldr sp, =(__abort_stack)
	
	/* UND mode stack */
	msr CPSR_c, #(ARM_MODE_UND | I_BIT | F_BIT)
	ldr sp, =(__undef_stack)
	
	/* SVC mode stack */
	msr CPSR_c, #(ARM_MODE_SVC | I_BIT | F_BIT)
	ldr sp, =(__supervisor_stack)
	
	/* SYS mode stack */
	msr CPSR_c, #(ARM_MODE_SYS | I_BIT | F_BIT)
	ldr sp, =(__stack)


	mov 	r14, #0

	bl 	SystemInit		/* Initialize caches, MPU, etc */

	ldr r0, =(_vector_table)
	mcr p15, 0, r0, c12, c0, 0 /* Set vector table address */

	bl	main

.Ldone:	b	.Ldone			/* Should never get here */

/* Called by newlib crt0 _start after it has set up the stacks */
software_init_hook:
