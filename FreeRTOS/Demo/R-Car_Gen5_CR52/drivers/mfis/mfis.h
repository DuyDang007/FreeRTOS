
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
};

int mfis_init(struct mfis_channel *ch);
int mfis_deinit(struct mfis_channel *ch);
int mfis_trigger_interrupt(struct mfis_channel *ch, uint16_t int_number);
int mfis_send_message(struct mfis_channel *ch, uint32_t value);
void mfis_interrupt_cb(void* data);

#endif // MFIS_H_