/*
 * Copyright (c) 2019-2020 Renesas Electronics Europe Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 */
 
#ifndef REGISTER_H
#define REGISTER_H

/* Standard includes. */
#include <stdio.h>
#include <stdint.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

/*-----------------------------------------------------------*/
#define BIT(nr)			(1UL << (nr))
#define BIT_MASK(nr)		(BIT(nr) - 1)
/*
 * Generic virtual read/write.  Note that we don't support half-word
 * read/writes.  We define __arch_*[bl] here, and leave __arch_*w
 * to the architecture specific code.
 */
#define __arch_getb(a)			(*(volatile unsigned char *)((u32)(a)))
#define __arch_getw(a)			(*(volatile unsigned short *)((u32)(a)))
#define __arch_getl(a)			(*(volatile unsigned int *)((u32)(a)))

#define __arch_putb(v,a)		(*(volatile unsigned char *)((u32)(a)) = (v))
#define __arch_putw(v,a)		(*(volatile unsigned short *)((u32)(a)) = (v))
#define __arch_putl(v,a)		(*(volatile unsigned int *)((u32)(a)) = (v))

#define __raw_writeb(v,a)	__arch_putb(v,a)
#define __raw_writew(v,a)	__arch_putw(v,a)
#define __raw_writel(v,a)	__arch_putl(v,a)

#define __raw_readb(a)		__arch_getb(a)
#define __raw_readw(a)		__arch_getw(a)
#define __raw_readl(a)		__arch_getl(a)

/*
 * TODO: The kernel offers some more advanced versions of barriers, it might
 * have some advantages to use them instead of the simple one here.
 */
#define reg_dmb()		__asm__ __volatile__ ("dmb" : : : "memory")
#define __iormb()	reg_dmb()
#define __iowmb()	reg_dmb()

#define writeb(v,c)	({ u8  __v = (v); __iowmb(); __arch_putb(__v,c); __v; })
#define writew(v,c)	({ u16 __v = (v); __iowmb(); __arch_putw(__v,c); __v; })
#define writel(v,c)	({ u32 __v = (v); __iowmb(); __arch_putl(__v,c); __v; })

#define readb(c)	({ u8  __v = __arch_getb(c); __iormb(); __v; })
#define readw(c)	({ u16 __v = __arch_getw(c); __iormb(); __v; })
#define readl(c)	({ u32 __v = __arch_getl(c); __iormb(); __v; })

#define setbitsl(dest, bits)	(writel(readl(dest) | (bits), (dest)))
#define clrbitsl(dest, bits)	(writel(readl(dest) & ~(bits), (dest)))

#endif		/* REGISTER_H */

