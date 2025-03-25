#include "smmu_private.h"
#include "smmu/smmu.h"
#include "stdio.h"
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include "cmsis_rcar_gen5.h"
#include "FreeRTOS.h"
#include "rcar_utils.h"

// #include "task.h"
/**
 * @brief Array storing base addresses for each SMMU domain.
 */
static const uintptr_t smmu_base_addresses[] = {
    [SMMU_DSP]      = 0xFA000000,
    [SMMU_HCN]      = 0xF8800000,
    [SMMU_HCS0]     = 0xFCC00000,
    [SMMU_HCS1]     = 0xFCE00000,
    [SMMU_IMN]      = 0xF8000000,
    [SMMU_IMS]      = 0xFC000000,
    [SMMU_MP]       = 0xFE000000,
    [SMMU_NPU0]     = 0xF8400000,
    [SMMU_NPU1]     = 0xFC400000,
    [SMMU_PERE]     = 0xFD000000,
    [SMMU_PERW]     = 0xFD800000,
    [SMMU_PV]       = 0xF8600000,
    [SMMU_RT]       = 0xFB000000,
    [SMMU_SCP]      = 0xFA800000,
    [SMMU_VI0]      = 0xF9000000,
    [SMMU_VI1]      = 0xF9800000,
    [SMMU_VIPN]     = 0xF8200000,
    [SMMU_VIPS]     = 0xFC200000,
    [SMMU_DSP_FMU]  = 0xCBF60000,
    [SMMU_HCN_FMU]  = 0xC9CB0000,
    [SMMU_HCS0_FMU] = 0xDE860000,
    [SMMU_HCS1_FMU] = 0xDE870000,
    [SMMU_IMN_FMU]  = 0xC1980000,
    [SMMU_IMS_FMU]  = 0xC1D80000,
    [SMMU_MP_FMU]   = 0xED600000,
    [SMMU_NPU0_FMU] = 0xD2F60000,
    [SMMU_NPU1_FMU] = 0xD6F60000,
    [SMMU_PERE_FMU] = 0xC09E0000,
    [SMMU_PERW_FMU] = 0xC05B0000,
    [SMMU_PV_FMU]   = 0xCB500000,
    [SMMU_RT_FMU]   = 0x19450000,
    [SMMU_SCP_FMU]  = 0xC1180000,
    [SMMU_VI0_FMU]  = 0xC5840000,
    [SMMU_VI1_FMU]  = 0xC5850000,
    [SMMU_VIPN_FMU] = 0xC3130000,
    [SMMU_VIPS_FMU] = 0xC3530000,
};

/*Default configured values */
#define SMMU_STRTAB_FMT_LINEAR      0b00
#define SMMU_STRTAB_FMT_2LVL        0b01

#define SMMU_QUEUE_SIZE     ((1 << SMMU_QUEUE_LOG2SIZE) * 16)

#define GENMASK(h, l)  (((~0UL) - (1UL << (l)) + 1) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

#define SMMU_STRTAB_BASE_CFG	0x88
#define STRTAB_BASE_CFG_FMT		GENMASK(17, 16)
#define STRTAB_BASE_CFG_FMT_LINEAR	0
#define STRTAB_BASE_CFG_FMT_2LVL	1
#define STRTAB_BASE_CFG_SPLIT		GENMASK(10, 6)
#define STRTAB_BASE_CFG_LOG2SIZE	GENMASK(5, 0)

#define SMMU_CR0_OFFSET			0x20
#define CR0_ATSCHK			(1 << 4)
#define CR0_CMDQEN			(1 << 3)
#define CR0_EVTQEN			(1 << 2)
#define CR0_PRIQEN			(1 << 1)
#define CR0_SMMUEN			(1 << 0)

#define SMMU_CR0ACK_OFFSET              0x24

#define SMMU_STRTAB_BASE_OFFSET		0x80
#define STRTAB_BASE_RA			(1UL << 62)
#define STRTAB_BASE_ADDR_MASK		GENMASK_ULL(51, 6)

