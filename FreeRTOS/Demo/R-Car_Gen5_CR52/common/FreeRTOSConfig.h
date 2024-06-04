/*
 * FreeRTOS Kernel
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* Toolchain integration */
#define configUSE_NEWLIB_REENTRANT 1    /* Required for thread-safety of newlib sprintf, dtoa, strtok, etc... */

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

/*
 * The FreeRTOS Cortex-A port implements a full interrupt nesting model.
 *
 * Interrupts that are assigned a priority at or below
 * configMAX_API_CALL_INTERRUPT_PRIORITY (which counter-intuitively in the ARM
 * generic interrupt controller [GIC] means a priority that has a numerical
 * value above configMAX_API_CALL_INTERRUPT_PRIORITY) can call FreeRTOS safe API
 * functions and will nest.
 *
 * Interrupts that are assigned a priority above
 * configMAX_API_CALL_INTERRUPT_PRIORITY (which in the GIC means a numerical
 * value below configMAX_API_CALL_INTERRUPT_PRIORITY) cannot call any FreeRTOS
 * API functions, will nest, and will not be masked by FreeRTOS critical
 * sections (although it is necessary for interrupts to be globally disabled
 * extremely briefly as the interrupt mask is updated in the GIC).
 *
 * FreeRTOS functions that can be called from an interrupt are those that end in
 * "FromISR".  FreeRTOS maintains a separate interrupt safe API to enable
 * interrupt entry to be shorter, faster, simpler and smaller.
 *
 * For the purpose of setting configMAX_API_CALL_INTERRUPT_PRIORITY 255
 * represents the lowest priority.
 */
#define configMAX_API_CALL_INTERRUPT_PRIORITY   18

#define configCPU_CLOCK_HZ						800000000UL
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	0
#define configUSE_TICKLESS_IDLE					0
#define configTICK_RATE_HZ						( ( TickType_t ) 1000 )
#define configPERIPHERAL_CLOCK_HZ  				( 33333000UL )
#define configUSE_PREEMPTION					1
#define configUSE_IDLE_HOOK						1
#define configUSE_TICK_HOOK						1
#define configMAX_PRIORITIES					( 256 )
#define configMINIMAL_STACK_SIZE				( ( unsigned short ) 200 )
#define configTOTAL_HEAP_SIZE					( 0x400000 )
#define configMAX_TASK_NAME_LEN					( 16 )
#define configUSE_16_BIT_TICKS					0
#define configIDLE_SHOULD_YIELD					1
#define configUSE_MUTEXES						1
#define configQUEUE_REGISTRY_SIZE				50
#define configCHECK_FOR_STACK_OVERFLOW			2
#define configUSE_RECURSIVE_MUTEXES				1
#define configUSE_MALLOC_FAILED_HOOK			1
#define configUSE_APPLICATION_TASK_TAG			1
#define configUSE_COUNTING_SEMAPHORES			1

/* This demo creates RTOS objects using both static and dynamic allocation. */
#define configSUPPORT_STATIC_ALLOCATION			1
#define configSUPPORT_DYNAMIC_ALLOCATION		1 /* Defaults to 1 anyway. */

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 					0
#define configMAX_CO_ROUTINE_PRIORITIES 		( 2 )

/* Software timer definitions. */
#define configUSE_TIMERS						1
#define configTIMER_TASK_PRIORITY				( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH				5
#define configTIMER_TASK_STACK_DEPTH			( configMINIMAL_STACK_SIZE * 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet				1
#define INCLUDE_uxTaskPriorityGet				1
#define INCLUDE_vTaskDelete						1
#define INCLUDE_vTaskCleanUpResources			1
#define INCLUDE_vTaskSuspend					1
#define INCLUDE_vTaskDelayUntil					1
#define INCLUDE_vTaskDelay						1
#define INCLUDE_xTimerPendFunctionCall			1
#define INCLUDE_eTaskGetState					1
#define INCLUDE_xTaskAbortDelay					1
#define INCLUDE_xTaskGetHandle					1
#define INCLUDE_xQueueGetMutexHolder 				1

// Fix me Renesas
//#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
/* Use FPU context for all tasks */
//#define configUSE_TASK_FPU_SUPPORT 2
//#else
/* Disable the FPU */
#define configUSE_TASK_FPU_SUPPORT 0
//#endif

