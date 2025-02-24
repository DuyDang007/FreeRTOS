/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "scmi/inc/rcar_scmi_common.h"
#include "scmi/inc/common.h"
#include "scmi/inc/base.h"
#include "scmi/inc/power.h"
#include "scmi/inc/system.h"
#include "state-manager/r_state_manager.h"
#include "state-manager/r_power_domain_id.h"

enum s2r_transition {
	MYSELF = 0,
	CA,
	FREERTOS2ND,
	AUTOSAR,
	NONE
};

enum s2r_transition cur_s2r_transition  = NONE;
bool s2r_others_completed = false;

static const char* agentid2str(int agent_id)
{
	switch (agent_id) {
		case SCMI_AGENT_ID_CA:
			return "Xen/Linux (CA)";

		case SCMI_AGENT_ID_FRTOS_2ND:
			return "Secondary FreeRTOS (CR)";

		case SCMI_AGENT_ID_AUTOSAR:
			return "Classic Autosar (CR)";

		case SCMI_AGENT_ID_FRTOS_1ST:
			return "Main FreeRTOS (CR)";

		default:
			break;
	}

	return "Unknown Agent";
}

static void system_notification(void *data)
{
	const char* flags_to_str[] = {"forceful", "graceful", "invalid"};
    const char* system_state_to_str[] = {
		"shutdown", "coldreset", "warmreset", "powerup", "suspend", "MAX"};
	int ret;
	scmi_syspower_state_notifier_t *notifier =
		(scmi_syspower_state_notifier_t *)data;
	static int cnt = 0;

	SCMI_LOG_INFO("%s has transited to %s %s",
			agentid2str(notifier->agent_id),
			flags_to_str[notifier->flags],
			system_state_to_str[notifier->system_state]);

#ifdef S2R_DRAFT_FLOW
	/* Return if no S2R transition is requested. */
	if (NONE == cur_s2r_transition)
		return;

	switch (cur_s2r_transition) {
		case MYSELF:
			SCMI_LOG_INFO("Step 2 (notif). Main FreeRTOS just sent S2R req.");
			SCMI_LOG_INFO("Step 8. Send s2r req assuming from CA to SCP.\r\n");
			ret = scmi_system_power_state_set(FLAGS_GRACEFUL, SYSTEM_STATE_SUSPEND);
			++cnt;
			++cur_s2r_transition;
			break;

		case CA:
			if (1 == cnt) {
				SCMI_LOG_INFO("Step 9. CA S2R done.");
				SCMI_LOG_INFO("Step 10. Send S2R req to 2nd FreeRTOS.\r\n");
			} else {
				SCMI_LOG_INFO("Step 10 (notif). Main FreeRTOS just sent s2r req to 2nd.");
				SCMI_LOG_INFO("Step 12. Send s2r req assuming from 2nd FreeRTOS to SCP.\r\n");
				++cur_s2r_transition;
			}
			ret = scmi_system_power_state_set(FLAGS_GRACEFUL, SYSTEM_STATE_SUSPEND);
			++cnt;
			break;

		case FREERTOS2ND:
			if (3 == cnt) {
				SCMI_LOG_INFO("Step 13. 2nd FreeRTOS S2R done.");
				SCMI_LOG_INFO("Step 14. Send S2R req to Autosar.\r\n");
			} else {
				SCMI_LOG_INFO("Step 14 (notif). Main FreeRTOS just sent s2r req to Autosar.");
				SCMI_LOG_INFO("Step 16. Assuming this msg is from Autosar to SCP.");
				++cur_s2r_transition;
			}
			ret = scmi_system_power_state_set(FLAGS_GRACEFUL, SYSTEM_STATE_SUSPEND);
			++cnt;
			break;

		case AUTOSAR:
			if (5 == cnt) {
				SCMI_LOG_INFO("Step 18. Classic Autosar S2R done.");
				s2r_others_completed = true;
				cnt = 0;
			}
			break;

		default:
			SCMI_LOG_ERR("Invalid transition.");
	}
#endif
}

