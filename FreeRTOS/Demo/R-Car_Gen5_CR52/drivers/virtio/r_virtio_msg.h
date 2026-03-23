/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef R_VIRTIO_MSG_H
#define R_VIRTIO_MSG_H

#include <stdint.h>

/*----------------------------------------------------------------------
 * Driver IDs — add new drivers here
 *--------------------------------------------------------------------*/
typedef enum {
    VIRTIO_SMMU_ID = 0,
    VIRTIO_GPIO_ID = 1,
    VIRTIO_I2C_ID  = 2,
} e_virtio_driver_id_t;

/*----------------------------------------------------------------------
 * Message type — REQUEST sent by FE, RESPONSE sent by BE
 *--------------------------------------------------------------------*/
typedef enum {
    VIRTIO_MSG_REQUEST  = 0,
    VIRTIO_MSG_RESPONSE = 1,
} e_virtio_msg_type_t;

/*----------------------------------------------------------------------
 * Standard frame layout transmitted over RPMsg
 *
 *  +----------+------------------------------------------------+
 *  | header   | driver_id, msg_type, seq_id, status            |
 *  +----------+------------------------------------------------+
 *  | payload  | driver-specific data                           |
 *  +----------+------------------------------------------------+
 *
 * FE fills : driver_id, msg_type=REQUEST, payload
 *            (seq_id is filled automatically by R_VIRTIO_SendDataSync)
 * BE fills : msg_type=RESPONSE, seq_id (echo from REQ), status, payload (opt)
 *--------------------------------------------------------------------*/
#define VIRTIO_PAYLOAD_MAX_SIZE  120U   /* keep total msg <= 128 bytes */

typedef struct {
    uint8_t  driver_id;  /* e_virtio_driver_id_t                  */
    uint8_t  msg_type;   /* e_virtio_msg_type_t                   */
    uint16_t pad;
    uint32_t seq_id;     /* FE auto-increments, BE echoes back    */
    int32_t  status;     /* result code — meaningful on RESPONSE  */
} st_virtio_msg_header_t;

typedef struct {
    st_virtio_msg_header_t hdr;
    uint8_t                payload[VIRTIO_PAYLOAD_MAX_SIZE] __attribute__((aligned(8)));
} st_virtio_msg_t;

#endif /* R_VIRTIO_MSG_H */
