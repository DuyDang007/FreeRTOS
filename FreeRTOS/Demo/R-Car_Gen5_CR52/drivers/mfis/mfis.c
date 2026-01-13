/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "mfis.h"
#include "interrupts.h"

/* MFIS */
/* Sender: CR52 - Receiver: CA720 */
#define MFIS_BASE   (0x18800000)
#define IICR(i)     (MFIS_BASE + 0x1000 * (i))        // Common communication control register Sender core to Receiver core ch[i]
#define EICR(i)     (MFIS_BASE + 0x1000 * (i) + 0x04) // Common communication control register Receiver core to Sender core ch[i]
#define IMBR(i)     (MFIS_BASE + 0x1000 * (i) + 0x40) // Common communication message register Sender core to Receiver core ch[i]
#define EMBR(i)     (MFIS_BASE + 0x1000 * (i) + 0x44) // Common communication message register Receiver core to Sender core ch[i]

#define MFIS_UNLOCK_WRITE	(0x189e0900)

/* Interrupt ID of MFIS, i=[0-63] */
#define INTID_S_R(i)    (0x0056 + i * 2) // Common INTID ch[i] from Sender to Receiver, unused
#define INTID_R_S(i)    (0x0057 + i * 2) // Common INTID ch[i] from Receiver to Sender
#define MFIS_INTID(i,type)    (0x0057 + i * 2 - type) // Common INTID ch[i] from Sender to Receiver, unused

/*--------------------------- MFIS Driver ---------------------------------*/

/* Get interrupt source number of a channel */
uint16_t mfis_get_int_source_num(struct mfis_channel *ch)
{
    uint16_t result = 0;
    if(ch->type == MFIS_TYPE_RECEVER)
    {
        result = (uint16_t)(*(volatile uint32_t *) EICR(ch->ch)) >> 1;
    }
    else
    {
        result = (uint16_t)(*(volatile uint32_t *) IICR(ch->ch)) >> 1;
    }

    result = (result == 0) ? 0xff : result;
    return result;
}

/* Get message of a channel */
uint16_t mfis_get_message(struct mfis_channel *ch)
{
    if(ch->type == MFIS_TYPE_RECEVER)
    {
        return (volatile uint16_t) EMBR(ch->ch);
    }
    else
    {
        return (volatile uint16_t) IMBR(ch->ch);
    }
    
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
    uint32_t value;
    if(ch->type == MFIS_TYPE_RECEVER)
    {
        value = *(volatile uint32_t *)EICR(ch->ch);
        *(volatile uint32_t *)EICR(ch->ch) = value & 0xFFFE;
    }
    else
    {
        value = *(volatile uint32_t *)EICR(ch->ch);
        *(volatile uint32_t *)IICR(ch->ch) = value & 0xFFFE;
    }
    
}

/* Initialize MFIS */
int mfis_init(struct mfis_channel *ch)
{
    //* Initialize */
    ch->int_source = 0;
    ch->recv_message = 0;
    unsigned int intid = MFIS_INTID(ch->ch, ch->type);
    /* Set callback function */
    Irq_SetupEntry(intid, (IrqHandlerFn)mfis_interrupt_cb, (void*) ch);
    /* Enable interrupt from Receiver to Sender */
    Irq_SetPriority(intid, IPRIORITY(2));
    Irq_Enable(intid);

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

    if (ch->type == MFIS_TYPE_RECEVER)
    {
        *(volatile uint32_t *)IICR(ch->ch) = (int_number << 1) | 0x01;
    }
    else
    {
        *(volatile uint32_t *)EICR(ch->ch) = (int_number << 1) | 0x01;
    }
    return 0;
}

/* Send a 32-bit int to Receiver via message register */
int mfis_send_message(struct mfis_channel *ch, uint32_t value)
{
    if(ch->ch > 63)
    {
        return -1;
    }

    if (ch->type == MFIS_TYPE_RECEVER)
    {
        *(volatile uint32_t *)IMBR(ch->ch) = value;
    }
    else
    {
        *(volatile uint32_t *)EMBR(ch->ch) = value;
    }
    return 0;
}
