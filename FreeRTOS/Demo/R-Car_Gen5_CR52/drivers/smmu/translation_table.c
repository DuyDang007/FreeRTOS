/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "translation_table.h"
#include "FreeRTOS.h"
#include "rcar_utils.h"
#include <string.h>

#define MAX_TABLE_SIZE      512
#define ENTRY_TABLE_MASK    MAX_TABLE_SIZE - 1
#define ENTRY_ADDR_MASK     0xFFFFFFFFF << 12
#define PAGE_MASK           0xFFF

static int get_entry_type(uint64_t *entry)
{
    return *entry & ENTRY_TYPE_MASK;
}

static int shift_table_level(int tbl_level)
{
    return (12 + 9 * (3 - tbl_level));
}

static uint64_t *Find_Entry_Table(uint64_t *ttb, uint64_t virt_addr, int tbl_level)
{
    uint64_t *table;
    uint16_t entry_idx;

    table = ttb;
    for (int i = 0; i < 4; i++) {
        entry_idx = (virt_addr >> shift_table_level(i)) & ENTRY_TABLE_MASK;
        table += entry_idx;

        if (i == tbl_level)
        {
            return table;
        }

        if (get_entry_type(table) != ENTRY_TYPE_TABLE)
        {
            printf("Can't find Entry.\n");
            return NULL;
        }

        table = (uint64_t *)(uintptr_t)(*table & ENTRY_ADDR_MASK);
    }

    printf("[%s][%d] Can't reach this line.\n", __func__, __LINE__);
    return NULL;
}

static uint64_t *Allocate_Table(void)
{
    uint64_t *new_table = aligned_malloc(1 << 12, MAX_TABLE_SIZE * sizeof(uint64_t));
    if (new_table) {
        memset(new_table, 0, MAX_TABLE_SIZE * sizeof(uint64_t));
    }

    return new_table;
}

static void Assign_Entry_Table(uint64_t *entry, uint64_t *table)
{
    *entry = (unsigned long)table | ENTRY_TYPE_TABLE;
}

static void Map_Region(uint64_t *ttb, struct st_mm_region *region_map)
{
    uint16_t entry_idx;
    uint64_t *table;
    uint64_t virt_addr = region_map->virt_addr;
    uint64_t phys_addr = region_map->phys_addr;
    uint64_t mem_size  = region_map->mem_size;
    uint64_t mem_attrs = region_map->mem_attrs | ENTRY_TYPE_BLOCK | BLOCK_ATTR_AF;
    uint64_t mem_block;
    uint8_t tbl_level;
    uint64_t *new_table;

    while (mem_size)
    {
        tbl_level = 0;
        table = ttb;
        entry_idx = (virt_addr >> shift_table_level(tbl_level)) & ENTRY_TABLE_MASK;

        if (get_entry_type(table + entry_idx) == ENTRY_TYPE_FAULT)
        {
            new_table = Allocate_Table();
            Assign_Entry_Table(table, new_table);
        }

        tbl_level++;
        table = (uint64_t *)(uintptr_t)(*(table + entry_idx) & ENTRY_ADDR_MASK);

        while (tbl_level < 4)
        {
            entry_idx = (virt_addr >> shift_table_level(tbl_level)) & ENTRY_TABLE_MASK;

            mem_block = 1ULL << shift_table_level(tbl_level);

            if (mem_size >= mem_block && !(phys_addr & (mem_block - 1)))
            {
                if (tbl_level == 3)
                {
                    *(table + entry_idx) = phys_addr | mem_attrs | ENTRY_TYPE_PAGE;
                }
                else
                {
                    *(table + entry_idx) = phys_addr | mem_attrs;
                }

                virt_addr += mem_block;
                phys_addr += mem_block;
                mem_size -= mem_block;

                if (entry_idx == MAX_TABLE_SIZE - 1)
                {
                    break;
                }

                continue;
            }
            else if (get_entry_type(table + entry_idx) == ENTRY_TYPE_FAULT)
            {
                new_table = Allocate_Table();
                Assign_Entry_Table(table + entry_idx, new_table);
            }
            else if (get_entry_type(table + entry_idx) == ENTRY_TYPE_BLOCK)
            {
                /* Not support this case */
                printf("Cannot unmap a larger block to map a smaller block\n");
                return;
            }

            tbl_level++;
            table = (uint64_t *)(uintptr_t)(*(table + entry_idx) & ENTRY_ADDR_MASK);
        }
    }
}

uint64_t *CreateTranslationTable(uint64_t *ttb, uint64_t va, uint64_t pa, uint64_t size)
{
    uint64_t *Table = ttb;

    if ((va & PAGE_MASK) || (pa & PAGE_MASK) || (size & PAGE_MASK))
    {
        printf("Cannot map! va, pa and size must be aligned with 4KB\n");
        return Table;
    }

    if (Table == NULL)
    {
        Table = Allocate_Table();
    }

    st_mm_region_t region_mem = {
        .virt_addr = va,
        .phys_addr = pa,
        .mem_size = size,
        .mem_attrs = BLOCK_MEM_TYPE(NORMAL_NC_MEM_TYPE) | BLOCK_TYPE_INNER_SHARE};

    Map_Region(Table, &region_mem);

    return Table;
}

void freeMemoryRegion(uint64_t *ttb, uint64_t va, uint64_t pa, uint64_t size)
{
    uint64_t *entry;
    int level;
    uint64_t blocksize;
    uint64_t current_pa = pa;
    uint64_t end_pa = pa + size;

    while (current_pa < end_pa)
    {
        for (level = 0; level < 4; level++)
        {
            entry = Find_Entry_Table(ttb, va, level);
            if (!entry)
            {
                printf("[%s] PTE not found for va: 0x%llx\n", __func__, va);
                return;
            }

            blocksize = 1ULL << shift_table_level(level);

            /* Check if entry points to the right PA */
            if ((*entry & ENTRY_ADDR_MASK) == (current_pa & ENTRY_ADDR_MASK))
            {
                /* Free if it's a table */
                if (get_entry_type(entry) == ENTRY_TYPE_TABLE)
                {
                    aligned_free((void *)(uintptr_t)(*entry & ENTRY_ADDR_MASK));
                }

                /* Set entry to 0 */
                *entry = 0;
                break; // Exit for loop once the correct entry is freed
            }
        }

        current_pa += blocksize; // Move to the next PA in the range
        va += blocksize;         // Move to the next VA in the range

        // if current_pa out range, break
        if (current_pa >= end_pa)
        {
            break;
        }
    }

} 
