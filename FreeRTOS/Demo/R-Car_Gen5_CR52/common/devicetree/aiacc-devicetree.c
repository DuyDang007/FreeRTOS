#include "devicetree-binding.h"

/* a dummy UART node, driver will return error if base address is 0 */
const struct uart_node hscif_dummy = {
    .compatible = "rcar_hscif",
    .base_address = 0x0,
    .irq = NULL,
    .baudrate = BAUDRATE_3000000,
    .status = OKAY
};
const struct uart_node hscif0 = {
    .compatible = "rcar_hscif",
    .base_address = 0x38010000,
    .irq = NULL,
    .baudrate = BAUDRATE_3000000,
    .status = OKAY
};
const struct uart_node hscif1 = {
    .compatible = "rcar_hscif",
    .base_address = 0x38014000,
    .irq = NULL,
    .baudrate = BAUDRATE_3000000,
    .status = OKAY
};

const struct uart_node *uart_list[] = { &hscif_dummy, &hscif_dummy, &hscif_dummy, &hscif_dummy, &hscif_dummy, &hscif0, &hscif1, NULL };