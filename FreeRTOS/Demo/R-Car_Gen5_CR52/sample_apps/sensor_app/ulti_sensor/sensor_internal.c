/*
 *
 * Copyright (c) 2026 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "sensor_internal.h"

/*
 * Register Offsets (from R-Car X5H User Manual Table 27.2).
 *
 * Trip point interrupt flow (Figure 27.6):
 *   1. Hardware comparator triggers when TEMP crosses IRQTEMPx threshold
 *   2. IRQSTR (status) bit is set automatically by hardware
 *   3. IRQEN (enable) gates whether the comparator is active
 *   4. IRQMSK (mask) gates whether the interrupt propagates to GIC
 *
 *   To use trip points: set IRQEN bit to enable, IRQMSK bit to unmask.
 *   To clear: write 0 to IRQSTR bit (W0C = Write-0-to-Clear).
 */
#define SCP_THS_REG_IRQSTR                      (0x0004U)   /**< Temperature Error Status (R/WC0) */
#define SCP_THS_REG_IRQMSK                      (0x0008U)   /**< Temperature Error Mask (R/W) */
#define SCP_THS_REG_IRQEN                       (0x0010U)   /**< Temperature Error Enable (R/W) */
#define SCP_THS_REG_IRQTEMP1                    (0x0014U)   /**< Threshold: IRQTEMP1[11:0], IRQTEMP4[27:16] */
#define SCP_THS_REG_IRQTEMP2                    (0x0018U)   /**< Threshold: IRQTEMP2[11:0], IRQTEMP5[27:16] */
#define SCP_THS_REG_IRQTEMP3                    (0x001CU)   /**< Threshold: IRQTEMP3[11:0], IRQTEMP6[27:16] */
#define SCP_THS_REG_THCTR                       (0x0020U)   /**< Control: TH_EN[5], THSST[0], CIVMTST[20] */
#define SCP_THS_REG_THSTR                       (0x0024U)   /**< Status (R) */
#define SCP_THS_REG_TEMP                        (0x0028U)   /**< Temperature TEMP_CODE[11:0] (R) */
#define SCP_THS_REG_IRQ_INJECTION               (0x006CU)   /**< Error Injection (R/W) */
#define SCP_THS_REG_TSC_ERROR_CTL               (0x0074U)   /**< TSC Error Control (R/W) */
#define SCP_THS_REG_SEQ_RESET                   (0x007CU)   /**< Sequence Reset (R/W) */
#define SCP_THS_REG_CIVMTST_VOLT1               (0x008CU)   /**< CIVM Test Voltage1 [12:0] (R) [FuSa] */
#define SCP_THS_REG_CIVMTST_VOLT2               (0x0090U)   /**< CIVM Test Voltage2 [12:0] (R) [FuSa] */
#define SCP_THS_REG_OP_CTRL                     (0x00A0U)   /**< Operation Control: OP_MODE[0] (R/W) */
#define SCP_THS_REG_TSC_ERROR_MON               (0x0130U)   /**< TSC Error Monitor (R) */
#define SCP_THS_REG_SEQ_ACT_MON                 (0x0140U)   /**< Sequence Act Monitor (R) */
#define SCP_THS_REG_THSFMON00                   (0x0180U)   /**< Fuse: THCODE_L (THCODE3) */
#define SCP_THS_REG_THSFMON01                   (0x0184U)   /**< Fuse: THCODE_U (THCODE1) */
#define SCP_THS_REG_THSFMON02                   (0x0188U)   /**< Fuse: THCODE_R (THCODE2) */
#define SCP_THS_REG_THSFMON03                   (0x018CU)   /**< Fuse: PTAT_L (PTAT3) */
#define SCP_THS_REG_THSFMON04                   (0x0190U)   /**< Fuse: PTAT_U (PTAT1) */
#define SCP_THS_REG_THSFMON05                   (0x0194U)   /**< Fuse: PTAT_R (PTAT2) */

/* Number of TSC (Thermal Sensor Circuit) instances on R-Car X5H.
 * Each TSC module provides independent temperature measurement with
 * its own A/D converter, calibration fuses, and trip point logic.
 * See R-Car X5H User Manual Chapter 27: Thermal Sensor (THS). */
#define SCP_THS_TSC_COUNT                       (4U)
/*******************************************************************************************************************//**
 * @enum e_scp_ths_state_t
 * THS hardware state machine states per Section 27.3.1 of R-Car X5H User Manual.
***********************************************************************************************************************/
typedef enum
{
    SCP_THS_STATE_STANDBY       = 0,    /**< THS disabled, initial state after reset */
    SCP_THS_STATE_IDLE          = 1,    /**< THS enabled, A/D converter off */
    SCP_THS_STATE_NORMAL        = 2,    /**< THS measuring temperature continuously */
    SCP_THS_STATE_FAILURE_TEST  = 3,    /**< THS failure test mode */
    SCP_THS_STATE_FAULT         = 4,    /**< Fault detected, needs SEQ_RESET */
    SCP_THS_STATE_COUNT                 /**< Number of states */
} e_scp_ths_state_t;
/* Per-TSC state machine state */
static e_scp_ths_state_t s_ths_state[SCP_THS_TSC_COUNT] =
{
    SCP_THS_STATE_STANDBY,
    SCP_THS_STATE_STANDBY,
    SCP_THS_STATE_STANDBY,
    SCP_THS_STATE_STANDBY,
};
/*******************************************************************************************************************//**
 * @enum e_scp_ths_event_t
 * THS state machine events that trigger state transitions.
***********************************************************************************************************************/
typedef enum
{
    SCP_THS_EVENT_ENABLE        = 0,    /**< Enable THS (Standby->Idle) */
    SCP_THS_EVENT_DISABLE       = 1,    /**< Disable THS (Idle->Standby) */
    SCP_THS_EVENT_START_MEAS    = 2,    /**< Start measurement (Idle->Normal) */
    SCP_THS_EVENT_STOP_MEAS     = 3,    /**< Stop measurement (Normal->Idle) */
    SCP_THS_EVENT_START_FTEST   = 4,    /**< Start failure test (Idle->FailureTest) */
    SCP_THS_EVENT_FTEST_DONE    = 5,    /**< Failure test complete (FailureTest->Idle) */
    SCP_THS_EVENT_HW_ERROR      = 6,    /**< Hardware error (Normal/FailureTest->Fault) */
    SCP_THS_EVENT_SEQ_RESET     = 7,    /**< Sequence reset (Fault->Idle) */
    SCP_THS_EVENT_COUNT                 /**< Number of events */
} e_scp_ths_event_t;
/* TEMP register mask */
#define SCP_THS_TEMP_CODE_MASK                  (0x00000FFFUL)  /**< 12-bit TEMP_CODE */
/*******************************************************************************************************************//**
 * @brief  Action function type for state machine transitions.
 * @param  tsc_id  TSC instance index (0-3).
 * @return SCP_RET_SUCCESS on success.
***********************************************************************************************************************/
typedef e_scp_retcode_t (*p_scp_ths_action_t)(uint32_t tsc_id);
/*******************************************************************************************************************//**
 * @struct st_scp_ths_state_machine_transition_t
 * A single row in the state machine lookup table.
***********************************************************************************************************************/
typedef struct
{
    e_scp_ths_state_t   current_state;  /**< Required current state */
    e_scp_ths_event_t   event;          /**< Triggering event */
    e_scp_ths_state_t   next_state;     /**< State after transition */
    p_scp_ths_action_t  action;         /**< Action to execute (NULL = no action) */
} st_scp_ths_state_machine_transition_t;

#define SCP_THS_STATE_MACHINE_TABLE_SIZE  (sizeof(s_ths_state_machine_table) / sizeof(s_ths_state_machine_table[0]))
/*
 * THCTR (THS Control Register, offset 0x20) bit masks.
 * Controls the THS operating mode. See Table 27.11:
 *
 *   Bit[20] CIVMTST : Failure test mode (0=normal, 1=enter failure test)
 *   Bit[5]  TH_EN   : THS power enable (0=standby, 1=enabled)
 *   Bit[0]  THSST   : Measurement start (0=idle, 1=measuring)
 *
 * Mode transition sequence:
 *   Standby->Idle:   TH_EN 0->1 (wait 100us)
 *   Idle->Normal:    THSST  0->1 (wait 2ms for A/D stabilization)
 *   Normal->Idle:    THSST  1->0
 *   Idle->Standby:   TH_EN  1->0
 */
