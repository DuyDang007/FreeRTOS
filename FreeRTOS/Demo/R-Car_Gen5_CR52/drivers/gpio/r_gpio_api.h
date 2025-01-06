/*************************************************************************************************************
* Copyright (c) 2024 Renesas Electronics Corporation
* This software is released under the MIT License
* http://opensource.org/licenses/mit-license.php
*************************************************************************************************************/

#ifndef R_GPIO_API_H_
#define R_GPIO_API_H_

#include "interrupts.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define PINS_EACH_GROUP 32

/* Shift to get port in bsp_io_port_t and bsp_io_port_pin_t enums. */
#define IOPORT_PRV_PORT_OFFSET		(10U)
#define IOPORT_PRV_PORT_BITS		(0xFF00U)
#define IOPORT_PRV_PIN_BITS		(0x00FFU)

/** Interrupt Input Mode*/
typedef enum e_rcar_interrupt_input
{
    RCAR_INTERRUPT_INPUT_RISING_EDGE = 10,        ///< Rising Edge
    RCAR_INTERRUPT_INPUT_FALLING_EDGE,       ///< Falling Edge
    RCAR_INTERRUPT_INPUT_BOTH_EDGE       ///< Both Edge
} rcar_interrupt_input_t;

/** Direction of individual pins */
typedef enum e_rcar_io_dir
{
    RCAR_IO_DIRECTION_INPUT = 0,        ///< Input
    RCAR_IO_DIRECTION_OUTPUT            ///< Output
} rcar_io_direction_t;

int gpioSetPinOutput(uint8_t group, uint8_t pin, bool lvl);
int gpioSetPortOutput(uint8_t group, uint32_t mask_levels,
		      uint32_t mask_pins);

bool gpioReadPin(uint8_t group, uint8_t pin);
uint32_t gpioReadPort(uint8_t group);

int gpioSetPinGeneralMode(uint8_t group, uint8_t pin,
			  rcar_io_direction_t option);
int gpioSetPortGeneralMode(uint8_t group, uint32_t mask_directions,
			   uint32_t mask_pins);

int gpioSetPinInterruptInputMode(uint8_t group, uint8_t pin,
				 rcar_interrupt_input_t option);
int gpioSetPortInterruptInputMode(uint8_t group, uint32_t mask_options,
				  uint32_t mask_pins);
void gpioClearInterruptHandler(uint32_t group_pin);
int gpioSetupIRQ(uint8_t group, IrqHandlerFn handler, void *ctx);

#ifdef __cplusplus
}
#endif

#endif /* R_GPIO_API_H_ */
