/*
 *
 * Copyright (c) 2026 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

 /**
 * @file
 * @brief SCMI sensor manager protocol
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "util.h"
#include <stdint.h>

#define SCMI_SENSOR_NAME_MAX      16u   /* short name in SENSOR_DESC */
#define SCMI_SENSOR_EXT_NAME_MAX  64u   /* extended name from SENSOR_NAME_GET */

#define SCMI_SENSOR_AXIS_EXT_NAME_MAX 64u

#define X5H_SENSOR_NUM 4u /* X5H support 4 thermal sensors */

struct scmi_sensor_protocol_attributes {
    uint16_t num_sensors;       /* attributes[15:0]  */
    uint8_t  max_async;         /* attributes[23:16] */
    uint64_t shared_mem_addr;   /* valid if shared_mem_len > 0 */
    uint32_t shared_mem_len;    /* 0 means shared memory not implemented */
};

// struct scmi_sensor_descriptor;

struct scmi_sensor_desc_page {
    uint32_t num_returned;
    uint32_t num_remaining;
    // struct scmi_sensor_descriptor *desc;
};

struct scmi_sensor_trip_point_config {
    /* spec-defined fields */
    uint32_t trip_point_id;
    uint32_t flags;
    int64_t  value_low;
    int64_t  value_high;
};

enum scmi_sensor_reading_format {
    SCMI_SENSOR_READING_FMT_UNKNOWN = 0,
    SCMI_SENSOR_READING_FMT_S32,
    SCMI_SENSOR_READING_FMT_S64,
    SCMI_SENSOR_READING_FMT_U32,
    SCMI_SENSOR_READING_FMT_U64,
    /* extend as per spec */
};

struct scmi_sensor_update_interval {
    uint32_t sec;       /* bits[20:5] */
    int8_t   exponent;  /* bits[4:0], two's complement */
};

/* Output (returned via struct like other APIs) */
struct scmi_sensor_config {
    uint32_t sensor_config; /* raw */

    /* Derived fields */
    uint8_t  enabled;
    uint8_t  timestamped;

    /* sensor_update_interval decoded from bits[31:11] (valid if non-zero) */
    uint8_t  update_interval_valid;
    struct scmi_sensor_update_interval update_interval; /* sec + exponent */
};

struct scmi_sensor_desc {
    uint32_t sensor_id;
    uint32_t sensor_attributes_low;
    uint32_t sensor_attributes_high;
    uint8_t sensor_name[SCMI_SENSOR_NAME_MAX];

    /* Derived flags */
    uint8_t  ext_attrs_supported;

    /* Extended attributes (valid only if ext_attrs_supported == 1 and scalar sensor) */
    uint32_t sensor_power_uW;

    uint8_t  resolution_valid;
    int8_t   resolution_exponent;  /* sensor_resolution[31:27], two's complement */
    uint32_t resolution_res;       /* sensor_resolution[26:0] */

    uint8_t  range_valid;
    int64_t  min_range;            /* composed from min_range_low/high */
    int64_t  max_range;            /* composed from max_range_low/high */
};

struct scmi_sensor_reading_get_info {
    /* number of readings copied into out[] */
    size_t readings_copied;
};

struct scmi_sensor_reading_desc {
    int32_t  sensor_value_low;
    int32_t  sensor_value_high;
    uint32_t timestamp_low;
    uint32_t timestamp_high;
};

/**
 * @brief Sensor protocol command message IDs
 */
enum scmi_sensor_message {
    SCMI_SENSOR_MSG_PROTOCOL_VERSION = 0x0,
    SCMI_SENSOR_MSG_PROTOCOL_ATTRIBUTES = 0x1,
    SCMI_SENSOR_MSG_PROTOCOL_MESSAGE_ATTRIBUTES = 0x2,
    SCMI_SENSOR_MSG_SENSOR_DESCRIPTION_GET = 0x3,
    SCMI_SENSOR_MSG_SENSOR_TRIP_POINT_NOTIFY = 0x4,
    SCMI_SENSOR_MSG_SENSOR_TRIP_POINT_CONFIG = 0x5,
    SCMI_SENSOR_MSG_SENSOR_READING_GET = 0x6,
    SCMI_SENSOR_MSG_SENSOR_AXIS_DESCRIPTION_GET = 0x7,
    SCMI_SENSOR_MSG_SENSOR_LIST_UPDATE_INTERVALS = 0x8,
    SCMI_SENSOR_MSG_SENSOR_CONFIG_GET = 0x9,
    SCMI_SENSOR_MSG_SENSOR_CONFIG_SET = 0xA,
    SCMI_SENSOR_MSG_SENSOR_CONTINUOUS_UPDATE_NOTIFY = 0xB,
    SCMI_SENSOR_MSG_SENSOR_NAME_GET = 0xC,
    SCMI_SENSOR_MSG_SENSOR_AXIS_NAME_GET = 0xD,
    SCMI_SENSOR_MSG_NEGOTIATE_PROTOCOL_VERSION = 0x10,
};

