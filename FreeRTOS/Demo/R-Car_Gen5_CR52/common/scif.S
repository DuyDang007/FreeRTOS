/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the names of the copyright holders nor the names of any
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef INTEGRITY_GUEST
	/* FIXME: Workaround for compatibility with INTEGRITY guest */
	#define SCIF SCIF1
#endif

	#include "scif.h"

	.global	console_init
	.global	console_putc
	.global	console_wait

console_init:
	ldr	r0, =CPG_BASE
	ldr	r1, [r0, #CPG_SMSTPCR3]
	and	r1, r1, #~MSTP310		/* MSTP310=0 */
	mvn	r2, r1
	str	r2, [r0, #CPG_CPGWPR]
	str	r1, [r0, #CPG_SMSTPCR3]

	/* Enable MOD clock for SCIF1 or SCIF2.
	   Do by setting the RMSTPCRn so that Linux will have
	   no effect on it */
#if SCIF==SCIF1
	ldr	r1, [r0, #CPG_RMSTPCR2]
	and	r1, r1, #~RMSTP206		/* RMSTP=0 -> SCIF1 */
	mvn	r2, r1
	str	r2, [r0, #CPG_CPGWPR]
	str	r1, [r0, #CPG_RMSTPCR2]
#endif

	ldr	r0, =PFC_BASE
	ldr	r1, [r0, #MOD_SEL1]
#if SCIF==SCIF1
	and	r1, r1, #~MOD_SEL1_SCIF2_MASK
#else
	and	r1, r1, #~MOD_SEL1_SCIF1_MASK
#endif
	mvn	r2, r1
	str	r2, [r0, #PMMR]
	str	r1, [r0, #MOD_SEL1]

#if SCIF==SCIF1
	ldr	r1, [r0, #IPSR11]
	and	r1, r1, #~IPSR11_MASK
	mov	r2, #IPSR11_INIT_DATA		/* IP11[19:16]=H'0, IP11[15:12]=H'0 */
#else
	ldr	r1, [r0, #IPSR12]
	and	r1, r1, #~IPSR12_MASK
	mov	r2, #IPSR12_INIT_DATA		/* IP12[7:4]=H'0, IP12[3:0]=H'0 */
#endif
	orr	r1, r1, r2
	mvn	r2, r1
	str	r2, [r0, #PMMR]
#if SCIF==SCIF1
	str	r1, [r0, #IPSR11]
#else
	str	r1, [r0, #IPSR12]
#endif

	ldr	r1, [r0, #GPSR5]
	and	r1, r1, #~GPSR5_MASK		/* GP2[12]=Peripheral function selected by IP4[23:22] */
	orr	r1, r1, #GPSR5_INIT_DATA	/* GP2[13]=Peripheral function selected by IP4[25:24] */
	mvn	r2, r1
	str	r2, [r0, #PMMR]
	str	r1, [r0, #GPSR5]

	ldr	r0, =SCIF_BASE
	mov	r1, #0
	strh	r1, [r0, #SCIF_SCSMRIR]
	/* Clear bits TE and RE in SCSCR to 0 */
	mov	r1, #(SCSCR_TE_DIS + SCSCR_RE_DIS)	/* TE=0,RE=0 */
	strh	r1, [r0, #SCIF_SCSCR]
	/* Set bits TFRST and RFRST in SCFCR to 1 */
	ldrh	r1, [r0, #SCIF_SCFCR]
	/* TFRESET=1,RFRESET=1 */
	orr	r1, r1, #(SCFCR_TFRST_EN + SCFCR_RFRS_EN)
	strh	r1, [r0, #SCIF_SCFCR]
	/* Read flags of ER, DR, BRK, and RDF in SCFSR 		   */
	/* and those of TO and ORER in SCLSR, then clear them to 0 */
	mov	r1, #SCFSR_INIT_DATA
	strh	r1, [r0, #SCIF_SCFSR]
	mov	r1, #0
	strh	r1, [r0, #SCIF_SCLSR]
	/* Set bits CKE[1:0] in SCSCR */
	ldrh	r1, [r0, #SCIF_SCSCR]
	and	r1, r1, #~SCSCR_CKE_MASK
	mov	r2, #SCSCR_CKE_INT_CLK		/* CKE=00 */
	orr	r1, r1, r2
	strh	r1, [r0, #SCIF_SCSCR]
	/* Set data transfer format in SCSMR */
	mov	r1, #SCSMR_INIT_DATA
	/* CA=0,CHR=0,PE=0,STOP=0,CKS=P */
	strh	r1, [r0, #SCIF_SCSMR]
	/* Set value in SCBRR */
#if SCIF_CLK == SCIF_INTERNAL_CLK
	ldr	r1, =PRR
	ldr	r1, [r1]
	ldr	r2, =PRR_PRODUCT_CUT_MASK
	and	r1, r1, r2
	mov	r2, #PRR_PRODUCT_H3_VER_10
	cmp	r1, r2
	beq	3f
	and	r1, r1, #PRR_PRODUCT_MASK
	mov	r2, #PRR_PRODUCT_E3
	cmp	r1, r2
	bne	4f

	ldr	r1, =RST_MODEMR
	ldr	r1, [r1]
	and	r1, r1, #MODEMR_MD12
	mov	r2, #MODEMR_MD12
	cmp	r1, r2
	bne	4f

	mov	r1, #SCBRR_115200BPS_E3_SSCG	/* 115200bps@60MHz SSCG */
	b	2f
4:
	mov	r1, #SCBRR_115200BPS		/* 115200bps */
	b	2f
3:
	/* H3 Ver.1.0 sets clock to doubling */
	mov	r1, #SCBRR_230400BPS
2:
	strb	r1, [r0, #SCIF_SCBRR]
#else
	mov	r1, #DL_INIT_DATA
	strh	r1, [r0, #SCIF_DL]
	mov	r1, #CKS_INIT_DATA		/* CKS=0,XIN=0 */
	strh	r1, [r0, #SCIF_CKS]
#endif
	/* 1-bit interval elapsed */
	mov	r1, #100			/* 100 times */
1:
	subs	r1, r1, #1
	bne	1b
	/* Set bits RTRG[1:0], TTRG[1:0], and MCE in SCFCR, */
	/* and clear bits FRST and RFRST to 0		    */
	mov	r1, #SCFCR_INIT_DATA
	strh	r1, [r0, #SCIF_SCFCR]
	/* Set bits TE and RE in SCSCR to 1 */
	ldrh	r1, [r0, #SCIF_SCSCR]
	orr	r1, r1, #(SCSCR_TE_EN + SCSCR_RE_EN)	/* TE=1,RE=1 */
	strh	r1, [r0, #SCIF_SCSCR]
	mov	r1, #1

	bx	lr

console_putc:
	ldr	r1, =SCIF_BASE
	cmp	r0, #0xA
	/* Prepend '\r' to '\n' */
	bne	2f
1:
	/* Check if the transmit FIFO is full */
	ldrh	r2, [r1, #SCIF_SCFDR]
	ubfx	r2, r2, #8, #5
	cmp	r2, #16
	bcs	1b
	mov	r2, #0x0D
	strb	r2, [r1, #SCIF_SCFTDR]
2:
	/* Check if the transmit FIFO is full */
	ldrh	r2, [r1, #SCIF_SCFDR]
	ubfx	r2, r2, #8, #5
	cmp	r2, #16
	bcs	2b
	strb	r0, [r1, #SCIF_SCFTDR]

	/* Clear TEND flag */
	ldrh	r2, [r1, #SCIF_SCFSR]
	and	r2, r2, #~SCFSR_TEND_MASK
	strh	r2, [r1, #SCIF_SCFSR]

	bx	lr


console_wait:
	ldr	r1, =SCIF_BASE
1:
	/* Check TEND flag */
	ldrh	r2, [r1, #SCIF_SCFSR]
	and	r2, r2, #SCFSR_TEND_MASK
	cmp	r2, #SCFSR_TEND_TRANS_END
	bne	1b

	bx	lr


	.end

