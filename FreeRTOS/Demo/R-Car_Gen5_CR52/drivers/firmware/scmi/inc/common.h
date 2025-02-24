/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SCMI_COMMON_H_
#define _SCMI_COMMON_H_

#include <stdint.h>

#define SCMI_SHORT_NAME_MAX_SIZE    16

struct scmi_message;
struct scmi_protocol;

struct scmi_dev {
	/** Name of the device instance */
	const char *name;
	/** Address of device instance config information */
	const void *config;
	/** Address of the API structure exposed by the device instance */
	const void *api;
	/** Address of the device instance private data */
	void *data;
};

typedef enum {
    SCMI_LOG_LEVEL_DBG     = 0,
    SCMI_LOG_LEVEL_INFO      ,
    SCMI_LOG_LEVEL_ERR       ,
    SCMI_LOG_LEVEL_MAX    
} e_scmi_log_level_t;

/* Configuration of log level */
#define SCMI_LOG_LEVEL_DEFAULT    SCMI_LOG_LEVEL_DBG

#ifdef SCMI_DEBUG
extern int printf_delay(const char *format, ...);

#include "FreeRTOS.h"
#include "task.h"

/* SCMI debug log macro */
#define SCMI_LOG(level, format, ...)\
    if (SCMI_LOG_LEVEL_DEFAULT <= (level))\
    {\
        printf_delay("SCMI [%s:%d] ", __func__, __LINE__);\
		vTaskDelay(2);\
        printf_delay(format "\r\n", ##__VA_ARGS__);\
		vTaskDelay(3);\
    }
#else   /* SCMI_DEBUG */
    #define SCMI_LOG(level, ...)
#endif  /* SCMI_DEBUG */

/* SCMI debug log */
#define SCMI_LOG_DBG(...) SCMI_LOG(SCMI_LOG_LEVEL_DBG, "[D] " __VA_ARGS__)
/* SCMI information log */
#define SCMI_LOG_INFO(...) SCMI_LOG(SCMI_LOG_LEVEL_INFO, "[I] " __VA_ARGS__)
/* SCMI error log */
#define SCMI_LOG_ERR(...) SCMI_LOG(SCMI_LOG_LEVEL_ERR, "[E] " __VA_ARGS__)


/*******************************************************************************************************************//**
 * @enum e_scmi_command_id_t
 * Common command identifiers.
***********************************************************************************************************************/
typedef enum {
    SCMI_PROTOCOL_VERSION            = 0,   /**< Returns the version of protocol. */
    SCMI_PROTOCOL_ATTRIBUTES         ,   /**< Returns properties that are associated with the protocol implementation. */
    SCMI_PROTOCOL_MESSAGE_ATTRIBUTES ,     /**< Takes a message_id as a parameter and returns implementation details specific to that message. */
} e_scmi_command_id_t;

/*******************************************************************************************************************//**
 * @struct scmi_msg_resp_version
 **********************************************************************************************************************/
struct scmi_msg_resp_version {
    int32_t     status;
    uint32_t    version;
};

/**
 * @brief scmi driver init.
 */
int scmi_driver_init(void);

/**
 * @brief Callback API for notifier of each protocol
 *
 * These callbacks execute right after interrupt context occurs.
 * Registration of callbacks is done via scmi_register_callback().
 *
 * @param data specific data depending on specific protocols
 */
typedef void (*scmi_notifier_callback_t)(void *data);

/**
 * @brief Register a callback function on each protocol for incoming messages.
 *
 * This function registers callback functions for each protocol.
 *
 * @param proto pointer to SCMI protocol
 * @param cb Callback function to execute on incoming message.
 *
 * @retval
 */
void scmi_notifier_callback_register(struct scmi_protocol *proto,
                                     scmi_notifier_callback_t cb);

#endif /* _SCMI_COMMON_H_ */