#define SMMU_STRTAB_BASE_CFG_OFFSET	0x88
#define STRTAB_BASE_CFG_FMT		GENMASK(17, 16)
#define STRTAB_BASE_CFG_FMT_LINEAR	0
#define STRTAB_BASE_CFG_FMT_2LVL	1
#define STRTAB_BASE_CFG_SPLIT		GENMASK(10, 6)
#define STRTAB_BASE_CFG_LOG2SIZE	GENMASK(5, 0)

#define SMMU_CMDQ_BASE_OFFSET		0x90
#define SMMU_CMDQ_PROD_OFFSET		0x98
#define SMMU_CMDQ_CONS_OFFSET		0x9c

#define SMMU_EVTQ_BASE_OFFSET		0xa0
#define SMMU_EVTQ_PROD_OFFSET		0xa8
#define SMMU_EVTQ_CONS_OFFSET		0xac

/**
 * @brief Initializes the SMMU.
 */

static smmu_l1ste_tbl_t* arm_smmu_alloc_l1_ste(e_smmu_domain_t smmu_device);
static st_smmu_cd_t* smmu_init_cd_table(e_smmu_domain_t smmu_device, uint32_t stream_id);

static inline void write32(uintptr_t addr, uint32_t value);
static void R_SMMU_InitCmdQueue(st_smmu_instance_ctrl_t * p_ctrl);
static void R_SMMU_InitEvtQueue(st_smmu_instance_ctrl_t * p_ctrl);
static int R_SMMU_WriteCmd(st_smmu_cmd_queue_t * cmd_queue, st_smmu_cmd_t *cmd);

static void cmd_queue_write(uint8_t * dest, st_smmu_cmd_t * cmd);
static bool cmd_queue_has_space(st_smmu_cmd_queue_t cmd_queue);
static bool cmd_queue_consumed(st_smmu_cmd_queue_t cmd_queue, uint32_t cur_rd, uint8_t cur_wrap);

int R_SMMU_Init(st_smmu_instance_ctrl_t * const p_ctrl) {
	int res = -1;
    if (!p_ctrl)
    {
        return -1; // Invalid input, return immediately
    }

    volatile st_smmu_reg_cr0_t *reg_cr0 = (st_smmu_reg_cr0_t *)(smmu_base_addresses[p_ctrl->smmu_domain] + SMMU_CR0_OFFSET);
    volatile st_smmu_reg_cr0_t *reg_cr0ack = (st_smmu_reg_cr0_t *)(smmu_base_addresses[p_ctrl->smmu_domain] + SMMU_CR0ACK_OFFSET);
    volatile st_smmu_reg_cmdq_cons_t * reg_cons = (st_smmu_reg_cmdq_cons_t *)(smmu_base_addresses[p_ctrl->smmu_domain] + SMMU_CMDQ_CONS_OFFSET);
    volatile st_smmu_reg_strtab_base_cfg_t *reg_strtab_cfg = (st_smmu_reg_strtab_base_cfg_t *)(smmu_base_addresses[p_ctrl->smmu_domain] + SMMU_STRTAB_BASE_CFG);
    
    reg_strtab_cfg->FMT = 1;
    reg_strtab_cfg->SPLIT = 8;
    reg_strtab_cfg->LOG2SIZE = 20;
	
    // Allocate and initialize Command queue and Event queue memory, base pointers and indexes.
    R_SMMU_InitCmdQueue(p_ctrl);
    reg_cr0->CMDQEN = ENABLE;
    
    uint32_t count = 0;
    while (reg_cr0ack->CMDQEN != ENABLE && count < 100000) {
        count++;
    }

    count = 0;
    R_SMMU_InitEvtQueue(p_ctrl);
    reg_cr0->EVENTQEN = ENABLE;
    while (reg_cr0ack->EVENTQEN != ENABLE && count < 100000) {
        count++;
    }
    if ((reg_cr0ack->EVENTQEN == ENABLE) && (reg_cr0ack->CMDQEN == ENABLE)){
        res = 0;
    } else {
		res = -1;
	}
	return res;
}

