/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "i2c/r_sci_i2c.h"
#include "lib/r_i2c_api.h"
#define SCI_I2C_CFG_DMAC_ENABLE 0
#if SCI_I2C_CFG_DMAC_ENABLE
 #include "r_dmac.h"
#endif

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "SI2C" in ASCII, used to determine if channel is open. */
#define SCI_I2C_OPEN                                (0x53493243ULL)

#define SCI_I2C_PRV_CODE_READ                       (0x01U)
#define SCI_I2C_PRV_CODE_10BIT                      (0xF0U)

#define SCI_I2C_PRV_SLAVE_10_BIT_ADDR_LEN_ADJUST    (2U)

#define SCI_I2C_PRV_MDDR_REG_MIN                    (0x80)
#define SCI_I2C_PRV_DUMMY_WRITE_DATA_FOR_READ_OP    (0xFFU)
#define SCI_I2C_PRV_DATA_REG_MASK                   (0xFFFFFF00)
#define SCI_I2C_PRV_GENERATE_REQUEST(R, X)    ((R & ~(uint32_t) (R_SCI0_ICR_IICSDAS_Msk | R_SCI0_ICR_IICSCLS_Msk)) \
                                               | (1U << R_SCI0_ICR_IICSDAS_Pos) | (1U << R_SCI0_ICR_IICSCLS_Pos)   \
                                               | X)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* I2C read/write enumeration */
typedef enum e_sci_i2c_transfer_dir_option
{
    SCI_I2C_TRANSFER_DIR_WRITE = 0x0,
    SCI_I2C_TRANSFER_DIR_READ  = 0x1
} sci_i2c_transfer_dir_t;

/* DMAC TXI/RXI enumeration */
typedef enum e_sci_i2c_dmac_interrupt_trigger
{
    SCI_I2C_DMAC_INTERRUPT_TRIGGER_TXI = 0x0,
    SCI_I2C_DMAC_INTERRUPT_TRIGGER_RXI = 0x1
} sci_i2c_dmac_interrupt_trigger_t;

/**********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/* Internal helper functions */
void        sci_i2c_notify(sci_i2c_instance_ctrl_t * const p_instance_ctrl, i2c_master_event_t const event);
static void sci_i2c_abort_seq_master(sci_i2c_instance_ctrl_t * const p_instance_ctrl);

static fsp_err_t sci_i2c_read_write(i2c_master_ctrl_t * const p_ctrl,
                                    uint8_t * const           p_buffer,
                                    uint32_t const            bytes,
                                    sci_i2c_transfer_dir_t    direction);

/* Functions that manipulate hardware */
static void sci_i2c_open_hw_master(sci_i2c_instance_ctrl_t * const p_instance_ctrl,
                                   i2c_master_cfg_t const * const  p_cfg);
static void sci_i2c_run_hw_master(sci_i2c_instance_ctrl_t * const p_instance_ctrl);

void sci_i2c_txi_isr(void);
void sci_i2c_tei_isr(void);

static void sci_i2c_txi_send_data(sci_i2c_instance_ctrl_t * const p_instance_ctrl);
static void sci_i2c_tei_send_address(sci_i2c_instance_ctrl_t * const p_instance_ctrl);

static void sci_i2c_tei_handler(sci_i2c_instance_ctrl_t * const p_instance_ctrl);
static void sci_i2c_txi_handler(sci_i2c_instance_ctrl_t * const p_instance_ctrl);
static void sci_i2c_txi_process_nack(sci_i2c_instance_ctrl_t * const p_instance_ctrl);
static void sci_i2c_issue_restart_or_stop(sci_i2c_instance_ctrl_t * const p_instance_ctrl);

#if SCI_I2C_CFG_DMAC_ENABLE
void             sci_i2c_rxi_isr(void);
static fsp_err_t sci_i2c_transfer_open(sci_i2c_instance_ctrl_t * p_instance_ctrl, i2c_master_cfg_t const * const p_cfg);
static fsp_err_t sci_i2c_transfer_configure(sci_i2c_instance_ctrl_t        * p_instance_ctrl,
                                            transfer_instance_t const      * p_transfer,
                                            sci_i2c_dmac_interrupt_trigger_t trigger);
static void sci_i2c_reconfigure_interrupts_for_transfer(sci_i2c_instance_ctrl_t * const p_instance_ctrl);
static void sci_i2c_enable_transfer_support_tx(sci_i2c_instance_ctrl_t * const p_instance_ctrl);

#endif

void sci_i2c_tx_dmac_callback(sci_i2c_instance_ctrl_t * p_instance_ctrl);
void sci_i2c_rx_dmac_callback(sci_i2c_instance_ctrl_t * p_instance_ctrl);

/**********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/* constant used as the source location for the DMAC dummy write  */
static const uint8_t g_dummy_write_data_for_read_op = SCI_I2C_PRV_DUMMY_WRITE_DATA_FOR_READ_OP;

