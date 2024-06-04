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

#ifndef __SCIF_H__
#define __SCIF_H__

#define BIT(nr)			(1UL << (nr))

#define RCAR_SCIF1_BASEADDR	0xE6E68000
#define RCAR_SCIF2_BASEADDR	0xE6E88000

#ifdef INTEGRITY_GUEST
	/* FIXME: Workaround for compatibility with INTEGRITY guest */
	#if SCIF==SCIF1
	#undef RCAR_BOARD
	#define RCAR_BOARD RCAR_SALVATOR_XS
	#endif
#endif

/*
 * On Salvator-XS and Ebisu boards, the main serial port is SCIF1.
 * On Salvator-XS, FreeRTOS uses SCIF1.
 * On E3 Ebisu board, if you are not running anything on the Cortex-A53 that
 * uses SCIF2, we can use it in FreeRTOS.
 */
#if (RCAR_BOARD == RCAR_SALVATOR_XS)
 #define STDOUT_BASEADDRESS RCAR_SCIF1_BASEADDR
#elif (RCAR_BOARD == RCAR_EBISU)
 #define STDOUT_BASEADDRESS RCAR_SCIF2_BASEADDR
#endif

#if (STDOUT_BASEADDRESS == RCAR_SCIF2_BASEADDR)
#define SCIF_INTR_ID				SCIF2_INTR_ID
#elif (STDOUT_BASEADDRESS == RCAR_SCIF1_BASEADDR)
#define SCIF_INTR_ID				SCIF1_INTR_ID
#else
#error "Define a UART port\n"
#endif


/* Product Register */
#define	PRR			(0xFFF00044)
#define	PRR_PRODUCT_MASK	(0x00007F00)
#define PRR_CUT_MASK		(0x000000FF)
#define PRR_PRODUCT_CUT_MASK	(PRR_PRODUCT_MASK | PRR_CUT_MASK)
#define	PRR_PRODUCT_H3_VER_10	(0x00004F00)	/* R-Car H3 Ver.1.0 */
#define	PRR_PRODUCT_E3		(0x00005700)	/* R-Car E3 */


#define	SCIF_INTERNAL_CLK	0	/* Internal clock(S3D4:66.66MHz) */
#define	SCIF_EXTARNAL_CLK	1	/* External clock(SCK2:14.7456MHz) */
#define	SCIF_CLK		    SCIF_INTERNAL_CLK
#define SCIF1               0
#define SCIF2               1
#if (STDOUT_BASEADDRESS == RCAR_SCIF2_BASEADDR)
#define	SCIF                SCIF2
#else
#define	SCIF                SCIF1
#endif


/* Pin functon */
#ifndef PFC_BASE
#define	PFC_BASE		(0xE6060000)
#endif

#define	PMMR			(0x0000)
#define	GPSR5			(0x0114)
#define	IPSR11			(0x022C)
#define	IPSR12			(0x0230)
#define	MOD_SEL1		(0x0504)

/* module stop */
#define	CPG_BASE		(0xE6150000)
#define	CPG_SMSTPCR3		(0x013C)
#define	CPG_RMSTPCR2		(0x0118)
#define RMSTP206		(1 << 6)

#define	MSTP310			(1 << 10)
#define	CPG_CPGWPR		(0x0900)

#define	SCIF_BASE		(STDOUT_BASEADDRESS)
#define	SCIF_SCSMR		0x00		/* Serial mode register */
#define	SCIF_SCBRR		0x04		/* Bit rate register */
#define	SCIF_SCSCR		0x08		/* Serial control register */
#define	SCIF_SCFTDR		0x0C		/* Transmit FIFO data register */
#define	SCIF_SCFSR		0x10		/* Serial status register */
#define SCIF_SCFRDR		0x14		/* Receive FIFO data register */
#define	SCIF_SCFCR		0x18		/* FIFO control register */
#define	SCIF_SCFDR		0x1C		/* FIFO data count register */
#define SCIF_SCSPTR		0x20		/* Serial port register */
#define	SCIF_SCLSR		0x24		/* Line status register */
#define	SCIF_DL			0x30		/* Frequency division register */
#define	SCIF_CKS		0x34		/* Clock Select register */
#define	SCIF_SCSMRIR	0x40		/* Serial mode register */

