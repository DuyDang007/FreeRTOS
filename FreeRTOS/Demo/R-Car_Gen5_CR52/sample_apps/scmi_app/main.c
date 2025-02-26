/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "interrupts.h"
#include "semphr.h"
#include "scmi/r_scmi_common.h"
#include "scmi/r_scmi_protocol_base.h"
#include "scmi/r_scmi_protocol_system.h"
#include "scmi/r_scmi_protocol_power.h"
#include "scmi/r_scmi_power_domain_id.h"

#define main_ScmiApp_TASK_PRIORITY ( tskIDLE_PRIORITY + 1 )
#define SCMI_LOG(...) \
    {\
        printf("SCMI [%s:%d] ", __func__, __LINE__);\
		vTaskDelay(3);\
        printf(__VA_ARGS__);\
		vTaskDelay(5);\
    }

static void prvScmiAppTask( void *pvParameters );
static void SCP2CR00InterruptHandler(void *data);
static void ScpIsrAgentCR52Task(void *pvParameters);
static void scmi_app_example(void);
SemaphoreHandle_t xSemaphore = NULL;
bool haveMsgFromSCP = false;

static void prvSetupHardware( void )
{
    portDISABLE_INTERRUPTS();
    Irq_Setup();
}

void main()
{
    prvSetupHardware();

	/* Set Handler for Irq */
    Irq_SetupEntry(SCP2CR00_INT_ID, SCP2CR00InterruptHandler, NULL);

	/* Set priority for Irq */
    Irq_SetPriority(SCP2CR00_INT_ID, IPRIORITY(1));

	/* Enable Irq */
    Irq_Enable(SCP2CR00_INT_ID);

	xSemaphore = xSemaphoreCreateBinary();
	if (xSemaphore == NULL) {
		SCMI_LOG("Error: Semaphore creation failed!\n");
	} else {
		xTaskCreate(ScpIsrAgentCR52Task, "ScpIsrAgentCR52Task", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, NULL);
	}

	xTaskCreate(prvScmiAppTask, "ScmiFreeRTOSApp", configMINIMAL_STACK_SIZE,
				NULL, main_ScmiApp_TASK_PRIORITY, NULL);

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
    for( ;; )
    {
    }
	/* Don't expect to reach here. */
	return;
}

static void prvScmiAppTask( void *pvParameters )
{

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

	SCMI_LOG("SCMI FreeRTOS app starting...\n");
	scmi_app_example();

    for( ;; )
    {
        SCMI_LOG("prvScmiAppTask ...\n");
        vTaskDelay(3000);
    }
}

