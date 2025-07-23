#ifndef __UCIEDRV_H_
#define __UCIEDRV_H_

/*******************************************************************************
  Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
  Defines
*******************************************************************************/

struct hdma_info_ex {
	uint32_t ucieCh;	// 0 or 1
	uint32_t dmaCh;		// 0 - 31
	uint64_t sar;
	uint64_t dar;
	uint64_t llp;		// Linked List Pointer Address
	uint32_t rw;		// Use RDCH(1), WRCH(0)
	uint32_t size;		// Transfer Size (byte)
	uint8_t  tc;		// Traffic Class (0 - 7)
	uint8_t  weight;	// weight (0 - 31)
};

typedef struct{
    uint32_t PLL9_CR0;
    uint32_t PLL9_CR1;
    uint32_t PLL9_DCR;
}PLL9Param;

// Transfer Mode
#define S4GT 0
#define S8GT 1
#define S12GT 2
#define S16GT 3

#define NORMAL_END  (0)
#define ERROR       (1)
#define ERROR_END   (1)

/* Remote Interrupt Address & Data (T.B.D) */
#define MSI_STOP_BASE			(0x7FFF0000U)
#define MSI_ABORT_BASE			(0x7FFF0200U)
#define MSI_WATERMARK_BASE		(0x7FFF0400U)
#define MSI_DATA				(0xA5A5A5A5U)

/* UCIE APB power management control registers*/
#define UCIE_APB0_UCIEPWRMNGCTRL    (0xDCE00070)
#define UCIE_APB1_UCIEPWRMNGCTRL    (0xDDE00070)

#define MODE_RC                      (1)
#define MODE_EP                      (0)
#define UCIE_CH0                     (0)
#define UCIE_CH1                     (1)

#define UCI1CORECKCR (uintptr_t)0xDE201084

#define LINKSPEED_4GTPS              (0x0UL)
#define LINKSPEED_8GTPS              (0x1UL)
#define LINKSPEED_12GTPS             (0x2UL)
#define LINKSPEED_16GTPS             (0x3UL)
#define LINKSPEED_MASK               (0xFUL)
#define LINKSPEED_OFFSET             (0x6UL)

#define UCIE_AXI_BASE(n)    (0xD8000000 + (n) * 0x1000000)
#define UCIE_APB_BASE(n)    (0xDC000000 + (n) * 0x1000000)

#define HSCS_APB 0xDE200000

#define	HSCS_APB_UCI0CORECKCR	(uintptr_t)(HSCS_APB  +	0x1080	)
#define	HSCS_APB_UCI1CORECKCR	(uintptr_t)(HSCS_APB  +	0x1084	)
#define	HSCS_APB_PLL9_0_CR0	(uintptr_t)(HSCS_APB  +	0x11EC	)
#define	HSCS_APB_PLL9_0_CR1	(uintptr_t)(HSCS_APB  +	0x11F0	)
#define	HSCS_APB_PLL9_0_CR2	(uintptr_t)(HSCS_APB  +	0x11F4	)
#define	HSCS_APB_PLL9_0SCR	(uintptr_t)(HSCS_APB  +	0x1308	)
#define	HSCS_APB_PLL9_0DCR	(uintptr_t)(HSCS_APB  +	0x130C	)
#define	HSCS_APB_PLL9_1_CR0	(uintptr_t)(HSCS_APB  +	0x11F8	)
#define	HSCS_APB_PLL9_1_CR1	(uintptr_t)(HSCS_APB  +	0x11FC	)
#define	HSCS_APB_PLL9_1_CR2	(uintptr_t)(HSCS_APB  +	0x1200	)
#define	HSCS_APB_PLL9_1SCR	(uintptr_t)(HSCS_APB  +	0x1310	)
#define	HSCS_APB_PLL9_1DCR	(uintptr_t)(HSCS_APB  +	0x1314	)
#define	HSCS_APB_CLKHSCSPKCPROT0	(uintptr_t)(HSCS_APB  +	0x1370	)

/*******************************************************************************
  Functions
*******************************************************************************/
void mem_write16(uintptr_t addr, uint16_t data);
void mem_write32(uintptr_t addr, uint32_t data);
uint32_t mem_read32(uintptr_t addr);

void Ucie_hdma_SetRegs(uint32_t ucieCh, uint32_t dmaCh, uint32_t rw, uint64_t srcAddr, uint64_t dstAddr, uint32_t size);
void Ucie_hdma_SetRegsLL(const struct hdma_info_ex *info);
void Ucie_hdma_start(uint32_t ucieCh, uint32_t dmaCh, uint32_t rw);
uint32_t Ucie_hdma_waitStop(uint32_t ucieCh, uint32_t dmaCh, uint32_t rw);
void Ucie_hdma_stop(uint32_t ucieCh, uint32_t dmaCh, uint32_t rw);
void Ucie_dump_aturegs(uint32_t ucieCh, uint32_t region, uint32_t inout);
void Ucie_pwrmngctrl_reg_modify(uint32_t address);
uint32_t Ucie_Setup(void);
uint32_t Ucie_Setup_Pre(uint32_t ucieCh, uint32_t RC);
uint32_t Ucie_Setup_Start_Linkup(uint32_t ucieCh, uint32_t RC, uint32_t linkSpeed);
uint32_t Ucie_Setup_Wait_FreqChange_Req(uint32_t ucieCh, bool is_rc);
uint32_t Ucie_Setup_Ack_FreqChange(uint32_t ucieCh);
uint32_t Ucie_Setup_Wait_Linkup(uint32_t ucieCh, bool is_rc);
uint32_t Ucie_Setup_PCIE_Pre(uint32_t ucieCh, uint32_t RC);
uint32_t Ucie_Setup_PCIE_Start_LinkUp(uint32_t ucieCh, uint32_t RC);
uint32_t Ucie_Setup_PCIE_Wait_LinkUp(uint32_t ucieCh);
uint32_t Ucie_Setup_PCIE_Post(uint32_t ucieCh, uint32_t RC);
void wait_time(uint32_t count);
uint32_t Ucie_Setup_rc(uint8_t ch);
uint32_t Ucie_Setup_ep(uint8_t ch);

#endif /* __UCIEDRV_H_ */