int R_StateManager_Init(void)
{
	int ret;
	struct scmi_protocol *proto;
	uint32_t version = 0U;

	ret = scmi_driver_init();
	if (ret) {
		SCMI_LOG_ERR("Error: Failed to init scmi driver.");
		return ret;
	}

	ret = scmi_base_version_get(&version);
	if (ret) {
		SCMI_LOG_ERR("Error: Failed to get scmi base protocol version.\r\n");
		return ret;
	}
	SCMI_LOG_INFO("SCMI protocol version=0x%x", version);

	ret = scmi_system_request_notify(true);
	if (ret) {
		SCMI_LOG_ERR("Error: Failed to request system notification.\r\n");
		return ret;
	}
	proto = scmi_system_proto_get();
	scmi_notifier_callback_register(proto, system_notification);

	return ret;
}

/* Show SCMI Protocols' information */
int R_StateManager_SCMI_Info_Show(void)
{
	int ret;

	{
		uint8_t num_protocols = 0, num_agents = 0;
		ret = scmi_base_attributes_get(&num_protocols, &num_agents);
		if (ret) {
			SCMI_LOG_INFO("Error: Failed to get scmi base protocol attributes.\r\n");
			return ret;
		}
		SCMI_LOG_INFO("SCMI base protocol num protos: %d, num agents: %d",
				num_protocols, num_agents);
	}
	{
		uint8_t vendor_id[16];
		ret = scmi_base_vendorid_get(false, vendor_id);
		if (ret) {
			SCMI_LOG_INFO("Error: Failed to get scmi base protocol vendor id.\r\n");
			return ret;
		}
		SCMI_LOG_INFO("SCMI base protocol vendor id: %s", vendor_id);

		memset(vendor_id, 0, 16);
		ret = scmi_base_vendorid_get(true, vendor_id);
		if (ret) {
			SCMI_LOG_INFO("Error: Failed to get scmi base protocol sub vendor id.\r\n");
			return ret;
		}
		SCMI_LOG_INFO("SCMI base protocol sub vendor id: %s", vendor_id);
	}
	{
		uint32_t impl_version = 0;
		ret = scmi_base_implementation_version_get(&impl_version);
		if (ret) {
			SCMI_LOG_INFO("Error: Failed to get scmi base protocol impl version.\r\n");
			return ret;
		}
		SCMI_LOG_INFO("SCMI base protocol impl_version: 0x%08x", impl_version);
	}
	{
		uint32_t num_protocols;
		uint8_t protocols[4];
		int i;
		ret = scmi_base_discover_list_protocols(&num_protocols, protocols);
		if (ret) {
			SCMI_LOG_INFO("Error: Failed to get scmi base protocol list protos.\r\n");
			return ret;
		}
		SCMI_LOG_INFO("SCMI base protocol num_protocols: %d", num_protocols);
		for (i = 0; i < num_protocols; ++i)
			SCMI_LOG_INFO("protocols[%d] = %d", i, protocols[i]);
	}
	{
		uint32_t agent_id = 0;
		uint8_t name[16];
		ret = scmi_base_discover_agent_get(0xFFFFFFFFU, &agent_id, name);
		if (ret) {
			SCMI_LOG_INFO("Error: Failed to get scmi base protocol agent get.\r\n");
			return ret;
		}
		SCMI_LOG_INFO("SCMI base protocol agent_id: %d, name: %s\r\n", agent_id, name);
	}
	{
		uint32_t version = 0U;
		ret = scmi_system_version_get(&version);
		if (ret) {
			SCMI_LOG_INFO("Error: Failed to get scmi system protocol version.\r\n");
			return ret;
		}
		SCMI_LOG_INFO("SCMI system protocol version=0x%x", version);
	}
	{
		uint32_t version = 0U;
		ret = scmi_power_version_get(&version);
		if (ret) {
			SCMI_LOG_INFO("Error: Failed to get scmi power domain protocol version.\r\n");
			return ret;
		}
		SCMI_LOG_INFO("SCMI PD protocol version=0x%x", version);
	}

	return 0;
}

#include "FreeRTOS.h"
#include "task.h"
int R_StateManager_RequestDeepStop(void)
{
	int ret;

#ifdef S2R_DRAFT_FLOW
	SCMI_LOG_INFO("System is suspending...");
	cur_s2r_transition = MYSELF;
	SCMI_LOG_INFO("Step 2. S2R request");
	ret = scmi_system_power_state_set(FLAGS_GRACEFUL, SYSTEM_STATE_SUSPEND);
	if (ret) {
		SCMI_LOG_ERR("Error: Failed to request S2R");
		return ret;
	}

	while (!s2r_others_completed) {
		vTaskDelay(1);
	}
	cur_s2r_transition = NONE;
	SCMI_LOG_INFO("Step 19. Shutdown with SUSPEND flag to SCP");
	ret = scmi_system_power_state_set(FLAGS_GRACEFUL, SYSTEM_STATE_SUSPEND);
	if (ret) {
		SCMI_LOG_ERR("Error: Failed to suspend system gracefully.");
		return ret;
	}
#endif

	return 0;
}

