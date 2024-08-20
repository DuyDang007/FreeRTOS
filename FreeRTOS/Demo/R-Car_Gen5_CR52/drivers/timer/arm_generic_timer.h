#ifndef __arm_generic_timer_h
#define __arm_generic_timer_h

#include "cmsis_cp15.h"

#define CNTFRQ_READ()         __get_CNTFRQ()
#define CNTFRQ_WRITE(v)       __set_CNTFRQ(v)
#define CNTPCT_READ()         __get_CNTPCT()
#define CNTP_CTL_WRITE(v)     __set_CNTP_CTL(v)
#define CNTP_CVAL_READ()      __get_CNTP_CVAL()
#define CNTP_CVAL_WRITE(v)    __set_CNTP_CVAL(v)
#define GENERIC_TIMER_CLK     25000000

#endif // __arm_generic_timer_h
