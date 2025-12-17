/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

/**
 * @file r_virtio.h
 * @brief Virtio abstraction layer for RPMsg communication.
 *
 * This module provides APIs to create and manage Virtio instances and
 * RPMsg endpoints for inter-processor communication between CR and CA.
 */

#ifndef R_VIRTIO_H
#define R_VIRTIO_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdbool.h>
#include <stdint.h>

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/* None */

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**
 * @enum e_mfis_channel
 * @brief MFIS communication channel identifiers.
 *
 * Defines logical communication channels between CR and CA cores.
 */
typedef enum e_mfis_channel
{
    /** Channel 0: CR to CA */
    MFIS_CR_TO_CA_CH0 = 0x00,

    /** Channel 1: CR to CA */
    MFIS_CR_TO_CA_CH1,

    /** Maximum number of MFIS channels */
    MFIS_CH_MAX = 8,
} e_mfis_channel_t;

/**
 * @brief Forward declaration of RPMsg endpoint structure.
 */
struct rpmsg_endpoint;

/**
 * @typedef st_virtio_endpoint_t
 * @brief Alias for RPMsg endpoint structure.
 */
typedef struct rpmsg_endpoint st_virtio_endpoint_t;

/**
 * @brief Forward declaration of Virtio instance control structure.
 */
typedef struct st_virtio_instance_ctrl st_virtio_instance_ctrl_t;

/**
 * @typedef virtio_ept_cb
 * @brief Callback function for receiving messages on an endpoint.
 *
 * @param[in] ept   Pointer to the RPMsg endpoint.
 * @param[in] data  Pointer to received payload buffer.
 * @param[in] len   Length of received data in bytes.
 * @param[in] src   Source address of the sender.
 * @param[in] priv  Private user data associated with the endpoint.
 *
 * @return 0 on success, negative value on error.
 */
typedef int (*virtio_ept_cb)(st_virtio_endpoint_t *ept,
                             void *data,
                             size_t len,
                             uint32_t src,
                             void *priv);

/**
 * @typedef virtio_ns_unbind_cb
 * @brief Callback invoked when an endpoint is unbound from name service.
 *
 * @param[in] ept Pointer to the RPMsg endpoint being destroyed.
 */
typedef void (*virtio_ns_unbind_cb)(st_virtio_endpoint_t *ept);

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/

/**
 * @brief Create a Virtio instance for a specific MFIS channel.
 *
 * This function initializes a Virtio device and prepares RPMsg
 * communication on the given MFIS channel.
 *
 * @param[in] mfis_ch MFIS channel to bind the Virtio instance to.
 *
 * @return Pointer to Virtio instance control structure on success.
 * @return NULL on failure.
 */
st_virtio_instance_ctrl_t * R_VIRTIO_Create(e_mfis_channel_t mfis_ch);

/**
 * @brief Release a Virtio instance.
 *
 * This function deinitializes the Virtio device and frees all
 * associated resources.
 *
 * @param[in] p_ctrl Pointer to Virtio instance control structure.
 *
 * @return 0 on success.
 * @return Non-zero value on failure.
 */
uint8_t R_VIRTIO_Release(st_virtio_instance_ctrl_t * p_ctrl);

/**
 * @brief Create an RPMsg endpoint associated with a Virtio instance.
 *
 * @param[in]  p_vdev_ctrl Pointer to Virtio instance control structure.
 * @param[out] p_ept       Pointer to RPMsg endpoint structure.
 * @param[in]  name        Endpoint service name.
 * @param[in]  ept_cb      Callback for receiving messages.
 * @param[in]  unbind_cb   Callback invoked when endpoint is destroyed.
 * @param[in]  priv        Private user data passed to callbacks.
 *
 * @return 0 on success.
 * @return Non-zero value on failure.
 */
uint8_t R_VIRTIO_CreateEP(st_virtio_instance_ctrl_t *p_vdev_ctrl,
                          st_virtio_endpoint_t * p_ept,
                          const char *name,
                          virtio_ept_cb ept_cb,
                          virtio_ns_unbind_cb unbind_cb,
                          void *priv);

/**
 * @brief Release an RPMsg endpoint.
 *
 * This function unregisters and destroys the specified RPMsg endpoint.
 *
 * @param[in] p_ept Pointer to RPMsg endpoint structure.
 *
 * @return 0 on success.
 * @return Non-zero value on failure.
 */
uint8_t R_VIRTIO_ReleaseEP(st_virtio_endpoint_t * p_ept);

#endif /* R_VIRTIO_H */
