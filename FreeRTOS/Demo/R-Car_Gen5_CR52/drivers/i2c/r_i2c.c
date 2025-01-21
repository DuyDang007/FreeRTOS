/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "i2c/r_i2c.h"
#include "r_i2c_api.h"

#define I2C_OPEN                                (0x00000001ULL)

static void i2c_abort_seq_master (i2c_instance_ctrl_t * const p_instance_ctrl);

int R_I2C_Open(i2c_master_ctrl_t * const p_ctrl, i2c_master_cfg_t const * const p_cfg) {

    i2c_instance_ctrl_t * p_instance_ctrl = (i2c_instance_ctrl_t *) p_ctrl;

    /* Record the configuration on the device for use later */
    p_instance_ctrl->p_cfg             = p_cfg;
    p_instance_ctrl->slave             = p_cfg->slave;
    p_instance_ctrl->addr_mode         = p_cfg->addr_mode;
    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_context         = p_cfg->p_context;
    p_instance_ctrl->p_callback_memory = NULL;

    p_instance_ctrl->p_buff    = NULL;
    p_instance_ctrl->total     = 0U;
    p_instance_ctrl->remain    = 0U;
    p_instance_ctrl->loaded    = 0U;
    p_instance_ctrl->read      = false;
    p_instance_ctrl->restart   = false;
    p_instance_ctrl->err       = false;
    p_instance_ctrl->restarted = false;
    p_instance_ctrl->open      = I2C_OPEN;

    RCar_I2C_Init(p_cfg->channel);

    return 0;
}

int R_I2C_Close(i2c_master_ctrl_t * const p_ctrl) {
    i2c_instance_ctrl_t * p_instance_ctrl = (i2c_instance_ctrl_t *) p_ctrl;

    /* Abort an in-progress transfer with this device only */
    i2c_abort_seq_master(p_instance_ctrl);

    /* The device is now considered closed */
    p_instance_ctrl->open = 0U;

    return 0;
}

int R_I2C_Read(i2c_master_ctrl_t * const p_ctrl,
                         uint8_t * const           p_dest,
                         uint32_t const            bytes,
                         bool const                restart) {
    i2c_instance_ctrl_t * p_instance_ctrl = (i2c_instance_ctrl_t *) p_ctrl;
    RCar_I2C_Read(p_instance_ctrl->p_cfg->channel, p_instance_ctrl->p_cfg->slave, p_dest, bytes);
    return 0;
}

int R_I2C_Write(i2c_master_ctrl_t * const p_ctrl,
                          uint8_t * const           p_src,
                          uint32_t const            bytes,
                          bool const                restart) {
    i2c_instance_ctrl_t * p_instance_ctrl = (i2c_instance_ctrl_t *) p_ctrl;
    RCar_I2C_Write(p_instance_ctrl->p_cfg->channel, p_instance_ctrl->p_cfg->slave, p_src, bytes);
    return 0;

}

int R_I2C_Abort(i2c_master_ctrl_t * const p_ctrl) {
    i2c_instance_ctrl_t * p_instance_ctrl = (i2c_instance_ctrl_t *) p_ctrl;
    i2c_abort_seq_master(p_instance_ctrl);
    return 0;
}

int R_I2C_SlaveAddressSet(i2c_master_ctrl_t * const    p_ctrl,
                                    uint32_t const               slave,
                                    i2c_master_addr_mode_t const addr_mode) {

    i2c_instance_ctrl_t * p_instance_ctrl = (i2c_instance_ctrl_t *) p_ctrl;

    /* Sets the address of the slave device */
    p_instance_ctrl->slave = slave;

    /* Sets the mode of addressing */
    p_instance_ctrl->addr_mode = addr_mode;

    return 0;
}

int R_I2C_CallbackSet(i2c_master_ctrl_t * const          p_ctrl,
                                void (                           * p_callback)(i2c_master_callback_args_t *),
                                void const * const                 p_context,
                                i2c_master_callback_args_t * const p_callback_memory) {

    i2c_instance_ctrl_t * p_instance_ctrl = (i2c_instance_ctrl_t *) p_ctrl;
     p_instance_ctrl->p_callback        = p_callback;
    p_instance_ctrl->p_context         = p_context;
    p_instance_ctrl->p_callback_memory = p_callback_memory;
    return 0;
}

int R_I2C_StatusGet(i2c_master_ctrl_t * const p_ctrl, i2c_master_status_t * p_status) {
    i2c_instance_ctrl_t * p_instance_ctrl = (i2c_instance_ctrl_t *) p_ctrl;
    p_status->open = (I2C_OPEN == p_instance_ctrl->open);
    return 0;
}

static void i2c_abort_seq_master (i2c_instance_ctrl_t * const p_instance_ctrl) {
    // Todo: Implement later.
}