/**********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/

/* Simple I2C on SCI HAL API mapping for I2C Master interface */
i2c_master_api_t const g_i2c_master_on_sci =
{
    .open            = R_SCI_I2C_Open,
    .read            = R_SCI_I2C_Read,
    .write           = R_SCI_I2C_Write,
    .abort           = R_SCI_I2C_Abort,
    .slaveAddressSet = R_SCI_I2C_SlaveAddressSet,
    .close           = R_SCI_I2C_Close,
    .callbackSet     = R_SCI_I2C_CallbackSet,
    .statusGet       = R_SCI_I2C_StatusGet
};

/*******************************************************************************************************************//**
 * @addtogroup SCI_I2C
 * @{
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/******************************************************************************************************************//**
 * Opens the I2C device.
 *
 * @retval  FSP_SUCCESS               Device opened without issue.
 * @retval  FSP_ERR_ALREADY_OPEN      Module is already open.
 * @retval  FSP_ERR_ASSERTION         Parameter check failure due to one or more reasons below:
 *                                    1. p_ctrl or p_cfg is NULL.
 *                                    2. extended parameter is NULL.
 *                                    3. Clock rate requested is greater than 400KHz.
 *                                    4. Invalid IRQ number assigned.
 *                                    5. Invalid clock setting assigned.
 *                                    6. If p_transfer_rx is not NULL, p_transfer_tx is NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT  Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_SCI_I2C_Open (i2c_master_ctrl_t * const p_ctrl, i2c_master_cfg_t const * const p_cfg)
{
    sci_i2c_instance_ctrl_t * p_instance_ctrl = (sci_i2c_instance_ctrl_t *) p_ctrl;

    /* Record the configuration on the device for use later */
    p_instance_ctrl->p_cfg             = p_cfg;
    p_instance_ctrl->slave             = p_cfg->slave;
    p_instance_ctrl->addr_mode         = p_cfg->addr_mode;
    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_context         = p_cfg->p_context;
    p_instance_ctrl->p_callback_memory = NULL;

#if SCI_I2C_CFG_DMAC_ENABLE

    /* Open transfer interfaces if available
     * In case of Read operations both p_transfer_tx and p_transfer_rx are used.
     * p_transfer_tx writes 0xFF to the TDR and p_transfer_rx reads from the RDR.
     * If p_transfer_tx is set to NULL, 0xFF would be written to TDR using the CPU
     * and the p_transfer_rx will be used to read the RDR.
     * Such a configuration will make the read operation CPU dependent and there would be
     * no benefit to use DMAC on p_transfer_rx only.
     *
     * In case of Write operation p_transfer_tx is used to write to the TDR.
     */
    err = sci_i2c_transfer_open(p_instance_ctrl, p_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

    p_instance_ctrl->p_buff    = NULL;
    p_instance_ctrl->total     = 0U;
    p_instance_ctrl->remain    = 0U;
    p_instance_ctrl->loaded    = 0U;
    p_instance_ctrl->read      = false;
    p_instance_ctrl->restart   = false;
    p_instance_ctrl->err       = false;
    p_instance_ctrl->restarted = false;
    p_instance_ctrl->open      = SCI_I2C_OPEN;
    return FSP_SUCCESS;
}

/******************************************************************************************************************//**
 * Performs a read from the I2C device.
 * The caller will be notified when the operation has completed (successfully) by an
 * I2C_MASTER_EVENT_RX_COMPLETE in the callback.
 *
 * @retval  FSP_SUCCESS                 Function executed without issue.
 * @retval  FSP_ERR_ASSERTION           The parameter p_ctrl, p_dest, p_callback is NULL, bytes is 0.
 * @retval  FSP_ERR_NOT_OPEN            Device was not even opened.
 **********************************************************************************************************************/
fsp_err_t R_SCI_I2C_Read (i2c_master_ctrl_t * const p_ctrl,
                          uint8_t * const           p_dest,
                          uint32_t const            bytes,
                          bool const                restart)
{
    sci_i2c_instance_ctrl_t * p_instance_ctrl = (sci_i2c_instance_ctrl_t *) p_ctrl;
#if SCI_I2C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl != NULL);
    FSP_ASSERT(bytes != 0U);
#endif
    fsp_err_t err = FSP_SUCCESS;

    /* Record the restart information about this transfer.
     * This is done here to keep the parameter (argument) list of iic_master_read_write to 4. */
    p_instance_ctrl->restart = restart;

    R_I2C_Read(p_instance_ctrl->p_cfg->channel, p_instance_ctrl->p_cfg->slave, p_dest, bytes);
    return err;
}

/******************************************************************************************************************//**
 * Performs a write to the I2C device.
 *
 *  This function will fail if there is already an in-progress I2C transfer on the associated channel. Otherwise, the
 *  I2C write operation will begin. When no callback is provided by the user, this function performs a blocking write.
 *  Otherwise, the write operation is non-blocking and the caller will be notified when the operation has finished by
 *  an I2C_EVENT_TX_COMPLETE in the callback.
 *
 * @retval  FSP_SUCCESS                 Function executed without issue.
 * @retval  FSP_ERR_ASSERTION           p_ctrl, p_src, p_callback is NULL.
 * @retval  FSP_ERR_NOT_OPEN            Device was not even opened.
 **********************************************************************************************************************/
