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

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************/
/* Include */
/*********************************************/
#include "util.h"
#include <stdint.h>

/**
 * @defgroup Sensor_Manager_Module Sensor Manager Module
 * @{
 * @brief This module provides APIs and typedefs to configure sensors and read sensor data.
 *
 * The Sensor Manager module provides APIs to handle SCMI Sensor Management commands:
 *      - 0x00, 0x01, 0x02, 0x03, 0x05, 0x06, 0x09, 0x0A
 */

/*********************************************/
/* Macro */
/*********************************************/
/**
 * @brief Length of short name in SENSOR_DESC.
 */
#define SCMI_SENSOR_NAME_MAX      16u
/**
 * @brief Number of sensors which is supported by X5H.
 */
#define X5H_SENSOR_NUM 4u
/**
 * @brief Mask of sec in sensor_update_interval in SENSOR_CONFIG_SET.
 */
#define SENSOR_CONFIG_SET_SEC_MASK 0xFF00
/**
 * @brief Position of sec in sensor_update_interval in SENSOR_CONFIG_SET.
 */
#define SENSOR_CONFIG_SET_SEC_POS 16u
/**
 * @brief Mask of Timestamp reporting in SENSOR_CONFIG_SET.
 */
#define SENSOR_CONFIG_SET_TIMESTAMP_MASK 0x0002
/**
 * @brief Position of Timestamp reporting in SENSOR_CONFIG_SET.
 */
#define SENSOR_CONFIG_SET_TIMESTAMP_POS 1u
/**
 * @brief Mask of Sensor State in SENSOR_CONFIG_SET.
 */
#define SENSOR_CONFIG_SET_STATE_MASK 0x0001
/**
 * @brief Position of Sensor State in SENSOR_CONFIG_SET.
 */
#define SENSOR_CONFIG_SET_STATE_POS 0u

/*********************************************/
/* Struct */
/*********************************************/
/**
 * @brief Attribute structure of PROTOCOL_ATTRIBUTES.
 */
struct scmi_sensor_protocol_attributes {
    uint16_t num_sensors;       /**< Number of sensors that are present */
    uint8_t  max_async;         /**< Maximum number of outstanding asynchronous commands */
    uint64_t shared_mem_addr;   /**< Sensor shared memory regionvalid if shared_mem_len > 0 */
    uint32_t shared_mem_len;    /**< Length of sensor shared memory region */
};

/* Struct of num_sensor_flags in SENSOR_DESCRIPTION_GET */
/**
 * @brief Struct of SENSOR_DESC in SENSOR_ DESCRIPTION_GET.
 */
struct scmi_sensor_desc_page {
    uint32_t num_returned;  /**< Bits[11:0] */
    uint32_t num_remaining; /**< Bits[31:16] */
};

/**
 * @brief Struct of sensor_update_interval in SENSOR_CONFIG_GET.
 */
struct scmi_sensor_update_interval {
    uint32_t sec;       /**< bits[20:5] */
    int8_t   exponent;  /**< bits[4:0], two's complement */
};

/**
 * @brief Struct of sensor_update_interval in SENSOR_CONFIG_GET.
 */
struct scmi_sensor_config {
    uint32_t sensor_config; /**< raw */

    uint8_t  enabled;       /**< Sensor State */
    uint8_t  timestamped;   /**< Timestamp reporting */

    struct scmi_sensor_update_interval update_interval; /**< sec + exponent */
};

/**
 * @brief Struct of SENSOR_DESC in SENSOR_ DESCRIPTION_GET.
 */
struct scmi_sensor_desc {
    uint32_t sensor_id;                        /**< Sensor ID */
    uint32_t sensor_attributes_low;            /**< sensor_attributes_low */
    uint32_t sensor_attributes_high;           /**< sensor_attributes_high */
    uint8_t sensor_name[SCMI_SENSOR_NAME_MAX]; /**< sensor name */