#define SCP_THS_THCTR_TH_EN                     (1UL << 5)
#define SCP_THS_THCTR_THSST                     (1UL << 0)
#define SCP_THS_THCTR_CIVMTST                   (1UL << 20)
/* Trip point count per THS module */
#define SCP_THS_TRIP_POINT_COUNT                (6U)
/* Max polling iterations for CIVMTST auto-clear in 1st test (Figure 27.10).
 * Each iteration waits 100us, so 500 iterations = 50ms timeout. */
#define SCP_THS_FTEST_POLL_MAX_RETRIES          (500U)
/* CIVMTST_VOLT register mask (13-bit value) */
#define SCP_THS_CIVMTST_VOLT_MASK               (0x00001FFFUL)  /**< 13-bit CIVMTST_VOLT_CODE */
/*
 * THS Characteristic Failure Test validation ranges (Section 27.5.1.1).
 *
 * 1st Test (Figure 27.10): CIVMTST_VOLT1 -> $VTHIU_VREFn, CIVMTST_VOLT2 -> $VTHIL_VREFn
 * These values must fall within Table 27.5 ranges.
 *
 * T.B.D: Update when characterization data is available (expected in UM rev 0.8).
 * Note: Silicon measurements show VOLT1 (VTHIU) values around 0x0F80-0x0FCA,
 *       VOLT2 (VTHIL) values around 0x0620-0x0644. Upper bounds widened accordingly.
 */
#define SCP_THS_FTEST_VTHIU_MIN                 (0x0100U)   /**< T.B.D: Min $VTHIU_VREFn */
#define SCP_THS_FTEST_VTHIU_MAX                 (0x1100U)   /**< T.B.D: Max $VTHIU_VREFn */
#define SCP_THS_FTEST_VTHIL_MIN                 (0x0100U)   /**< T.B.D: Min $VTHIL_VREFn */
#define SCP_THS_FTEST_VTHIL_MAX                 (0x1100U)   /**< T.B.D: Max $VTHIL_VREFn */
/*******************************************************************************************************************//**
 * @enum e_scp_timer_duration_t
 * Pre-defined timer durations used by SCP firmware subsystems.
 * Values represent the duration in microseconds.
 * Using an enum avoids magic numbers in caller code.
 **********************************************************************************************************************/
typedef enum
{
    SCP_TIMER_DURATION_100US    = 100U,     /**< 100 us — THS TH_EN stabilization (Section 27.3.2) */
    SCP_TIMER_DURATION_1MS      = 1000U,    /**< 1 ms   — General purpose */
    SCP_TIMER_DURATION_2MS      = 2000U,    /**< 2 ms   — THS THSST A/D stabilization (Section 27.3.3.3) */
    SCP_TIMER_DURATION_10MS     = 10000U,   /**< 10 ms  — General purpose */
    SCP_TIMER_DURATION_100MS    = 100000U   /**< 100 ms — General purpose */
} e_scp_timer_duration_t;

static e_scp_retcode_t r_scp_ths_state_machine_process_event(uint32_t tsc_id, e_scp_ths_event_t event);
static e_scp_retcode_t r_scp_ths_action_enable(uint32_t tsc_id);
static e_scp_retcode_t r_scp_ths_action_disable(uint32_t tsc_id);
static e_scp_retcode_t r_scp_ths_action_start_meas(uint32_t tsc_id);
static e_scp_retcode_t r_scp_ths_action_start_ftest(uint32_t tsc_id);
static e_scp_retcode_t r_scp_ths_action_stop_meas(uint32_t tsc_id);
static e_scp_retcode_t r_scp_ths_action_hw_error(uint32_t tsc_id);
static e_scp_retcode_t r_scp_ths_action_ftest_done(uint32_t tsc_id);
static e_scp_retcode_t r_scp_ths_action_seq_reset(uint32_t tsc_id);
static e_scp_retcode_t r_scp_ths_sensor_activate(uint32_t tsc_id);
static e_scp_retcode_t r_scp_ths_sensor_deactivate(uint32_t tsc_id);

/*
 * State Machine Lookup Table — defines all valid state transitions.
 * Each row: { current_state, event, next_state, action }
 *
 * Based on R-Car X5H THS state machine (Section 27.3.1, Table 27.2):
 *   Standby --[ENABLE]--> Idle         (Mode Entry 2: TH_EN 0->1, wait 100us)
 *   Idle    --[DISABLE]--> Standby     (Mode Entry 1: TH_EN 1->0)
 *   Idle    --[START_MEAS]--> Normal   (Mode Entry 3: THSST 0->1, wait 2ms)
 *   Normal  --[STOP_MEAS]--> Idle      (Mode Entry 4: THSST 1->0, clear IRQ)
 *   Idle    --[START_FTEST]--> FTest   (Mode Entry 5: CIVMTST 0->1)
 *   FTest   --[FTEST_DONE]--> Idle     (HW auto-clears CIVMTST)
 *   Normal  --[HW_ERROR]--> Fault
 *   FTest   --[HW_ERROR]--> Fault
 *   Fault   --[SEQ_RESET]--> Idle      (SEQ_RESET toggle 0->1->0)
 */
static const st_scp_ths_state_machine_transition_t s_ths_state_machine_table[] =
{
    /* current_state,               event,                     next_state,                  action */
    { SCP_THS_STATE_STANDBY,        SCP_THS_EVENT_ENABLE,      SCP_THS_STATE_IDLE,          r_scp_ths_action_enable      },

    { SCP_THS_STATE_IDLE,           SCP_THS_EVENT_DISABLE,     SCP_THS_STATE_STANDBY,       r_scp_ths_action_disable     },
    { SCP_THS_STATE_IDLE,           SCP_THS_EVENT_START_MEAS,  SCP_THS_STATE_NORMAL,        r_scp_ths_action_start_meas  },
    { SCP_THS_STATE_IDLE,           SCP_THS_EVENT_START_FTEST, SCP_THS_STATE_FAILURE_TEST,  r_scp_ths_action_start_ftest },

    { SCP_THS_STATE_NORMAL,         SCP_THS_EVENT_STOP_MEAS,   SCP_THS_STATE_IDLE,          r_scp_ths_action_stop_meas   },
    { SCP_THS_STATE_NORMAL,         SCP_THS_EVENT_HW_ERROR,    SCP_THS_STATE_FAULT,         r_scp_ths_action_hw_error    },

    { SCP_THS_STATE_FAILURE_TEST,   SCP_THS_EVENT_HW_ERROR,    SCP_THS_STATE_FAULT,         r_scp_ths_action_hw_error    },
    { SCP_THS_STATE_FAILURE_TEST,   SCP_THS_EVENT_FTEST_DONE,  SCP_THS_STATE_IDLE,          r_scp_ths_action_ftest_done  },

    { SCP_THS_STATE_FAULT,          SCP_THS_EVENT_SEQ_RESET,   SCP_THS_STATE_IDLE,          r_scp_ths_action_seq_reset   },
};


#define SCP_LOG(level, ...)
/* SCP debug log */
#define SCP_LOG_DBG(...)    SCP_LOG(SCP_LOG_LEVEL_DBG, __VA_ARGS__)
/* SCP information log */
#define SCP_LOG_INFO(...)   SCP_LOG(SCP_LOG_LEVEL_INFO, __VA_ARGS__)
/* SCP error log */
#define SCP_LOG_ERR(...)    SCP_LOG(SCP_LOG_LEVEL_ERR, "[Error] " __VA_ARGS__)
/* TSC Base Addresses (from R-Car X5H User Manual Table 1.6).
 * Each TSC module occupies a 0x8000 byte MMIO region.
 * These addresses are physical addresses accessed directly
 * from the CR52 (Cortex-R52) core running the SCP firmware. */