void R_SMMU_Attach(st_smmu_instance_ctrl_t * p_ctrl, uint32_t stream_id) {
    if (!p_ctrl)
    {
        return; // Invalid input, return immediately
    }
    smmu_init_cd_table(p_ctrl->smmu_domain, stream_id);
}

void R_SMMU_Detach(st_smmu_instance_ctrl_t * p_ctrl, uint32_t stream_id) {
    // TODO
}

void R_SMMU_Map(st_smmu_instance_ctrl_t * p_ctrl, uint64_t va, uint64_t pa, uint32_t size) {
    // TODO
}

void R_SMMU_Unmap(st_smmu_instance_ctrl_t * p_ctrl, uint64_t va, uint64_t pa, uint32_t size) {
    // TODO
}

/**
 * @brief Reads and processes events from the Event Queue (EVTQ).
 */
void R_SMMU_ProcessEventQueue(void) {
    // TODO: Implement EVTQ processing
}

/**
 * @brief Issues a TLB invalidation command.
 */
int R_SMMU_InvalidateTLB(st_smmu_instance_ctrl_t * p_ctrl) {
    st_smmu_cmd_t cmd_inv_tlb = {0};
    cmd_inv_tlb.opcode = CMDQ_OP_TLBI_NSNH_ALL;
    return R_SMMU_IssueCommand(p_ctrl, &cmd_inv_tlb, true);
}

/**
 * @brief Enables SMMU for translation.
 */
int R_SMMU_Enable(st_smmu_instance_ctrl_t * p_ctrl)
{
    volatile st_smmu_reg_cr0_t *smmu_cr0ack = (st_smmu_reg_cr0_t*)(smmu_base_addresses[p_ctrl->smmu_domain] + SMMU_CR0ACK_OFFSET);
    volatile st_smmu_reg_cr0_t *reg_cr0 = (st_smmu_reg_cr0_t *)(smmu_base_addresses[p_ctrl->smmu_domain] + SMMU_CR0_OFFSET);
    reg_cr0->SMMUEN = ENABLE;

    uint32_t count = 0;
    while (smmu_cr0ack->SMMUEN != ENABLE && count < 100000) {
        count++;
    }
	if (smmu_cr0ack->SMMUEN == ENABLE) {
		return 0;
	} else {
		return -1;
	}
}

/**
 * @brief Disables SMMU.
 */
void R_SMMU_Disable(st_smmu_instance_ctrl_t * p_ctrl)
{
   volatile st_smmu_reg_cr0_t *reg_cr0 = (st_smmu_reg_cr0_t *)(smmu_base_addresses[p_ctrl->smmu_domain] + SMMU_CR0_OFFSET);
   reg_cr0->SMMUEN = DISABLE;
}


static inline void write32(uintptr_t addr, uint32_t value) {
    *(volatile uint32_t *)addr = value;
}

#define EXTRACT_BITS(x, index1, index2) \
    (index1 > index2 ? 0 : ((x << (sizeof(x)*8 - 1 - index2)) >> (sizeof(x)*8 - 1 - index2 + index1)))

