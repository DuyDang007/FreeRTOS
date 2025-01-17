/*************************************************************************************************************
* dmac_ctrl_c
* Copyright (c) 2025 Renesas Electronics Corporation
* This software is released under the MIT License
* http://opensource.org/licenses/mit-license.php
*************************************************************************************************************/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "rt_dmac/rdmac_common.h"
#include "rdmac_reg.h"

#include "FreeRTOS.h"
#include "task.h"

/* Return code of API */
#define drv_OK                                (0U)    /* API completed without any error. */
#define drv_FAIL                              (1U)    /* Failed. */

#define RT_DMACP(X)    (X + 4)

extern int printf_delay(const char *format, ...);

uint16_t R_RTDMAC_RcarDmacExec(DMAC_t dev, uint8_t ch, rDmacCfg_t *cfg, rDmacDescCfg_t *descCfg);
uint16_t R_RTDMAC_RcarDmacStop(DMAC_t dev, uint8_t ch);

/*!
 *  @brief          DMA intialize
 *  @details
 *  @param[in]      mode
 *  @return         drv_OK
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
uint16_t R_RTDMAC_RcarDmacCtrlInit(DMAC_t dev, rDmacPriorityMode_t mode)
{
    uint8_t dev_p;

    /* Set Priority Mode */
    dev_p = RT_DMACP(dev);
    R_RTDMAC_Update_RDMOR(dev_p, mode);
    return drv_OK;
}

/*!
 *  @brief          DMA Execute
 *  @details
 *  @param[in]      cfg
 *  @param[in]      descCfg
 *  @param[in]      cb
 *  @return         drv_OK
 *  @return         drv_ERR_NOT_INITIALIZED
 *  @return         drv_RTDMAC_CH_BUSY
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
uint16_t R_RTDMAC_RcarDmacExec(DMAC_t dev, uint8_t ch, rDmacCfg_t *cfg, rDmacDescCfg_t *descCfg)
{
    uint32_t Value;
    uint8_t dev_p;
    uint16_t ret = drv_OK;

    dev_p = RT_DMACP(dev);         // Convert to RT_DMAC Public's register
    /* RTDMAC Initial Check */
    Value = R_RTDMAC_Get_RDMOR(dev_p);

    if (0 == (Value & DRV_RTDMAC_REG_RDMOR_DME))
    {
        ret = drv_FAIL;
    }
    else {
        /* CH Busy Check */
        Value = R_RTDMAC_Get_RDMCHCR(dev, ch);
        if (0 != (Value & DRV_RTDMAC_REG_RDMCHCR_DE))
        {
	        printf_delay("-----CH_BUSY-----\n");
            ret = drv_FAIL;
        }
        else
        {
            /* cfg Set */
            R_RTDMAC_Update_RDMCHCLR(dev, ch);
            R_RTDMAC_Update_RDMSAR(dev, ch, cfg->mSrcAddr);
            R_RTDMAC_Update_RDMDAR(dev, ch, cfg->mDestAddr);
            R_RTDMAC_Update_RDMTCR(dev, ch, cfg->mTransferCount);
            R_RTDMAC_Update_RDMRS_ID(dev, ch, cfg->mResource);

            if (DRV_DMAC_DMA_NO_DESCRIPTOR != (cfg->mDMAMode && descCfg))
            {
                /* descCfg Set */
                R_RTDMAC_Update_RDMCHCRB_RST(dev, ch);
                R_RTDMAC_Update_RDMDPBASE(dev, ch, descCfg->mDescBaseAddr);
            }

            R_RTDMAC_Update_RDMCHCRB(dev, ch, cfg, descCfg);
            if ( DRV_DMAC_DMA_DESC_READOUT == cfg->mDMAMode )
            {
                R_RTDMAC_Update_RDMDPCR(dev, ch, descCfg->mDescIndex);
            }

            /* DMA Start */
            R_RTDMAC_Update_RDMCHCR(dev, ch, cfg, descCfg);

            printf_delay("DMA Start: SrcAddr=0x%lx, DestAddr=0x%lx\n", cfg->mSrcAddr, cfg->mDestAddr);
        }
    }
    return ret;
}

/*!
 *  @brief          DMA Stop
 *  @details
 *  @return         drv_OK
 *  @return         drv_ERR_NOT_INITIALIZED
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
uint16_t R_RTDMAC_RcarDmacStop(DMAC_t dev, uint8_t ch)
{
    uint32_t Value;
    uint8_t dev_p;
    uint16_t ret = drv_OK;

    dev_p = RT_DMACP(dev);       // Convert to RT_DMAC Public's register
    /* RTDMAC Initial Check */
    Value = R_RTDMAC_Get_RDMOR(dev_p);
    if (0 == (Value & DRV_RTDMAC_REG_RDMOR_DME))
    {
        ret = drv_FAIL;
    }
    else
    {
        /* DMA Stop */
        ret = R_RTDMAC_Clear_RDMCHCR_DE(dev, ch);
        R_RTDMAC_Update_RDMCHCLR(dev, ch);
    }
    return ret;
}
