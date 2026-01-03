#include "devicetree-binding.h"
#include "state-manager/r_clock_domain_id.h"

/* ------------------------------ UART NODES ----------------------------------- */
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

/* ------------------------------ GPIO NODES ----------------------------------- */
const irq_id gpio_irq[] = {
    {GIC_SPI, 0x20, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x24, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x28, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x2C, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x30, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x34, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x38, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x3C, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x40, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x44, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x48, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2}
};

const struct gpio_node gpio0 = {
    .compatible = "rcar_gpio",
    .base_address = 0xC1080000U,
    .irq = &gpio_irq[0],
    .status = OKAY
};
const struct gpio_node gpio1 = {
    .compatible = "rcar_gpio",
    .base_address = 0xC1080800U,
    .irq = &gpio_irq[1],
    .status = OKAY
};
const struct gpio_node gpio2 = {
    .compatible = "rcar_gpio",
    .base_address = 0xC1081000U,
    .irq = &gpio_irq[2],
    .status = OKAY
};
const struct gpio_node gpio3 = {
    .compatible = "rcar_gpio",
    .base_address = 0xC0800000U,
    .irq = &gpio_irq[3],
    .status = OKAY
};
const struct gpio_node gpio4 = {
    .compatible = "rcar_gpio",
    .base_address = 0xC0800800U,
    .irq = &gpio_irq[4],
    .status = OKAY
};
const struct gpio_node gpio5 = {
    .compatible = "rcar_gpio",
    .base_address = 0xC0400000U,
    .irq = &gpio_irq[5],
    .status = OKAY
};
const struct gpio_node gpio6 = {
    .compatible = "rcar_gpio",
    .base_address = 0xC0400800U,
    .irq = &gpio_irq[6],
    .status = OKAY
};
const struct gpio_node gpio7 = {
    .compatible = "rcar_gpio",
    .base_address = 0xC0401000U,
    .irq = &gpio_irq[7],
    .status = OKAY
};
const struct gpio_node gpio8 = {
    .compatible = "rcar_gpio",
    .base_address = 0xC0401800U,
    .irq = &gpio_irq[8],
    .status = OKAY
};
const struct gpio_node gpio9 = {
    .compatible = "rcar_gpio",
    .base_address = 0xC9B00000U,
    .irq = &gpio_irq[9],
    .status = OKAY
};
const struct gpio_node gpio10 = {
    .compatible = "rcar_gpio",
    .base_address = 0xC9B00800U,
    .irq = &gpio_irq[10],
    .status = OKAY
};

const struct gpio_node *gpio_list[] = { &gpio0, &gpio1, &gpio2, &gpio3, &gpio4, &gpio5, &gpio6, &gpio7, &gpio8, &gpio9, &gpio10, NULL };

/* ------------------------------ I2C NODES ----------------------------------- */
const irq_id i2c_irq[] = {
    {GIC_SPI, 0x0392, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x025D, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x025E, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x025F, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x0260, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x0261, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x0262, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x0263, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x0264, IRQ_TYPE_LEVEL, DEFAULT_IRQ_PRIORITY + 2}
};
const struct i2c_node i2c0 = {
    .compatible = "rcar_i2c",
    .base_address = 0xc11d0000,
    .irq = &i2c_irq[0],
    .clock_id = X5H_CLOCK_ID_MDLC_I2C0,
    .status = OKAY
};
const struct i2c_node i2c1 = {
    .compatible = "rcar_i2c",
    .base_address = 0xc06c0000,
    .irq = &i2c_irq[1],
    .clock_id = X5H_CLOCK_ID_MDLC_I2C1,
    .status = OKAY
};
const struct i2c_node i2c2 = {
    .compatible = "rcar_i2c",
    .base_address = 0xc06c8000,
    .irq = &i2c_irq[2],
    .clock_id = X5H_CLOCK_ID_MDLC_I2C2,
    .status = OKAY
};
const struct i2c_node i2c3 = {
    .compatible = "rcar_i2c",
    .base_address = 0xc06d0000,
    .irq = &i2c_irq[3],
    .clock_id = X5H_CLOCK_ID_MDLC_I2C3,
    .status = OKAY
};
const struct i2c_node i2c4 = {
    .compatible = "rcar_i2c",
    .base_address = 0xc06d8000,
    .irq = &i2c_irq[4],
    .clock_id = X5H_CLOCK_ID_MDLC_I2C4,
    .status = OKAY
};
const struct i2c_node i2c5 = {
    .compatible = "rcar_i2c",
    .base_address = 0xc06e0000,
    .irq = &i2c_irq[5],
    .clock_id = X5H_CLOCK_ID_MDLC_I2C5,
    .status = OKAY
};
const struct i2c_node i2c6 = {
    .compatible = "rcar_i2c",
    .base_address = 0xc06e8000,
    .irq = &i2c_irq[6],
    .clock_id = X5H_CLOCK_ID_MDLC_I2C6,
    .status = OKAY
};
const struct i2c_node i2c7 = {
    .compatible = "rcar_i2c",
    .base_address = 0xc06f0000,
    .irq = &i2c_irq[7],
    .clock_id = X5H_CLOCK_ID_MDLC_I2C7,
    .status = OKAY
};
const struct i2c_node i2c8 = {
    .compatible = "rcar_i2c",
    .base_address = 0xc06f8000,
    .irq = &i2c_irq[8],
    .clock_id = X5H_CLOCK_ID_MDLC_I2C8,
    .status = OKAY
};

const struct i2c_node *i2c_list[] = { &i2c0, &i2c1, &i2c2, &i2c3, &i2c4, &i2c5, &i2c6, &i2c7, &i2c8, NULL };

/* ------------------------------ TAUD NODES ----------------------------------- */
const irq_id taud0_irq[] = {
    {GIC_SPI, 0x0397, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x0398, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x0399, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x039A, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x039B, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x039C, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x039D, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x039E, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x039F, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03A0, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03A1, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03A2, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03A3, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03A4, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03A5, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03A6, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
};
const irq_id taud1_irq[] = {
    {GIC_SPI, 0x03A7, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03A8, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03A9, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03AA, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03AB, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03AC, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03AD, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03AE, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03AF, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03B0, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03B1, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03B2, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03B3, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03B4, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03B5, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
    {GIC_SPI, 0x03B6, IRQ_TYPE_EDGE, DEFAULT_IRQ_PRIORITY + 2},
};
const struct taud_node taud0 = {
    .compatible = "rcar_taud",
    .channel_count = 16,
    .base_address = 0xC1392000U,
    .irq = taud0_irq,
    .clock_id = X5H_CLOCK_ID_MDLC_TAUD0,
    .status = OKAY
};
const struct taud_node taud1 = {
    .compatible = "rcar_taud",
    .channel_count = 16,
    .base_address = 0xC1393000U,
    .irq = taud1_irq,
    .clock_id = X5H_CLOCK_ID_MDLC_TAUD1,
    .status = OKAY
};

const struct taud_node *taud_list[] = { &taud0, &taud1, NULL };