static smmu_l1ste_tbl_t* arm_smmu_alloc_l1_ste(e_smmu_domain_t smmu_device) {
    volatile st_smmu_reg_strtab_base_t *smmu_strtab = (st_smmu_reg_strtab_base_t *)(smmu_base_addresses[smmu_device] + SMMU_STRTAB_BASE_OFFSET);
    volatile st_smmu_reg_strtab_base_cfg_t *smmu_strtab_cfg = (st_smmu_reg_strtab_base_cfg_t *)(smmu_base_addresses[smmu_device] + SMMU_STRTAB_BASE_CFG_OFFSET);

    uint32_t num_l1_entry = (1 << (smmu_strtab_cfg->LOG2SIZE - smmu_strtab_cfg->SPLIT));
    num_l1_entry = (num_l1_entry <= TOTAL_L1_STE_ENTRY_SUPPORT) ? num_l1_entry : TOTAL_L1_STE_ENTRY_SUPPORT;

    /*TODO Not yet align. Must align follow spec of field ADDR of res SMMU_CMDQ_BASE */
    //smmu_l1ste_tbl_t *l1_ste_tbl = pvPortMalloc(num_l1_entry*sizeof(smmu_l1ste_tbl_t));
    smmu_l1ste_tbl_t *l1_ste_tbl = aligned_malloc(1 << (smmu_strtab_cfg->LOG2SIZE - smmu_strtab_cfg->SPLIT + 3),num_l1_entry*sizeof(smmu_l1ste_tbl_t));
    if (l1_ste_tbl == NULL)  {
        return NULL;
    }
    
    memset(l1_ste_tbl, 0, num_l1_entry*sizeof(smmu_l1ste_tbl_t));

    smmu_strtab->ADDR = (uintptr_t)l1_ste_tbl >> 6;
    L1C_InvalidateDCacheAll();
    __DSB();
    return l1_ste_tbl;
}

static st_smmu_ste_t* arm_smmu_alloc_l2_ste(e_smmu_domain_t smmu_device, uint32_t l1_ste_idx) {
    volatile st_smmu_reg_strtab_base_t *smmu_strtab = (st_smmu_reg_strtab_base_t *)(smmu_base_addresses[smmu_device] + SMMU_STRTAB_BASE_OFFSET);
    volatile st_smmu_reg_strtab_base_cfg_t *smmu_strtab_cfg = (st_smmu_reg_strtab_base_cfg_t *)(smmu_base_addresses[smmu_device] + SMMU_STRTAB_BASE_CFG_OFFSET);

    uint32_t max_l1_ste_idx = (1 << (smmu_strtab_cfg->LOG2SIZE - smmu_strtab_cfg->SPLIT)) - 1;
    max_l1_ste_idx = max_l1_ste_idx < TOTAL_L1_STE_ENTRY_SUPPORT ? max_l1_ste_idx : TOTAL_L1_STE_ENTRY_SUPPORT;

    if (l1_ste_idx > max_l1_ste_idx) {
        return NULL;
    }

    uintptr_t l1_ste_base = (smmu_strtab->ADDR) << 6;
    smmu_l1ste_tbl_t *l1_ste_tbl = (smmu_l1ste_tbl_t*)l1_ste_base;

    if(l1_ste_tbl == NULL) {
        l1_ste_tbl = arm_smmu_alloc_l1_ste(smmu_device);
        if(l1_ste_tbl == NULL) {
            return NULL;
        }
    }
    
    uint8_t l2_addr_align = 6 + smmu_strtab_cfg->SPLIT;
    uint32_t l2_tbl_size = (1 << smmu_strtab_cfg->SPLIT);
    /* Leak 1 << 14 byte mem */
    st_smmu_ste_t *l2_ste_tbl = aligned_malloc(1 << l2_addr_align, l2_tbl_size*sizeof(st_smmu_ste_t));
    if (l2_ste_tbl == NULL) {
        return NULL;
    }
    
    (l1_ste_tbl + l1_ste_idx)->l2tbl_base = (uintptr_t)l2_ste_tbl >> 6;
    (l1_ste_tbl + l1_ste_idx)->span = smmu_strtab_cfg->SPLIT + 1;
    L1C_InvalidateDCacheAll();
    __DSB();
    return l2_ste_tbl;
}

