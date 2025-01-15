#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <stdbool.h>
#include "watchdog/r_wwdt_api.h"
#include "r_wwdt_reg.h"

const uint16_t timeout_ch0_19[] = { 15, 31, 62, 124, 250, 500, 1000, 2000 };
const uint16_t timeout_ch20[] = { 2, 4, 8, 17, 34, 68, 136, 273 };

#define TIMEOUT_TO_X(timeout_ms, channel)    \
    ({ \
        const uint16_t *timeout_array = (channel == 0xC1380000) ? timeout_ch20 : timeout_ch0_19; \
        uint8_t result = 7; \
        for (uint8_t i = 0; i < 8; ++i) { \
            if (timeout_ms <= timeout_array[i]) { \
                result = i; \
                break; \
            } \
        } \
        result; \
    })


#define printf_delay(fmt, ...)      \
	vTaskDelay(1);             \
printf(fmt, ##__VA_ARGS__);         \

/*static void r_wdt_wait_cycles(uint8_t cycles)
{
	uint8_t delay;
	delay = DIV_ROUND_UP(cycles * 10000000, clk_rate);

	vTaskDelay(delay);
}*/

void R_WWDT_Init(wwdt_unit_t unit, wwdt_wsize_t wsize, uint32_t timeout_msec, bool irq_75p, wwdt_erm_t err_mode)
{
	uint8_t val;
	uintptr_t wwdt_base_addr = R_WWDT_PRV_GetRegbase(unit);
	val = r_wwdt_read(wwdt_base_addr + WDTA0MD);
	if(!err_mode)
		val &= ~WDTA0ERM;
	val |= WDTA0OVF(TIMEOUT_TO_X(timeout_msec, wwdt_base_addr)) | WSIZE(wsize);
	if(irq_75p)
		val |= WDTA0WIE;

	r_wwdt_write(wwdt_base_addr + WDTA0MD, val);
}

uint32_t R_WWDT_Refresh(wwdt_unit_t unit)
{
	uintptr_t wwdt_base_addr = R_WWDT_PRV_GetRegbase(unit);

	r_wwdt_write(wwdt_base_addr + WDTA0WDTE, WDTA0RUN);

	return 0;
}
