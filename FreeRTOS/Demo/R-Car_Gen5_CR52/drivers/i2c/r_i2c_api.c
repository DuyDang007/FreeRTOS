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
#include <stdio.h>

#define printf_delay(fmt, ...)      \
        vTaskDelay(10);             \
printf(fmt, ##__VA_ARGS__);         \

static int32_t  loc_WaitMsrEvent(r_i2c_Unit_t Unit, uint32_t EventMask);
static uint32_t loc_ReadCommon(r_i2c_Unit_t Unit, uint32_t SlaveAddr,
                               uint8_t *Bytes, uint32_t NumBytes);

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
            return -1;
        }

        /* Copy the byte into the buffer */
        Bytes[0] = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICRXD);

        return 0;
    } else {

        /* Suspend data transfer */
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, 0x88);

        /* Clear ICMSR_MAT and ICMSR_MDR bits to resume transmission
         * of data */
        val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMSR) & (uint32_t)0x7f;
        val &= (uint32_t)~(R_I2C_MAT_BIT | R_I2C_MDR_BIT);

        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, val);

        /* Wait for Data Empty event */
        r = loc_WaitMsrEvent(Unit, R_I2C_MDR_BIT);
        if (r < 0) {
	    printf_delay("[loc_ReadCommon] loc_WaitMsrEvent : Return value(r) is %d.(Wait for Data Empty event 1) Failed(0)\r\n",r);
            return -1;
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
            Bytes[i++] = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICRXD);
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
        Bytes[i++] = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICRXD);

        return 0;
    }
}


/*
 * Note: the slave address is 7 bits long, i.e. does not include the
 * direction bit.
 */
uint32_t RCar_I2C_Write(r_i2c_Unit_t Unit, uint32_t SlaveAddr, const uint8_t * Bytes,
                     uint32_t NumBytes)
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

    /* Load the first byte into the shift register */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICTXD, Bytes[0]);

    do {
        val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMCR);
    } while ((val & R_I2C_FSDA_BIT) != (uint32_t)0);

    /* Set Master Control register (MDBS=1, MIE=1, ESG=1) */
    R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, 0x89);

    /* Wait for the slave address to be transmitted*/
    r = loc_WaitMsrEvent(Unit, R_I2C_MAT_BIT);
    if (r < 0) {
        printf_delay("[R_I2C_Write] loc_WaitMsrEvent : Return value(r) is %d.(Wait for the slave address to be transmitted) Failed(0)\r\n",r);
        return -1;
    }

    if (NumBytes == 1) {
        /* If there is only 1 byte to transmit, generate a STOP
         * condition after transmission */
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, 0x8A);

        /* Clear ICMSR_MAT and ICMSR_MDE bits to resume transmission
         * of data */
        val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMSR) & 0x7f;
        val &= ~(R_I2C_MAT_BIT | R_I2C_MDE_BIT);
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, val);

        /* Wait for transmission to complete */
        r = loc_WaitMsrEvent(Unit, R_I2C_MST_BIT);
        if (r < 0) {
	    printf_delay("[R_I2C_Write] loc_WaitMsrEvent :Return value(r) is %d.(Wait for transmission to complete) Failed(0)\r\n",r);
            return -1;
        } else {
            return 0;
        }
    } else {
        int i;

        /* Clear ESG bit in ICMCR reg */
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, 0x88);

        /* Clear ICMSR_MAT and ICMSR_MDE bits to resume transmission
         * of data */
        val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMSR) & 0x7f;
        val &= ~(R_I2C_MAT_BIT | R_I2C_MDE_BIT);
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, val);

        /* Wait for Data Empty event */
        r = loc_WaitMsrEvent(Unit, R_I2C_MDE_BIT);
        if (r < 0) {
	    printf_delay("[R_I2C_Write] loc_WaitMsrEvent : Return value(r) is %d.(Wait for Data Empty event 1) Failed(0)\r\n",r);
            return -1;
        }

        i = 1;
        while (i < NumBytes) {
            /* Load the next byte into the shift register */
            R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICTXD, Bytes[i++]);

            /* Clear ICMSR_MDE bit */
            val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMSR) & 0x7f;
            val &= ~R_I2C_MDE_BIT;
            R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, val);

            /* Wait for Data Empty event */
            r = loc_WaitMsrEvent(Unit, R_I2C_MDE_BIT);
            if (r < 0) {
		printf_delay("[R_I2C_Write] loc_WaitMsrEvent : Return value(r) is %d.(Wait for Data Empty event 2) Failed(%u)\r\n",r,--i);
                return --i;
            }
        }

        /* Generate a STOP condition after transmission */
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMCR, 0x8A);

        /* Clear ICMSR_MDE bit */
        val = R_I2C_PRV_RegRead32(i2c_base_addr + R_I2C_ICMSR) & 0x7f;
        val &= ~R_I2C_MDE_BIT;
        R_I2C_PRV_RegWrite32(i2c_base_addr + R_I2C_ICMSR, val);

        /* Wait for transmission to complete */
        r = loc_WaitMsrEvent(Unit, R_I2C_MST_BIT);
        if (r < 0) {
	    printf_delay("[R_I2C_Write] loc_WaitMsrEvent : Return value(r) is %d.(Wait for transmission to complete) Failed(%u)\r\n",r,--i);
            return --i;
        } else {
            return 0;
        }
    }
}

// Read from slave at specified register offset
uint32_t RCar_I2C_ReadRegMap(r_i2c_Unit_t Unit, uint32_t SlaveAddr, uint32_t SlaveReg,
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
        return -1;
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
        return -1;
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

// Read from slave at default offset 0x00
uint32_t RCar_I2C_Read(r_i2c_Unit_t Unit, uint32_t SlaveAddr, uint8_t *Bytes, uint32_t NumBytes)
{
    return RCar_I2C_ReadRegMap(Unit, SlaveAddr, (uint32_t)0x00, Bytes, NumBytes);
}