static st_smmu_ste_t* arm_smmu_init_ste(e_smmu_domain_t smmu_device, uint32_t stream_id){
    volatile st_smmu_reg_strtab_base_t *smmu_strtab = (st_smmu_reg_strtab_base_t *)(smmu_base_addresses[smmu_device] + SMMU_STRTAB_BASE_OFFSET);
    volatile st_smmu_reg_strtab_base_cfg_t *smmu_strtab_cfg = (st_smmu_reg_strtab_base_cfg_t *)(smmu_base_addresses[smmu_device] + SMMU_STRTAB_BASE_CFG_OFFSET);
    uint32_t max_l1_ste_idx = (1 << (smmu_strtab_cfg->LOG2SIZE - smmu_strtab_cfg->SPLIT)) - 1;
    max_l1_ste_idx = max_l1_ste_idx < TOTAL_L1_STE_ENTRY_SUPPORT ? max_l1_ste_idx : TOTAL_L1_STE_ENTRY_SUPPORT;

    uint16_t max_l2_ste_idx = (1 << smmu_strtab_cfg->SPLIT) - 1;

    uint32_t l1_ste_idx = EXTRACT_BITS(stream_id, smmu_strtab_cfg->SPLIT, smmu_strtab_cfg->LOG2SIZE - 1);

    uint16_t l2_ste_idx = EXTRACT_BITS(stream_id, 0, smmu_strtab_cfg->SPLIT - 1);

    if (l1_ste_idx > max_l1_ste_idx || l2_ste_idx > max_l2_ste_idx) {
        return NULL;
    }

    uintptr_t l1_ste_base = smmu_strtab->ADDR << 6;
    smmu_l1ste_tbl_t *l1_ste_tbl = (smmu_l1ste_tbl_t*)l1_ste_base;
    if (l1_ste_tbl == NULL) {
        l1_ste_tbl = arm_smmu_alloc_l1_ste(smmu_device);
        if(l1_ste_tbl == NULL){
            return NULL;
        }
    }

    uintptr_t l2_ste_base = (l1_ste_tbl + l1_ste_idx)->l2tbl_base << 6;
    st_smmu_ste_t *l2_ste_tbl = (st_smmu_ste_t*)l2_ste_base;
    if(l2_ste_tbl == NULL) {
        l2_ste_tbl = arm_smmu_alloc_l2_ste(smmu_device, l1_ste_idx);
        if(l2_ste_tbl == NULL) {
            return NULL;
        }
    }
    
    (l2_ste_tbl + l2_ste_idx)->valid = STRTAB_STE_V;
    // (l2_ste_tbl + l2_ste_idx)->config = STRTAB_STE_CFG_BYPASS;
    (l2_ste_tbl + l2_ste_idx)->config = STRTAB_STE_CFG_S1_TRANS; // translate S1, bypass S2
    (l2_ste_tbl + l2_ste_idx)->s1cdmax = 0;
    (l2_ste_tbl + l2_ste_idx)->s1cir = STRTAB_STE_1_S1C_CACHE_WBRA;
    (l2_ste_tbl + l2_ste_idx)->s1cor = STRTAB_STE_1_S1C_CACHE_WBRA;
    (l2_ste_tbl + l2_ste_idx)->s1csh = STRTAB_STE_1_S1C_SH_ISH;
    (l2_ste_tbl + l2_ste_idx)->strw = STRTAB_STE_1_STRW_EL2;
    (l2_ste_tbl + l2_ste_idx)->s1stalld = STRTAB_STE_1_S1STALLD;
    L1C_InvalidateDCacheAll();
    __DSB();

    return l2_ste_tbl + l2_ste_idx;
}

