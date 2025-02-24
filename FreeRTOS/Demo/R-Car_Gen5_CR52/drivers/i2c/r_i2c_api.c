/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "FreeRTOS.h"
#include "task.h"
#include "r_i2c_api.h"
#include "r_i2c_regs.h"
#include "dmac/dmac_common.h"
#include "dmac/sysdmac_ctrl.h"
#include <stdio.h>
#include <stdbool.h>
#define printf_delay(fmt, ...)      \
        vTaskDelay(1);             \
printf(fmt, ##__VA_ARGS__);         \

static int32_t  loc_WaitMsrEvent(r_i2c_Unit_t Unit, uint32_t EventMask);
static uint32_t loc_ReadCommon(r_i2c_Unit_t Unit, uint32_t SlaveAddr,
                               uint8_t *Bytes, uint32_t NumBytes);
typedef struct {
    uint8_t * buf;
    uint32_t len;
    uint32_t pos;
    bool dma_single;
    bool dma_cont;
} msg;

static msg r_i2c_msg;
static bool ID_DONE = false;

static int32_t loc_WaitMsrEvent(r_i2c_Unit_t Unit, uint32_t EventMask)
{
    uint32_t val;
    uintptr_t i2c_base_addr = R_I2C_PRV_GetRegbase(Unit);

    /* Ignore reserved bits */
    EventMask &= 0x7f;

    do {
        val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMSR) & (uint32_t)0x7F;

        /* Check if the master has received a NACK response */
        uint32_t a = (uint32_t)val & ((uint32_t)R_I2C_MNR_BIT);
        if ((val & R_I2C_MNR_BIT) != (uint32_t)0) {
            break;
        }
    } while (!(val & EventMask));

    return (val & (uint32_t)R_I2C_MNR_BIT) ? -1 : 0;
}

void RCar_I2C_Init(r_i2c_Unit_t Unit, uint32_t I2C_ClockRate)
{
    uintptr_t i2c_base_addr = R_I2C_PRV_GetRegbase(Unit);

    switch (I2C_ClockRate)
    {
        case 100000:
            /* Set Clock Control register */
            R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICCCR, 0x1e);
            break;

        case 400000:
	    /* Set Clock Control register */
	    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICCCR, 0xae);
            break;

        case 1000000:
	    /* Set SCL Mask Control regiters (Variable Duty ratio only) */
	    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMPR, 0x13);

	    /* Set SCL High Control regiters (Variable Duty ratio only) */
	    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICHPR, 0x15);

	    /* Set SCL Low Control regiters (Variable Duty ratio only) */
	    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICLPR, 0x15);

	    /* Set Clock Control register 2 */
	    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICCCR2, 0x87);
            break;

        default:
            printf_delay("Invalid I2C ClockRate\n");
            break;
    }

    /* Set First Bit Setup Cycle register (1st bit setup cycle = 17*Tcyc) */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICFBSCR, 0x07);
}

