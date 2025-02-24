/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef __arm_generic_timer_h
#define __arm_generic_timer_h

#ifndef __ASSEMBLER__
#include "cmsis_cp15.h"

#define CNTFRQ_READ()         __get_CNTFRQ()
#define CNTFRQ_WRITE(v)       __set_CNTFRQ(v)
#define CNTPCT_READ()         __get_CNTPCT()
#define CNTP_CTL_WRITE(v)     __set_CNTP_CTL(v)
#define CNTP_CVAL_READ()      __get_CNTP_CVAL()
#define CNTP_CVAL_WRITE(v)    __set_CNTP_CVAL(v)

#endif // __ASSEMBLER__

#define GENERIC_TIMER_CLK     25000000UL

#endif // __arm_generic_timer_h