    uint8_t  ext_attrs_supported;  /**< Derived flags */

    uint32_t sensor_power_uW;      /**< Extended attributes (valid only if ext_attrs_supported == 1 and scalar sensor) */

    uint8_t  resolution_valid;     /**< validate resolution */ 
    int8_t   resolution_exponent;  /**< sensor_resolution[31:27], two's complement */
    uint32_t resolution_res;       /**< sensor_resolution[26:0] */

    uint8_t  range_valid;          /**< validate range */
    int64_t  min_range;            /**< composed from min_range_low/high */
    int64_t  max_range;            /**< composed from max_range_low/high */
};

/**
 * @brief Struct of SENSOR_READING in SENSOR_READING_GET.
 */
struct scmi_sensor_reading_desc {
    int32_t  sensor_value_low;  /**< Lower 32 bits of the sensor value */
    int32_t  sensor_value_high; /**< Higher 32 bits of the sensor value */
    uint32_t timestamp_low;     /**< Lower 32 bits of the timestamp */
    uint32_t timestamp_high;    /**< Higher 32 bits of the timestamp*/
};

/*********************************************/
/* Enum */
/*********************************************/
/**
 * @brief Sensor protocol command message IDs.
 */
enum scmi_sensor_message {
    SCMI_SENSOR_MSG_PROTOCOL_VERSION = 0x0,                /**< Get the implemented SCMI Sensor protocol version. */
    SCMI_SENSOR_MSG_PROTOCOL_ATTRIBUTES = 0x1,             /**< Get protocol-level attributes (e.g., number of sensors). */
    SCMI_SENSOR_MSG_PROTOCOL_MESSAGE_ATTRIBUTES = 0x2,     /**< Get attributes for a specific message ID (e.g., supported/implemented). */
    SCMI_SENSOR_MSG_SENSOR_DESCRIPTION_GET = 0x3,          /**< Get sensor descriptors/capabilities for one or more sensors. */
    SCMI_SENSOR_MSG_SENSOR_TRIP_POINT_NOTIFY = 0x4,        /**< Enable/disable trip-point event notifications for a sensor. */
    SCMI_SENSOR_MSG_SENSOR_TRIP_POINT_CONFIG = 0x5,        /**< Configure one or more trip points for a sensor. */
    SCMI_SENSOR_MSG_SENSOR_READING_GET = 0x6,              /**< Read the current (or latest) sensor measurement value. */
    SCMI_SENSOR_MSG_SENSOR_AXIS_DESCRIPTION_GET = 0x7,     /**< Get axis descriptors for multi-axis sensors. */
    SCMI_SENSOR_MSG_SENSOR_LIST_UPDATE_INTERVALS = 0x8,    /**< List supported sensor update intervals. */
    SCMI_SENSOR_MSG_SENSOR_CONFIG_GET = 0x9,               /**< Get sensor configuration (e.g., enabled state, update interval). */
    SCMI_SENSOR_MSG_SENSOR_CONFIG_SET = 0xA,               /**< Set sensor configuration (e.g., enable/disable, update interval). */
    SCMI_SENSOR_MSG_SENSOR_CONTINUOUS_UPDATE_NOTIFY = 0xB, /**< Enable/disable continuous update event notifications for a sensor. */
    SCMI_SENSOR_MSG_SENSOR_NAME_GET = 0xC,                 /**< Get the name string for a sensor. */
    SCMI_SENSOR_MSG_SENSOR_AXIS_NAME_GET = 0xD,            /**< Get the name string for a sensor axis. */
    SCMI_SENSOR_MSG_NEGOTIATE_PROTOCOL_VERSION = 0x10,     /**< Negotiate the protocol version to use with the platform. */
};

/**
 * @enum scmi_sensor_type
 * @brief SCMI sensor unit type enumeration.
 *
 * This enumeration defines the measurement unit associated with a sensor value
 * as specified by the SCMI Sensor protocol.
 */
