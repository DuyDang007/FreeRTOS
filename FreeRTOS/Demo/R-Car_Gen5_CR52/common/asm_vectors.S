/*
 * Copyright (C) 2019-2020 Renesas Electronics Europe Ltd. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 */

    .text
    .code 32

    .extern _reset
    .extern FreeRTOS_IRQ_Handler
    .extern FreeRTOS_SWI_Handler

    .global _Reset

.section .isr_vector

_Reset:
    ldr pc, =_reset			/* Reset Vector */
    ldr pc, =undefined_handler
    ldr pc, =svc_handler
    ldr pc, =prefetch_handler
    ldr pc, =abort_handler
    ldr pc, =reserved_handler
    ldr pc, =irq_handler
    ldr pc, =fiq_handler


undefined_handler:
    b  undefined_handler

svc_handler:
    MCR     p15, 0, R0, c7, c5, 6       /* BPIALL */
    DSB
    b  FreeRTOS_SWI_Handler

prefetch_handler:
    b  prefetch_handler

abort_handler:
    b  abort_handler

reserved_handler:
    b  reserved_handler

irq_handler:
    MCR     p15, 0, R0, c7, c5, 6       /* BPIALL */
    DSB
    b  FreeRTOS_IRQ_Handler

fiq_handler:
    b  fiq_handler

.end