fsp_err_t R_SCI_I2C_Write (i2c_master_ctrl_t * const p_ctrl,
                           uint8_t * const           p_src,
                           uint32_t const            bytes,
                           bool const                restart)
{
    sci_i2c_instance_ctrl_t * p_instance_ctrl = (sci_i2c_instance_ctrl_t *) p_ctrl;
#if SCI_I2C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl != NULL);
#endif
    fsp_err_t err = FSP_SUCCESS;

    /* Record the restart information about this transfer.
     * This is done here to keep the parameter (argument) list of iic_master_read_write to 4. */
    ((sci_i2c_instance_ctrl_t *) p_ctrl)->restart = restart;

     R_I2C_Write(p_instance_ctrl->p_cfg->channel, p_instance_ctrl->p_cfg->slave, p_src, bytes);
    return err;
}

/******************************************************************************************************************//**
 * Aborts any in-progress transfer and forces the I2C peripheral into a ready state.
 *
 *  This function will safely terminate any in-progress I2C transfer with the device. If a transfer is aborted, the user
 *  will be notified via callback with an abort event. Since the callback is optional, this function will also return
 *  a specific error code in this situation.
 *
 * @retval  FSP_SUCCESS                 Transaction was aborted without issue.
 * @retval  FSP_ERR_ASSERTION           p_ctrl is NULL.
 * @retval  FSP_ERR_NOT_OPEN            Device was not even opened.
 **********************************************************************************************************************/