static uint32_t loc_ReadCommon(r_i2c_Unit_t Unit, uint32_t SlaveAddr,
                               uint8_t *Bytes, uint32_t NumBytes)
{
    uintptr_t i2c_base_addr = R_I2C_PRV_GetRegbase(Unit);
    uint32_t val;
    uint32_t i;
    int r;

    (void) SlaveAddr;

    /* Wait for the slave address to be transmitted*/
    r = loc_WaitMsrEvent(Unit, R_I2C_MAT_BIT);
    if (r < 0) {
	    printf_delay("[loc_ReadCommon] loc_WaitMsrEvent : Return value(r) is %d.(Wait for the slave address to be transmitted) Failed(0)\r\n",r);
        return 0;
    }

    if (NumBytes == (uint32_t)1) {
        /* If there is only 1 byte to receive, generate a STOP
         * condition after the byte has been received */
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, 0x8A);

        /* Clear ICMSR_MAT and ICMSR_MDR bits to resume transfer of
         * data */
        val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMSR) & (uint32_t)0x7f;
        val &= (uint32_t)~(R_I2C_MAT_BIT | R_I2C_MDR_BIT);
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, val);

        /* Wait for transfer to complete */
        r = loc_WaitMsrEvent(Unit, (uint32_t)R_I2C_MDR_BIT);
        if (r < 0) {
	        printf_delay("[loc_ReadCommon] loc_WaitMsrEvent : Return value(r) is %d.(Wait for transfer to complete) Failed(0)\r\n",r);
            return 0;
        }

        /* Copy the byte into the buffer */
        Bytes[0] = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICRXD);

        return 1;
    } else {

        /* Suspend data transfer */
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, 0x88);

        /* Clear ICMSR_MAT and ICMSR_MDR bits to resume transmission
         * of data */
        val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMSR) & (uint32_t)0x7f;
        val &= (uint32_t)~(R_I2C_MAT_BIT | R_I2C_MDR_BIT);
	    printf_delay("[R_I2C_Write]: line :%d\r\n", __LINE__);
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, val);

        /* Wait for Data Empty event */
        r = loc_WaitMsrEvent(Unit, R_I2C_MDR_BIT);
        if (r < 0) {
	        printf_delay("[loc_ReadCommon] loc_WaitMsrEvent : Return value(r) is %d.(Wait for Data Empty event 1) Failed(0)\r\n",r);
            return 0;
        }

        /* Copy the first byte into the buffer */
        Bytes[0] = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICRXD);

        i = 1;
        while (i < (NumBytes - (uint32_t)1)) {
            /* Clear ICMSR_MDR bit */
            val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMSR) & (uint32_t)0x7f;
            val &= (uint32_t)~R_I2C_MDR_BIT;
            R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, val);

            /* Wait for Data Empty event */
            r = loc_WaitMsrEvent(Unit, R_I2C_MDR_BIT);
            if (r < 0) {
		printf_delay("[loc_ReadCommon] loc_WaitMsrEvent : Return value(r) is %d.(Wait for Data Empty event 2) Failed(%u)\r\n",r,i);
                return i;
            }

            /* Copy the next byte into the buffer */
            Bytes[i] = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICRXD);
            i++;
        }

        /* Generate a STOP condition after transmission */
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, 0x8A);

        /* Clear ICMSR_MDE bit */
        val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMSR) & (uint32_t)0x7f;
        val &= (uint32_t)~R_I2C_MDR_BIT;
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, val);

        /* Wait for transmission to complete */
        r = loc_WaitMsrEvent(Unit, R_I2C_MDR_BIT);
        if (r < 0) {
	    printf_delay("[loc_ReadCommon] loc_WaitMsrEvent : Return value(r) is %d.(Wait for transmission to complete) Failed(%u)\r\n",r,i);
            return i;
        }

        /* Copy the last byte into the buffer */
        Bytes[i] = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICRXD);
        i++;
        return i;
    }
}

static int rcar_i2c_dma_unmap(r_i2c_Unit_t Unit)
{
    uintptr_t i2c_base_addr = R_I2C_PRV_GetRegbase(Unit);

    ID_DONE = true;
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICDMAER, 0);

    return 0;
}

static void rcar_i2c_dma_callback(r_i2c_Unit_t Unit )
{
    r_i2c_msg.pos = r_i2c_msg.pos;

    rcar_i2c_dma_unmap(Unit);
}


