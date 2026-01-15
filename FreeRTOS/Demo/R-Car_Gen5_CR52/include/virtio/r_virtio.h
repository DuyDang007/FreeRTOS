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
 * @brief Identifier for supported virtio drivers.
 *
 * This enumeration defines driver IDs used to identify and dispatch
 * virtio driver requests. Each value represents a specific virtio-based device driver.
 */
typedef enum e_virtio_driver_id {
    VIRTIO_SMMU_ID,
    VIRTIO_GPIO_ID,
    VIRTIO_I2C_ID,
} e_driver_id_t;

struct st_virtio_smmu_payload_req;
struct st_virtio_smmu_payload_resp;
/**
 * @brief Virtio message structure.
 *
 * This structure represents a generic virtio message used to transfer
 * driver-specific requests. The driver_id field identifies the target
 * virtio driver, while the payload contains driver-dependent data.
 */
typedef struct st_virtio_msg {
    e_driver_id_t driver_id;
    uint8_t payload[128] __attribute__((aligned(8)));
} st_virtio_msg_t;

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
    MFIS_CH_MAX = 64,
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

typedef struct st_rsc_table_info 
{
	uintptr_t rsc_mem_pa; /**< rsc table physical address */
	size_t rsc_mem_size; /**< Size of the rsc table */
	uintptr_t vring_mem_pa; /**< vring physical address */
	size_t vring_mem_offset; /**< Offset of each vring */
	uintptr_t shared_buf_pa; /**< Shared buffer physical address */
	size_t shared_buf_size; /**< Size of the shared buffer */
} st_rsc_table_info_t;

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
st_virtio_instance_ctrl_t * R_VIRTIO_BE_Create(e_mfis_channel_t mfis_ch);

/**
 * @brief Create a Virtio instance for a specific MFIS channel (Frontend).
 *
 * This function initializes a Virtio frontend device and sets up RPMsg
 * communication using the provided resource table information.
 * It binds the Virtio FE instance to the given MFIS channel and prepares
 * shared resources such as vrings and buffers.
 *
 * @param[in] mfis_ch        MFIS channel to bind the Virtio instance to.
 * @param[in] rsc_table_info Pointer to resource table information used
 *                           to configure shared memory and vrings.
 *
 * @return Pointer to Virtio instance control structure on success.
 * @return NULL on failure.
 */
st_virtio_instance_ctrl_t * R_VIRTIO_FE_Create(e_mfis_channel_t mfis_ch, st_rsc_table_info_t * rsc_table_info);

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