fsp_err_t R_SCI_I2C_Abort (i2c_master_ctrl_t * const p_ctrl)
{
    sci_i2c_instance_ctrl_t * p_instance_ctrl = (sci_i2c_instance_ctrl_t *) p_ctrl;
#if SCI_I2C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl != NULL);
    FSP_ERROR_RETURN(SCI_I2C_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Abort any transfer happening on the channel */
    sci_i2c_abort_seq_master(p_instance_ctrl);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Sets address and addressing mode of the slave device.
 *
 * This function is used to set the device address and addressing mode of the slave without reconfiguring the entire bus.
 *
 * @retval  FSP_SUCCESS                 Address of the slave is set correctly.
 * @retval  FSP_ERR_ASSERTION           p_ctrl is NULL.
 * @retval  FSP_ERR_NOT_OPEN            Device was not even opened.
 * @retval  FSP_ERR_IN_USE              An I2C Transaction is in progress.
 *
 **********************************************************************************************************************/
fsp_err_t R_SCI_I2C_SlaveAddressSet (i2c_master_ctrl_t * const    p_ctrl,
                                     uint32_t const               slave,
                                     i2c_master_addr_mode_t const addr_mode)
{
    sci_i2c_instance_ctrl_t * p_instance_ctrl = (sci_i2c_instance_ctrl_t *) p_ctrl;

    fsp_err_t err = FSP_SUCCESS;

#if SCI_I2C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl != NULL);
    FSP_ERROR_RETURN(SCI_I2C_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    /* Fail if there is already a transfer in progress */
    FSP_ERROR_RETURN(((0 == p_instance_ctrl->remain) && (false == p_instance_ctrl->restarted)), FSP_ERR_IN_USE);
#endif

    /* Sets the address of the slave device */
    p_instance_ctrl->slave = slave;

    /* Sets the mode of addressing */
    p_instance_ctrl->addr_mode = addr_mode;

    return err;
}

/*******************************************************************************************************************//**
 * Updates the user callback and has option of providing memory for callback structure.
 * Implements i2c_master_api_t::callbackSet
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_SCI_I2C_CallbackSet (i2c_master_ctrl_t * const          p_ctrl,
                                 void (                           * p_callback)(i2c_master_callback_args_t *),
                                 void const * const                 p_context,
                                 i2c_master_callback_args_t * const p_callback_memory)
{
    sci_i2c_instance_ctrl_t * p_instance_ctrl = (sci_i2c_instance_ctrl_t *) p_ctrl;

#if (SCI_I2C_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_callback);
    FSP_ERROR_RETURN(SCI_I2C_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Store callback and context */
    p_instance_ctrl->p_callback        = p_callback;
    p_instance_ctrl->p_context         = p_context;
    p_instance_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Provides driver status.
 *
 * @retval     FSP_SUCCESS                   Status stored in p_status.
 * @retval     FSP_ERR_ASSERTION             NULL pointer.
 **********************************************************************************************************************/
fsp_err_t R_SCI_I2C_StatusGet (i2c_master_ctrl_t * const p_ctrl, i2c_master_status_t * p_status)
{
    sci_i2c_instance_ctrl_t * p_instance_ctrl = (sci_i2c_instance_ctrl_t *) p_ctrl;

#if SCI_I2C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl != NULL);
    FSP_ASSERT(p_status != NULL);
#endif

    p_status->open = (SCI_I2C_OPEN == p_instance_ctrl->open);

    return FSP_SUCCESS;
}

/******************************************************************************************************************//**
 * Closes the I2C device. Power down I2C peripheral.
 *
 *  This function will safely terminate any in-progress I2C transfer with the device. If a transfer is aborted, the user
 *  will be notified via callback with an abort event. Since the callback is optional, this function will also return
 *  a specific error code in this situation.
 *
 * @retval  FSP_SUCCESS                 Device closed without issue.
 * @retval  FSP_ERR_ASSERTION           The parameter p_ctrl is NULL.
 * @retval  FSP_ERR_NOT_OPEN            Device was not even opened.
 **********************************************************************************************************************/
fsp_err_t R_SCI_I2C_Close (i2c_master_ctrl_t * const p_ctrl)
{
    sci_i2c_instance_ctrl_t * p_instance_ctrl = (sci_i2c_instance_ctrl_t *) p_ctrl;

#if SCI_I2C_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl != NULL);
    FSP_ERROR_RETURN(SCI_I2C_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Abort an in-progress transfer with this device only */
    sci_i2c_abort_seq_master(p_instance_ctrl);

    /* The device is now considered closed */
    p_instance_ctrl->open = 0U;

#if SCI_I2C_CFG_DMAC_ENABLE
    if (NULL != p_instance_ctrl->p_cfg->p_transfer_rx)
    {
        p_instance_ctrl->p_cfg->p_transfer_rx->p_api->close(p_instance_ctrl->p_cfg->p_transfer_rx->p_ctrl);
    }

    if (NULL != p_instance_ctrl->p_cfg->p_transfer_tx)
    {
        p_instance_ctrl->p_cfg->p_transfer_tx->p_api->close(p_instance_ctrl->p_cfg->p_transfer_tx->p_ctrl);
    }
#endif

    
    return FSP_SUCCESS;
}

/******************************************************************************************************************//**
 * Single point for managing the logic around aborting a transfer when operating as a master.
 *
 * @param[in]       p_instance_ctrl  Pointer to control struct of specific device
 **********************************************************************************************************************/
static void sci_i2c_abort_seq_master (sci_i2c_instance_ctrl_t * const p_instance_ctrl)
{
    /* Safely stop the hardware from operating
     * - Disable Serial Reception and Transmission.
     * - Disable channel interrupts.
     */
//    p_instance_ctrl->p_reg->CCR0 = 0;
//    FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->CCR0_b.RE, 0);
//    FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->CCR0_b.TE, 0);

    /* Set SDA and SCL to high impedance state. */
//    p_instance_ctrl->p_reg->ICR |= R_SCI0_ICR_IICSCLS_Msk | R_SCI0_ICR_IICSDAS_Msk;

    /* Update the transfer descriptor to show no longer in-progress and an error */
    p_instance_ctrl->remain    = 0U;
    p_instance_ctrl->restarted = false;

    /* Update the transfer descriptor to make sure interrupts no longer process */
    p_instance_ctrl->addr_loaded = p_instance_ctrl->addr_total;
    p_instance_ctrl->loaded      = p_instance_ctrl->total;
}


/******************************************************************************************************************//**
 * ISR for ACK/RXI interrupt
 *
 **********************************************************************************************************************/
#if SCI_I2C_CFG_DMAC_ENABLE
void sci_i2c_rxi_isr (void)
{
    SCI_I2C_CFG_MULTIPLEX_INTERRUPT_ENABLE;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;

    /* Nothing to be done here. */

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE;

    SCI_I2C_CFG_MULTIPLEX_INTERRUPT_DISABLE;
}


/*******************************************************************************************************************//**
 * Callback that must be called after a RX DMAC transfer completes.
 *
 * @param[in]     p_instance_ctrl     Pointer to SCI_I2C instance control block
 **********************************************************************************************************************/
void sci_i2c_rx_dmac_callback (sci_i2c_instance_ctrl_t * p_instance_ctrl)
{
    SCI_I2C_CFG_MULTIPLEX_INTERRUPT_ENABLE;

    /* Now that the transfer using DMAC is finished, enable the corresponding IRQ. */
    R_BSP_IrqEnable(p_instance_ctrl->p_cfg->rxi_irq);

    /* This interrupt is invoked once DMAC supported Read transfer is completed. Nothing else to be done here. */

    SCI_I2C_CFG_MULTIPLEX_INTERRUPT_DISABLE;
}

/******************************************************************************************************************//**
 * ISR for NACK/TXI interrupt
 *
 **********************************************************************************************************************/
void sci_i2c_txi_isr (void)
{
    SCI_I2C_CFG_MULTIPLEX_INTERRUPT_ENABLE;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;

    IRQn_Type                 irq             = R_FSP_CurrentIrqGet();
    sci_i2c_instance_ctrl_t * p_instance_ctrl = (sci_i2c_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    /* Call the handler */
    sci_i2c_txi_handler(p_instance_ctrl);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE;

    SCI_I2C_CFG_MULTIPLEX_INTERRUPT_DISABLE;
}

/*******************************************************************************************************************//**
 * Callback that must be called after a TX DMAC transfer completes.
 *
 * @param[in]     p_instance_ctrl     Pointer to SCI_I2C instance control block
 **********************************************************************************************************************/
void sci_i2c_tx_dmac_callback (sci_i2c_instance_ctrl_t * p_instance_ctrl)
{
    SCI_I2C_CFG_MULTIPLEX_INTERRUPT_ENABLE;

    /* Now that the transfer using DMAC is finished, enable the corresponding IRQ. */
    R_BSP_IrqEnable(p_instance_ctrl->p_cfg->txi_irq);

    /* Call the handler */
    sci_i2c_txi_handler(p_instance_ctrl);

    SCI_I2C_CFG_MULTIPLEX_INTERRUPT_DISABLE;
}

#endif
/******************************************************************************************************************//**
 * Handles the STI interrupt
 *
 *********************************************************************************************************************/
void sci_i2c_tei_isr (void)
{
#if 0
    SCI_I2C_CFG_MULTIPLEX_INTERRUPT_ENABLE;

    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE;

    IRQn_Type                 irq             = R_FSP_CurrentIrqGet();
    sci_i2c_instance_ctrl_t * p_instance_ctrl = (sci_i2c_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    /* Call the handler */
    sci_i2c_tei_handler(p_instance_ctrl);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE;

    SCI_I2C_CFG_MULTIPLEX_INTERRUPT_DISABLE;
#endif
}

/******************************************************************************************************************//**
 * Handles the NACK/TXI interrupt .
 *
 * @param[in]       p_instance_ctrl     pointer to the I2C control block.
 **********************************************************************************************************************/

static void sci_i2c_txi_handler (sci_i2c_instance_ctrl_t * const p_instance_ctrl)
{
#if 0
    /* Check if ACK is detected  */
    if (0U == (p_instance_ctrl->p_reg->ISR & R_SCI0_ISR_IICACKR_Msk))
    {
        /* 10 bit address, transfer the remaining address or generate RESTART */
        if (p_instance_ctrl->addr_total != p_instance_ctrl->addr_loaded)
        {
            /* Issue a RESTART as this is 10 bit address read and we have already transmitted the LSB */
            if ((p_instance_ctrl->read) && (2U == p_instance_ctrl->addr_loaded))
            {
                /* Initiate a restart condition.
                 * - The IICRSTARREQ, IICSDAS, IICSCLS bits must be set simultaneously.
                 * - IICDL, IICINTM, IICCSC, and IICACKT settings must be preserved.
                 */
                p_instance_ctrl->p_reg->ICR = SCI_I2C_PRV_GENERATE_REQUEST(p_instance_ctrl->p_reg->ICR,
                                                                           R_SCI0_ICR_IICRSTAREQ_Msk);
            }
            /* Transmit the LSB of the address */
            else
            {
#if SCI_I2C_CFG_DMAC_ENABLE
                sci_i2c_enable_transfer_support_tx(p_instance_ctrl);
#endif

                /* Write 1byte p_instance_ctrl->addr_low to TDR register */
                p_instance_ctrl->p_reg->TDR = SCI_I2C_PRV_DATA_REG_MASK | p_instance_ctrl->addr_low;

                /* Update tracker that we have completed an address byte */
                p_instance_ctrl->addr_remain--;
                p_instance_ctrl->addr_loaded++;
            }
        }

#if SCI_I2C_CFG_DMAC_ENABLE

        /* This is the first interrupt after the completion of DMAC operation, ignore it */
        else if (true == p_instance_ctrl->activation_on_txi)
        {
            p_instance_ctrl->activation_on_txi = false;
        }
#endif

        /* Transmit/Receive data */
        else if (0U < p_instance_ctrl->remain)
        {
            sci_i2c_txi_send_data(p_instance_ctrl);
        }
        /* All transfers complete, send the required bus condition */
        else
        {
            sci_i2c_issue_restart_or_stop(p_instance_ctrl);
        }
    }
    /* NACK detected: Either a NACK interrupt or TXI interrupt with NACK flag set */
    else
    {
        sci_i2c_txi_process_nack(p_instance_ctrl);
    }
#endif
}

/******************************************************************************************************************//**
 * Handles the START/STOP interrupt.
 *
 * @param[in]       p_instance_ctrl     pointer to the I2C control block.
 **********************************************************************************************************************/
static void sci_i2c_tei_handler (sci_i2c_instance_ctrl_t * const p_instance_ctrl)
{
#if 0
    /* Clear the interrupt STI flag */
    p_instance_ctrl->p_reg->ICFCLR = R_SCI0_ICFCLR_IICSTIFC_Msk;

    if (p_instance_ctrl->err)
    {
        /* Set SDA and SCL to high impedance state. */
        p_instance_ctrl->p_reg->ICR |= R_SCI0_ICR_IICSCLS_Msk | R_SCI0_ICR_IICSDAS_Msk;

        /* Notify the user of the error */
        sci_i2c_notify(p_instance_ctrl, I2C_MASTER_EVENT_ABORTED);
    }
    /* Address transmission is not complete, this could be the START condition prior to the transmission
     *  of first address byte or RESTART condition in between a 10 bit address read */
    else if (p_instance_ctrl->addr_total != p_instance_ctrl->addr_loaded)
    {
        /* Check to send address */
        sci_i2c_tei_send_address(p_instance_ctrl);
    }
    /*This a STOP or RESTART after completion of the read/write operation */
    else if (0U == p_instance_ctrl->remain)
    {
        i2c_master_event_t event = p_instance_ctrl->read ? I2C_MASTER_EVENT_RX_COMPLETE : I2C_MASTER_EVENT_TX_COMPLETE;

        /* Set the SCL/SDA lines to the required state */
        if (p_instance_ctrl->restarted)
        {
            /* Configure SDA and SCL for serial output. */
            p_instance_ctrl->p_reg->ICR &= ~(uint32_t) (R_SCI0_ICR_IICSDAS_Msk | R_SCI0_ICR_IICSCLS_Msk);
        }
        else
        {
            /* Set SDA and SCL to high impedance state. */
            p_instance_ctrl->p_reg->ICR |= R_SCI0_ICR_IICSCLS_Msk | R_SCI0_ICR_IICSDAS_Msk;

            /* Disable the transmitter and receiver */
            p_instance_ctrl->p_reg->CCR0 &= ~(uint32_t) (R_SCI0_CCR0_TE_Msk | R_SCI0_CCR0_RE_Msk);

            /* Dummy read to ensure that interrupts are disabled. */
            volatile uint32_t dummy = p_instance_ctrl->p_reg->CCR0;
            FSP_PARAMETER_NOT_USED(dummy);
        }

        /* Notify anyone waiting that the transfer is finished */
        sci_i2c_notify(p_instance_ctrl, event);
    }
    else
    {
        /*Do nothing*/
    }
#endif
}

#if SCI_I2C_CFG_DMAC_ENABLE

/*******************************************************************************************************************//**
 * Configures SCI I2C related transfer drivers (if enabled).
 *
 * @param[in]     p_instance_ctrl   Pointer to SCI I2C specific control structure
 * @param[in]     p_cfg             Pointer to SCI I2C specific configuration structure
 *
 * @retval FSP_SUCCESS                 If configures SCI I2C related transfer drivers
 * @retval FSP_ERR_ASSERTION           Transfer configuration for tx/rx not proper.
 **********************************************************************************************************************/
static fsp_err_t sci_i2c_transfer_open (sci_i2c_instance_ctrl_t * const p_instance_ctrl,
                                        i2c_master_cfg_t const * const  p_cfg)
{
    fsp_err_t err = FSP_SUCCESS;

    if (NULL != p_cfg->p_transfer_rx)
    {
        err = sci_i2c_transfer_configure(p_instance_ctrl, p_cfg->p_transfer_rx, SCI_I2C_DMAC_INTERRUPT_TRIGGER_RXI);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    if (NULL != p_cfg->p_transfer_tx)
    {
        err = sci_i2c_transfer_configure(p_instance_ctrl, p_cfg->p_transfer_tx, SCI_I2C_DMAC_INTERRUPT_TRIGGER_TXI);
        if (FSP_SUCCESS != err)
        {
            if (NULL != p_cfg->p_transfer_rx)
            {
                err = p_cfg->p_transfer_rx->p_api->close(p_cfg->p_transfer_rx->p_ctrl);
            }

            return err;
        }
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Configures DMAC
 * @param[in]     p_instance_ctrl            Pointer to I2C specific control structure
 * @param[in]     p_transfer                 Pointer to DMAC instance structure
 * @param[in]     trigger                    TXI or RXI to be set as trigger
 *
 * @retval        FSP_SUCCESS                Transfer interface is configured with valid parameters.
 * @retval        FSP_ERR_ASSERTION          Pointer to transfer instance for I2C receive is NULL.
 **********************************************************************************************************************/
static fsp_err_t sci_i2c_transfer_configure (sci_i2c_instance_ctrl_t        * p_instance_ctrl,
                                             transfer_instance_t const      * p_transfer,
                                             sci_i2c_dmac_interrupt_trigger_t trigger)
{
    fsp_err_t err;

    /* Set default transfer info and open receive transfer module, if enabled. */
 #if (SCI_I2C_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_transfer->p_api);
    FSP_ASSERT(NULL != p_transfer->p_ctrl);
    FSP_ASSERT(NULL != p_transfer->p_cfg);
    FSP_ASSERT(NULL != p_transfer->p_cfg->p_info);
 #endif
    transfer_info_t * p_info = p_transfer->p_cfg->p_info;
    if (SCI_I2C_DMAC_INTERRUPT_TRIGGER_RXI == trigger)
    {
        p_info->mode           = TRANSFER_MODE_NORMAL;
        p_info->src_addr_mode  = TRANSFER_ADDR_MODE_FIXED;
        p_info->dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED;
        p_info->p_src          = (void *) (&(p_instance_ctrl->p_reg->RDR));
    }
    else
    {
        /* In case of read operation using DMAC, the TXI interrupt will trigger the DMAC to write 0xFF into TDR
         * (See Figure "Example of the procedure for master reception operations in simple I2C mode (when ICR.IICINTM
         * is 1, and transmission interrupts and reception interrupts are in use.)" in the RZ microprocessor manual). */

        /* In case of Write operation this will be reconfigured */
        p_info->mode           = TRANSFER_MODE_NORMAL;
        p_info->src_addr_mode  = TRANSFER_ADDR_MODE_FIXED;
        p_info->dest_addr_mode = TRANSFER_ADDR_MODE_FIXED;
        p_info->p_dest         = (void *) (&(p_instance_ctrl->p_reg->TDR));
    }

    err = p_transfer->p_api->open(p_transfer->p_ctrl, p_transfer->p_cfg);
    FSP_ERROR_RETURN((FSP_SUCCESS == err), err);

    return FSP_SUCCESS;
}

/******************************************************************************************************************//**
 * Reconfigure the address mode for transfer interface
 *
 * @param[in]  p_instance_ctrl     transfer control block
 *
 * @retval FSP_SUCCESS                       Address mode for transfer interface reconfigured.
 **********************************************************************************************************************/
static void sci_i2c_reconfigure_interrupts_for_transfer (sci_i2c_instance_ctrl_t * const p_instance_ctrl)
{
    const transfer_instance_t * p_transfer_tx = p_instance_ctrl->p_cfg->p_transfer_tx;

    if (NULL != p_transfer_tx)
    {
        transfer_info_t * p_info = p_transfer_tx->p_cfg->p_info;

        /* Disable the transmitter and receiver for reconfiguring interrupt source */
        p_instance_ctrl->p_reg->CCR0 = 0;
        FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->CCR0_b.RE, 0);
        FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->CCR0_b.TE, 0);

        if (p_instance_ctrl->read)
        {
            /* Re-adjust address modes */
            p_info->mode           = TRANSFER_MODE_NORMAL;
            p_info->src_addr_mode  = TRANSFER_ADDR_MODE_FIXED;
            p_info->dest_addr_mode = TRANSFER_ADDR_MODE_FIXED;
            p_info->p_src          = (void *) &g_dummy_write_data_for_read_op;
        }
        else                           /* This is a write operation */
        {
            /* Re-adjust address modes */
            p_info->mode           = TRANSFER_MODE_NORMAL;
            p_info->src_addr_mode  = TRANSFER_ADDR_MODE_INCREMENTED;
            p_info->dest_addr_mode = TRANSFER_ADDR_MODE_FIXED;
        }

        /* Set the interrupt source to RXI/TXI */
        p_instance_ctrl->p_reg->ICR |= R_SCI0_ICR_IICACKT_Msk;
        p_instance_ctrl->p_reg->CCR0 =
            (R_SCI0_CCR0_TE_Msk | R_SCI0_CCR0_RE_Msk | R_SCI0_CCR0_TEIE_Msk | R_SCI0_CCR0_TIE_Msk);
        FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->CCR0_b.RE, 1);
        FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->CCR0_b.TE, 1);
    }
}

/*******************************************************************************************************************//**
 * Enables the dmac transfer interface for the transmit operation
 *
 * @param[in]       p_instance_ctrl  Pointer to transfer control block
 **********************************************************************************************************************/
static void sci_i2c_enable_transfer_support_tx (sci_i2c_instance_ctrl_t * const p_instance_ctrl)
{
    const transfer_instance_t * p_transfer_tx = p_instance_ctrl->p_cfg->p_transfer_tx;

    if ((NULL != p_transfer_tx) && (!p_instance_ctrl->read))
    {
        /* Enable transfer interface to write to TDR
         * Re-configures the source buffer to the user buffer as this is a Write operation.
         */
        p_transfer_tx->p_cfg->p_info->p_src  = (void *) (p_instance_ctrl->p_buff);
        p_transfer_tx->p_cfg->p_info->length = p_instance_ctrl->remain;

        /* Disable the corresponding IRQ when transferring using DMAC. */
        R_BSP_IrqDisable(p_instance_ctrl->p_cfg->txi_irq);

        p_transfer_tx->p_api->reconfigure(p_transfer_tx->p_ctrl, p_transfer_tx->p_cfg->p_info);

        p_instance_ctrl->remain = 0U;
        p_instance_ctrl->loaded = p_instance_ctrl->total;

        p_instance_ctrl->activation_on_txi = true;
    }
}

#endif

/*******************************************************************************************************************//**
 * Check for the receive condition.
 *
 * @param[in]  p_instance_ctrl     Pointer to transfer control block
 **********************************************************************************************************************/
static void sci_i2c_txi_send_data (sci_i2c_instance_ctrl_t * const p_instance_ctrl)
{
#if 0
    /* As per the hardware manual, a byte should be written to TDR to generate SCL.
     * If we are doing an I2C read, we will write 0xFF to the TDR  */
    uint8_t data = g_dummy_write_data_for_read_op;

    /* This is a write operation, update the data byte from user buffer */
    if (!p_instance_ctrl->read)
    {
        data = p_instance_ctrl->p_buff[p_instance_ctrl->loaded];

        /* Update trackers */
        p_instance_ctrl->remain--;
        p_instance_ctrl->loaded++;
    }
    /* This is a read operation, move data from RDR and do dummy write to TDR. */
    else
    {
        /* Skip updating the buffer on first interrupt as it is the ACK of address */
        if (true == p_instance_ctrl->do_dummy_read)
        {
            p_instance_ctrl->do_dummy_read = false;
#if SCI_I2C_CFG_DMAC_ENABLE

            /* If transfer interface is available, use it.
             * Enable the transfer interfaces if the number of bytes to be read is greater than 2.
             * The last two bytes will be read through interrupt, this is for support NACK before STOP.
             */
            if ((NULL != p_instance_ctrl->p_cfg->p_transfer_tx) && (NULL != p_instance_ctrl->p_cfg->p_transfer_rx) &&
                (p_instance_ctrl->total > 2U))
            {
                /* Enable RXI interrupt */
                p_instance_ctrl->p_reg->CCR0 =
                    (R_SCI0_CCR0_TE_Msk | R_SCI0_CCR0_RE_Msk | R_SCI0_CCR0_TEIE_Msk | R_SCI0_CCR0_TIE_Msk |
                     R_SCI0_CCR0_RIE_Msk);
                FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->CCR0_b.RE, 1);
                FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->CCR0_b.TE, 1);

                /* Enable transfer interface for reading data from RDR */
                p_instance_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->p_dest = (void *) (p_instance_ctrl->p_buff);
                p_instance_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info->length = p_instance_ctrl->total - 2U;

                /* Disable the corresponding IRQ when transferring using DMAC. */
                R_BSP_IrqDisable(p_instance_ctrl->p_cfg->rxi_irq);

                p_instance_ctrl->p_cfg->p_transfer_rx->p_api->reconfigure(p_instance_ctrl->p_cfg->p_transfer_rx->p_ctrl,
                                                                          p_instance_ctrl->p_cfg->p_transfer_rx->p_cfg->p_info);

                /* Enable transfer interface to do dummy write into TDR */
                p_instance_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info->length = p_instance_ctrl->total - 2U;

                /* Disable the corresponding IRQ when transferring using DMAC. */
                R_BSP_IrqDisable(p_instance_ctrl->p_cfg->txi_irq);

                p_instance_ctrl->p_cfg->p_transfer_tx->p_api->reconfigure(p_instance_ctrl->p_cfg->p_transfer_tx->p_ctrl,
                                                                          p_instance_ctrl->p_cfg->p_transfer_tx->p_cfg->p_info);

                /* Update the tracker variables */
                p_instance_ctrl->remain = 2U;
                p_instance_ctrl->loaded = p_instance_ctrl->total - 2U;

                /* Mark DMAC activation for TXI */
                p_instance_ctrl->activation_on_txi = true;
            }
#endif

            /* If number of bytes to be received is greater than one, set ACK transmission
             * NACK transmission will be set before the last byte is read.
             */
            if (1U < p_instance_ctrl->remain)
            {
                p_instance_ctrl->p_reg->ICR_b.IICACKT = 0;
            }
        }
        /* Read data into the buffer */
        else
        {
            p_instance_ctrl->p_buff[p_instance_ctrl->loaded] = (uint8_t) (p_instance_ctrl->p_reg->RDR & UINT8_MAX);

            /* Update trackers */
            p_instance_ctrl->remain--;
            p_instance_ctrl->loaded++;
        }

        /* Enable NACK transmission prior to the reception of the last byte */
        if (1U == p_instance_ctrl->remain)
        {
            p_instance_ctrl->p_reg->ICR_b.IICACKT = 1;
        }
    }

    /* Write 1 byte data to data register (see Figure "Example of the procedure for master transmission operations in
     * simple I2C mode (when ICR.IICINTM is 1, and when confirming ACK / NACK by address transmission only.)" and
     * Figure "Example of the procedure for master reception operations in simple I2C mode (when ICR.IICINTM is 1, and
     * transmission interrupts and reception interrupts are in use.)" in the RZ microprocessor manual).
     * - In case of write operation this will be data from user buffer.
     * - In case of read operation this will be 0xFF as required by HW.
     */
    p_instance_ctrl->p_reg->TDR = SCI_I2C_PRV_DATA_REG_MASK | data;
#endif
}
