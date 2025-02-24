#ifndef _MEMORY_MAP_H
#define _MEMORY_MAP_H

#include <stdint.h>
#include "rcar_utils.h"

typedef enum e_mem_attr {
    DEVICE_ATTR,
    RAM_ATTR,
    RAM_NOCACHE_ATTR,
    RAM_TEXT_ATTR,
    RAM_RO_ATTR,
    SRAM_ATTR,
    FLASH_ATTR
} e_mem_attr_t;

typedef struct st_memory_region {
    e_memory_type_t type;
    st_memory_t mem_addr;
    e_mem_attr_t attr;
} st_memory_region_t;

// Define all peripheral address regions
#define PERIPHERAL_START_0          0x18800000
#define PERIPHERAL_SIZE_0           0x00080000  // to 0x1888_0000

#define PERIPHERAL_START_1          0x188C0000
#define PERIPHERAL_SIZE_1           0x07740000  // to 0x2000_0000

#define CA_CMA_ADDRESS              0x50000000
#define CA_CMA_SIZE                 0x0FF00000  // to 0x5FF0_0000

#define OSAL_MEMORY_ADDRESS         0x60000000
#define OSAL_MEMORY_SIZE            0x20000000  // to 0x8000_0000

#define SHARED_DRAM_ADDRESS         0x80000000
#define SHARED_DRAM_SIZE            0x20000000  // to 0xA000_0000

#define PERIPHERAL_START_2          0xC0000000
#define PERIPHERAL_SIZE_2           0x40000000  // to 0x1_0000_0000

static const st_memory_region_t RCAR_MEMMORY_ARR[] = {
    {.type = CMA,           .mem_addr = {.base_address = (uint32_t) CA_CMA_ADDRESS,      .size = (uint32_t) CA_CMA_SIZE},         .attr = DEVICE_ATTR},
    {.type = OSAL,          .mem_addr = {.base_address = (uint32_t) OSAL_MEMORY_ADDRESS, .size = (uint32_t) OSAL_MEMORY_SIZE},    .attr = DEVICE_ATTR},
    {.type = SHARE_MEM,     .mem_addr = {.base_address = (uint32_t) SHARED_DRAM_ADDRESS, .size = (uint32_t) SHARED_DRAM_SIZE},    .attr = DEVICE_ATTR},
    {.type = PERIPHERAL,    .mem_addr = {.base_address = (uint32_t) PERIPHERAL_START_0,  .size = (uint32_t) PERIPHERAL_SIZE_0},   .attr = DEVICE_ATTR},
    {.type = PERIPHERAL,    .mem_addr = {.base_address = (uint32_t) PERIPHERAL_START_1,  .size = (uint32_t) PERIPHERAL_SIZE_1},   .attr = DEVICE_ATTR},
    {.type = PERIPHERAL,    .mem_addr = {.base_address = (uint32_t) PERIPHERAL_START_2,  .size = (uint32_t) PERIPHERAL_SIZE_2},   .attr = DEVICE_ATTR}
};

#endif // _MEMORY_MAP_H