static st_smmu_cd_t* smmu_init_cd_table(e_smmu_domain_t smmu_device, uint32_t stream_id) {
 
    st_smmu_ste_t *l2_ste_ptr = arm_smmu_init_ste(smmu_device, stream_id); 
    if (l2_ste_ptr == NULL) {
        return NULL;
    }

    /* Leak 1 << 6 byte mem */
    st_smmu_cd_t *cd_tbl = aligned_malloc(1 << 6, sizeof(st_smmu_cd_t));
    if (cd_tbl == NULL) {
        return NULL;
    }
    
    // cd_tbl->t0sz = 16;
    cd_tbl->t0sz = 32;
    cd_tbl->ir0 = CTXDESC_CD_IR_RAWAWB;
    cd_tbl->or0 = CTXDESC_CD_OR_RAWAWB;
    cd_tbl->sh0 = CTXDESC_CD_SH_ISH;
    cd_tbl->epd1 = CTXDESC_CD_TCR_EPD1;
    cd_tbl->valid = 1;
    // cd_tbl->ips = 3; // IPA 42 bit
    cd_tbl->ips = 0;    // IPA 32 bit
    cd_tbl->aa64 = CTXDESC_CD_AA64;
    cd_tbl->ars = (CTXDESC_CD_A << 2 | CTXDESC_CD_R << 1);
    cd_tbl->aset = CTXDESC_CD_ASET;
    cd_tbl->mair0 = 0xf404ff44;
    
    //TODO: remove me
    uint32_t ttb0;
//    initTranslationTable(&ttb0, 0x60000000UL, 0x60000000UL, 0x4000);
//    cd_tbl->ttb0_base = (ttb0 >> 4);
   
    l2_ste_ptr->s1cdptr = (uintptr_t)cd_tbl >> 6;
    
    L1C_InvalidateDCacheAll();
    __DSB();

    return cd_tbl;
}

int R_SMMU_IssueCommand(st_smmu_instance_ctrl_t * p_ctrl, st_smmu_cmd_t *cmd, bool sync){
    /* 1. Determine if there is space to insert commands */
    // Increase WR of CMDQ_PROD
    if ( !cmd_queue_has_space(p_ctrl->cmd_queue)) {
        return - 1;
    }
    
    /*2. Write command into the queue */
    R_SMMU_WriteCmd(&(p_ctrl->cmd_queue), cmd);

    /* 5. If we are inserting a CMD_SYNC, we must wait for it to complete */
    if (sync) {
        // Get Read index before sending CMD_SYNC
        uint32_t cur_rd = p_ctrl->cmd_queue.cons_reg->RD;
        uint8_t cur_wrap = p_ctrl->cmd_queue.cons_reg->RD_WRAP;

        st_smmu_cmd_t cmd_sync = {0};
        cmd_sync.opcode = CMDQ_OP_CMD_SYNC;
        R_SMMU_WriteCmd(&(p_ctrl->cmd_queue), &cmd_sync);
        // Pull until completion
        uint32_t count = 0;
        while (!cmd_queue_consumed(p_ctrl->cmd_queue, cur_rd, cur_wrap)) {
            count++;
            if (count >= 1000000) {
                return -1;
            }
        }
        if (count < 1000000)
            return 0;
    }
	return 0; // Don't verify if command has completed
}

static void R_SMMU_InitCmdQueue(st_smmu_instance_ctrl_t * p_ctrl){
    // Initialize CMDQ_BASE
    p_ctrl->cmd_queue.base_reg = (st_smmu_reg_cmdq_base_t *)(smmu_base_addresses[p_ctrl->smmu_domain] + SMMU_CMDQ_BASE_OFFSET);
    // p_ctrl->cmd_queue.base_reg->ADDR = (uint64_t)pvPortMalloc(SMMU_QUEUE_SIZE);
    uint64_t cmd_queue_ptr = (uintptr_t)aligned_malloc(1<<12, SMMU_QUEUE_SIZE);// align 4k
    p_ctrl->cmd_queue.base_reg->ADDR = cmd_queue_ptr >> 5;
    p_ctrl->cmd_queue.base_reg->LOG2SIZE = SMMU_QUEUE_LOG2SIZE;
    p_ctrl->cmd_queue.base_reg->RA = 0;


    // Initialize CMDQ_PROD and CMDQ_CONS
    p_ctrl->cmd_queue.prod_reg = (st_smmu_reg_cmdq_prod_t *)(smmu_base_addresses[p_ctrl->smmu_domain] + SMMU_CMDQ_PROD_OFFSET);
    p_ctrl->cmd_queue.cons_reg = (st_smmu_reg_cmdq_cons_t *)(smmu_base_addresses[p_ctrl->smmu_domain] + SMMU_CMDQ_CONS_OFFSET);

    p_ctrl->cmd_queue.prod_reg->WR = 0;
    p_ctrl->cmd_queue.prod_reg->WR_WRAP = 0;

    p_ctrl->cmd_queue.cons_reg->RD = 0;
    p_ctrl->cmd_queue.cons_reg->RD_WRAP = 0;

}

