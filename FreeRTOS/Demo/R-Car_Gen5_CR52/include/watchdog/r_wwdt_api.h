#ifndef _R_WWDT_API_H
#define _R_WWDT_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "fsp/fsp_common_api.h"
#include "fsp/r_transfer_api.h"
/**
 * List of WWDT channels
 */
#pragma once
typedef enum {
    R_WWDT0 = 0,
    R_WWDT1,
    R_WWDT2,
    R_WWDT3,
    R_WWDT4,
    R_WWDT5,
    R_WWDT6,
    R_WWDT7,
    R_WWDT8,
    R_WWDT9,
    R_WWDT10,
    R_WWDT11,
    R_WWDT12,
    R_WWDT13,
    R_WWDT14,
    R_WWDT15,
    R_WWDT16,
    R_WWDT17,
    R_WWDT18,
    R_WWDT19,
    R_WWDT20,
    R_WWDT_LAST		/**<delimiter */
} wwdt_unit_t;

typedef enum {
    WINDOW_25P = 0,
    WINDOW_50P = 1,
    WINDOW_75P = 2,
    WINDOW_100P = 3
} wwdt_wsize_t;

typedef enum {
    ERM_NMI_MODE = 0,
    ERM_RESET_MODE = 1
} wwdt_erm_t;

/**
 * Initialize the Windowed Watchdog Timer (WWDT) with the specified configuration.
 *
 * @param[in] unit         - WWDT unit number, see @ref r_wwdt_Unit_t for available units.
 * @param[in] wsize        - Window size value, which defines the percentage of open window.
 *                           Valid values:
 *                           - WINDOW_25P:  25% of open window
 *                           - WINDOW_50P:  50% of open window
 *                           - WINDOW_75P:  75% of open window
 *                           - WINDOW_100P:  100% of open window
 * @param[in] timeout_msec - Timeout value in milliseconds. This determines the time before the WWDT triggers.
 *                           For WWDT0-19, the timeout values are:
 *                           - 15ms, 31ms, 62ms, 124ms, 250ms, 500ms, 1000ms, and 2000ms.
 *                           For WWDT20, the timeout values are:
 *                           - 2ms, 4ms, 8ms, 17ms, 34ms, 68ms, 136ms, and 273ms.
 * @param[in] irq_75p      - Enable or disable the 75% interrupt function. Set to `true` to enable, `false` to disable.
 * @param[in] err_mode     - Error handling mode. Choose how to handle WWDT errors:
 *                           - `true` (default): Reset mode, the system will reset when an error occurs.
 *                           - `false`: NMI mode, the system will generate a Non-Maskable Interrupt (NMI) on error.
 *
 * @retval 0 if the initialization is successful, non-zero value in case of failure.
 */

void R_WWDT_Init(wwdt_unit_t unit, wwdt_wsize_t wsize, uint32_t timeout_msec, bool irq_75p, wwdt_erm_t err_mode);

/**
 * Refresh the Windowed Watchdog Timer (WWDT) to prevent it from triggering a reset or NMI.
 *
 * @param[in] unit - WWDT unit number, see @ref r_wwdt_Unit_t for available units. This defines the channel
 *                  that needs to be refreshed.
 *
 * @retval 0 if successful, a non-zero value if there is an error.
 */

uint32_t R_WWDT_Refresh(wwdt_unit_t unit);

#ifdef __cplusplus
}
#endif

#endif /* R_WWDT_API_H_ */