static bool rcar_i2c_dma(r_i2c_Unit_t Unit, bool is_read)
{
    uintptr_t i2c_base_addr = R_I2C_PRV_GetRegbase(Unit);
    uint8_t *buf;
    uint32_t len;
    int ret;

    if (r_i2c_msg.dma_single == 0)
        return false;

    if (is_read) {
        /*
         * The last two bytes needs to be fetched using PIO in
         * order for the STOP phase to work.
         */
        buf = r_i2c_msg.buf;
        len = r_i2c_msg.len - (uint32_t)2;
    } else {
        /*
         * First byte in message was sent using PIO.
         */
        buf = r_i2c_msg.buf + (uint32_t)1;
        len = r_i2c_msg.len - (uint32_t)1;
    }

    /* Define configure DMA Controller */
    rDmacCfg_t cfg =
    {
        //Fill in the configuration details
        .mSrcAddr = ((is_read) ? (i2c_base_addr + R_I2C_ICRXD) : (uintptr_t)(buf)),
        .mDestAddr = (is_read) ? (uintptr_t)buf : (i2c_base_addr + R_I2C_ICTXD),
        .mTransferCount = len,
        .mDMAMode = DRV_DMAC_DMA_NO_DESCRIPTOR, // Assuming DRV_DMAC_DMA_NO_DESCRIPTOR is defined
        .mSrcAddrMode = (is_read) ? DRV_RTDMAC_ADDR_FIXED : DRV_RTDMAC_ADDR_INCREMENTED,
        .mDestAddrMode = (is_read) ? DRV_RTDMAC_ADDR_INCREMENTED:  DRV_RTDMAC_ADDR_FIXED,
        .mResource = (is_read) ? MID_RID_I2C1_MST_RX : MID_RID_I2C1_MST_TX,
        .mTransferUnit = DRV_RTDMAC_TRANS_UNIT_1BYTE,
        .mResource = DRV_RTDMAC_RESOUCE_MAX, // Assuming DRV_RTDMAC_MEMORY is defined
        .mLowSpeed = DRV_RTDMAC_SPEED_NORMAL, // Assuming DRV_RTDMAC_SPEED_NORMAL is defined
        .mPrioLevel = 0
    };

    rDmacIrqCfg_t rDmacIrqHandler_t_irq =
    {
        .Unit = SYS_DMAC3,
        .SubCh = DMAC_CH1,
        .irq_channel = INTID_SYSDMA3_CH1
    };
    // Initialize DMA transfer
    R_SYSDMAC_RcarDmacCtrlInit(SYS_DMAC3, DRV_RTDMAC_PRIO_FIX);

    //ret = R_SYSDMAC_RcarCallBackSet(&rDmacIrqHandler_t_irq, (void *)rcar_i2c_dma_callback, &rDmacIrqHandler_t_irq);
    //if (ret)
    //    printf_delay("CallbackSet Failed: ret = %d\n", ret);

    /* Enable DMA Master Received/Transmitted */
    if (is_read == true) {
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICDMAER, R_I2C_RMDMAE);
    }
    else {
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICDMAER, R_I2C_TMDMAE);
    }
    /* Call DMA API to start transfer */
    int dmaStatus = R_SYSDMAC_RcarDmacExec(SYS_DMAC3, DMAC_CH1, &cfg, 0);

    return (dmaStatus) ? false : true;
}

/*
 * Note: the slave address is 7 bits long, i.e. does not include the
 * direction bit.
 */
uint32_t RCar_I2C_Write(r_i2c_Unit_t Unit, uint32_t SlaveAddr, const uint8_t * Bytes,
                     uint32_t NumBytes, bool dma_single)
{
    uintptr_t i2c_base_addr = R_I2C_PRV_GetRegbase(Unit);
    uint32_t val;
    int r;

    /* Clear Master Status register */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, 0);

    /* Set Master Interrupt Enable register (MDEE=1, MATE=1)*/
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMIER, R_I2C_MDE_BIT | R_I2C_MAT_BIT);

    /* Set Master Address register (slave addr and write mode) */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMAR, ((SlaveAddr << 1) & (0xFFFFFFFE)));

    do {
        val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMCR);
    } while ((val & R_I2C_FSDA_BIT) != (uint32_t)0);

    r_i2c_msg.buf = (uint8_t *)Bytes;
    r_i2c_msg.len = NumBytes;
    r_i2c_msg.pos = 0;
    r_i2c_msg.dma_single = dma_single;

    /* Set Master Control register (MDBS=1, MIE=1, ESG=1) */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, 0x89);
}

static void rcar_i2c_irq_send(r_i2c_Unit_t Unit, uint32_t msr)
{
    uintptr_t i2c_base_addr = R_I2C_PRV_GetRegbase(Unit);
    uint32_t irqs_to_clear = (uint32_t)R_I2C_MDE_BIT;

    if ((msr & R_I2C_MDE_BIT) == (uint32_t)0) {
        return;
    }

    if ((msr & R_I2C_MAT_BIT) != (uint32_t)0) {
        irqs_to_clear |= (uint32_t)R_I2C_MAT_BIT;
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, 0x88);
    }

    if (r_i2c_msg.pos == (uint32_t)1 && rcar_i2c_dma(Unit, false)) {
	    return;
    }

    if (r_i2c_msg.pos < r_i2c_msg.len) {
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICTXD, r_i2c_msg.buf[r_i2c_msg.pos]);
        r_i2c_msg.pos++;
    } else {
        /* Generate a STOP condition after transmission */
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, (uint32_t)0x8A);
    }
    /* Clear irq after handle */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, ~(irqs_to_clear) & (uint32_t)0x7f);
}

