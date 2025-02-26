/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SCMI_COMMON_H_
#define _SCMI_COMMON_H_

#define SCMI_SHORT_NAME_MAX_SIZE    16

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
#define SCMI_LOG_LEVEL_DEFAULT    SCMI_LOG_LEVEL_ERR

#ifdef SCMI_DEBUG
extern int printf_delay(const char *format, ...);

/* SCMI debug log macro */
#define SCMI_LOG(level, ...)\
    if (SCMI_LOG_LEVEL_DEFAULT <= (level))\
    {\
        printf_delay("SCMI [%s:%d] ", __func__, __LINE__);\
        printf_delay(__VA_ARGS__);\
    }
#else   /* SCMI_DEBUG */
    #define SCMI_LOG(level, ...)
#endif  /* SCMI_DEBUG */

/* SCMI debug log */
#define SCMI_LOG_DBG(...)    SCMI_LOG(SCMI_LOG_LEVEL_DBG, __VA_ARGS__)
/* SCMI information log */
#define SCMI_LOG_INFO(...)   SCMI_LOG(SCMI_LOG_LEVEL_INFO, __VA_ARGS__)
/* SCMI error log */
#define SCMI_LOG_ERR(...)    SCMI_LOG(SCMI_LOG_LEVEL_ERR, "[Error] " __VA_ARGS__)


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

#endif /* _SCMI_COMMON_H_ */

