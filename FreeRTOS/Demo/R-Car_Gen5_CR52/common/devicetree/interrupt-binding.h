#include <stdint.h>

/* Default interrupt priority */
#define DEFAULT_IRQ_PRIORITY 0x01 /* set to configMAX_SYSCALL_INTERRUPT_PRIORITY */

typedef enum {
    GIC_SPI,
    GIC_PPI,
    GIC_SGI
} e_gic_type_t;

typedef enum {
    IRQ_TYPE_LEVEL,
    IRQ_TYPE_EDGE
} e_irq_type_t;

/* Elements are in order of:
   <e_gic_type_t INT_ID e_irq_type_t PRIORITY>
 */
typedef int32_t irq_id[4];
