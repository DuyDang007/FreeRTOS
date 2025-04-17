/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef __TRANSLATION_TABLE_H__
#define __TRANSLATION_TABLE_H__

/*
 * Block memory type.
 */
#define NORMAL_NC_MEM_TYPE 3

/*
 * Entry table definitions.
 *
 */
#define ENTRY_TYPE_MASK     (3 << 0)
#define ENTRY_TYPE_FAULT    (0 << 0)
#define ENTRY_TYPE_TABLE    (3 << 0)
#define ENTRY_TYPE_PAGE     (3 << 0)
#define ENTRY_TYPE_BLOCK    (1 << 0)

/*
 * Block definitions.
 */
#define BLOCK_MEM_TYPE(x) ((x) << 2)
#define BLOCK_TYPE_INNER_SHARE (3 << 8)
#define BLOCK_ATTR_AF (1 << 10)

typedef struct st_mm_region
{
    uint64_t virt_addr;
    uint64_t phys_addr;
    uint64_t mem_size;
    uint64_t mem_attrs;
} st_mm_region_t;

uint64_t *CreateTranslationTable(uint64_t *ttb, uint64_t va, uint64_t pa, uint64_t size);
void freeMemoryRegion(uint64_t *ttb, uint64_t va, uint64_t pa, uint64_t size);

#endif /* __TRANSLATION_TABLE_H__ */
