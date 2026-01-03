#include "devicetree-binding.h"

const struct uart_node scif0 = {
    .compatible = "rcar_scif",
    .base_address = 0xc0700000,
    .irq = NULL,
    .baudrate = BAUDRATE_115200,
    .status = OKAY
};
const struct uart_node scif1 = {
    .compatible = "rcar_scif",
    .base_address = 0xc0704000,
    .irq = NULL,
    .baudrate = BAUDRATE_115200,
    .status = OKAY
};
const struct uart_node scif3 = {
    .compatible = "rcar_scif",
    .base_address = 0xc0708000,
    .irq = NULL,
    .baudrate = BAUDRATE_115200,
    .status = OKAY
};
const struct uart_node scif4 = {
    .compatible = "rcar_scif",
    .base_address = 0xc070C000,
    .irq = NULL,
    .baudrate = BAUDRATE_115200,
    .status = OKAY
};
const struct uart_node hscif0 = {
    .compatible = "rcar_hscif",
    .base_address = 0xc0710000,
    .irq = NULL,
    .baudrate = BAUDRATE_115200,
    .status = OKAY
};
const struct uart_node hscif1 = {
    .compatible = "rcar_hscif",
    .base_address = 0xc0714000,
    .irq = NULL,
    .baudrate = BAUDRATE_115200,
    .status = OKAY
};
const struct uart_node hscif2 = {
    .compatible = "rcar_hscif",
    .base_address = 0xc0718000,
    .irq = NULL,
    .baudrate = BAUDRATE_115200,
    .status = OKAY
};
const struct uart_node hscif3 = {
    .compatible = "rcar_hscif",
    .base_address = 0xc071C000,
    .irq = NULL,
    .baudrate = BAUDRATE_115200,
    .status = OKAY
};
/* a dummy UART node, driver will return error if base address is 0 */
const struct uart_node hscif_dummy = {
    .compatible = "rcar_hscif",
    .base_address = 0x0,
    .irq = NULL,
    .baudrate = BAUDRATE_115200,
    .status = OKAY
};

const struct uart_node *uart_list[] = { &scif0, &scif1, &hscif_dummy, &scif3, &scif4, &hscif0, &hscif1, &hscif2, &hscif3, NULL };