/* mode pin */
#define	RST_MODEMR		(0xE6160060)
#define	MODEMR_MD12		(0x00001000)	/* MD12 bit mask */

#define	SCSMR_CA_MASK		(1 << 7)
#define	SCSMR_CA_ASYNC		(0x0000)
#define	SCSMR_CHR_MASK		(1 << 6)
#define	SCSMR_CHR_8	    	(0x0000)
#define	SCSMR_PE_MASK		(1 << 5)
#define	SCSMR_PE_DIS		(0x0000)
#define	SCSMR_STOP_MASK		(1 << 3)
#define	SCSMR_STOP_1		(0x0000)
#define	SCSMR_CKS_MASK		(3 << 0)
#define	SCSMR_CKS_DIV1		(0x0000)
#define	SCSMR_INIT_DATA		(SCSMR_CA_ASYNC + SCSMR_CHR_8 + \
			                 SCSMR_PE_DIS + SCSMR_STOP_1 +	\
			                 SCSMR_CKS_DIV1)

#define	SCBRR_230400BPS		(8)		/* 230400bps@66MHz */
#define	SCBRR_115200BPS_E3_SSCG	(15)		/* 115200bps@60MHz SSCG */
#define	SCBRR_115200BPS		(17)		/* 115200bps@66MHz */
#define	SCBRR_INIT_DATA		(SCBRR_230400BPS)

#define	SCSCR_TE_MASK		(1 << 5)
#define	SCSCR_TE_DIS		(0x0000)
#define	SCSCR_TE_EN	    	(0x0020)
#define	SCSCR_RE_MASK		(1 << 4)
#define	SCSCR_RE_DIS		(0x0000)
#define	SCSCR_RE_EN	    	(0x0010)
#define	SCSCR_CKE_MASK		(3 << 0)
#define SCSCR_CKE_INT		(0x0000)
#define SCSCR_CKE_BRG		(0x0002)
#if SCIF_CLK == SCIF_EXTARNAL_CLK
#define	SCSCR_CKE_INT_CLK	(SCSCR_CKE_BRG)
#else
#define	SCSCR_CKE_INT_CLK	(SCSCR_CKE_INT)
#endif

#define	SCFSR_TEND_MASK		(1 << 6)
#define	SCFSR_TEND_TRANS_END	(0x0040)
#define	SCFSR_INIT_DATA		(0x0000)

#define	SCFCR_TTRG_MASK		(3 << 4)
#define	SCFCR_TTRG_8		(0x0000)
#define	SCFCR_TTRG_0		(0x0030)
#define	SCFCR_TFRST_MASK	(1 << 2)
#define	SCFCR_TFRST_DIS		(0x0000)
#define	SCFCR_TFRST_EN		(0x0004)
#define	SCFCR_RFRS_MASK		(1 << 1)
#define	SCFCR_RFRS_DIS		(0x0000)
#define	SCFCR_RFRS_EN		(0x0002)
#define	SCFCR_INIT_DATA		(SCFCR_TTRG_8)

#define SCFDR_T_MASK		(0x1f << 8)
#define SCFDR_R_MASK		(0x1f << 0)

#define	DL_INIT_DATA		(8)		/* 14.7456MHz/(115200bps*16)*/
			
#define	CKS_CKS_DIV_MASK	(1 << 15)
#define	CKS_CKS_DIV_CLK		(0x0000)
#define	CKS_XIN_MASK		(1 << 14)
#define	CKS_XIN_SCIF_CLK	(0x0000)
#define	CKS_INIT_DATA		(CKS_CKS_DIV_CLK + CKS_XIN_SCIF_CLK)

#if SCIF==SCIF1
#define	GPSR5_MASK	(3 << 5)
#else
#define	GPSR5_MASK	(3 << 10)
#endif

#define	GPSR5_RX1_A	(1 << 5)
#define	GPSR5_TX1_A	(1 << 6)
#define	GPSR5_RX2_A	(1 << 11)
#define	GPSR5_TX2_A	(1 << 10)
#define	GPSR5_SCK2	(1 << 9)

