/**********************************************************/
/* Sample program : Memory Access Function                */
/* File Name      : mem_io.c                              */
/* Copyright (C) Renesas Electronics Corp. 2024.          */
/**********************************************************/

#define AArch64 1
#define SYSTEM_DEF_X5H 1

#include "drivers/pcie/common.h"
#include "drivers/pcie/mem_io.h"

//////////////////////////////////////////////////////////////////////
//
// I/O function
//
//////////////////////////////////////////////////////////////////////

uint8_t read8(const volatile void *addr)
{
	return *((volatile uint8_t*)(addr));
}

uint16_t read16(const volatile void *addr)
{
	return *((volatile uint16_t*)(addr));
}

uint32_t read32(const volatile void *addr)
{
	return *((volatile uint32_t*)(addr));
}

uint64_t read64(const volatile void *addr)
{
	return *((volatile uint64_t*)(addr));
}

void write8(volatile void *addr, uint8_t data)
{
	*((volatile uint8_t*)(addr)) = data;
}

void write16(volatile void *addr, uint16_t data)
{
	*((volatile uint16_t*)(addr)) = data;
}

void write32(volatile void *addr, uint32_t data)
{
	*((volatile uint32_t*)(addr)) = data;
}

void write64(volatile void *addr, uint64_t data)
{
	*((volatile uint64_t*)(addr)) = data;
}

uint8_t mem_read8(const volatile uintptr_t addr)
{
	return *((volatile uint8_t*)(addr));
}

uint16_t mem_read16(const volatile uintptr_t addr)
{
	return *((volatile uint16_t*)(addr));
}

uint32_t mem_read32(const volatile uintptr_t addr)
{
	return *((volatile uint32_t*)(addr));
}

uint64_t mem_read64(const volatile uintptr_t addr)
{
	return *((volatile uint64_t*)(addr));
}

void mem_write8(volatile uintptr_t addr, uint8_t data)
{
	*((volatile uint8_t*)(addr)) = data;
}

void mem_write16(volatile uintptr_t addr, uint16_t data)
{
	*((volatile uint16_t*)(addr)) = data;
}

void mem_write32(volatile uintptr_t addr, uint32_t data)
{
	*((volatile uint32_t*)(addr)) = data;
}

void mem_write64(volatile uintptr_t addr, uint64_t data)
{
	*((volatile uint64_t*)(addr)) = data;
}

void mem_bitclrset32(uintptr_t addr, uint32_t clr, uint32_t set)
{
	mem_write32(addr, (mem_read32(addr) & ~clr) | set);
}

void mem_bitset32(uintptr_t addr, uint32_t bit)
{
	*((volatile uint32_t*)(addr)) = *((volatile uint32_t*)(addr)) | (0x1 << bit);
}

uint32_t mem_bitread32(uintptr_t addr, uint32_t bit)
{
	return ( *((volatile uint32_t*)(addr)) & (0x1 << bit));
}

void mem_bitclr32(uintptr_t addr, uint32_t bit)
{
	*((volatile uint32_t*)(addr)) = *((volatile uint32_t*)(addr)) & ~(0x1 << bit);
}

void mem_bitset64(uintptr_t addr, uint32_t bit)
{
	*((volatile uint32_t*)(addr)) = *((volatile uint32_t*)(addr)) | (0x1 << bit);
}

uint64_t mem_bitread64(uintptr_t addr, uint32_t bit)
{
	return ( *((volatile uint32_t*)(addr)) & (0x1 << bit));
}

void mem_bitclr64(uintptr_t addr, uint32_t bit)
{
	*((volatile uint32_t*)(addr)) = *((volatile uint32_t*)(addr)) & ~(0x1 << bit);
}