int R_StateManager_SysReboot(void)
{
	int ret;

	SCMI_LOG_INFO("System is resetting...");
	ret = scmi_system_power_state_set(FLAGS_GRACEFUL, SYSTEM_STATE_COLD_RESET);
	if (ret) {
		SCMI_LOG_ERR("Error: Failed to set system suspend gracefully.");
		return ret;
	}

	return 0;
}

int R_StateManager_SysPowerOff(void)
{
	int ret;

	SCMI_LOG_INFO("System is shutting down...");
	ret = scmi_system_power_state_set(FLAGS_FORCEFUL, SYSTEM_STATE_SHUTDOWN);
	if (ret) {
		SCMI_LOG_ERR("Error: Failed to shutdown system forcefully.");
		return ret;
	}

	return 0;
}

int R_StateManager_Power_Get(int domain_id, e_power_state_t *state)
{
	struct scmi_power_state_config pwr_cfg;
	int ret;

	pwr_cfg.domain_id = domain_id;
	ret = scmi_power_state_get(&pwr_cfg);
	if (ret) {
		SCMI_LOG_ERR("Failed to get power domain %d (%d)\r\n", domain_id, ret);
		return ret;
	}

	if (pwr_cfg.power_state == SCMI_POWER_STATE_ON)
		*state = POWER_ON;
	else if (pwr_cfg.power_state == SCMI_POWER_STATE_OFF)
		*state = POWER_OFF;

	return 0;
}

int R_StateManager_PowerOff(int domain_id)
{
	struct scmi_power_state_config pwr_cfg;
	int ret;

	pwr_cfg.domain_id = domain_id;
	if ((X5H_POWER_DOMAIN_ID_VIPN <= domain_id) &&
			(X5H_POWER_DOMAIN_ID_P_RPU_CORE00 > domain_id)) {
		pwr_cfg.flags = 0;
	} else if ((X5H_POWER_DOMAIN_ID_P_RPU_CORE00 <= domain_id) &&
			(X5H_POWER_DOMAIN_ID_COUNT > domain_id)) {
		pwr_cfg.flags = SCMI_POWER_STATE_SET_FLAGS_ASYNC;
	} else {
		SCMI_LOG_ERR("Invalid power domain ID.\r\n");
		return -EINVAL;
	}
	pwr_cfg.power_state= SCMI_POWER_STATE_OFF;

	ret = scmi_power_state_set(&pwr_cfg);
	if (ret) {
		SCMI_LOG_ERR("Failed to set power domain %d OFF (%d)\r\n", domain_id, ret);
		return ret;
	}

	return 0;
}

int R_StateManager_PowerOn(int domain_id)
{
	struct scmi_power_state_config pwr_cfg;
	int ret;

	pwr_cfg.domain_id = domain_id;
	if ((X5H_POWER_DOMAIN_ID_VIPN <= domain_id) &&
			(X5H_POWER_DOMAIN_ID_P_RPU_CORE00 > domain_id)) {
		pwr_cfg.flags = 0;
	} else if ((X5H_POWER_DOMAIN_ID_P_RPU_CORE00 <= domain_id) &&
			(X5H_POWER_DOMAIN_ID_COUNT > domain_id)) {
		pwr_cfg.flags = SCMI_POWER_STATE_SET_FLAGS_ASYNC;
	} else {
		SCMI_LOG_ERR("Invalid power domain ID.\r\n");
		return -EINVAL;
	}
	pwr_cfg.power_state= SCMI_POWER_STATE_ON;

	ret = scmi_power_state_set(&pwr_cfg);
	if (ret) {
		SCMI_LOG_ERR("Failed to set power domain %d ON (%d)\r\n", domain_id, ret);
		return ret;
	}

	return 0;
}

int R_StateManager_SetClock(int clock_id, uint32_t *rates)
{
	return 0;
}

int R_StateManager_GetClock(int clock_id, uint32_t *rates)
{
	return 0;
}

int R_StateManager_ClockOff(int clock_id)
{
	return 0;
}

int R_StateManager_ClockOn(int clock_id)
{
	return 0;
}