uint32_t RCar_I2C_Read(r_i2c_Unit_t Unit, uint32_t SlaveAddr, uint8_t *Bytes, uint32_t NumBytes, bool dma_single)
{
    uintptr_t i2c_base_addr = R_I2C_PRV_GetRegbase(Unit);
    uint32_t val;

    /* Clear Master Status register */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, 0);

    /* Set Master Interrupt Enable register (MDRE=1, MATE=1)*/
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMIER, R_I2C_MDR_BIT | R_I2C_MAT_BIT);

    /* Set Master Address register (slave addr + 0x01 read mode) */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMAR, (SlaveAddr << 1) | (uint32_t)0x01);
    do {
        val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMCR);
    } while ((val & R_I2C_FSDA_BIT) != (uint32_t)0);

    r_i2c_msg.len = NumBytes;
    r_i2c_msg.pos = 0;
    r_i2c_msg.dma_single = dma_single;

    /* Set Master Control register (MDBS=1, MIE=1, ESG=1) */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, (uint32_t)0x89);
    while(!ID_DONE) {
        for (uint32_t i = 0; i < r_i2c_msg.len; i++)
            Bytes[i] = r_i2c_msg.buf[i];
    }
}

static void rcar_i2c_irq_recv(r_i2c_Unit_t Unit, uint32_t msr)
{
    uintptr_t i2c_base_addr = R_I2C_PRV_GetRegbase(Unit);
    uint32_t irqs_to_clear = (uint32_t)R_I2C_MDR_BIT;

    if ((msr & R_I2C_MDR_BIT) == (uint32_t)0)
        return;

    if ((msr & R_I2C_MAT_BIT) != (uint32_t)0) {
        irqs_to_clear |= (uint32_t)R_I2C_MAT_BIT;
        /* Suspend data transfer */
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, 0x88);
        /*
         * Address transfer phase finished, but no data at this point.
         * Try to use DMA to receive data.
         */
        rcar_i2c_dma(Unit, true);
    } else if (r_i2c_msg.pos < r_i2c_msg.len) {
        /* get receive data */
        r_i2c_msg.buf[r_i2c_msg.pos] = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICRXD);
        r_i2c_msg.pos++;
    }

    /* If next received data is the _LAST_, prepare _STOP_ here */
    if (r_i2c_msg.pos + (uint32_t)1 ==  r_i2c_msg.len) {
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, 0x8A);
    }

    /* Clear irq after handle */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, ~(irqs_to_clear) & (uint32_t)0x7f);

    if (r_i2c_msg.pos == r_i2c_msg.len)
	ID_DONE = true;
}