void SCP2CR00InterruptHandler(void *data) {
    (void)data;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	/* Need to do something!!! */
	R_SCMI_IsrHandler();
	haveMsgFromSCP = true;
    xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void ScpIsrAgentCR52Task(void *pvParameters) {
    ( void ) pvParameters;

    for(;;) {
		vTaskDelay(1);
        if (xSemaphoreTake(xSemaphore, portMAX_DELAY) != pdTRUE)
			continue;
		if (!haveMsgFromSCP)
			continue;
		haveMsgFromSCP = false;
		//mfis_isr_agent_cr52();
    }
}
/*
 * SCMI application examples
 */
/* SCMI Base Protocol */
static int scmi_base_proto_app(void)
{
	int ret;

	{
		uint32_t version = 0U;
		ret = R_SCMI_BaseVersionGet(&version);
		if (ret) {
			SCMI_LOG("Error: Failed to get scmi base protocol version.");
			return ret;
		}
		SCMI_LOG("SCMI base protocol version=0x%x\r\n", version);
	}
	{
		uint8_t num_protocols = 0, num_agents = 0;
		ret = R_SCMI_BaseAttributesGet(&num_protocols, &num_agents);
		if (ret) {
			SCMI_LOG("Error: Failed to get scmi base protocol attributes.");
			return ret;
		}
		SCMI_LOG("SCMI base protocol num protos: %d, num agents: %d\r\n",
				num_protocols, num_agents);
	}
	{
		uint8_t vendor_id[16];
		ret = R_SCMI_BaseVendorIdGet(false, vendor_id);
		if (ret) {
			SCMI_LOG("Error: Failed to get scmi base protocol vendor id.");
			return ret;
		}
		SCMI_LOG("SCMI base protocol vendor id: %s\r\n", vendor_id);

		memset(vendor_id, 0, 16);
		ret = R_SCMI_BaseVendorIdGet(true, vendor_id);
		if (ret) {
			SCMI_LOG("Error: Failed to get scmi base protocol sub vendor id.");
			return ret;
		}
		SCMI_LOG("SCMI base protocol sub vendor id: %s\r\n", vendor_id);
	}
	{
		uint32_t impl_version = 0;
		ret = R_SCMI_BaseImplementationVersionGet(&impl_version);
		if (ret) {
			SCMI_LOG("Error: Failed to get scmi base protocol impl version.");
			return ret;
		}
		SCMI_LOG("SCMI base protocol impl_version: 0x%08x\r\n", impl_version);
	}
	{
		uint32_t num_protocols;
		uint8_t protocols[4];
		int i;
		ret = R_SCMI_BaseDiscoverListProtocols(&num_protocols, protocols);
		if (ret) {
			SCMI_LOG("Error: Failed to get scmi base protocol list protos.");
			return ret;
		}
		SCMI_LOG("SCMI base protocol num_protocols: %d\r\n", num_protocols);
		for (i = 0; i < num_protocols; ++i)
			SCMI_LOG("protocols[%d] = %d\r\n", i, protocols[i]);
	}
	{
		uint32_t agent_id = 0;
		uint8_t name[16];
		ret = R_SCMI_BaseDiscoverAgentGet(0xFFFFFFFFU, &agent_id, name);
		if (ret) {
			SCMI_LOG("Error: Failed to get scmi base protocol agent get.");
			return ret;
		}
		SCMI_LOG("SCMI base protocol agent_id: %d, name: %s\r\n", agent_id, name);
	}

	return 0;
}

/* SCMI System Protocol */
static int scmi_system_proto_app(void)
{
	uint32_t version = 0U;
	int ret;

	ret = R_SCMI_SystemVersionGet(&version);
	if (ret) {
		SCMI_LOG("Error: Failed to get scmi system protocol version.");
		return ret;
	}
	SCMI_LOG("SCMI system protocol version=0x%x\r\n", version);

	return 0;
}

/* SCMI Power Domain Protocol */
static int scmi_powerdomain_proto_app(void)
{
	int ret;

	{
		uint32_t version = 0U;
		ret = R_SCMI_PowerVersionGet(&version);
		if (ret) {
			SCMI_LOG("Error: Failed to get scmi power domain protocol version.");
			return ret;
		}
		SCMI_LOG("SCMI PD protocol version=0x%x\r\n", version);
	}
#define APMU 1
#define MDLC 2
#define PD_TEST MDLC
	{
		struct scmi_power_state_config pwr_cfg;
#if (PD_TEST == APMU)
		pwr_cfg.domain_id = X5H_POWER_DOMAIN_ID_PD_P_RPU_CORER00;
		pwr_cfg.flags = X5H_PD_POWER_STATE_SET_FLAG_ASYNC;
#else // MDLC
		pwr_cfg.domain_id = X5H_POWER_DOMAIN_ID_VIPN;
		pwr_cfg.flags = 0;
#endif
		const char* state_name[] = { "Power ON", "Power OFF", "Invalid state"};
		const char* state = NULL;

		/* Get power domain state */
		ret = R_SCMI_PowerStateGet(&pwr_cfg);
		if (ret) {
			SCMI_LOG("Error: Failed to get scmi power domain %d state.",
					pwr_cfg.domain_id);
			return ret;
		}
		if (pwr_cfg.power_state == X5H_PD_POWER_STATE_ON)
			state = state_name[0];
		else if (pwr_cfg.power_state == X5H_PD_POWER_STATE_OFF)
			state = state_name[1];
		else
			state = state_name[2];
		SCMI_LOG("Domain %s state: %s\r\n",
				(pwr_cfg.domain_id == X5H_POWER_DOMAIN_ID_VIPN) ?
					"X5H_POWER_DOMAIN_ID_VIPN" :
					"X5H_POWER_DOMAIN_ID_PD_P_RPU_CORER00",
				state);

		/* Set power domain state */
		pwr_cfg.power_state = X5H_PD_POWER_STATE_ON;
		ret = R_SCMI_PowerStateSet(&pwr_cfg);
		if (ret) {
			SCMI_LOG("Error: Failed to set scmi power domain %d state.",
					pwr_cfg.domain_id);
			return ret;
		}
		SCMI_LOG("SCMI PD protocol: setting pd OK");
		memset(&pwr_cfg, 0, sizeof(pwr_cfg));
		/* Get power domain state */
		ret = R_SCMI_PowerStateGet(&pwr_cfg);
		if (ret) {
			SCMI_LOG("Error: Failed to get scmi power domain %d state.",
					pwr_cfg.domain_id);
			return ret;
		}
		if (pwr_cfg.power_state == X5H_PD_POWER_STATE_ON)
			state = state_name[0];
		else if (pwr_cfg.power_state == X5H_PD_POWER_STATE_OFF)
			state = state_name[1];
		else
			state = state_name[2];
		SCMI_LOG("Domain %s state: %s\r\n",
				(pwr_cfg.domain_id == X5H_POWER_DOMAIN_ID_VIPN) ?
					"X5H_POWER_DOMAIN_ID_VIPN" :
					"X5H_POWER_DOMAIN_ID_PD_P_RPU_CORER00",
				state);
	}

	return 0;
}

static void scmi_app_example(void)
{
	int ret;
	ret = R_SCMI_DriverInit();
	if (ret) {
		SCMI_LOG("Error: Failed to init scmi driver.");
		return;
	}

	/* SCMI Base */
	ret = scmi_base_proto_app();
	if (ret)
		return;

	/* SCMI System */
	ret = scmi_system_proto_app();
	if (ret)
		return;

	/* SCMI Power Domain */
	ret = scmi_powerdomain_proto_app();
	if (ret)
		return;
}