enum scmi_sensor_type {
    SCMI_SENSOR_UNIT_NONE                        = 0,
    SCMI_SENSOR_UNIT_UNSPECIFIED                 = 1,
    SCMI_SENSOR_UNIT_DEGREES_C                   = 2,
    SCMI_SENSOR_UNIT_DEGREES_F                   = 3,
    SCMI_SENSOR_UNIT_DEGREES_K                   = 4,
    SCMI_SENSOR_UNIT_VOLTS                       = 5,
    SCMI_SENSOR_UNIT_AMPS                        = 6,
    SCMI_SENSOR_UNIT_WATTS                       = 7,
    SCMI_SENSOR_UNIT_JOULES                      = 8,
    SCMI_SENSOR_UNIT_COULOMBS                    = 9,
    SCMI_SENSOR_UNIT_VA                          = 10,
    SCMI_SENSOR_UNIT_NITS                        = 11,
    SCMI_SENSOR_UNIT_LUMENS                      = 12,
    SCMI_SENSOR_UNIT_LUX                         = 13,
    SCMI_SENSOR_UNIT_CANDELAS                    = 14,
    SCMI_SENSOR_UNIT_KPA                         = 15,
    SCMI_SENSOR_UNIT_PSI                         = 16,
    SCMI_SENSOR_UNIT_NEWTONS                     = 17,
    SCMI_SENSOR_UNIT_CFM                         = 18,
    SCMI_SENSOR_UNIT_RPM                         = 19,
    SCMI_SENSOR_UNIT_HERTZ                       = 20,
    SCMI_SENSOR_UNIT_SECONDS                     = 21,
    SCMI_SENSOR_UNIT_MINUTES                     = 22,
    SCMI_SENSOR_UNIT_HOURS                       = 23,
    SCMI_SENSOR_UNIT_DAYS                        = 24,
    SCMI_SENSOR_UNIT_WEEKS                       = 25,
    SCMI_SENSOR_UNIT_MILS                        = 26,
    SCMI_SENSOR_UNIT_INCHES                      = 27,
    SCMI_SENSOR_UNIT_FEET                        = 28,
    SCMI_SENSOR_UNIT_CUBIC_INCHES                = 29,
    SCMI_SENSOR_UNIT_CUBIC_FEET                  = 30,
    SCMI_SENSOR_UNIT_METERS                      = 31,
    SCMI_SENSOR_UNIT_CUBIC_CENTIMETERS           = 32,
    SCMI_SENSOR_UNIT_CUBIC_METERS                = 33,
    SCMI_SENSOR_UNIT_LITERS                      = 34,
    SCMI_SENSOR_UNIT_FLUID_OUNCES                = 35,
    SCMI_SENSOR_UNIT_RADIANS                     = 36,
    SCMI_SENSOR_UNIT_STERADIANS                  = 37,
    SCMI_SENSOR_UNIT_REVOLUTIONS                 = 38,
    SCMI_SENSOR_UNIT_CYCLES                      = 39,
    SCMI_SENSOR_UNIT_GRAVITIES                   = 40,
    SCMI_SENSOR_UNIT_OUNCES                      = 41,
    SCMI_SENSOR_UNIT_POUNDS                      = 42,
    SCMI_SENSOR_UNIT_FOOT_POUNDS                 = 43,
    SCMI_SENSOR_UNIT_OUNCE_INCHES                = 44,
    SCMI_SENSOR_UNIT_GAUSS                       = 45,
    SCMI_SENSOR_UNIT_GILBERTS                    = 46,
    SCMI_SENSOR_UNIT_HENRIES                     = 47,
    SCMI_SENSOR_UNIT_FARADS                      = 48,
    SCMI_SENSOR_UNIT_OHMS                        = 49,
    SCMI_SENSOR_UNIT_SIEMENS                     = 50,
    SCMI_SENSOR_UNIT_MOLES                       = 51,
    SCMI_SENSOR_UNIT_BECQUERELS                  = 52,
    SCMI_SENSOR_UNIT_PPM                         = 53,
    SCMI_SENSOR_UNIT_DECIBELS                    = 54,
    SCMI_SENSOR_UNIT_DBA                         = 55,
    SCMI_SENSOR_UNIT_DBC                         = 56,
    SCMI_SENSOR_UNIT_GRAYS                       = 57,
    SCMI_SENSOR_UNIT_SIEVERTS                    = 58,
    SCMI_SENSOR_UNIT_COLOR_TEMP_DEG_K            = 59,
    SCMI_SENSOR_UNIT_BITS                        = 60,
    SCMI_SENSOR_UNIT_BYTES                       = 61,
    SCMI_SENSOR_UNIT_WORDS                       = 62,
    SCMI_SENSOR_UNIT_DOUBLEWORDS                 = 63,
    SCMI_SENSOR_UNIT_QUADWORDS                   = 64,
    SCMI_SENSOR_UNIT_PERCENTAGE                  = 65,
    SCMI_SENSOR_UNIT_PASCALS                     = 66,
    SCMI_SENSOR_UNIT_COUNTS                      = 67,
    SCMI_SENSOR_UNIT_GRAMS                       = 68,
    SCMI_SENSOR_UNIT_NEWTON_METERS               = 69,
    SCMI_SENSOR_UNIT_HITS                        = 70,
    SCMI_SENSOR_UNIT_MISSES                      = 71,
    SCMI_SENSOR_UNIT_RETRIES                     = 72,
    SCMI_SENSOR_UNIT_OVERRUNS_OVERFLOWS          = 73,
    SCMI_SENSOR_UNIT_UNDERRUNS                   = 74,
    SCMI_SENSOR_UNIT_COLLISIONS                  = 75,
    SCMI_SENSOR_UNIT_PACKETS                     = 76,
    SCMI_SENSOR_UNIT_MESSAGES                    = 77,
    SCMI_SENSOR_UNIT_CHARACTERS                  = 78,
    SCMI_SENSOR_UNIT_ERRORS                      = 79,
    SCMI_SENSOR_UNIT_CORRECTED_ERRORS            = 80,
    SCMI_SENSOR_UNIT_UNCORRECTABLE_ERRORS        = 81,
    SCMI_SENSOR_UNIT_SQUARE_MILS                 = 82,
    SCMI_SENSOR_UNIT_SQUARE_INCHES               = 83,
    SCMI_SENSOR_UNIT_SQUARE_FEET                 = 84,
    SCMI_SENSOR_UNIT_SQUARE_CENTIMETERS          = 85,
    SCMI_SENSOR_UNIT_SQUARE_METERS               = 86,
    SCMI_SENSOR_UNIT_RADIANS_PER_SECOND          = 87,
    SCMI_SENSOR_UNIT_BEATS_PER_MINUTE            = 88,
    SCMI_SENSOR_UNIT_METERS_PER_SECOND_SQUARED   = 89,
    SCMI_SENSOR_UNIT_METERS_PER_SECOND           = 90,
    SCMI_SENSOR_UNIT_CUBIC_METERS_PER_SECOND     = 91,
    SCMI_SENSOR_UNIT_MILLIMETERS_OF_MERCURY      = 92,
    SCMI_SENSOR_UNIT_RADIANS_PER_SECOND_SQUARED  = 93,
    SCMI_SENSOR_UNIT_OEM_UNIT                    = 255,
};

