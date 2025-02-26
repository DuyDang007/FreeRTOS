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

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/**
 * @brief ARM Generic Timer Interrupt number
 */
#define R_OS_BSP_GENERIC_ARM_TIMER_IRQNUM 30

/**
 * @brief HSCIF Interrupt number
 */
#define HSCIF_INT_ID	623
#define SCP2CR00_INT_ID 214 

/**
 * @brief Reserved priority bit
 */
#define RESERVED_PRIORITY_BIT 3

/**
 * @brief Max priority value
 */
#define MAX_PRIORITY_VALUE 31

/**
 * @brief Macro to check if the priority exceeds the maximum value
 */
#define IPRIORITY(x) (((x <= MAX_PRIORITY_VALUE && x >= 0) ? x : MAX_PRIORITY_VALUE) << RESERVED_PRIORITY_BIT)

/**
 * @brief Interrupt IDs of GPIO group
 */
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

/**
 * @defgroup Interrupt_Controller Interrupt Controller
 * @{
 * @brief This module provides functions and structures for managing interrupts.
 *
 * The Interrupt Module includes functionality for setting up, enabling, disabling,
 * and handling interrupts. It also provides mechanisms for setting interrupt priorities,
 * reading and writing to registers, and managing interrupt contexts.
 */


/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**
 * @brief Typedef for the IRQ handler function pointer.
 *
 * This typedef defines a function pointer type for IRQ handlers. The handler
 * function takes a single argument, which is a pointer to data specific to the
 * interrupt.
 *
 * @param data Pointer to the data specific to the interrupt.
 */
typedef void (*IrqHandlerFn)(void *data);

/**
 * @brief Structure to hold interrupt context information.
 *
 * This structure is used to store context-specific data and channel information
 * for handling interrupts.
 */
typedef struct
{
    void *ctx;          ///< Pointer to the context-specific data.
    int channel_info;   ///< Information about the channel of the merged interrupt.
} Context_t;

/**
 * @brief Initializes the IRQ system.
 */
void Irq_Setup(void);

/**
 * @brief Sets up an IRQ entry with the specified handler and context.
 *
 * @param id The ID of the IRQ.
 * @param Handler The function to handle the IRQ.
 * @param Context The context to be passed to the handler.
 */
void Irq_SetupEntry(unsigned int id, IrqHandlerFn Handler, Context_t *Context);

/**
 * @brief Removes an IRQ entry.
 *
 * @param id The ID of the IRQ to be removed.
 */
void Irq_RemoveEntry(unsigned int id);

/**
 * @brief Enables the specified IRQ.
 *
 * @param id The ID of the IRQ to be enabled.
 */
void Irq_Enable(unsigned int id);

/**
 * @brief Disables the specified IRQ.
 *
 * @param id The ID of the IRQ to be disabled.
 */
void Irq_Disable(unsigned int id);

/**
 * @brief Sets the priority of the specified IRQ.
 *
 * @param id The ID of the IRQ.
 * @param priority The priority level to be set.
 */
void Irq_SetPriority(unsigned int id, uint8_t priority);

/**
 * @brief Gets the ID of the currently active IRQ.
 *
 * @return The ID of the active IRQ.
 */
unsigned int FreeRTOS_GetActiveIRQ(void);

/**
 * @brief Marks the end of the interrupt for the specified IRQ.
 *
 * @param id The ID of the IRQ.
 */
void FreeRTOS_EndOfInterrupt(unsigned int id);

/**
 * @brief Gets the table ID for the specified merged IRQ.
 *
 * @param id The ID of the IRQ.
 *
 * @retval The table ID.
 * @retval -1 The IRQ ID is not a merged interrupt.
 */
int Irq_GetTableId(unsigned int id);

/**
 * @brief Reads a value from the specified register address.
 *
 * @param addr The address of the register.
 *
 * @return The value read from the register.
 */
uint32_t Irq_RegRead(uint32_t addr);

/**
 * @brief Writes a value to the specified register address.
 *
 * @param addr The address of the register.
 * @param val The value to be written to the register.
 */
void Irq_RegWrite(uint32_t addr, uint32_t val);

/**
 * @brief Sets up a merged IRQ entry.
 *
 * @param id The ID of the IRQ.
 *
 * @retval 0 on success.
 * @retval -1 The IRQ ID is not a merged interrupt.
 */
int Irq_MergeSetup(unsigned int id);

/**
 * @brief Gets the status register for the merged IRQ.
 *
 * @param id The ID of the IRQ.
 * @return The status register value.
 *
 * @retval Value of status register if it is a merged interrupt.
 * @retval -1 The IRQ ID is not a merged interrupt.
 */
int Irq_GetMergeStatReg(unsigned int id);

/** @} */ // end of Interrupt_Controller

#endif /* __INTERRUPTS_H_*/