uint32_t R_I2C_ReadRegMap(r_i2c_Unit_t Unit, uint32_t SlaveAddr, uint32_t SlaveReg,
                          uint8_t *Bytes, uint32_t NumBytes)
{
    uintptr_t i2c_base_addr = R_I2C_PRV_GetRegbase(Unit);
    uint32_t val;
    int r;

    /* Clear Master Status register */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, 0);

    /* Set Master Interrupt Enable register (MDEE=1, MATE=1)*/
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMIER, R_I2C_MDE_BIT
                         | R_I2C_MDR_BIT | R_I2C_MAT_BIT);

    /* Set Master Address register (slave addr + 0x00 write mode) */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMAR, (SlaveAddr << 1) & (0xFFFFFFFE));

    /* Load the slave register address into the shift register */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICTXD, SlaveReg);

    do {
        val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMCR);
    } while ((val & R_I2C_FSDA_BIT) != (uint32_t)0);

    /* Set Master Control register (MDBS=1, MIE=1, ESG=1) */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, 0x89);

    /* Wait for the slave address to be transmitted */
    r = loc_WaitMsrEvent(Unit, R_I2C_MAT_BIT);
    if (r < 0) {
	printf_delay("[R_I2C_ReadRegMap] loc_WaitMsrEvent : Return value(r) is %d.(Wait for the slave address to be transmitted) Failed(0)\r\n",r);
        return 0;
    }
    /* Clear ESG bit in ICMCR reg */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, (uint32_t)0x88);

    /* Resume transmission (slave reg address) */
    val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMSR) & (uint32_t)0x7f;
    val &= (uint32_t)~(R_I2C_MAT_BIT | R_I2C_MDE_BIT);
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, val);

    /* Wait for the slave register address to be transmitted */
    r = loc_WaitMsrEvent(Unit, R_I2C_MDE_BIT);
    if (r < 0) {
	printf_delay("[R_I2C_ReadRegMap] loc_WaitMsrEvent : Return value(r) is %d.(Wait for the slave register address to be transmitted) Failed(0)\r\n",r);
        return 0;
    }
    /* Change from Write mode to Read mode */
    /* Set Master Address register (slave addr + 0x01 read mode) */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMAR, (SlaveAddr << 1) | (uint32_t)0x01);

    /* Set again the ESG bit in ICMCR, because we want a repeated
     * START condition on the bus when the data tranfer is resumed */
    /* Set Master Control register (MDBS=1, MIE=1, ESG=1) */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, (uint32_t)0x89);

    /* Clear ICMSR_MDE bits to resume transmission of data */
    val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMSR) & (uint32_t)0x7f;
    val &= (uint32_t)~R_I2C_MDE_BIT;
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, val);

    return loc_ReadCommon(Unit, SlaveAddr, Bytes, NumBytes);
}

int R_I2C_SetInterruptCallback(r_i2c_Unit_t Unit, IrqHandlerFn handler, void *ctx)
{
    uintptr_t i2c_base_addr = R_I2C_PRV_GetRegbase(Unit);
    uint32_t int_id;

    switch (Unit) {
	case R_I2C_IF0:
	    int_id = INTID_I2C_IF0;
	    break;
	case R_I2C_IF1:
	    int_id = INTID_I2C_IF1;
	    break;
	case R_I2C_IF2:
	    int_id = INTID_I2C_IF2;
	    break;
	case R_I2C_IF3:
	    int_id = INTID_I2C_IF3;
	    break;
	case R_I2C_IF4:
	    int_id = INTID_I2C_IF4;
	    break;
	case R_I2C_IF5:
	    int_id = INTID_I2C_IF5;
	    break;
	case R_I2C_IF6:
	    int_id = INTID_I2C_IF6;
	    break;
	case R_I2C_IF7:
	    int_id = INTID_I2C_IF7;
	    break;
	case R_I2C_IF8:
	    int_id = INTID_I2C_IF8;
	    break;
	default:
	    int_id = INTID_NO_EXIST;
	    goto setup_irq_fail;
	}

    /* Set Handler for Irq */
    Irq_SetupEntry(int_id, handler, ctx);

    /* Set priority for Irq */
    Irq_SetPriority(int_id, IPRIORITY(3));

    /* Enable Irq */
    Irq_Enable(int_id);

    return 0;

setup_irq_fail:
    printf("IRQ FAILED: no INTID exist!\n");
    return -1;
}

int R_I2C_Irq_handler(r_i2c_Unit_t Unit)
{
    uintptr_t i2c_base_addr = R_I2C_PRV_GetRegbase(Unit);
    static uint32_t msr;
    static uint32_t val = 0;

    /* Only handle interrupts that are currently enabled */
    msr = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMSR) & (uint32_t)0x7f;
    msr &= R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMIER);

    if (val == (uint32_t)0)
        val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMAR) & (uint32_t)0x1;

    if ((msr & R_I2C_MAL_BIT) != (uint32_t)0) {
        /* Arbitration lost */
        goto out;
    }

    if ((msr & R_I2C_MNR_BIT) != (uint32_t)0) {
        /* HW automatically sends STOP after received NACK */
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMIER, R_I2C_MST_BIT);
    }

    if ((msr & R_I2C_MST_BIT) != (uint32_t)0) {
        /* Last data */
        ID_DONE = true;
        goto out;
    }

    if (val != (uint32_t)0)
        rcar_i2c_irq_recv(Unit, msr);
    else
        rcar_i2c_irq_send(Unit, msr);

    return 0;

out:
    if (ID_DONE) {
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMIER, 0);
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, 0);
    }

    return 0;
}