enum scmi_sensor_trip_point_event_ctrl {
    SCMI_SENSOR_TP_EVENT_DISABLE        = 0,
    SCMI_SENSOR_TP_EVENT_POSITIVE_ONLY  = 1,
    SCMI_SENSOR_TP_EVENT_NEGATIVE_ONLY  = 2,
    SCMI_SENSOR_TP_EVENT_EITHER_DIR     = 3,
};

/**
 * @brief Get the SCMI Sensor protocol version.
 *
 * Queries the version of the Sensor protocol as defined by the SCMI specification.
 *
 * @param[out] version
 *  Pointer to receive the 32-bit protocol version.
 *
 * @retval 0  Success.
 * @retval <0 Error (invalid argument, transport/SCMI failure, etc.).
 */
int scmi_sensor_protocol_version_get(uint32_t *version);

/**
 * @brief Get SCMI Sensor protocol attributes.
 *
 * Retrieves protocol-level capabilities/attributes (e.g., number of sensors,
 * supported features), as returned in @p attr.
 *
 * @param[out] attr
 *  Pointer to the structure that receives protocol attributes.
 *
 * @retval 0  Success.
 * @retval <0 Error.
 */
int scmi_sensor_protocol_attributes_get(struct scmi_sensor_protocol_attributes *attr);

