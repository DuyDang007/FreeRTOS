/*
 *
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef LDR_LOGGING_H
#define LDR_LOGGING_H

/**
 * @brief Log severity levels.
 */
typedef enum e_log_level {
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} e_log_level_t;

/**
 * @brief Logs a formatted message for a given module and severity level.
 *
 * @param log_level The log severity.
 * @param module_name Name of the module generating the log.
 * @param fmt Format string describing the log message.
 * @param ... Additional arguments for the format string.
 */
void LDR_LOG(e_log_level_t log_level, const char* module_name, const char* fmt, ...);

#endif /* LDR_LOGGING_H */