enum scmi_sensor_type {
    SCMI_SENSOR_UNIT_NONE                        = 0,   /**< No unit. */
    SCMI_SENSOR_UNIT_UNSPECIFIED                 = 1,   /**< Unspecified unit. */
    SCMI_SENSOR_UNIT_DEGREES_C                   = 2,   /**< Degrees Celsius (°C). */
    SCMI_SENSOR_UNIT_DEGREES_F                   = 3,   /**< Degrees Fahrenheit (°F). */
    SCMI_SENSOR_UNIT_DEGREES_K                   = 4,   /**< Degrees Kelvin (K). */
    SCMI_SENSOR_UNIT_VOLTS                       = 5,   /**< Volts (V). */
    SCMI_SENSOR_UNIT_AMPS                        = 6,   /**< Amperes (A). */
    SCMI_SENSOR_UNIT_WATTS                       = 7,   /**< Watts (W). */
    SCMI_SENSOR_UNIT_JOULES                      = 8,   /**< Joules (J). */
    SCMI_SENSOR_UNIT_COULOMBS                    = 9,   /**< Coulombs (C). */
    SCMI_SENSOR_UNIT_VA                          = 10,  /**< Volt-amperes (VA). */
    SCMI_SENSOR_UNIT_NITS                        = 11,  /**< Nits. */
    SCMI_SENSOR_UNIT_LUMENS                      = 12,  /**< Lumens (lm). */
    SCMI_SENSOR_UNIT_LUX                         = 13,  /**< Lux (lx). */
    SCMI_SENSOR_UNIT_CANDELAS                    = 14,  /**< Candelas (cd). */
    SCMI_SENSOR_UNIT_KPA                         = 15,  /**< Kilopascals (kPa). */
    SCMI_SENSOR_UNIT_PSI                         = 16,  /**< Pounds per square inch (psi). */
    SCMI_SENSOR_UNIT_NEWTONS                     = 17,  /**< Newtons (N). */
    SCMI_SENSOR_UNIT_CFM                         = 18,  /**< Cubic feet per minute (CFM). */
    SCMI_SENSOR_UNIT_RPM                         = 19,  /**< Revolutions per minute (RPM). */
    SCMI_SENSOR_UNIT_HERTZ                       = 20,  /**< Hertz (Hz). */
    SCMI_SENSOR_UNIT_SECONDS                     = 21,  /**< Seconds (s). */
    SCMI_SENSOR_UNIT_MINUTES                     = 22,  /**< Minutes (min). */
    SCMI_SENSOR_UNIT_HOURS                       = 23,  /**< Hours (h). */
    SCMI_SENSOR_UNIT_DAYS                        = 24,  /**< Days (d). */
    SCMI_SENSOR_UNIT_WEEKS                       = 25,  /**< Weeks (wk). */
    SCMI_SENSOR_UNIT_MILS                        = 26,  /**< Mils. */
    SCMI_SENSOR_UNIT_INCHES                      = 27,  /**< Inches (in). */
    SCMI_SENSOR_UNIT_FEET                        = 28,  /**< Feet (ft). */
    SCMI_SENSOR_UNIT_CUBIC_INCHES                = 29,  /**< Cubic inches (in^3). */
    SCMI_SENSOR_UNIT_CUBIC_FEET                  = 30,  /**< Cubic feet (ft^3). */
    SCMI_SENSOR_UNIT_METERS                      = 31,  /**< Meters (m). */
    SCMI_SENSOR_UNIT_CUBIC_CENTIMETERS           = 32,  /**< Cubic centimeters (cm^3). */
    SCMI_SENSOR_UNIT_CUBIC_METERS                = 33,  /**< Cubic meters (m^3). */
    SCMI_SENSOR_UNIT_LITERS                      = 34,  /**< Liters (L). */
    SCMI_SENSOR_UNIT_FLUID_OUNCES                = 35,  /**< Fluid ounces (fl oz). */
    SCMI_SENSOR_UNIT_RADIANS                     = 36,  /**< Radians (rad). */
    SCMI_SENSOR_UNIT_STERADIANS                  = 37,  /**< Steradians (sr). */
    SCMI_SENSOR_UNIT_REVOLUTIONS                 = 38,  /**< Revolutions (rev). */
    SCMI_SENSOR_UNIT_CYCLES                      = 39,  /**< Cycles. */
    SCMI_SENSOR_UNIT_GRAVITIES                   = 40,  /**< Gravities (g). */
    SCMI_SENSOR_UNIT_OUNCES                      = 41,  /**< Ounces (oz). */
    SCMI_SENSOR_UNIT_POUNDS                      = 42,  /**< Pounds (lb). */
    SCMI_SENSOR_UNIT_FOOT_POUNDS                 = 43,  /**< Foot-pounds (ft·lbf). */
    SCMI_SENSOR_UNIT_OUNCE_INCHES                = 44,  /**< Ounce-inches (oz·in). */
    SCMI_SENSOR_UNIT_GAUSS                       = 45,  /**< Gauss (G). */
    SCMI_SENSOR_UNIT_GILBERTS                    = 46,  /**< Gilberts. */
    SCMI_SENSOR_UNIT_HENRIES                     = 47,  /**< Henries (H). */
    SCMI_SENSOR_UNIT_FARADS                      = 48,  /**< Farads (F). */
    SCMI_SENSOR_UNIT_OHMS                        = 49,  /**< Ohms (Ω). */
    SCMI_SENSOR_UNIT_SIEMENS                     = 50,  /**< Siemens (S). */
    SCMI_SENSOR_UNIT_MOLES                       = 51,  /**< Moles (mol). */
    SCMI_SENSOR_UNIT_BECQUERELS                  = 52,  /**< Becquerels (Bq). */
    SCMI_SENSOR_UNIT_PPM                         = 53,  /**< Parts per million (ppm). */
    SCMI_SENSOR_UNIT_DECIBELS                    = 54,  /**< Decibels (dB). */
    SCMI_SENSOR_UNIT_DBA                         = 55,  /**< A-weighted decibels (dBA). */
    SCMI_SENSOR_UNIT_DBC                         = 56,  /**< C-weighted decibels (dBC). */
    SCMI_SENSOR_UNIT_GRAYS                       = 57,  /**< Grays (Gy). */
    SCMI_SENSOR_UNIT_SIEVERTS                    = 58,  /**< Sieverts (Sv). */
    SCMI_SENSOR_UNIT_COLOR_TEMP_DEG_K            = 59,  /**< Color temperature in kelvin (K). */
    SCMI_SENSOR_UNIT_BITS                        = 60,  /**< Bits (b). */
    SCMI_SENSOR_UNIT_BYTES                       = 61,  /**< Bytes (B). */
    SCMI_SENSOR_UNIT_WORDS                       = 62,  /**< Words. */
    SCMI_SENSOR_UNIT_DOUBLEWORDS                 = 63,  /**< Doublewords. */
    SCMI_SENSOR_UNIT_QUADWORDS                   = 64,  /**< Quadwords. */
    SCMI_SENSOR_UNIT_PERCENTAGE                  = 65,  /**< Percentage (%). */
    SCMI_SENSOR_UNIT_PASCALS                     = 66,  /**< Pascals (Pa). */
    SCMI_SENSOR_UNIT_COUNTS                      = 67,  /**< Counts. */
    SCMI_SENSOR_UNIT_GRAMS                       = 68,  /**< Grams (g). */
    SCMI_SENSOR_UNIT_NEWTON_METERS               = 69,  /**< Newton-meters (N·m). */
    SCMI_SENSOR_UNIT_HITS                        = 70,  /**< Hits. */
    SCMI_SENSOR_UNIT_MISSES                      = 71,  /**< Misses. */
    SCMI_SENSOR_UNIT_RETRIES                     = 72,  /**< Retries. */
    SCMI_SENSOR_UNIT_OVERRUNS_OVERFLOWS          = 73,  /**< Overruns/overflows. */
    SCMI_SENSOR_UNIT_UNDERRUNS                   = 74,  /**< Underruns. */
    SCMI_SENSOR_UNIT_COLLISIONS                  = 75,  /**< Collisions. */
    SCMI_SENSOR_UNIT_PACKETS                     = 76,  /**< Packets. */
    SCMI_SENSOR_UNIT_MESSAGES                    = 77,  /**< Messages. */
    SCMI_SENSOR_UNIT_CHARACTERS                  = 78,  /**< Characters. */
    SCMI_SENSOR_UNIT_ERRORS                      = 79,  /**< Errors. */
    SCMI_SENSOR_UNIT_CORRECTED_ERRORS            = 80,  /**< Corrected errors. */
    SCMI_SENSOR_UNIT_UNCORRECTABLE_ERRORS        = 81,  /**< Uncorrectable errors. */
    SCMI_SENSOR_UNIT_SQUARE_MILS                 = 82,  /**< Square mils. */
    SCMI_SENSOR_UNIT_SQUARE_INCHES               = 83,  /**< Square inches (in^2). */
    SCMI_SENSOR_UNIT_SQUARE_FEET                 = 84,  /**< Square feet (ft^2). */
    SCMI_SENSOR_UNIT_SQUARE_CENTIMETERS          = 85,  /**< Square centimeters (cm^2). */
    SCMI_SENSOR_UNIT_SQUARE_METERS               = 86,  /**< Square meters (m^2). */
    SCMI_SENSOR_UNIT_RADIANS_PER_SECOND          = 87,  /**< Radians per second (rad/s). */
    SCMI_SENSOR_UNIT_BEATS_PER_MINUTE            = 88,  /**< Beats per minute (BPM). */
    SCMI_SENSOR_UNIT_METERS_PER_SECOND_SQUARED   = 89,  /**< Meters per second squared (m/s^2). */
    SCMI_SENSOR_UNIT_METERS_PER_SECOND           = 90,  /**< Meters per second (m/s). */
    SCMI_SENSOR_UNIT_CUBIC_METERS_PER_SECOND     = 91,  /**< Cubic meters per second (m^3/s). */
    SCMI_SENSOR_UNIT_MILLIMETERS_OF_MERCURY      = 92,  /**< Millimeters of mercury (mmHg). */
    SCMI_SENSOR_UNIT_RADIANS_PER_SECOND_SQUARED  = 93,  /**< Radians per second squared (rad/s^2). */
    SCMI_SENSOR_UNIT_OEM_UNIT                    = 255, /**< OEM-defined unit. */
};

/**
 * @brief Event control for the trip-point in SENSOR_TRIP_POINT_CONFIG.
 */
enum scmi_sensor_trip_point_event_ctrl {
    SCMI_SENSOR_TP_EVENT_DISABLE        = 0, /**< disables event generation */
    SCMI_SENSOR_TP_EVENT_POSITIVE_ONLY  = 1, /**< enables event generation in a positive direction*/
    SCMI_SENSOR_TP_EVENT_NEGATIVE_ONLY  = 2, /**< enables event generation in a negative direction*/
    SCMI_SENSOR_TP_EVENT_EITHER_DIR     = 3, /**< enables event generation */
};

/*********************************************/
/* Prototype */
/*********************************************/
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
 *
 * @retval 0  Success.
 * @retval <0 Error.
 */
int scmi_sensor_reading_get(uint32_t sensor_id,
                uint8_t async_read,
                struct scmi_sensor_reading_desc *out, size_t out_cap);

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

#ifdef __cplusplus
}
#endif

/** @} */ // end of SCMI_Protocol_Sensor_Module

#endif /* SENSOR_H */