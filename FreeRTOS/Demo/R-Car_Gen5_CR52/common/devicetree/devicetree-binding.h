#ifndef DEVICETREE_BINDING_H
#define DEVICETREE_BINDING_H

#include <stdint.h>
#include <stddef.h>
#include "interrupt-binding.h"

/* Node status values */
typedef enum {
    DISABLED,
    OKAY
} e_node_status_t;

/* ------- UART --------- */
typedef enum {
    BAUDRATE_115200,
    BAUDRATE_921600,
    BAUDRATE_1843200,
    BAUDRATE_3000000
} e_uart_baudrate_t;

struct uart_node {
    const char *compatible;
    const uint32_t base_address;
    const irq_id *irq;
    const int32_t clock_id;
    const e_uart_baudrate_t baudrate;
    const e_node_status_t status;
};

extern const struct uart_node *uart_list[];

/* ------- GPIO ---------- */
struct gpio_node {
    const char *compatible;
    const uint32_t base_address;
    const irq_id *irq;
    const e_node_status_t status;
};
extern const struct gpio_node *gpio_list[];

/* ------- I2C ---------- */
struct i2c_node {
    const char *compatible;
    const uint32_t base_address;
    const irq_id *irq;
    const int32_t clock_id;
    const e_node_status_t status;
};

extern const struct i2c_node *i2c_list[];


/* ------------------ HELPER FUNCTIONS --------------- */

/** Count the number of node in a node array. Make sure the last element is always NULL
 *  and it will be excluded
 */
static inline uint32_t dt_count_node(void *nodes[]) {
    size_t count = 0;
    while (nodes[count] != NULL) {
        count++;
    }
    return count;
}


#endif /* DEVICETREE_BINDING_H */