static void R_SMMU_InitEvtQueue(st_smmu_instance_ctrl_t * p_ctrl){
    // Initialize CMDQ_BASE
    p_ctrl->evt_queue.base_reg = (st_smmu_reg_eventq_base_t *)(smmu_base_addresses[p_ctrl->smmu_domain] + SMMU_CMDQ_BASE_OFFSET);
    p_ctrl->evt_queue.base_reg->ADDR = (uintptr_t)pvPortMalloc(SMMU_QUEUE_SIZE) >> 5;
    p_ctrl->evt_queue.base_reg->LOG2SIZE = SMMU_QUEUE_LOG2SIZE;
    p_ctrl->evt_queue.base_reg->WA = 0;

    // Initialize CMDQ_PROD and CMDQ_CONS
    p_ctrl->evt_queue.prod_reg = (st_smmu_reg_eventq_prod_t *)(smmu_base_addresses[p_ctrl->smmu_domain] + SMMU_CMDQ_PROD_OFFSET);
    p_ctrl->evt_queue.cons_reg = (st_smmu_reg_eventq_cons_t *)(smmu_base_addresses[p_ctrl->smmu_domain] + SMMU_CMDQ_CONS_OFFSET);

    p_ctrl->evt_queue.prod_reg->WR = 0;
    p_ctrl->evt_queue.prod_reg->WR_WRAP = 0;

    p_ctrl->evt_queue.cons_reg->RD = 0;
    p_ctrl->evt_queue.cons_reg->RD_WRAP = 0;
}

static int R_SMMU_WriteCmd(st_smmu_cmd_queue_t * cmd_queue, st_smmu_cmd_t *cmd) {
    uint32_t queue_index = cmd_queue->prod_reg->WR;       // Get current WR index

    // Get pointer to queue entry
    uint8_t *entry_addr = (uint8_t *)(uintptr_t)(cmd_queue->base_reg->ADDR << 5); // 16 bytes = 2x u64
    entry_addr += 16 * queue_index; // go to next entry

    // Write the command to queue
    cmd_queue_write(entry_addr, cmd);

    // Ensure memory ordering before updating producer register
    __DSB();

    // Increment producer index (WR)
    queue_index++;
    if (queue_index == (1 << SMMU_QUEUE_LOG2SIZE)) {
        queue_index = 0;
        cmd_queue->prod_reg->WR_WRAP ^= 1; // Toggle wrap bit on wraparound
    }
    cmd_queue->prod_reg->WR = queue_index;
}

static void cmd_queue_write(uint8_t * dest, st_smmu_cmd_t * cmd)
{
    for (uint8_t i = 0; i < 16; i++) {
       dest[i] = *((uint8_t *)cmd + i);
    }
}

static bool cmd_queue_has_space(st_smmu_cmd_queue_t cmd_queue) {
    if ((cmd_queue.cons_reg->RD != cmd_queue.prod_reg->WR) || (cmd_queue.prod_reg->WR_WRAP == cmd_queue.cons_reg->RD_WRAP)) {
        return true;
    }
    return false;
}

static bool cmd_queue_consumed(st_smmu_cmd_queue_t cmd_queue, uint32_t cur_rd, uint8_t cur_wrap) {
     if (cur_wrap == cmd_queue.cons_reg->RD_WRAP && cur_rd < cmd_queue.cons_reg->RD) {
         return true;
     }
     if (cur_wrap != cmd_queue.cons_reg->RD_WRAP && cur_rd > cmd_queue.cons_reg->RD) {
         return true;
    }
	return false;
}