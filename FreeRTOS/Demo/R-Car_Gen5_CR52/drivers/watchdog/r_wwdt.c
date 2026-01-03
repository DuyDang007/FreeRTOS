/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <stdbool.h>
#include "watchdog/r_wwdt_api.h"
#include "state-manager/r_clock_domain_id.h"
#include "state-manager/r_state_manager.h"
#include "r_wwdt_reg.h"
#include "devicetree-binding.h"

const uint16_t timeout_ch0_19[] = { 15, 31, 62, 124, 250, 500, 1000, 2000 };
const uint16_t timeout_ch20[] = { 2, 4, 8, 17, 34, 68, 136, 273 };
static uint32_t clk_rate;

#define DIV_ROUND_UP(a, b) (((a) + (b) - 1U) / (b))
#define TIMEOUT_TO_X(timeout_ms, channel)    \
    ({ \
        const uint16_t *timeout_array = (channel == R_WWDT20) ? timeout_ch20 : timeout_ch0_19; \
        uint8_t result = 7; \
        for (uint8_t i = 0; i < 8; ++i) { \
            if (timeout_ms <= timeout_array[i]) { \
                result = i; \
                break; \
            } \
        } \
        result; \
    })
#define CLK_LSIOSC	240000
#define RCLK		32800

static uint32_t r_rst_read(uintptr_t Addr)
{
	return *((volatile uint32_t *)Addr);
}

static void r_wdt_wait_cycles(uint8_t cycles)
{
	uint8_t delay;
	delay = DIV_ROUND_UP(cycles * 1000000U, clk_rate);

	vTaskDelay(delay);
}

void R_WWDT_Init(wwdt_unit_t unit, wwdt_wsize_t wsize, uint32_t timeout_msec, bool irq_75p, wwdt_erm_t err_mode)
{
	uint8_t val;
	uintptr_t wwdt_base_addr;
	int clock_id, clock_id_0, clock_id_1, ret;
	const struct watchdog_node *wwdt;

	if((uint32_t)unit > dt_count_node((void *)window_watchdog_list) - 1 ||
	   window_watchdog_list[unit]->status != OKAY) {
		printf("Error: Wrong WWDT Unit.\r\n");
		return;
	}
	wwdt = window_watchdog_list[unit];
	wwdt_base_addr = wwdt->base_address;
	
	clock_id = X5H_CLOCK_ID_MDLC_WDT0;
	ret = R_StateManager_ClockOn(clock_id);
	if (ret)
		printf("Error: Failed to set clock id %d ON.\r\n", clock_id);
	
	clock_id_0 = wwdt->clock_domain_id[0];
	clock_id_1 = wwdt->clock_domain_id[1];
	ret = R_StateManager_ResetAssert(clock_id_0);
	if (ret)
			printf("Error: Failed to reset clock id %d.\r\n", clock_id_0);
	ret = R_StateManager_ResetAssert(clock_id_1);
	if (ret)
			printf("Error: Failed to reset clock id %d.\r\n", clock_id_1);

	ret = R_StateManager_ResetDeassert(clock_id_0);
	if (ret)
			printf("Error: Failed to DeassertReset clock id %d.\r\n", clock_id_0);

	ret = R_StateManager_ResetDeassert(clock_id_1);
	if (ret)
			printf("Error: Failed to DeassertReset clock id %d.\r\n", clock_id_1);

	clk_rate = (unit == R_WWDT20) ? CLK_LSIOSC : RCLK;
	val = r_wwdt_read(wwdt_base_addr + WDTA0MD);
	if (!err_mode)
		val &= ~WDTA0ERM;
	val |= WDTA0OVF(TIMEOUT_TO_X(timeout_msec, unit)) | WSIZE(wsize);
	if (irq_75p)
		val |= WDTA0WIE;

	r_wwdt_write(wwdt_base_addr + WDTA0MD, val);

	/* Enable Generating internal reset when WWDT overflow */
	r_wwdt_write(RST_DM0_BASE + RST_RESKCPROT0, RST_KCPROT_DIS);
	r_wwdt_write(RST_DM0_BASE + RST_RESFC, r_rst_read(RST_DM0_BASE + RST_RESFC) & ~RST_SRES1FC4);
	r_wwdt_write(RST_DM0_BASE + RST_WDTRSTCR, r_rst_read(RST_DM0_BASE + RST_WDTRSTCR) & ~WWDT_RSTMSK);
}

uint32_t R_WWDT_Refresh(wwdt_unit_t unit)
{
	uintptr_t wwdt_base_addr;
	const struct watchdog_node *wwdt;

	if((uint32_t)unit > dt_count_node((void *)window_watchdog_list) - 1 ||
	   window_watchdog_list[unit]->status != OKAY) {
		printf("Error: Wrong WWDT Unit.\r\n");
		return 1;
	}
	wwdt_base_addr = window_watchdog_list[unit]->base_address;

	r_wdt_wait_cycles(3);
	r_wwdt_write(wwdt_base_addr + WDTA0WDTE, WDTA0RUN);

	return 0;
}