#define SCP_THS_TSC1_BASE                       (0x0C6520000ULL)
#define SCP_THS_TSC2_BASE                       (0x0C6528000ULL)
#define SCP_THS_TSC3_BASE                       (0x0C6530000ULL)
#define SCP_THS_TSC4_BASE                       (0x0C6538000ULL)
/*
 * Temperature calibration constants from R-Car X5H User Manual
 * Section 27.3.3.4 (temperature conversion formula).
 *
 * The THS uses a 3-point calibration scheme:
 *   - PTAT1/THCODE1 : measured at upper temperature point
 *   - PTAT2/THCODE2 : measured at room temperature point
 *   - PTAT3/THCODE3 : measured at low temperature point
 *
 * Constants are empirically derived for each SoC revision:
 *   167, 41  : used in the low-temp-range formula
 *   167, 126 : used in the high-temp-range formula
 *   60       : Tj_T calculation offset
 */
#define SCP_THS_CALIB_CONST_167                 (167L)
#define SCP_THS_CALIB_CONST_41                  (41L)
#define SCP_THS_CALIB_CONST_126                 (126L)
#define SCP_THS_CALIB_CONST_60                  (60L)
/*
 * TMU0 register offsets.
 *
 * Register map (channel 0 within TMU0 module):
 *   TSTR  (0x04) : Timer Start Register — bit 0 starts/stops channel 0
 *   TCOR0 (0x08) : Timer Constant Register — auto-reload value on underflow
 *   TCNT0 (0x0C) : Timer Counter — 32-bit down-counter, read/write
 *   TCR0  (0x10) : Timer Control Register — clock select, interrupt enable, UNF flag
 */
#define SCP_TMU0_REG_TSTR                       (0x04U)
#define SCP_TMU0_REG_TCOR0                      (0x08U)
#define SCP_TMU0_REG_TCNT0                      (0x0CU)
#define SCP_TMU0_REG_TCR0                       (0x10U)
/*
 * TSTR (Timer Start Register) bit masks.
 *   Bit[0] STR0 : 1 = channel 0 counting, 0 = channel 0 stopped
 */
#define SCP_TMU0_TSTR_STR0                      (0x01U)
/*
 * TCR0 (Timer Control Register) bit layout:
 *
 *   Bit[8]   UNF  : Underflow flag (R/W, write 0 to clear)
 *                    Set to 1 by hardware when TCNT underflows.
 *   Bit[5]   UNIE : Underflow Interrupt Enable
 *                    1 = interrupt enabled on underflow, 0 = disabled
 *   Bits[2:0] TPSC : Timer Prescaler
 *                    000 = internal clock (no division = SASYNCD4_RT_MAIN)
 *                    001 = clock/4,  010 = clock/16, 011 = clock/64
 *                    100 = clock/256, 101 = clock/1024
 */
#define SCP_TMU0_TCR_UNF                        (1U << 8)
#define SCP_TMU0_TCR_UNIE                       (1U << 5)
#define SCP_TMU0_TCR_TPSC_MASK                  (0x07U)
#define SCP_TMU0_TCR_TPSC_DIV1                  (0x00U)    /**< No prescaler */
/*
 * TMU0 input clock frequency: SASYNCD4_RT_MAIN = 16.667 MHz.
 * With no prescaler (TPSC = 000), the counter decrements at this rate.
 *
 * Tick-to-microsecond conversion:
 *   count = duration_us * SCP_TMU0_CLOCK_FREQ_KHZ / 1000
 *
 * Using kHz (16667) avoids floating-point and keeps integer precision:
 *   100 us  → 100 * 16667 / 1000 = 1667 ticks
 *   2000 us → 2000 * 16667 / 1000 = 33334 ticks
 */
#define SCP_TMU0_CLOCK_FREQ_KHZ                 (16667U)
/*
 * Convert a duration in microseconds to TMU0 counter ticks.
 *
 *   ticks = duration_us * clock_freq_kHz / 1000
 *
 * Example: 100 us * 16667 kHz / 1000 = 1666.7 → 1667 ticks (rounded up via +999).
 * We round up to ensure the actual delay is >= the requested duration.
 */
#define SCP_TMU0_US_TO_TICKS(us)    (((uint32_t)(us) * SCP_TMU0_CLOCK_FREQ_KHZ + 999U) / 1000U)
/*
 * TMU0 register base address on R-Car X5H.
 * TMU0 is allocated to the CM33 (SCP) core.
 * See R-Car X5H User Manual Chapter 164: Timer Unit (TMU).
 */
#define SCP_TMU0_BASE                           (0x1C030000UL)
/* THSFMON register mask */
#define SCP_THS_FUSE_DATA_MASK                  (0x00000FFFUL)  /**< 12-bit fuse data */
/*
 * Default calibration values when OTP fuses are not programmed (all zero).
 * These are approximate values derived from R-Car Gen3/Gen4 family typical
 * characteristics.  Temperature readings using these values will be approximate
 * but functional.  The CM33 driver logs "CALIB FALLBACK" when these are used.
 *
 *   THCODE1 (THCODE_U) = upper temperature calibration code  (~126 deg C)
 *   THCODE2 (THCODE_R) = room temperature calibration code   (~25 deg C)
 *   THCODE3 (THCODE_L) = low temperature calibration code    (~-41 deg C)
 *   PTAT1   (PTAT_U)   = upper PTAT reference value
 *   PTAT2   (PTAT_R)   = room PTAT reference value
 *   PTAT3   (PTAT_L)   = low PTAT reference value
 */