#if SCIF==SCIF1
#define	GPSR5_INIT_DATA		(GPSR5_RX1_A + GPSR5_TX1_A)
#else
#if SCIF_CLK == SCIF_EXTARNAL_CLK
#define	GPSR5_INIT_DATA		(GPSR5_RX2_A + GPSR5_TX2_A + GPSR5_SCK2)
#else
#define	GPSR5_INIT_DATA		(GPSR5_RX2_A + GPSR5_TX2_A)
#endif
#endif

#define	IPSR11_MASK	    	(255 << 0)
#define	IPSR11_RX1_A		(0 << 12)
#define	IPSR11_TX1_A		(0 << 16)
#define	IPSR11_INIT_DATA	(IPSR11_RX1_A + IPSR11_TX1_A)

#define	IPSR12_MASK	    	(255 << 0)
#define	IPSR12_RX2_A		(0 << 4)
#define	IPSR12_TX2_A		(0 << 0)
#define	IPSR12_INIT_DATA	(IPSR12_RX2_A + IPSR12_TX2_A)

#define	MOD_SEL1_SCIF2_MASK	(1 << 12)
#define	MOD_SEL1_SCIF2		(0 << 12)

#define	MOD_SEL1_SCIF1_MASK	(1 << 11)
#define	MOD_SEL1_SCIF1		(0 << 11)

/*----------------------------------------------------------------------------*/
#define SCIF0_INTR_ID		184
#define SCIF1_INTR_ID		185
#define SCIF2_INTR_ID		196
#define SCIF3_INTR_ID		55
#define SCIF4_INTR_ID		48
#define SCIF5_INTR_ID		49
/*----------------------------------------------------------------------------*/
/*		Name		Offset		Size	Init-val */
#define SCSMR		0x00	/* 16:	0x0000 */
#define SCBRR		0x04	/* 8:	0xFF */
#define SCSCR		0x08	/* 16:	0x0000 */
#define SCFTDR		0x0C	/* 8:	Undefined */
#define SCFSR		0x10	/* 16:	0x0060 */
#define SCFRDR		0x14	/* 8:	Undefined */
#define SCFCR		0x18	/* 16:	0x0000 */
#define SCFDR		0x1C	/* 16:	0x0000 */
#define SCSPTR		0x20	/* 16:	0x00XX (bit6,4,2,0 Undefined) */
#define SCLSR		0x24	/* 16:	0x0000 */
#define DL			0x30	/* 16:	0x0000 */
#define CKS			0x34	/* 16:	0x0000 */
/* Status Code */
#define SCIF_ER		BIT(7)
#define SCIF_TEND	BIT(6)
#define SCIF_TDFE	BIT(5)
#define SCIF_BRK	BIT(4)
#define SCIF_FER	BIT(3)
#define SCIF_PER	BIT(2)
#define SCIF_RDF	BIT(1)
#define SCIF_DR		BIT(0)
#define SCIF_ERRORS	(SCIF_PER | SCIF_FER | SCIF_ER | SCIF_BRK)


/* SCIF_INTRRUPT_OFFSET_BIT */
#define SCSCR_CKE		0x0001		/* bit[1:0] */
#define SCSCR_TOIE		BIT(2)
#define SCSCR_REIE		BIT(3)
#define SCSCR_RE		BIT(4)
#define SCSCR_TE		BIT(5)
#define SCSCR_RIE		BIT(6)
#define SCSCR_TIE		BIT(7)
#define SCSCR_TEIE		BIT(11)

#define SCIF_ORER					0x0001

#define	SCxSR_TEND					SCIF_TEND
#define	SCxSR_ERRORS				SCIF_ERRORS
#define	SCxSR_RDxF					SCIF_RDF
#define	SCxSR_TDxE					SCIF_TDFE
#define	SCxSR_FER					SCIF_FER
#define	SCxSR_PER					SCIF_PER
#define	SCxSR_BRK					SCIF_BRK
#define	SCxSR_ORER					SCIF_ORER



#endif	/* __SCIF_H__ */
