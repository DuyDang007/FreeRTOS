/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef MFIS_H_
#define MFIS_H_

#include<stdint.h>

/* MFIS instance for each channel */
struct mfis_channel
{
    uint8_t ch;
    uint16_t int_source;
    uint32_t recv_message;
    void (*cb_function)(void*);
    void* arg;
};

int mfis_init(struct mfis_channel *ch);
int mfis_deinit(struct mfis_channel *ch);
int mfis_trigger_interrupt(struct mfis_channel *ch, uint16_t int_number);
int mfis_send_message(struct mfis_channel *ch, uint32_t value);
uint16_t mfis_get_int_source_num(struct mfis_channel *ch);
uint16_t mfis_get_message(struct mfis_channel *ch);

#endif // MFIS_H_