/* This demo makes use of one or more example stats formatting functions.  These
format the raw data provided by the uxTaskGetSystemState() function in to human
readable ASCII form.  See the notes in the implementation of vTaskList() within
FreeRTOS/Source/tasks.c for limitations. */
#define configUSE_STATS_FORMATTING_FUNCTIONS	0

/* Run time stats are not generated.  portCONFIGURE_TIMER_FOR_RUN_TIME_STATS and
portGET_RUN_TIME_COUNTER_VALUE must be defined if configGENERATE_RUN_TIME_STATS
is set to 1. */
#define configGENERATE_RUN_TIME_STATS 0
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#define portGET_RUN_TIME_COUNTER_VALUE()

/* The size of the global output buffer that is available for use when there
are multiple command interpreters running at once (for example, one on a UART
and one on TCP/IP).  This is done to prevent an output buffer being defined by
each implementation - which would waste RAM.  In this case, there is only one
command interpreter running. */
#define configCOMMAND_INT_MAX_OUTPUT_SIZE 2096


#ifndef __ASSEMBLER__
/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
void vMainAssertCalled( const char *pcFileName, uint32_t ulLineNumber );
#define configASSERT( x ) if( ( x ) == 0 ) { vMainAssertCalled( __FILE__, __LINE__ ); }

/* Handle tasks that return because they have finished running */
void vDeleteCallingTask( void );
#define configTASK_RETURN_ADDRESS vDeleteCallingTask


/****** Hardware specific settings. *******************************************/

#define configINTERRUPT_CONTROLLER_BASE_ADDRESS         ( 0xF0001000U )
#define configINTERRUPT_CONTROLLER_CPU_INTERFACE_OFFSET ( 0xF0000100U - 0xF0001000U )
#define configUNIQUE_INTERRUPT_PRIORITIES               32

/*
 * The application must provide a function that configures a peripheral to
 * create the FreeRTOS tick interrupt, then define configSETUP_TICK_INTERRUPT()
 * in FreeRTOSConfig.h to call the function.  This file contains a function
 * that is suitable for use on R-Car.  FreeRTOS_Tick_Handler() must
 * be installed as the peripheral's interrupt handler.
 */
void vConfigureTickInterrupt( void );
#define configSETUP_TICK_INTERRUPT() vConfigureTickInterrupt()

void vClearTickInterrupt( void );
#define configCLEAR_TICK_INTERRUPT() vClearTickInterrupt()

#define fabs( x ) __builtin_fabs( x )

/*
 * Trace
 * Look up the address and size of RecorderData in the map file, e.g.
 * $ sort *.map | grep -wA1 RecorderData
 * 40054704 B RecorderData
 * 40074778 B init_hwtc_count
 *
 * Using a debugger dump this memory to a file. For example with Lauterbach:
 *  Data.SAVE.Binary trace.bin 0x40054704..0x40074778
 * You can then load the file into Percepio Tracealyzer.
 */
#define configUSE_TRACE_FACILITY				0

/*
 * Save the current task control block to the ContextIDR. This allows debuggers
 * to set breakpoints associated with a ContextIDR so you can step though tasks.
 */
#if !(configUSE_TRACE_FACILITY)
#define traceTASK_SWITCHED_IN() \
{ \
	__asm volatile ("mcr p15, 0x0,%0,c13,c0,0x1" : :"r" (pxCurrentTCB)); \
}
#endif

#define RCAR_BOOTTIME_MEASURE 1

#if RCAR_BOOTTIME_MEASURE
 /* When measuring boot time, you can enable the console but it will affect it */
 #define configUSE_CONSOLE_TASK					0
#else
 #define configUSE_CONSOLE_TASK					1
#endif /* RCAR_BOOTTIME_MEASURE */

/* Ebisu board only has 1 UART and that is used by the Cortex-A53 cores.
 * If you don't run anything on the CA53 you can enable the FreeRTOS console */
#if (RCAR_BOARD == RCAR_EBISU)
 #undef configUSE_CONSOLE_TASK
 #define configUSE_CONSOLE_TASK					0
#endif

#include "trcRecorder.h" /* Should be in the end, after the #define. */

#endif /* __ASSEMBLER__ */

#endif /* FREERTOS_CONFIG_H */

