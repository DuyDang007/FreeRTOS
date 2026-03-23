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

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "semphr.h"
#include "r_virtio_msg.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define VIRTIO_SEND_TIMEOUT_MS   500U

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

typedef struct st_virtio_context
{
    SemaphoreHandle_t   sem; /* using for R_VIRTIO_SendDataSync */
    uint32_t            s_seq_counter;
    uint32_t            seq_id;
    st_virtio_msg_t     *resp_buf;
    void * p_context;
} st_virtio_context_t;

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
                          st_virtio_context_t *priv);

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

/**
 * @brief Send data over an RPMsg endpoint (non-blocking, fire-and-forget).
 *
 * @param[in] ept  Pointer to the RPMsg endpoint.
 * @param[in] data Pointer to the data buffer to send.
 * @param[in] len  Length of data in bytes.
 *
 * @return 0 on success, negative value on error.
 */
uint8_t R_VIRTIO_SendData(struct rpmsg_endpoint *ept, const void *data, int len);

/**
 * @brief Send a request and block until a response is received from the backend.
 *
 * This function fills hdr.seq_id and hdr.msg_type automatically, sends
 * the request frame, then suspends the calling task until the backend
 * sends back a response frame with a matching seq_id, or until timeout.
 *
 * The frontend endpoint passed here must have been created with
 * R_VIRTIO_FE_ResponseCb as its receive callback.
 *
 * @param[in]  ept        Frontend RPMsg endpoint.
 * @param[in]  req        Request frame; hdr.driver_id and payload must be filled
 *                        by the caller before this call.
 * @param[out] resp       Buffer to receive the response frame (may be NULL if
 *                        the caller only needs the return status code).
 * @param[in]  timeout_ms Timeout in milliseconds; 0 uses VIRTIO_SEND_TIMEOUT_MS.
 *
 * @return  0          Success; resp->hdr.status holds the backend result.
 * @return -ETIMEDOUT  Backend did not reply within the timeout period.
 * @return -ENOMEM     Concurrent request limit (VIRTIO_PENDING_MAX) exceeded.
 * @return -EINVAL     ept or req is NULL.
 */
int R_VIRTIO_SendDataSync(struct rpmsg_endpoint *ept,
                          st_virtio_msg_t       *req,
                          st_virtio_msg_t       *resp,
                          uint32_t               timeout_ms);

/**
 * @brief RPMsg receive callback to register on the frontend endpoint.
 *
 * This function matches incoming RESPONSE frames to pending
 * R_VIRTIO_SendDataSync callers by seq_id and unblocks them.
 *
 * Pass this as the ept_cb argument when calling R_VIRTIO_CreateEP
 * on the frontend side. Do NOT use this callback on the backend endpoint.
 *
 * @param[in] ept   Pointer to the RPMsg endpoint (unused).
 * @param[in] data  Pointer to received data buffer.
 * @param[in] len   Length of received data in bytes.
 * @param[in] src   Source address (unused).
 * @param[in] priv  Private user data (unused).
 *
 * @return RPMSG_SUCCESS always.
 */
int R_VIRTIO_ResponseCb(struct rpmsg_endpoint *ept,
                            void *data, size_t len,
                            uint32_t src, void *priv);

/**
 * @brief Dispatch an incoming request message to the appropriate driver handler.
 *
 * Called by the backend endpoint callback. Routes the message to the
 * correct handler based on msg->hdr.driver_id.
 *
 * @param[in,out] msg Pointer to the virtio message. The handler writes
 *                    its result into msg->hdr.status.
 *
 * @return Result code from the invoked driver handler.
 */
uint32_t VirtIO_driver_handler(st_virtio_msg_t *msg);
#ifdef __cplusplus
}
#endif

#endif /* R_VIRTIO_H */