/**
 * @brief Get attributes for a Sensor protocol message.
 *
 * Used to discover whether a given @p message_id is supported and to obtain
 * message-specific attribute bits.
 *
 * @param[in]  message_id
 *  Sensor protocol message identifier.
 * @param[out] attributes
 *  Pointer to receive the message attributes bitfield.
 *
 * @retval 0  Success.
 * @retval <0 Error.
 */
int scmi_sensor_protocol_message_attributes_get(uint32_t message_id, uint32_t *attributes);

/**
 * @brief Get sensor descriptions (paged).
 *
 * Returns sensor description entries starting at @p desc_index. The function writes
 * up to @p out_cap entries into @p out. Paging/continuation information is returned
 * via @p page.
 *
 * @param[in]  desc_index
 *  Start index within the sensor description list.
 * @param[out] out
 *  Output array receiving sensor description entries.
 * @param[in]  out_cap
 *  Capacity of @p out in number of entries.
 * @param[out] page
 *  Page information (e.g., number of entries returned, whether more pages remain),
 *  as defined by @c struct scmi_sensor_desc_page.
 *
 * @retval 0  Success.
 * @retval <0 Error.
 */
int scmi_sensor_description_get(uint32_t desc_index,
                struct scmi_sensor_desc *out, size_t out_cap,
                struct scmi_sensor_desc_page *page);

/**
 * @brief Configure a sensor trip point.
 *
 * Configures trip point event control and associated threshold values.
 *
 * @param[in] sensor_id
 *  Identifier of the sensor to configure.
 * @param[in] trip_point_id
 *  Trip point identifier within the sensor.
 * @param[in] ctrl
 *  Trip point event control (enable/disable/mode), as defined by
 *  @c enum scmi_sensor_trip_point_event_ctrl.
 * @param[in] trip_point_val_low
 *  Low threshold value for the trip point (units/scale per sensor description).
 * @param[in] trip_point_val_high
 *  High threshold value for the trip point (units/scale per sensor description).
 *
 * @retval 0  Success.
 * @retval <0 Error.
 */
int scmi_sensor_trip_point_config(uint32_t sensor_id,
                  uint8_t trip_point_id,
                  enum scmi_sensor_trip_point_event_ctrl ctrl,
                  uint32_t trip_point_val_low,
                  uint32_t trip_point_val_high);

/**
 * @brief Get sensor reading(s).
 *
 * Retrieves the current sensor reading data. If supported, @p async_read can be used
 * to request an asynchronous read. The function writes up to @p out_cap entries into
 * @p out and returns additional read metadata in @p info.
 *
 * @param[in]  sensor_id
 *  Identifier of the sensor to read.
 * @param[in]  async_read
 *  Asynchronous read request flag (0 = synchronous, non-zero = asynchronous),
 *  if supported by the platform/firmware.
 * @param[out] out
 *  Output array receiving reading descriptor/value entries.
 * @param[in]  out_cap
 *  Capacity of @p out in number of entries.
 * @param[out] info
 *  Read operation metadata (e.g., number of entries returned, async status,
 *  timestamps), as defined by @c struct scmi_sensor_reading_get_info.
 *
 * @retval 0  Success.
 * @retval <0 Error.
 */
int scmi_sensor_reading_get(uint32_t sensor_id,
                uint8_t async_read,
                struct scmi_sensor_reading_desc *out, size_t out_cap,
                struct scmi_sensor_reading_get_info *info);

/**
 * @brief Get the current configuration of a sensor.
 *
 * @param[in]  sensor_id
 *  Identifier of the sensor to query.
 * @param[out] cfg
 *  Pointer to a structure that receives the sensor configuration.
 *
 * @retval 0  Success.
 * @retval <0 Error.
 */
int scmi_sensor_config_get(uint32_t sensor_id, struct scmi_sensor_config *cfg);

/**
 * @brief Set the configuration of a sensor.
 *
 * @param[in] sensor_id
 *  Identifier of the sensor to configure.
 * @param[in] sensor_config
 *  Sensor configuration word/bitfield as defined by the SCMI Sensor protocol.
 *
 * @retval 0  Success.
 * @retval <0 Error.
 */
int scmi_sensor_config_set(uint32_t sensor_id, uint32_t sensor_config);

/** @} */ // end of SCMI_Protocol_Sensor_Module

#endif /* SENSOR_H */