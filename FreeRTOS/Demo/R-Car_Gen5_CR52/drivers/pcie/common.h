/**********************************************************/
/* Sample program : T/M Common Module Header              */
/* File Name      : common.h                              */
/* Copyright (C) Renesas Electronics Corp. 2024.          */
/**********************************************************/

#ifndef __COMMON_H_
#define __COMMON_H_

typedef signed long long int	int64_t;
typedef signed int				int32_t;
typedef signed short			int16_t;
typedef signed char				int8_t;

typedef unsigned long long int	uint64_t;
typedef unsigned int			uint32_t;
typedef unsigned short			uint16_t;
typedef unsigned char			uint8_t;

//#define NO_DDR_DEBUG

#ifdef AArch64
typedef uint64_t				uintptr_t;
#define CPU_BYTE_SIZE			SIZE_64BIT
#endif

#ifdef AArch32
//typedef uint32_t				uintptr_t;
#define CPU_BYTE_SIZE			SIZE_32BIT
#endif


/******************************************************************************/
/* INCLUDE FILE                                                               */
/******************************************************************************/

#include "bit.h"
#include "mem_io.h"
//#include "log_func.h"
//#include "paratbl.h"
//#include "tmmodule.h"
//#include "dbgserial_drv.h"
//#include "armasm.h"
//#include "ramckmdl.h"

//#include "debugmode.h"
//#include "product.h"
#include "stdfunc.h"

#include "reg_rcar.h"

/******************************************************************************/
/* PUBLIC CONSTANT DEFINE                                                     */
/******************************************************************************/
#define		DIS_RTN				0		/* Disable Return				*/
#define		ENB_RTN				1		/* Enable Return				*/

#define		INT_CODE	0x25			/* "%"					*/
#define		BS_CODE		0x08			/* "BS"					*/
#define		CR_CODE		0x0d			/* "CR"					*/
#define		SP_CODE		0x20			/* " "					*/
#define		LF_CODE		0x0a			/* "LF"					*/
#define		CTRL_A_CODE	0x01			/* Ctrl+A				*/
#define		CTRL_B_CODE	0x02			/* Ctrl+B				*/
#define		CTRL_C_CODE	0x03			/* Ctrl+C				*/
#define		CTRL_D_CODE	0x04			/* Ctrl+D				*/
#define		CTRL_S_CODE	0x13			/* Ctrl+S				*/

#define		ACCESS_8BIT		1		// Old name : BYTE_ACCESS
#define		ACCESS_16BIT	2		// Old name : WORD_ACCESS
#define		ACCESS_32BIT	4		// Old name : LONG_ACCESS
#define		ACCESS_64BIT	8		// New

#define		SIZE_8BIT		1		// Old name : BYTE_SIZE
#define		SIZE_16BIT		2		// Old name : WORD_SIZE
#define		SIZE_32BIT		4		// Old name : LONG_SIZE
#define		SIZE_64BIT		8		// New

/******************************************************************************/
/* PUBLIC MACRO                                                               */
/******************************************************************************/
#define countof(n)				(sizeof(n)/sizeof(n[0]))


/******************************************************************************/
/* GLOBAL VARIABLE EXTERN                                                     */
/******************************************************************************/

extern const char CODE_BASE_ADDR[];
extern const char WORK_RAM_ADDR[];

extern const char SP_BASE_ADDR[];

extern const char SP_BORDER_START[];
extern const char SP_BORDER_END[];

extern const char SP_BASE_CPU0[];


/******************************************************************************/
/* GLOBAL FUNCTION PROTOTYPE                                                  */
/******************************************************************************/
void mprintf(const char *format,...);
void RtnFirmAsm(void);

#endif		/* __COMMON_H_ */