#define SCP_THS_CALIB_DEFAULT_THCODE1            (3383U)   /* 0xd37 — from X5H board OTP */
#define SCP_THS_CALIB_DEFAULT_THCODE2            (2751U)   /* 0xabf — from X5H board OTP */
#define SCP_THS_CALIB_DEFAULT_THCODE3            (2080U)   /* 0x820 — from X5H board OTP */
#define SCP_THS_CALIB_DEFAULT_PTAT1              (2260U)   /* 0x8d4 — from X5H board OTP */
#define SCP_THS_CALIB_DEFAULT_PTAT2              (1450U)   /* 0x5aa — from X5H board OTP */
#define SCP_THS_CALIB_DEFAULT_PTAT3              (590U)    /* 0x24e — from X5H board OTP */
/* TSC base address table */
static const uint64_t s_tsc_base_addr[SCP_THS_TSC_COUNT] =
{
    SCP_THS_TSC1_BASE,
    SCP_THS_TSC2_BASE,
    SCP_THS_TSC3_BASE,
    SCP_THS_TSC4_BASE,
};
/*******************************************************************************************************************//**
 * @struct st_scp_ths_calib_t
 * Factory calibration parameters read from THSFMON registers.
***********************************************************************************************************************/
typedef struct
{
    uint32_t thcode1;   /**< THCODE_U (upper temperature calibration code) */
    uint32_t thcode2;   /**< THCODE_R (room temperature calibration code) */
    uint32_t thcode3;   /**< THCODE_L (low temperature calibration code) */
    uint32_t ptat1;     /**< PTAT_U (upper PTAT value) */
    uint32_t ptat2;     /**< PTAT_R (room PTAT value) */
    uint32_t ptat3;     /**< PTAT_L (low PTAT value) */
} st_scp_ths_calib_t;
/***********************************************************************************************************************
* Start of function r_scp_ths_set_sensor_enabled()
* @brief  Enable or disable a TSC instance.
*         Delegates to r_scp_ths_sensor_activate() or r_scp_ths_sensor_deactivate().
***********************************************************************************************************************/
e_scp_retcode_t r_scp_ths_set_sensor_enabled(uint32_t tsc_id, uint32_t enable)
{
    if (tsc_id >= SCP_THS_TSC_COUNT)
    {
        return SCP_RET_INVALID_PARAMETERS;
    }

    if (0U != enable)
    {
        return r_scp_ths_sensor_activate(tsc_id);
    }
    else
    {
        return r_scp_ths_sensor_deactivate(tsc_id);
    }
}
/***********************************************************************************************************************
* Start of function r_scp_ths_get_sensor_enabled()
* @brief  Returns whether a TSC instance is currently enabled.
*         Enabled means the hardware is in Idle or Normal state (TH_EN=1).
*         Callers do not need to inspect state machine internals.
***********************************************************************************************************************/
e_scp_retcode_t r_scp_ths_get_sensor_enabled(uint32_t tsc_id, uint32_t *enabled)
{
    if ((tsc_id >= SCP_THS_TSC_COUNT) || (NULL == enabled))
    {
        return SCP_RET_INVALID_PARAMETERS;
    }

    *enabled = ((SCP_THS_STATE_IDLE == s_ths_state[tsc_id]) ||
                (SCP_THS_STATE_NORMAL == s_ths_state[tsc_id]))
               ? 1U : 0U;

    return SCP_RET_SUCCESS;
}
/***********************************************************************************************************************
* Start of function r_scp_ths_sensor_activate()
* @brief  Activate a single TSC instance: Standby -> Idle.
*
* Sequence:
*   1. If already in Idle or Normal state: return success immediately.
*   2. If in Standby: trigger ENABLE event (Standby -> Idle, sets TH_EN=1, reads calibration).
*
* Note: Characteristic tests (1st and 2nd) are only performed during bootup
*       in r_scp_ths_init(). Post-boot activation does not re-run tests.
***********************************************************************************************************************/
static e_scp_retcode_t r_scp_ths_sensor_activate(uint32_t tsc_id)
{
    if (tsc_id >= SCP_THS_TSC_COUNT)
    {
        return SCP_RET_INVALID_PARAMETERS;
    }

    /* Already active — no action needed */
    if ((SCP_THS_STATE_IDLE == s_ths_state[tsc_id]) ||
        (SCP_THS_STATE_NORMAL == s_ths_state[tsc_id]))
    {
        return SCP_RET_SUCCESS;
    }

    if (SCP_THS_STATE_STANDBY != s_ths_state[tsc_id])
    {
        SCP_LOG_ERR("THS[%u] activate: unexpected current state=%d\n",
                     tsc_id, (int)s_ths_state[tsc_id]);
        return SCP_RET_PROTOCOL_ERROR;
    }

    /* Standby -> Idle (TH_EN: 0->1, wait 100us, read calibration fuses) */
    return r_scp_ths_state_machine_process_event(tsc_id, SCP_THS_EVENT_ENABLE);
}
/***********************************************************************************************************************
* End of function r_scp_ths_sensor_activate()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function r_scp_ths_sensor_deactivate()
* @brief  Deactivate a single TSC instance: transition to Standby.
*
* Sequence:
*   1. If already in Standby: return success immediately.
*   2. If in Normal state: STOP_MEAS event (Normal -> Idle).
*   3. If in Idle state: DISABLE event (Idle -> Standby, TH_EN: 1->0).
***********************************************************************************************************************/
static e_scp_retcode_t r_scp_ths_sensor_deactivate(uint32_t tsc_id)
{
    e_scp_retcode_t ercd = SCP_RET_SUCCESS;

    if (tsc_id >= SCP_THS_TSC_COUNT)
    {
        return SCP_RET_INVALID_PARAMETERS;
    }

    /* Already in standby — nothing to do */
    if (SCP_THS_STATE_STANDBY == s_ths_state[tsc_id])
    {
        return SCP_RET_SUCCESS;
    }

    /* If measuring, stop first: Normal -> Idle */
    if (SCP_THS_STATE_NORMAL == s_ths_state[tsc_id])
    {
        ercd = r_scp_ths_state_machine_process_event(tsc_id, SCP_THS_EVENT_STOP_MEAS);
        if (SCP_RET_SUCCESS != ercd)
        {
            SCP_LOG_ERR("THS[%u] deactivate: failed to stop measurement, ret=%d\n",
                         tsc_id, (int)ercd);
            return ercd;
        }
    }

    /* Idle -> Standby */
    if (SCP_THS_STATE_IDLE == s_ths_state[tsc_id])
    {
        ercd = r_scp_ths_state_machine_process_event(tsc_id, SCP_THS_EVENT_DISABLE);
        if (SCP_RET_SUCCESS != ercd)
        {
            SCP_LOG_ERR("THS[%u] deactivate: failed to disable, ret=%d\n", tsc_id, (int)ercd);
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* Start of function r_scp_ths_reg_read()
* @brief  Read a 32-bit TSC register.
* @param  tsc_id   TSC instance index (0-3), selects the base address.
* @param  offset   Register offset from the TSC base (e.g. SCP_THS_REG_TEMP).
* @return The 32-bit value read from the register.
***********************************************************************************************************************/
static uint32_t r_scp_ths_reg_read(uint32_t tsc_id, uint32_t offset)
{
    volatile uint32_t *reg = (volatile uint32_t *)(uintptr_t)(s_tsc_base_addr[tsc_id] + offset);
    return *reg;
}
/***********************************************************************************************************************
* Start of function r_scp_ths_convert_temp_code()
* Converts TEMP_CODE to temperature in millidegrees Celsius.
*
* Formula from R-Car X5H User Manual Section 27.3.3.4:
*   When TEMP_CODE < THCODE2:
*     T = (167 * (PTAT3 - PTAT2) * (THCODE3 - TEMP_CODE)) /
*         ((PTAT3 - PTAT1) * (THCODE3 - THCODE2)) - 41
*   When TEMP_CODE >= THCODE2:
*     T = (167 * (PTAT1 - PTAT2) * (THCODE1 - TEMP_CODE)) /
*         ((PTAT1 - PTAT3) * (THCODE2 - THCODE1)) + 126
*
* Return value is in millidegrees C (multiply by 1000).
***********************************************************************************************************************/
static int32_t r_scp_ths_convert_temp_code(uint32_t temp_code, const st_scp_ths_calib_t *calib)
{
    int32_t temp_c;
    int64_t numerator;
    int64_t denominator;

    if (temp_code < calib->thcode2)
    {
        numerator   = (int64_t)SCP_THS_CALIB_CONST_167 *
                      ((int64_t)calib->ptat3 - (int64_t)calib->ptat2) *
                      ((int64_t)calib->thcode3 - (int64_t)temp_code);
        denominator = ((int64_t)calib->ptat3 - (int64_t)calib->ptat1) *
                      ((int64_t)calib->thcode3 - (int64_t)calib->thcode2);

        if (denominator != 0)
        {
            temp_c = (int32_t)(numerator / denominator) - (int32_t)SCP_THS_CALIB_CONST_41;
        }
        else
        {
            temp_c = 0;
        }
    }
    else
    {
        numerator   = (int64_t)SCP_THS_CALIB_CONST_167 *
                      ((int64_t)calib->ptat1 - (int64_t)calib->ptat2) *
                      ((int64_t)calib->thcode1 - (int64_t)temp_code);
        denominator = ((int64_t)calib->ptat1 - (int64_t)calib->ptat3) *
                      ((int64_t)calib->thcode2 - (int64_t)calib->thcode1);

        if (denominator != 0)
        {
            temp_c = (int32_t)(numerator / denominator) + (int32_t)SCP_THS_CALIB_CONST_126;
        }
        else
        {
            temp_c = 0;
        }
    }

    /* Convert degrees C to millidegrees C */
    return temp_c * 1000;
}
/* Per-TSC calibration data (cached on init) */
static st_scp_ths_calib_t s_ths_calib[SCP_THS_TSC_COUNT];
e_scp_retcode_t r_scp_ths_read_temperature(uint32_t tsc_id, int32_t *temp_mc){
    e_scp_retcode_t ercd;
    uint32_t temp_code;

    if ((tsc_id >= SCP_THS_TSC_COUNT) || (NULL == temp_mc))
    {
        return SCP_RET_INVALID_PARAMETERS;
    }

    /* THS must be in IDLE state; measurement is triggered on-demand per agent request */
    if (SCP_THS_STATE_IDLE != s_ths_state[tsc_id])
    {
        return SCP_RET_PROTOCOL_ERROR;
    }

    /* Start measurement on-demand: Idle -> Normal (THSST=1, wait 2ms, validate TEMP_CODE) */
    ercd = r_scp_ths_state_machine_process_event(tsc_id, SCP_THS_EVENT_START_MEAS);
    if (SCP_RET_SUCCESS != ercd)
    {
        /* START_MEAS returned HARDWARE_ERROR: first TEMP_CODE was already outside a
         * configured threshold. State is now NORMAL. Follow spec FSM fault path:
         *   Normal mode -> [Error] -> Detected fault -> [Sequencer reset] -> THS idle mode. */
        (void)r_scp_ths_state_machine_process_event(tsc_id, SCP_THS_EVENT_HW_ERROR);
        (void)r_scp_ths_state_machine_process_event(tsc_id, SCP_THS_EVENT_SEQ_RESET);
        return ercd;
    }

    /* Read TEMP_CODE (available after 2ms A/D stabilization in START_MEAS) */
    temp_code = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_TEMP) & SCP_THS_TEMP_CODE_MASK;

    /* Convert to millidegrees C using calibration data */
    *temp_mc = r_scp_ths_convert_temp_code(temp_code, &s_ths_calib[tsc_id]);

    /* Stop measurement: Normal -> Idle (THSST=0, wait 2ms, clear IRQEN/IRQMSK) */
    (void)r_scp_ths_state_machine_process_event(tsc_id, SCP_THS_EVENT_STOP_MEAS);

    return SCP_RET_SUCCESS;
}

/***********************************************************************************************************************
* Start of function r_scp_ths_state_machine_process_event()
* @brief  Core state machine engine: look up (current_state, event) in the transition
*         table and execute the corresponding action + state change.
*
* Algorithm:
*   1. Validate tsc_id bounds.
*   2. Linear-scan s_ths_state_machine_table[] for a row matching
*      (current_state == s_ths_state[tsc_id]) AND (event == requested event).
*   3. If found: execute row.action(tsc_id), then transition to row.next_state.
*   4. If not found: return INVALID_PARAMETERS (illegal state/event combo).
*
* State transitions happen even on HARDWARE_ERROR returns so that the
* state machine properly enters the FAULT state when an error is detected.
***********************************************************************************************************************/
static e_scp_retcode_t r_scp_ths_state_machine_process_event(uint32_t tsc_id, e_scp_ths_event_t event)
{
    e_scp_retcode_t ercd = SCP_RET_INVALID_PARAMETERS;
    uint32_t i;

    if (tsc_id >= SCP_THS_TSC_COUNT)
    {
        return SCP_RET_INVALID_PARAMETERS;
    }

    /* Search the lookup table for a matching (current_state, event) pair */
    for (i = 0U; i < SCP_THS_STATE_MACHINE_TABLE_SIZE; i++)
    {
        if ((s_ths_state_machine_table[i].current_state == s_ths_state[tsc_id]) &&
            (s_ths_state_machine_table[i].event == event))
        {
            /* Execute the action */
            if (NULL != s_ths_state_machine_table[i].action)
            {
                ercd = s_ths_state_machine_table[i].action(tsc_id);
            }
            else
            {
                ercd = SCP_RET_SUCCESS;
            }

            /* Transition to the next state */
            if (SCP_RET_SUCCESS == ercd || SCP_RET_HARDWARE_ERROR == ercd)
            {
                s_ths_state[tsc_id] = s_ths_state_machine_table[i].next_state;
            }
            break;
        }
    }

    if (i >= SCP_THS_STATE_MACHINE_TABLE_SIZE)
    {
        SCP_LOG_ERR("THS[%u] state machine: no transition for state=%d event=%d\n",
                     tsc_id, (int)s_ths_state[tsc_id], (int)event);
    }

    return ercd;
}
/***********************************************************************************************************************
* Start of function r_scp_ths_reg_write()
* @brief  Write a 32-bit value to a TSC register.
* @param  tsc_id   TSC instance index (0-3), selects the base address.
* @param  offset   Register offset from the TSC base.
* @param  value    The 32-bit value to write.
***********************************************************************************************************************/
static void r_scp_ths_reg_write(uint32_t tsc_id, uint32_t offset, uint32_t value)
{
    volatile uint32_t *reg = (volatile uint32_t *)(uintptr_t)(s_tsc_base_addr[tsc_id] + offset);
    *reg = value;
}
/***********************************************************************************************************************
* Start of function r_scp_mode_entry_2()
* @brief  Mode Entry 2: THS standby mode → THS idle mode (TH_EN: 0→1).
*         See Table 27.3 and Figure 27.3 (Setting from Standby Mode to Idle Mode).
* @param  tsc_id   TSC instance index (0-3).
***********************************************************************************************************************/
static void r_scp_mode_entry_2(uint32_t tsc_id)
{
    uint32_t thctr = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_THCTR);
    thctr |= SCP_THS_THCTR_TH_EN;
    r_scp_ths_reg_write(tsc_id, SCP_THS_REG_THCTR, thctr);
}

/***********************************************************************************************************************
* Start of function r_scp_timer_reg_write8()
* @brief  Write an 8-bit value to a TMU0 register.
* @param  offset   Register offset from TMU0 base.
* @param  value    The 8-bit value to write.
***********************************************************************************************************************/
static void r_scp_timer_reg_write8(uint32_t offset, uint8_t value)
{
    volatile uint8_t *reg = (volatile uint8_t *)(uintptr_t)(SCP_TMU0_BASE + offset);
    *reg = value;
}
/***********************************************************************************************************************
* Start of function r_scp_timer_reg_read8()
* @brief  Read an 8-bit TMU0 register.
* @param  offset   Register offset from TMU0 base.
* @return The 8-bit register value.
***********************************************************************************************************************/
static uint8_t r_scp_timer_reg_read8(uint32_t offset)
{
    volatile uint8_t *reg = (volatile uint8_t *)(uintptr_t)(SCP_TMU0_BASE + offset);
    return *reg;
}
/***********************************************************************************************************************
* Start of function r_scp_timer_reg_write16()
* @brief  Write a 16-bit value to a TMU0 register (TCR0).
* @param  offset   Register offset from TMU0 base.
* @param  value    The 16-bit value to write.
***********************************************************************************************************************/
static void r_scp_timer_reg_write16(uint32_t offset, uint16_t value)
{
    volatile uint16_t *reg = (volatile uint16_t *)(uintptr_t)(SCP_TMU0_BASE + offset);
    *reg = value;
}
/***********************************************************************************************************************
* Start of function r_scp_timer_reg_read16()
* @brief  Read a 16-bit TMU0 register (TCR0).
* @param  offset   Register offset from TMU0 base.
* @return The 16-bit register value.
***********************************************************************************************************************/
static uint16_t r_scp_timer_reg_read16(uint32_t offset)
{
    volatile uint16_t *reg = (volatile uint16_t *)(uintptr_t)(SCP_TMU0_BASE + offset);
    return *reg;
}
/***********************************************************************************************************************
* Start of function r_scp_timer_reg_write32()
* @brief  Write a 32-bit value to a TMU0 register (TCOR0 / TCNT0).
* @param  offset   Register offset from TMU0 base.
* @param  value    The 32-bit value to write.
***********************************************************************************************************************/
static void r_scp_timer_reg_write32(uint32_t offset, uint32_t value)
{
    volatile uint32_t *reg = (volatile uint32_t *)(uintptr_t)(SCP_TMU0_BASE + offset);
    *reg = value;
}
/***********************************************************************************************************************
* Start of function r_scp_timer_start_sync()
* @brief  Start a synchronous (blocking) one-shot timer.
*
* Sequence:
*   1. Stop timer if running
*   2. Clear UNF flag in TCR0
*   3. Load TCOR0 with tick count (auto-reload value, not used for one-shot)
*   4. Load TCNT0 with tick count
*   5. Start counter (TSTR.STR0 = 1)
*   6. Poll TCR0.UNF until it becomes 1 (underflow occurred)
*   7. Stop counter and clear UNF
*
* @param  duration_us  Duration in microseconds. Use e_scp_timer_duration_t enum values.
***********************************************************************************************************************/
void r_scp_timer_start_sync(uint32_t duration_us)
{
    uint32_t ticks;
    uint16_t tcr;

    if (0U == duration_us)
    {
        return;
    }

    ticks = SCP_TMU0_US_TO_TICKS(duration_us);
    if (0U == ticks)
    {
        ticks = 1U;
    }

    /* Stop timer if running */
    r_scp_timer_reg_write8(SCP_TMU0_REG_TSTR,
        r_scp_timer_reg_read8(SCP_TMU0_REG_TSTR) & (uint8_t)(~SCP_TMU0_TSTR_STR0));

    /* Clear UNF flag (write 0 to bit 8), keep prescaler setting, no interrupt */
    r_scp_timer_reg_write16(SCP_TMU0_REG_TCR0, SCP_TMU0_TCR_TPSC_DIV1);

    /* Load counter value */
    r_scp_timer_reg_write32(SCP_TMU0_REG_TCOR0, ticks);
    r_scp_timer_reg_write32(SCP_TMU0_REG_TCNT0, ticks);

    /* Start counter */
    r_scp_timer_reg_write8(SCP_TMU0_REG_TSTR,
        r_scp_timer_reg_read8(SCP_TMU0_REG_TSTR) | SCP_TMU0_TSTR_STR0);

    /* Poll for underflow */
    do
    {
        tcr = r_scp_timer_reg_read16(SCP_TMU0_REG_TCR0);
    } while (0U == (tcr & (uint16_t)SCP_TMU0_TCR_UNF));

    /* Stop counter */
    r_scp_timer_reg_write8(SCP_TMU0_REG_TSTR,
        r_scp_timer_reg_read8(SCP_TMU0_REG_TSTR) & (uint8_t)(~SCP_TMU0_TSTR_STR0));

    /* Clear UNF */
    r_scp_timer_reg_write16(SCP_TMU0_REG_TCR0, SCP_TMU0_TCR_TPSC_DIV1);
}
/***********************************************************************************************************************
* Start of function r_scp_ths_read_calibration()
* @brief  Read the 6 factory calibration fuse values for a TSC module.
* @param  tsc_id   TSC instance index (0-3).
* @param  calib    Output structure to store the calibration data.
*
* Register-to-field mapping (12-bit values, upper bits reserved):
*   THSFMON00 -> thcode3 (THCODE_L = low temperature calibration code)
*   THSFMON01 -> thcode1 (THCODE_U = upper temperature calibration code)
*   THSFMON02 -> thcode2 (THCODE_R = room temperature calibration code)
*   THSFMON03 -> ptat3   (PTAT_L   = low PTAT reference value)
*   THSFMON04 -> ptat1   (PTAT_U   = upper PTAT reference value)
*   THSFMON05 -> ptat2   (PTAT_R   = room PTAT reference value)
***********************************************************************************************************************/
static void r_scp_ths_read_calibration(uint32_t tsc_id, st_scp_ths_calib_t *calib)
{
    calib->thcode3 = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_THSFMON00) & SCP_THS_FUSE_DATA_MASK;
    calib->thcode1 = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_THSFMON01) & SCP_THS_FUSE_DATA_MASK;
    calib->thcode2 = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_THSFMON02) & SCP_THS_FUSE_DATA_MASK;
    calib->ptat3   = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_THSFMON03) & SCP_THS_FUSE_DATA_MASK;
    calib->ptat1   = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_THSFMON04) & SCP_THS_FUSE_DATA_MASK;
    calib->ptat2   = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_THSFMON05) & SCP_THS_FUSE_DATA_MASK;

    /* If all OTP fuses are zero (unprogrammed engineering sample silicon),
     * the 3-point calibration formula will divide by zero and return 0 degC.
     * Use approximate default values derived from R-Car Gen3/Gen4 typical
     * characteristics so that temperature measurement is functional. */
    if ((0U == calib->thcode1) && (0U == calib->thcode2) && (0U == calib->thcode3) &&
        (0U == calib->ptat1)   && (0U == calib->ptat2)   && (0U == calib->ptat3))
    {
        SCP_LOG_ERR("THS[%u] CALIB FALLBACK: OTP fuses all zero, using default calibration\n", tsc_id);
        calib->thcode1 = SCP_THS_CALIB_DEFAULT_THCODE1;
        calib->thcode2 = SCP_THS_CALIB_DEFAULT_THCODE2;
        calib->thcode3 = SCP_THS_CALIB_DEFAULT_THCODE3;
        calib->ptat1   = SCP_THS_CALIB_DEFAULT_PTAT1;
        calib->ptat2   = SCP_THS_CALIB_DEFAULT_PTAT2;
        calib->ptat3   = SCP_THS_CALIB_DEFAULT_PTAT3;
    }
    else
    {
        SCP_LOG_ERR("THS[%u] CALIB OTP: thcode1=%u thcode2=%u thcode3=%u ptat1=%u ptat2=%u ptat3=%u\n",
                     tsc_id, calib->thcode1, calib->thcode2, calib->thcode3,
                     calib->ptat1, calib->ptat2, calib->ptat3);
    }
}
/***********************************************************************************************************************
* Start of function r_scp_ths_action_enable()
* Figure 27.3: Standby -> Idle flow.
*   Step 1: Execute Mode Entry 2 (TH_EN: 0->1)
*   Step 2: Wait 100 μs
*   End: THS idle mode. Read calibration fuses (valid only after TH_EN=1).
***********************************************************************************************************************/
static e_scp_retcode_t r_scp_ths_action_enable(uint32_t tsc_id)
{
    /* Figure 27.3: Execute Mode Entry 2 (TH_EN: 0->1) */
    r_scp_mode_entry_2(tsc_id);

    /* Wait 100us for THS to stabilize (Figure 27.3, Usage Note 1 in Section 27.4) */
    r_scp_timer_start_sync((uint32_t)SCP_TIMER_DURATION_100US);

    /* Read calibration data now that THS is in idle mode */
    r_scp_ths_read_calibration(tsc_id, &s_ths_calib[tsc_id]);

    return SCP_RET_SUCCESS;
}
/***********************************************************************************************************************
* Start of function r_scp_mode_entry_1()
* @brief  Mode Entry 1: THS idle mode → THS standby mode (TH_EN: 1→0).
*         See Table 27.3 and Figure 27.9 (Standby Mode Setting).
* @param  tsc_id   TSC instance index (0-3).
***********************************************************************************************************************/
static void r_scp_mode_entry_1(uint32_t tsc_id)
{
    uint32_t thctr = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_THCTR);
    thctr &= ~SCP_THS_THCTR_TH_EN;
    r_scp_ths_reg_write(tsc_id, SCP_THS_REG_THCTR, thctr);
}
/***********************************************************************************************************************
* Start of function r_scp_mode_entry_3()
* @brief  Mode Entry 3: THS idle mode → Normal mode (THSST: 0→1).
*         See Table 27.3 and Figure 27.7 (Normal Mode Start Setting).
* @param  tsc_id   TSC instance index (0-3).
***********************************************************************************************************************/
static void r_scp_mode_entry_3(uint32_t tsc_id)
{
    uint32_t thctr = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_THCTR);
    thctr |= SCP_THS_THCTR_THSST;
    r_scp_ths_reg_write(tsc_id, SCP_THS_REG_THCTR, thctr);
}
/***********************************************************************************************************************
* Start of function r_scp_mode_entry_4()
* @brief  Mode Entry 4: Normal mode → THS idle mode (THSST: 1→0).
*         See Table 27.3 and Figure 27.8 (Normal Mode Stop Setting).
* @param  tsc_id   TSC instance index (0-3).
***********************************************************************************************************************/
static void r_scp_mode_entry_4(uint32_t tsc_id)
{
    uint32_t thctr = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_THCTR);
    thctr &= ~SCP_THS_THCTR_THSST;
    r_scp_ths_reg_write(tsc_id, SCP_THS_REG_THCTR, thctr);
}
/***********************************************************************************************************************
* Start of function r_scp_mode_entry_5()
* @brief  Mode Entry 5: THS idle mode → THS failure test mode (CIVMTST: 0→1).
*         See Table 27.3 and Figure 27.10 (Flow Chart of the 1st Test).
* @param  tsc_id   TSC instance index (0-3).
***********************************************************************************************************************/
static void r_scp_mode_entry_5(uint32_t tsc_id)
{
    uint32_t thctr = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_THCTR);
    thctr |= SCP_THS_THCTR_CIVMTST;
    r_scp_ths_reg_write(tsc_id, SCP_THS_REG_THCTR, thctr);
}
/***********************************************************************************************************************
* Start of function r_scp_ths_action_disable()
* Figure 27.9: Standby Mode Setting (Idle -> Standby flow).
*   Step 1: Execute Mode Entry 1 (TH_EN: 1->0)
*   End: THS standby mode.
***********************************************************************************************************************/
static e_scp_retcode_t r_scp_ths_action_disable(uint32_t tsc_id)
{
    /* Figure 27.9: Execute Mode Entry 1 (TH_EN: 1->0) */
    r_scp_mode_entry_1(tsc_id);

    return SCP_RET_SUCCESS;
}
/***********************************************************************************************************************
* Start of function r_scp_ths_get_trip_reg_info()
* Maps trip_id (0-5) to register offset, bit shift, and IRQEN bit positions.
*
* IRQTEMP register layout:
*   IRQTEMP1 (0x14): bits[11:0]=IRQTEMP1, bits[27:16]=IRQTEMP4
*   IRQTEMP2 (0x18): bits[11:0]=IRQTEMP2, bits[27:16]=IRQTEMP5
*   IRQTEMP3 (0x1C): bits[11:0]=IRQTEMP3, bits[27:16]=IRQTEMP6
*
* IRQEN bit layout:
*   bits[0-2]:      TEMP1_EN,   TEMP2_EN,   TEMP3_EN    (exceed threshold)
*   bits[3-5]:      TEMPD1_EN,  TEMPD2_EN,  TEMPD3_EN   (fall below threshold)
*   bits[16-18]:    TEMP4_EN,   TEMP5_EN,   TEMP6_EN    (exceed threshold)
*   bits[19-21]:    TEMPD4_EN,  TEMPD5_EN,  TEMPD6_EN   (fall below threshold)
***********************************************************************************************************************/
static void r_scp_ths_get_trip_reg_info(uint32_t trip_id, uint32_t *reg_offset,
                                         uint32_t *bit_shift, uint32_t *en_exceed_bit,
                                         uint32_t *en_fall_bit)
{
    if (trip_id < 3U)
    {
        /* Trip 0-2 map to IRQTEMP1-3 bits[11:0] */
        *reg_offset   = SCP_THS_REG_IRQTEMP1 + (trip_id * 4U);
        *bit_shift    = 0U;
        *en_exceed_bit = (1UL << trip_id);        /* bits 0, 1, 2 */
        *en_fall_bit   = (1UL << (trip_id + 3U)); /* bits 3, 4, 5 */
    }
    else
    {
        /* Trip 3-5 map to IRQTEMP1-3 bits[27:16] */
        *reg_offset   = SCP_THS_REG_IRQTEMP1 + ((trip_id - 3U) * 4U);
        *bit_shift    = 16U;
        *en_exceed_bit = (1UL << (trip_id - 3U + 16U));  /* bits 16, 17, 18 */
        *en_fall_bit   = (1UL << (trip_id - 3U + 19U));  /* bits 19, 20, 21 */
    }
}
/***********************************************************************************************************************
* Start of function r_scp_ths_action_start_meas()
* Figure 27.7: Normal Mode Start Setting (Idle -> Normal flow).
*   Pre-condition: IRQMSK, IRQTEMP1-6, IRQEN are configured via r_scp_ths_set_trip_point()
*                 before this function is called (Section 27.3.3.3).
*   Step 1: Execute Mode Entry 3 (THSST: 0->1)
*   Step 2: Wait 2.0 ms
*   Step 3: Validate first TEMP_CODE against all configured threshold ranges (Figure 27.7).
*           Section 27.3.3.2: THS error detection is edge-based and requires the previous
*           measurement to be inside the threshold in order to fire. If the first reading is
*           already outside a configured threshold, HW will never detect that crossing.
*           Software must therefore verify the first result is within range.
*           If any violation is found: undo Normal mode (Mode Entry 4, wait 2ms, clear
*           IRQEN/IRQMSK per Figure 27.8) and return PROTOCOL_ERROR so the state machine
*           stays in IDLE. The caller should reconfigure trip points before retrying.
*   End (on success): Normal mode.
***********************************************************************************************************************/
static e_scp_retcode_t r_scp_ths_action_start_meas(uint32_t tsc_id)
{
    uint32_t temp_code;
    uint32_t irqen;
    uint32_t irqtemp_reg;
    uint32_t threshold_val;
    uint32_t trip_id;
    uint32_t reg_offset;
    uint32_t bit_shift;
    uint32_t en_exceed_bit;
    uint32_t en_fall_bit;
    uint32_t out_of_range = 0U;

    /* Figure 27.7: Execute Mode Entry 3 (THSST: 0->1) */
    r_scp_mode_entry_3(tsc_id);

    /* Wait 2ms for A/D converter stabilization (Figure 27.7) */
    r_scp_timer_start_sync((uint32_t)SCP_TIMER_DURATION_2MS);

    /* Figure 27.7 / Section 27.3.3.2: Validate the first TEMP_CODE reading.
     * For each threshold that is enabled in IRQEN, check whether TEMP_CODE already
     * violates it. A violation here means the system started Normal mode in an
     * out-of-range condition, and the edge-based HW comparator will never catch it. */
    temp_code = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_TEMP) & SCP_THS_TEMP_CODE_MASK;
    irqen     = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_IRQEN);

    for (trip_id = 0U; trip_id < SCP_THS_TRIP_POINT_COUNT; trip_id++)
    {
        r_scp_ths_get_trip_reg_info(trip_id, &reg_offset, &bit_shift, &en_exceed_bit, &en_fall_bit);

        irqtemp_reg   = r_scp_ths_reg_read(tsc_id, reg_offset);
        threshold_val = (irqtemp_reg >> bit_shift) & SCP_THS_TEMP_CODE_MASK;

        /* TEMP_CODE already exceeds an enabled "rise-above" threshold */
        if ((0U != (irqen & en_exceed_bit)) && (temp_code > threshold_val))
        {
            SCP_LOG_ERR("THS[%u] normal start: TEMP_CODE=0x%03x already exceeds trip[%u]=0x%03x\n",
                         tsc_id, temp_code, trip_id, threshold_val);
            out_of_range = 1U;
        }

        /* TEMP_CODE already fell below an enabled "fall-below" threshold */
        if ((0U != (irqen & en_fall_bit)) && (temp_code < threshold_val))
        {
            SCP_LOG_ERR("THS[%u] normal start: TEMP_CODE=0x%03x already below trip[%u]=0x%03x\n",
                         tsc_id, temp_code, trip_id, threshold_val);
            out_of_range = 1U;
        }
    }

    if (0U != out_of_range)
    {
        /* Do NOT undo Normal mode here. Hardware is in Normal mode (THSST=1).
         * Return HARDWARE_ERROR so the state machine transitions IDLE -> NORMAL.
         * The caller must then route through the spec FSM fault path:
         *   Normal mode -> [Error] -> Detected fault -> [Sequencer reset] -> THS idle mode. */
        return SCP_RET_HARDWARE_ERROR;
    }

    return SCP_RET_SUCCESS;
}
/***********************************************************************************************************************
* Start of function r_scp_ths_action_start_ftest()
* Figure 27.10: Flow Chart of the 1st Test (Idle -> Failure Test flow).
*   Step 1: Execute Mode Entry 5 (CIVMTST: 0->1)
*   Step 2: Wait for HW to auto-clear CIVMTST (poll: CIVMTST=0?)
*   Step 3: Read CIVMTST_VOLT1 → hold in $VTHIU_VREFn
*   Step 4: Validate $VTHIU_VREFn against Table 27.5 range
*   Step 5: Read CIVMTST_VOLT2 → hold in $VTHIL_VREFn
*   Step 6: Validate $VTHIL_VREFn against Table 27.5 range
*   End: TSCn THS idle mode (HW auto-returns after CIVMTST clears).
***********************************************************************************************************************/
static e_scp_retcode_t r_scp_ths_action_start_ftest(uint32_t tsc_id)
{
    uint32_t thctr;
    uint32_t volt1;
    uint32_t volt2;
    uint32_t retries;

    /* Figure 27.10: Execute Mode Entry 5 (CIVMTST: 0->1) */
    r_scp_mode_entry_5(tsc_id);

    /* Poll for HW to auto-clear CIVMTST (Figure 27.10: "THS failure test end? CIVMTST=0?") */
    retries = SCP_THS_FTEST_POLL_MAX_RETRIES;
    do
    {
        r_scp_timer_start_sync((uint32_t)SCP_TIMER_DURATION_100US);
        thctr = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_THCTR);
    } while ((0U != (thctr & SCP_THS_THCTR_CIVMTST)) && (--retries > 0U));

    if (0U != (thctr & SCP_THS_THCTR_CIVMTST))
    {
        SCP_LOG_ERR("THS[%u] 1st test: CIVMTST not auto-cleared (THCTR=0x%08x)\n",
                     tsc_id, thctr);
        return SCP_RET_HARDWARE_ERROR;
    }

    /* Read and validate CIVMTST_VOLT1 ($VTHIU_VREFn) — Table 27.5 */
    volt1 = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_CIVMTST_VOLT1) & SCP_THS_CIVMTST_VOLT_MASK;
    if ((volt1 < SCP_THS_FTEST_VTHIU_MIN) || (volt1 > SCP_THS_FTEST_VTHIU_MAX))
    {
        SCP_LOG_ERR("THS[%u] 1st test: VOLT1=0x%04x out of range [0x%04x..0x%04x]\n",
                     tsc_id, volt1, (uint32_t)SCP_THS_FTEST_VTHIU_MIN, (uint32_t)SCP_THS_FTEST_VTHIU_MAX);
        return SCP_RET_HARDWARE_ERROR;
    }

    /* Read and validate CIVMTST_VOLT2 ($VTHIL_VREFn) — Table 27.5 */
    volt2 = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_CIVMTST_VOLT2) & SCP_THS_CIVMTST_VOLT_MASK;
    if ((volt2 < SCP_THS_FTEST_VTHIL_MIN) || (volt2 > SCP_THS_FTEST_VTHIL_MAX))
    {
        SCP_LOG_ERR("THS[%u] 1st test: VOLT2=0x%04x out of range [0x%04x..0x%04x]\n",
                     tsc_id, volt2, (uint32_t)SCP_THS_FTEST_VTHIL_MIN, (uint32_t)SCP_THS_FTEST_VTHIL_MAX);
        return SCP_RET_HARDWARE_ERROR;
    }

    return SCP_RET_SUCCESS;
}
/***********************************************************************************************************************
* Start of function r_scp_ths_action_stop_meas()
* Figure 27.8: Normal Mode Stop Setting (Normal -> Idle flow).
*   Step 1: Execute Mode Entry 4 (THSST: 1->0)
*   Step 2: Wait 2.0 ms
*   Step 3: Write B'0 to all bits in IRQEN register
*   Step 4: Write B'0 to all bits in IRQMSK register
*   End: THS idle mode.
***********************************************************************************************************************/
static e_scp_retcode_t r_scp_ths_action_stop_meas(uint32_t tsc_id)
{
    /* Figure 27.8: Execute Mode Entry 4 (THSST: 1->0) */
    r_scp_mode_entry_4(tsc_id);

    /* Wait 2ms for mode transition to complete (Figure 27.8) */
    r_scp_timer_start_sync((uint32_t)SCP_TIMER_DURATION_2MS);

    /* Write B'0 to all bits in IRQEN register (Figure 27.8) */
    r_scp_ths_reg_write(tsc_id, SCP_THS_REG_IRQEN, 0U);

    /* Write B'0 to all bits in IRQMSK register (Figure 27.8) */
    r_scp_ths_reg_write(tsc_id, SCP_THS_REG_IRQMSK, 0U);

    return SCP_RET_SUCCESS;
}
/***********************************************************************************************************************
* Start of function r_scp_ths_action_hw_error()
* Normal/FailureTest -> Fault. Log the error.
*
* TSC_ERROR_MON (r0p70) bit layout:
*   Bit[7] ERR_ACKTOV  : Acknowledge timeout
*   Bit[4] ERR_THCODE  : THCODE is all 0
*   Bit[3] ERR_THCNTOV : Counter overflow
*   Bit[2] Reserved    : (was THFAIL1 in r0p51)
*   Bit[1] THFAIL0     : All THCODE bits are 0
*   Bit[0] TSC_ERROR   : Aggregated (OR of ERR_ACKTOV, ERR_THCODE, ERR_THCNTOV, THFAIL0)
***********************************************************************************************************************/
static e_scp_retcode_t r_scp_ths_action_hw_error(uint32_t tsc_id)
{
    uint32_t error_mon = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_TSC_ERROR_MON);
    SCP_LOG_ERR("THS[%u] hardware error: TSC_ERROR_MON=0x%08x\n", tsc_id, error_mon);
    (void)error_mon;

    return SCP_RET_HARDWARE_ERROR;
}
/***********************************************************************************************************************
* Start of function r_scp_ths_action_ftest_done()
* Failure Test -> Idle. CIVMTST is auto-cleared by HW; just acknowledge.
***********************************************************************************************************************/
static e_scp_retcode_t r_scp_ths_action_ftest_done(uint32_t tsc_id)
{
    (void)tsc_id;
    return SCP_RET_SUCCESS;
}
/***********************************************************************************************************************
* Start of function r_scp_ths_action_seq_reset()
* Fault -> Idle.
*
* Performs a full cleanup sequence to return THS to a clean Idle state:
*   1. Toggle SEQ_RESET (0->1->0): reinitializes hardware sequencer and error values
*      (Section 27.5, SEQ_RESET register description).
*   2. Explicitly clear THSST in THCTR: if the fault originated from Normal mode,
*      THSST (A/D converter enable) may still be set because Mode Entry 4 was never
*      executed. Clear it to ensure the hardware is in Idle mode (THSST=0).
*   3. Clear IRQEN = 0: if the fault originated from Normal mode, trip point enables
*      may still be active. Per Figure 27.8, IRQEN must be cleared when leaving Normal.
*   4. Clear IRQMSK = 0: same reason as IRQEN.
*   5. Clear IRQSTR = 0: acknowledge any pending temperature error status flags
*      (W0C register) that were set before or during the fault.
***********************************************************************************************************************/
static e_scp_retcode_t r_scp_ths_action_seq_reset(uint32_t tsc_id)
{
    uint32_t thctr;

    /* Step 1: Toggle SEQ_RESET: 0->1->0 */
    r_scp_ths_reg_write(tsc_id, SCP_THS_REG_SEQ_RESET, 1U);
    r_scp_ths_reg_write(tsc_id, SCP_THS_REG_SEQ_RESET, 0U);

    /* Step 2: Ensure THSST is cleared (Idle state: TH_EN=1, THSST=0) */
    thctr = r_scp_ths_reg_read(tsc_id, SCP_THS_REG_THCTR);
    thctr &= ~SCP_THS_THCTR_THSST;
    r_scp_ths_reg_write(tsc_id, SCP_THS_REG_THCTR, thctr);

    /* Step 3: Clear IRQEN and IRQMSK (per Figure 27.8 Normal->Idle cleanup) */
    r_scp_ths_reg_write(tsc_id, SCP_THS_REG_IRQEN,  0U);
    r_scp_ths_reg_write(tsc_id, SCP_THS_REG_IRQMSK, 0U);

    /* Step 4: Clear any pending temperature error status flags (W0C) */
    r_scp_ths_reg_write(tsc_id, SCP_THS_REG_IRQSTR,  0U);

    return SCP_RET_SUCCESS;
}