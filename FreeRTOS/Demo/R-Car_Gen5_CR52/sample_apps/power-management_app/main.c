/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "FreeRTOS.h"
#include "task.h"
#include "interrupts.h"
#include "state-manager/r_state_manager.h"
#include "state-manager/r_power_domain_id.h"

#define pmApp_TASK_PRIORITY ( tskIDLE_PRIORITY + 1 )
#define PM_LOG(format, ...) \
    {\
        printf("PM [%s:%d] ", __func__, __LINE__);\
		vTaskDelay(5);\
        printf(format "\r\n", ##__VA_ARGS__);\
		vTaskDelay(5);\
    }

static void prvPmAppTask(void *pvParameters);
static void pmAppExample(void);

static void prvSetupHardware(void)
{
    portDISABLE_INTERRUPTS();
    Irq_Setup();
}

void main()
{

    prvSetupHardware();

	xTaskCreate(prvPmAppTask, "powerManagementApp", configMINIMAL_STACK_SIZE,
				NULL, pmApp_TASK_PRIORITY, NULL);

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
    for( ;; )
    {
    }
	/* Don't expect to reach here. */
	return;
}

static void prvPmAppTask(void *pvParameters )
{
    /* Remove compiler warning about unused parameter. */
    (void) pvParameters;

	PM_LOG("PowerManagement FreeRTOS starting...\n");
	pmAppExample();

    for( ;; )
    {
        PM_LOG("prvPmAppTask...\n");
        vTaskDelay(3000);
    }
}

static int pmPowerdomainTest(int domain_id)
{
	int ret;
	const char* state_name[] = { "Power ON", "Power OFF"};
	e_power_state_t get_pwr_state;
	e_power_state_t set_pwr_state;

	/* Get power domain state */
	ret = R_StateManager_Power_Get(domain_id, &get_pwr_state);
	if (ret) {
		PM_LOG("Error: Failed to get scmi power domain %d state.\r\n",
				domain_id);
		return ret;
	}
	PM_LOG("Domain %d current state: %s", domain_id,
			state_name[get_pwr_state]);

	/* Set power domain state */
	if (get_pwr_state == POWER_ON) {
		set_pwr_state = POWER_OFF;
		ret = R_StateManager_PowerOff(domain_id);
	} else {
		set_pwr_state = POWER_ON;
		ret = R_StateManager_PowerOn(domain_id);
	}
	if (ret) {
		PM_LOG("Error: Failed to set scmi power domain %d state.\r\n",
				domain_id);
		return ret;
	}
	PM_LOG("Setting Domain %d state %s without error. Verifying...",
			domain_id, state_name[set_pwr_state]);

	/* Get power domain state again to verify */
	ret = R_StateManager_Power_Get(domain_id, &get_pwr_state);
	if (ret) {
		PM_LOG("Error: Failed to get scmi power domain %d state.\r\n",
				domain_id);
		return ret;
	}
	if (set_pwr_state != get_pwr_state) {
		PM_LOG("Domain %d state: Set failed, set %s but get %s\r\n",
				domain_id, state_name[set_pwr_state],
				state_name[get_pwr_state]);
		return -EIO;
	} else {
		PM_LOG("Domain %d state: Get/Set OK\r\n", domain_id);
	}

	return 0;
}

static void pmAppExample(void)
{
	int ret;
	int domain_id;

	ret = R_StateManager_Init();
	if (ret) {
		PM_LOG("Error: Failed to init State Manager.\r\n");
		return;
	}

	ret = R_StateManager_SCMI_Info_Show();
	if (ret) {
		PM_LOG("Error: Failed to show SCMI information.\r\n");
		return;
	}

#define TEST_ALL_PDS_LIST
	PM_LOG("Setting/getting MDLC power domains");
#ifdef TEST_ALL_PDS_LIST
	for (domain_id = X5H_POWER_DOMAIN_ID_VIPN;
		 domain_id < X5H_POWER_DOMAIN_ID_P_RPU_CORE00;
		 ++domain_id)
#else
	domain_id = X5H_POWER_DOMAIN_ID_VIPN;
#endif /* TEST_ALL_PDS_LIST */
	{
		pmPowerdomainTest(domain_id);
	}

#if 0
	PM_LOG("Setting/getting APMU power domains");
#ifdef TEST_ALL_PDS_LIST
	for (domain_id = X5H_POWER_DOMAIN_ID_P_RPU_CORE00;
		 domain_id < X5H_POWER_DOMAIN_ID_COUNT;
		 ++domain_id)
#else
	domain_id = X5H_POWER_DOMAIN_ID_P_RPU_CORE00;
#endif /* TEST_ALL_PDS_LIST */
	{
		pmPowerdomainTest(domain_id);
	}
#endif

	//PM_LOG("Wait 30s before requesting DeepStop...");
	//vTaskDelay(1000*30);

	ret = R_StateManager_RequestDeepStop();
	if (ret) {
		PM_LOG("Error: Failed to request DeepStop.\r\n");
		return;
	}
}

