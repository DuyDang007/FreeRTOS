/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "mfis.h"
#include "interrupts.h"
#include "devicetree-binding.h"

/* MFIS */
/* Sender: CR52 - Receiver: CA720 */
#define MFIS_BASE   mfis_get_base_address()
#define IICR(i)     (MFIS_BASE + 0x1000 * (i))        // Common communication control register Sender core to Receiver core ch[i]
#define EICR(i)     (MFIS_BASE + 0x1000 * (i) + 0x04) // Common communication control register Receiver core to Sender core ch[i]
#define IMBR(i)     (MFIS_BASE + 0x1000 * (i) + 0x40) // Common communication message register Sender core to Receiver core ch[i]
#define EMBR(i)     (MFIS_BASE + 0x1000 * (i) + 0x44) // Common communication message register Receiver core to Sender core ch[i]

#define MFIS_UNLOCK_WRITE	(0x189e0900)

/* Interrupt ID of MFIS, i=[0-63] */
#define INTID_R_S(i)    (mfis_get_irq_base() + i * 2) // Common INTID ch[i] from Receiver to Sender

/*--------------------------- MFIS Driver ---------------------------------*/
static inline uint32_t mfis_get_base_address(void)
{
    /* There is only one MFIS unit */
    return mfis_list[0]->base_address;
}

static inline uint32_t mfis_get_irq_base(void)
{
    return mfis_list[0]->irq[0][1];
}

static inline uint32_t mfis_get_irq_priority(void)
{
    return mfis_list[0]->irq[0][3];
}

/* Get interrupt source number of a channel */
uint16_t mfis_get_int_source_num(struct mfis_channel *ch)
{
    return (volatile uint16_t) EICR(ch->ch);
}

/* Get message of a channel */
uint16_t mfis_get_message(struct mfis_channel *ch)
{
    return (volatile uint16_t) EMBR(ch->ch);
}

/* Interrupt callback sample */
void mfis_interrupt_cb(void* data)
{
    struct mfis_channel *ch = (struct mfis_channel*) data;
    ch->int_source = mfis_get_int_source_num(ch);
    ch->recv_message = mfis_get_message(ch);
    if(ch->cb_function != (void*)0)
        ch->cb_function(ch->arg);

    /* Clear interrupt flag */
    *(volatile uint32_t *)EICR(ch->ch) = 0;
}

/* Initialize MFIS */
int mfis_init(struct mfis_channel *ch)
{
    /* Initialize */
    ch->int_source = 0;
    ch->recv_message = 0;
    /* Set callback function */
    Irq_SetupEntry(INTID_R_S(ch->ch), (IrqHandlerFn)mfis_interrupt_cb, (void*) ch);
    /* Enable interrupt from Receiver to Sender */
    Irq_SetPriority(INTID_R_S(ch->ch), mfis_get_irq_priority());
    Irq_Enable(INTID_R_S(ch->ch));

    /* Unlock MFIS register write protection */
    *(volatile uint32_t *)(MFIS_UNLOCK_WRITE) = 0xACC00001U;

    return 0;
}

int mfis_deinit(struct mfis_channel *ch)
{
    Irq_Disable(INTID_R_S(ch->ch));
    Irq_RemoveEntry(INTID_R_S(ch->ch));
    return 0;
}

/* Trigger interrupt to Receiver channel ch
int_number is 15-bit integer for interrupt source 
*/
int mfis_trigger_interrupt(struct mfis_channel *ch, uint16_t int_number)
{
    if(ch->ch > 63 || int_number > 0x7FFF)
    {
        return -1;
    }

    *(volatile uint32_t *)IICR(ch->ch) = (int_number << 1) | 0x01;
    return 0;
}

/* Send a 32-bit int to Receiver via message register */
int mfis_send_message(struct mfis_channel *ch, uint32_t value)
{
    if(ch->ch > 63)
    {
        return -1;
    }

    *(volatile uint32_t *)IMBR(ch->ch) = value;
    return 0;
}
