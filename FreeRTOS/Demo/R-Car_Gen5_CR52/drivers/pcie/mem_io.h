/**********************************************************/
/* Sample program : Memory Access Function Header         */
/* File Name      : mem_io.h                              */
/* Copyright (C) Renesas Electronics Corp. 2024.          */
/**********************************************************/

#pragma once

//#include <stdint.h>

/******************************************************************************/
/* GLOBAL FUNCTION PROTOTYPE                                                  */
/******************************************************************************/
uint8_t read8(const volatile void *addr);
uint16_t read16(const volatile void *addr);
uint32_t read32(const volatile void *addr);
uint64_t read64(const volatile void *addr);
void write8(volatile void *addr, uint8_t data);
void write16(volatile void *addr, uint16_t data);
void write32(volatile void *addr, uint32_t data);
void write64(volatile void *addr, uint64_t data);

uint8_t mem_read8(const volatile uintptr_t addr);
uint16_t mem_read16(const volatile uintptr_t addr);
uint32_t mem_read32(const volatile uintptr_t addr);
uint64_t mem_read64(const volatile uintptr_t addr);
void mem_write8(volatile uintptr_t addr, uint8_t data);
void mem_write16(volatile uintptr_t addr, uint16_t data);
void mem_write32(volatile uintptr_t addr, uint32_t data);
void mem_write64(volatile uintptr_t addr, uint64_t data);

void mem_bitclrset32(uintptr_t addr, uint32_t clr, uint32_t set);
void mem_bitset32(uintptr_t addr, uint32_t bit);
void mem_bitclr32(uintptr_t addr, uint32_t bit);
uint32_t mem_bitread32(uintptr_t addr, uint32_t bit);
void mem_bitset64(uintptr_t addr, uint32_t bit);
void mem_bitclr64(uintptr_t addr, uint32_t bit);
uint64_t mem_bitread64(uintptr_t addr, uint32_t bit);

////////////////////////////////////////////////////////////
//
// inline function
//
////////////////////////////////////////////////////////////
#ifdef AArch64
static inline uint8_t readb(const volatile void *addr)
{
	uint8_t val;
	asm volatile("ldrb %w0, %1"
		     : "=r" (val)
		     : "Qo" (*(volatile uint8_t*)addr));
	return val;
}

static inline uint16_t readw(const volatile void *addr)
{
	uint16_t val;
	asm volatile("ldrh %w0, %1"
		     : "=r" (val)
		     : "Qo" (*(volatile uint16_t*)addr));
	return val;
}

static inline uint32_t readl(const volatile void *addr)
{
	uint32_t val;
	asm volatile("ldr %w0, %1"
		     : "=r" (val)
		     : "Qo" (*(volatile uint32_t*)addr));
	return val;
}

static inline uint64_t readq(const volatile void *addr)
{
	uint64_t val;
	asm volatile("ldr %x0, %1"
		     : "=r" (val)
		     : "Qo" (*(volatile uint64_t*)addr));
	return val;
}

static inline void writeb(uint8_t val, volatile void *addr)
{
	asm volatile("strb %w1, %0"
		     : : "Qo" (*(volatile uint8_t*)addr), "r" (val));
}

static inline void writew(uint16_t val, volatile void *addr)
{
	asm volatile("strh %w1, %0"
		     : : "Qo" (*(volatile uint16_t*)addr), "r" (val));
}

static inline void writel(uint32_t val, volatile void *addr)
{
	asm volatile("str %w1, %0"
		     : : "Qo" (*(volatile uint32_t*)addr), "r" (val));
}

static inline void writeq(uint64_t val, volatile void *addr)
{
	asm volatile("str %x1, %0"
		     : : "Qo" (*(volatile uint64_t*)addr), "r" (val));
}
#endif

#ifdef AArch32
static inline uint8_t readb(const volatile void *addr)
{
	uint8_t val;
	asm volatile("ldrb %0, %1"
		     : "=r" (val)
		     : "Qo" (*(volatile uint8_t*)addr));
	return val;
}

static inline uint16_t readw(const volatile void *addr)
{
	uint16_t val;
	asm volatile("ldrh %0, %1"
		     : "=r" (val)
		     : "Qo" (*(volatile uint16_t*)addr));
	return val;
}

static inline uint32_t readl(const volatile void *addr)
{
	uint32_t val;
	asm volatile("ldr %0, %1"
		     : "=r" (val)
		     : "Qo" (*(volatile uint32_t*)addr));
	return val;
}

/*
static inline uint64_t readq(const volatile void *addr)
{
}
*/

static inline void writeb(uint8_t val, volatile void *addr)
{
	asm volatile("strb %1, %0"
		     : : "Qo" (*(volatile uint8_t*)addr), "r" (val));
}

static inline void writew(uint16_t val, volatile void *addr)
{
	asm volatile("strh %1, %0"
		     : : "Qo" (*(volatile uint16_t*)addr), "r" (val));
}

static inline void writel(uint32_t val, volatile void *addr)
{
	asm volatile("str %1, %0"
		     : : "Qo" (*(volatile uint32_t*)addr), "r" (val));
}

/*
static inline void writeq(uint64_t val, volatile void *addr)
{
}
*/
#endif

