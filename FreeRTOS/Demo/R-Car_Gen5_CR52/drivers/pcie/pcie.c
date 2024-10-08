/*******************************************************************************
  Includes
*******************************************************************************/

#define AArch64	1
#define SYSTEM_DEF_X5H 1

#include "drivers/pcie/common.h"
#include "drivers/pcie/pcie.h"
//#include <stdint.h>

/*******************************************************************************
  Defines
*******************************************************************************/

#define         NORMAL_END      0
#define         ERROR           1
#define         ERROR_END       1

struct reg_access {
	uintptr_t offset;
	uint32_t ope;
	uint32_t data;
};

/* operation define */
#define NOP		(0)
#define READ	(1)
#define WRITE	(2)

#define REG_READ(addr)			mem_read32(addr)
#define REG_WRITE(addr, data)	{ mem_write32(addr, data); }
#define WRITE_1(addr, mask)		{ mem_write32(addr, mem_read32(addr)|mask); }
#define WRITE_0(addr, mask)		{ mem_write32(addr, mem_read32(addr)&(~mask)); }

#define PCIE6_CXL_CH0_UPPER		(0x00000280U)
#define PCIE6_CXL_CH0_LOWER		(0x00000000U)
#define PCIE6_CXL_CH1_UPPER		(0x000002C0U)
#define PCIE6_CXL_CH1_LOWER		(0x00000000U)

/*******************************************************************************
  Global variables
*******************************************************************************/

/* tl_mem_reqs_ib_dw_vip_wire_cpcie_vtb/DUT_registers_access.log */
const static struct reg_access ut_regs[] = {
	{    0x0,  READ,   0x351912U },
	{   0x40,  READ, 0xda135001U },
	{   0x50,  READ,  0x1807005U },
	{   0x70,  READ, 0x80420010U },
	{  0x100,  READ, 0x14830001U },
	{  0x148,  READ, 0x15810003U },
	{  0x158,  READ, 0x17810019U },
	{  0x178,  READ, 0x1a010026U },
	{  0x1a0,  READ, 0x1c810027U },
	{  0x1c8,  READ, 0x1f01002aU },
	{  0x1f0,  READ, 0x20810031U },
	{  0x208,  READ, 0x22410017U },
	{  0x224,  READ, 0x2481000dU },
	{  0x248,  READ, 0x2601001eU },
	{  0x260,  READ, 0x27010021U },
	{  0x270,  READ, 0x27c10022U },
	{  0x27c,  READ, 0x37c1000bU },
	{  0x280,  READ, 0x10040002U },
	{  0x37c,  READ, 0x3e01000bU },
	{  0x380,  READ,  0x6410001U },
	{  0x3e0,  READ, 0x3ec10025U },
	{  0x3ec,  READ, 0x3f81001fU },
	{  0x3f8,  READ, 0x4601000bU },
	{  0x3fc,  READ,  0x5410004U },
	{  0x460,  READ, 0x4781000bU },
	{  0x464,  READ,  0x1810005U },
	{  0x478,  READ, 0x4901000bU },
	{  0x47c,  READ,  0x1800006U },
	{  0x490,  READ, 0x4a01002fU },
	{  0x4a0,  READ, 0x4c010023U },
	{  0x4a4,  READ,  0x2021e98U },
	{  0x4a8,  READ,   0x270007U },
	{  0x4c0,  READ, 0x4e810023U },
	{  0x4c4,  READ,  0x2801e98U },
	{  0x4c8,  READ,        0x3U },
	{  0x4e8,  READ, 0x50c10023U },
	{  0x4ec,  READ,  0x2401e98U },
	{  0x4f0,  READ,        0x8U },
	{  0x50c,  READ, 0x56810023U },
	{  0x510,  READ,  0x1001e98U },
	{  0x514,  READ,        0x4U },
	{  0x568,  READ, 0x5a410032U },
	{  0x5a4,  READ, 0x5b810033U },
	{  0x5b8,  READ, 0x5dc10034U },
	{   0xa0,  READ,  0x3010006U },
	{   0xa0, WRITE,  0x3010005U },
	{  0x3a4,  READ,        0x1U },
	{  0x3a4, WRITE,        0x0U },
	{  0x3a4,  READ,        0x0U },
	{  0x3a4, WRITE,        0x0U },
	{  0x1cc,  READ,      0x501U },
	{  0x1cc, WRITE,      0x503U },
	{  0x1cc,  READ,      0x503U },
	{  0x1cc, WRITE,      0x503U },
	{  0x1d0,  READ,      0x200U },
	{  0x1d0, WRITE,      0x200U },
	{  0xc8c,  READ,        0x0U },
	{  0xc8c, WRITE,        0x1U },
	{  0x1d0,  READ,      0x200U },
	{  0x1d0, WRITE,      0x100U },
	{  0xc48,  READ,   0x800000U },
	{  0xc48, WRITE,   0x800000U },
	{   0x80,  READ, 0x10110000U },
	{   0x80, WRITE, 0x10112000U },
	{   0x80,  READ, 0x10112000U },
	{   0x80, WRITE, 0x10112000U },
	{   0x70,  READ, 0x80420010U },
	{   0x70, WRITE, 0x80420010U },
	{  0xc8c,  READ,        0x1U },
	{  0xc8c, WRITE,        0x1U },
	{  0x3a4, WRITE,        0x0U },
	{ 0x1054, WRITE,        0x0U },
	{ 0x1060, WRITE,        0x0U },
	{  0x164,  READ, 0x7f7f7f7fU },
	{  0x164, WRITE, 0x21073130U },
	{  0x168,  READ, 0x7f7f7f7fU },
	{  0x168, WRITE, 0x71061321U },
	{  0x16c,  READ, 0x7f7f7f7fU },
	{  0x16c, WRITE, 0x25194900U },
	{  0x170,  READ, 0x7f7f7f7fU },
	{  0x170, WRITE, 0x12520759U },
	{  0x894, WRITE,      0xfd1U },
	{  0x89c, WRITE,        0x0U },
	{  0x898, WRITE,    0x10bc0U },
	{  0x89c, WRITE,        0x1U },
	{  0x898, WRITE,     0xbd00U },
	{  0x89c, WRITE,        0x2U },
	{  0x898, WRITE,     0xdc80U },
	{  0x89c, WRITE,        0x3U },
	{  0x898, WRITE,     0x8dc0U },
	{  0x89c, WRITE,        0x4U },
	{  0x898, WRITE,      0xfc0U },
	{  0x89c, WRITE,        0x5U },
	{  0x898, WRITE,      0xe46U },
	{  0x89c, WRITE,        0x6U },
	{  0x898, WRITE,      0xdc8U },
	{  0x89c, WRITE,        0x7U },
	{  0x898, WRITE,     0xdb06U },
	{  0x89c, WRITE,        0x8U },
	{  0x898, WRITE,     0x8bc8U },
	{  0x89c, WRITE,        0x9U },
	{  0x898, WRITE,      0xd0bU },
	{  0x89c, WRITE,        0xaU },
	{  0x898, WRITE,    0x17a00U },
	{  0x19c, WRITE, 0x95724559U },
	{  0x894, WRITE,      0xfd1U },
	{  0x89c, WRITE,        0x0U },
	{  0x898, WRITE,    0x10bc0U },
	{  0x89c, WRITE,        0x1U },
	{  0x898, WRITE,     0xbd00U },
	{  0x89c, WRITE,        0x2U },
	{  0x898, WRITE,     0xdc80U },
	{  0x89c, WRITE,        0x3U },
	{  0x898, WRITE,     0x8dc0U },
	{  0x89c, WRITE,        0x4U },
	{  0x898, WRITE,      0xfc0U },
	{  0x89c, WRITE,        0x5U },
	{  0x898, WRITE,      0xe46U },
	{  0x89c, WRITE,        0x6U },
	{  0x898, WRITE,      0xdc8U },
	{  0x89c, WRITE,        0x7U },
	{  0x898, WRITE,     0xdb06U },
	{  0x89c, WRITE,        0x8U },
	{  0x898, WRITE,     0x8bc8U },
	{  0x89c, WRITE,        0x9U },
	{  0x898, WRITE,      0xd0bU },
	{  0x89c, WRITE,        0xaU },
	{  0x898, WRITE,    0x17a00U },
	{  0x1e8, WRITE, 0x80970247U },
	{  0x1ec, WRITE, 0x30637148U },
	{  0x894, WRITE,      0xfd1U },
	{  0x89c, WRITE,        0x0U },
	{  0x898, WRITE,    0x10bc0U },
	{  0x89c, WRITE,        0x1U },
	{  0x898, WRITE,     0xbd00U },
	{  0x89c, WRITE,        0x2U },
	{  0x898, WRITE,     0xdc80U },
	{  0x89c, WRITE,        0x3U },
	{  0x898, WRITE,     0x8dc0U },
	{  0x89c, WRITE,        0x4U },
	{  0x898, WRITE,      0xfc0U },
	{  0x89c, WRITE,        0x5U },
	{  0x898, WRITE,      0xe46U },
	{  0x89c, WRITE,        0x6U },
	{  0x898, WRITE,      0xdc8U },
	{  0x89c, WRITE,        0x7U },
	{  0x898, WRITE,     0xdb06U },
	{  0x89c, WRITE,        0x8U },
	{  0x898, WRITE,     0x8bc8U },
	{  0x89c, WRITE,        0x9U },
	{  0x898, WRITE,      0xd0bU },
	{  0x89c, WRITE,        0xaU },
	{  0x898, WRITE,    0x17a00U },
	{  0x200, WRITE, 0x69472399U },
	{  0x204, WRITE, 0x58994590U },
	{  0x894, WRITE,      0xfd1U },
	{  0x89c, WRITE,        0x0U },
	{  0x898, WRITE,      0xfc0U },
	{  0x89c, WRITE,        0x1U },
	{  0x898, WRITE,      0xe85U },
	{  0x89c, WRITE,        0x2U },
	{  0x898, WRITE,      0xd0bU },
	{  0x89c, WRITE,        0x3U },
	{  0x898, WRITE,     0x5e80U },
	{  0x89c, WRITE,        0x4U },
	{  0x898, WRITE,     0xbd00U },
	{  0x89c, WRITE,        0x5U },
	{  0x898, WRITE,    0xc0bcdU },
	{  0x89c, WRITE,        0x6U },
	{  0x898, WRITE,    0xc8b08U },
	{  0x89c, WRITE,        0x7U },
	{  0x898, WRITE,   0x140b4dU },
	{  0x89c, WRITE,        0x8U },
	{  0x898, WRITE,   0x140acfU },
	{  0x89c, WRITE,        0x9U },
	{  0x898, WRITE,   0x143a0fU },
	{  0x89c, WRITE,        0xaU },
	{  0x898, WRITE,    0x17a00U },
	{   0x78,  READ,   0x102910U },
	{   0x78, WRITE,   0x102970U },
	{   0x10, WRITE,        0x0U },
	{   0x14, WRITE,        0x0U },
	{  0x81c,  READ,       0x5fU },
	{  0x81c, WRITE,       0x40U },
	{   0x1c, WRITE,      0x101U },
	{   0x30, WRITE,        0x0U },
	{   0x1c, WRITE,     0x5f50U },
	{   0x20, WRITE,   0x3f0030U },
	{   0x24, WRITE, 0xffff0001U },
	{   0x28, WRITE,       0x30U },
	{   0x2c, WRITE,       0x41U },
	{    0x8, WRITE, 0x1f000000U },
	{    0xc, WRITE,        0x0U },
	{   0x10, WRITE, 0x1f00ffffU },
	{   0x14, WRITE, 0x1f000000U },
	{   0x18, WRITE,        0x0U },
	{    0x0, WRITE,        0x4U },
	{    0x4, WRITE, 0x80000000U },
	{    0x4, WRITE,        0x0U },
	{    0x8, WRITE, 0x1f010000U },
	{    0xc, WRITE,        0x0U },
	{   0x10, WRITE, 0x1f01ffffU },
	{   0x14, WRITE, 0x1f010000U },
	{   0x18, WRITE,        0x0U },
	{    0x0, WRITE,   0x100004U },
	{    0x4, WRITE, 0x80000000U },
	{    0x4, WRITE,        0x0U },
	{    0x4,  READ,   0x110000U },
	{    0x4, WRITE,   0x110007U },
	{  0x81c,  READ,       0x40U },
	{  0x81c, WRITE,    0x10060U },
	{  0x108, WRITE,   0x21c000U },
	{  0x10c, WRITE,        0x0U },
	{  0x110, WRITE,   0x21c000U },
	{  0x114, WRITE,   0x21c000U },
	{  0x118, WRITE,        0x0U },
	{  0x100, WRITE,        0x0U },
	{  0x104, WRITE, 0x80000000U },
	{  0x108, WRITE, 0xf4984000U },
	{  0x10c, WRITE, 0xf2bbe647U },
	{  0x110, WRITE, 0xf4984000U },
	{  0x114, WRITE, 0xf4984000U },
	{  0x118, WRITE, 0xf2bbe647U },
	{  0x100, WRITE,        0x0U },
	{  0x104, WRITE, 0x80000000U },
	{  0x108, WRITE, 0x15d88000U },
	{  0x10c, WRITE,       0x10U },
	{  0x110, WRITE, 0x15d88000U },
	{  0x114, WRITE, 0x15d88000U },
	{  0x118, WRITE,       0x10U },
	{  0x100, WRITE,        0x0U },
	{  0x104, WRITE, 0x80000000U },
	{  0x108, WRITE,   0x132000U },
	{  0x10c, WRITE,        0x0U },
	{  0x110, WRITE,   0x132000U },
	{  0x114, WRITE,   0x132000U },
	{  0x118, WRITE,        0x0U },
	{  0x100, WRITE,        0x0U },
	{  0x104, WRITE, 0x80000000U },
	{  0x108, WRITE, 0x2295d000U },
	{  0x10c, WRITE,       0x14U },
	{  0x110, WRITE, 0x2295d000U },
	{  0x114, WRITE, 0x2295d000U },
	{  0x118, WRITE,       0x14U },
	{  0x100, WRITE,        0x0U },
	{  0x104, WRITE, 0x80000000U },
	{  0x108, WRITE, 0xd9619000U },
	{  0x10c, WRITE, 0x425629d4U },
	{  0x110, WRITE, 0xd9619000U },
	{  0x114, WRITE, 0xd9619000U },
	{  0x118, WRITE, 0x425629d4U },
	{  0x100, WRITE,        0x0U },
	{  0x104, WRITE, 0x80000000U },
	{  0x108, WRITE,   0x2b0000U },
	{  0x10c, WRITE,        0x0U },
	{  0x110, WRITE,   0x2b0000U },
	{  0x114, WRITE,   0x2b0000U },
	{  0x118, WRITE,        0x0U },
	{  0x100, WRITE,        0x0U },
	{  0x104, WRITE, 0x80000000U },
	{  0x108, WRITE, 0xbcd99000U },
	{  0x10c, WRITE,       0x18U },
	{  0x110, WRITE, 0xbcd99000U },
	{  0x114, WRITE, 0xbcd99000U },
	{  0x118, WRITE,       0x18U },
	{  0x100, WRITE,        0x0U },
	{  0x104, WRITE, 0x80000000U },
	{  0x108, WRITE,    0x74000U },
	{  0x10c, WRITE,        0x0U },
	{  0x110, WRITE,    0x74000U },
	{  0x114, WRITE,    0x74000U },
	{  0x118, WRITE,        0x0U },
	{  0x100, WRITE,        0x0U },
	{  0x104, WRITE, 0x80000000U },
	{   0x78,  READ,   0x102970U },
	{   0x78,  READ,   0x102970U },
	{   0x78, WRITE,   0x102910U },
	{   0x78,  READ,   0x102910U },
	{   0x78, WRITE,   0x102930U },
	{   0x78,  READ,   0x102930U },
	{   0x78, WRITE,   0x102950U },
	{   0x78,  READ,   0x102950U },
	{   0x78, WRITE,   0x102970U },
	{   0x78,  READ,   0x102970U },
	{   0x78, WRITE,   0x102970U },
	{    0x0,   NOP,         0x0 },
};

/*******************************************************************************
  Functions
*******************************************************************************/
uint32_t Pcie_set_axireg_ch0(void);
uint32_t Pcie_set_axireg_ch1(void);

/*******************************************************
 * NAME		: Pcie_SimUT
 * FUNCTION	: Sim UT scenario porting
 * INPUT	: none
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
uint32_t Pcie_SimUT(void)
{
	uint32_t ret;
	uintptr_t base;
	uint32_t data;
	uint32_t index;

	/* UT (tl_mem_reqs_ib_dw_vip_wire_cpcie_vtb) */
	mprintf("[PCIE UT]\n");

	/* [debug] ベースアドレス確認 */
	base = PCIE60_APB_BASE;			/* PCIe ch0 (APB) Region 0 */
	data = mem_read32(base);
	if (data != ut_regs[0].data) {
		mprintf("Check ID: 0x%x=0x%x: NG\n", base, data);

		base = PCIE60_AXI_BASE;		/* PCIe ch0 register (AXI) Region 0 */
		data = mem_read32(base);
		if (data != ut_regs[0].data) {
			mprintf("Check ID: 0x%x=0x%x: NG\n", base, data);
			//SetEndTP(0,0,0,0,0);
			return ERROR_END;
		}
	}

	/* Exec UT register access */
	ret = NORMAL_END;
	index = 0;
	while (ut_regs[index].ope != NOP) {
		switch (ut_regs[index].ope) {
		case READ:
			data = mem_read32(base + ut_regs[index].offset);
			if (data == ut_regs[index].data) {
				mprintf("RD: 0x%x=0x%x: OK\n", base + ut_regs[index].offset, data);
			} else {
				ret = ERROR_END;
				mprintf("RD: 0x%x=0x%x: NG\n", base + ut_regs[index].offset, data);
			}
			break;

		case WRITE:
			mem_write32(base + ut_regs[index].offset, ut_regs[index].data);
			break;
		}
		index++;
	}

	mprintf("Finish\n");
	while(1);

	//SetEndTP(0,0,0,0,0);
	return ret;
}

/*******************************************************
 * NAME		: Pcie_setup
 * FUNCTION	: Setup ch0 and ch1
 * INPUT	: none
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
uint32_t Pcie_setup(void)
{
	// PCIE6 ch0 as RC - PHY0 (4lane)
	// PCIE6 ch1 as EP - PHY1 (4lane)

	uint32_t val, mask;
	uint32_t s_l0_state;
	uintptr_t baApb_ch0 = PCIE60_APB_BASE;
	uintptr_t baApb_ch1 = PCIE61_APB_BASE;
	uintptr_t baAxi_ch0 = PCIE60_AXI_BASE;
	uintptr_t baAxi_ch1 = PCIE61_AXI_BASE;
	uint32_t timeout = 10000;
	uint32_t linkup_flg, rxack_flg;
	uint32_t scaling_factor;

	/*===========================================================*/
	// [Configuration]
	// TIMER_CTRL_MAX_FUNC_NUM_OFF (AXI+0x718), FAST_LINK_SCALING_FACTOR[30:29]
	scaling_factor = 0x3 << 29;
	/*===========================================================*/

	mprintf("CLKHSCSPKCPROT0\n");
	REG_WRITE(0xC6481370, 0xa5a5a501);			// base+offset=0xC6481088
	while (mem_read32(0xC6481370) != 0x00000001) {
		timeout--;
		if (timeout==0) {mprintf("** TIMEOUT\n"); break;}
	}
	timeout = 10000;

	mprintf("POSTHSCSCKCR\n");
	REG_WRITE(0xC6481088, 0x00000000);			// base+offset=0xC6481088
	while (mem_read32(0xC6481088) != 0x00000000) {
		timeout--;
		if (timeout==0) {mprintf("** TIMEOUT\n"); break;}
	}
	timeout = 10000;

	/* PCIE6 ch0 (APB) */
	WRITE_1  (baApb_ch0+0x00000, 0x00000010);			// Set as RC
	WRITE_1  (baApb_ch0+0x00814, 0x40004000);			// Set Repeat clock.
	WRITE_1  (baApb_ch0+0x00084, 0x00000c00);			// PCIe Application Layer setting.
	WRITE_1  (baApb_ch0+0x00804, 0x00000100);			// lb_sel=1; 2 channel Mode. PHY1 Setting
	WRITE_1  (baApb_ch0+0x00000, 0x00000001);			// Bifurcation mode setting
	WRITE_0  (baApb_ch0+0x00824, 0x00000001);			// Clear phy1_cm0_rescal_mode_i

	WRITE_0  (baApb_ch0+0x0001c, 0x00010000);			// Releasing app_hold_phy_rst
	WRITE_0  (baApb_ch1+0x0001c, 0x00010000);			// Releasing app_hold_phy_rst

	/* Release lane reset */
	REG_WRITE(baApb_ch0+0x00808, 0x000000ff);			// pipe_lane*_reset_n

	/* PHY0,1 Load ROM */
	WRITE_1  (baApb_ch0+0x00804, 0x00010001);			// Make DCCM the BootLoader target
	WRITE_1  (baApb_ch0+0x00804, 0x00030003);			// Make DCCM the BootLoader target,BootLoader starting

	mprintf("PHY0,1 WAIT_BOOTLOAD_DONE\n");
	mask = 0x00040004;
	while ((REG_READ (baApb_ch0+0x00804) & mask) != mask) {
		timeout--;
		if (timeout==0) {mprintf("** TIMEOUT\n"); break;}
	}
	timeout = 10000;
	mprintf("---\n");

	/* PHY0 Fastsim */
	WRITE_1(baApb_ch0+0x8c028, 0x00000001);			// PDM Register Target.FAST Sim
	WRITE_1(baApb_ch0+0x8e8d0, 0x10000000);			// PDM Register Target.FAST Sim
	WRITE_1(baApb_ch0+0x8ead0, 0x10000000);			// PDM Register Target.FAST Sim
	WRITE_1(baApb_ch0+0x8ecd0, 0x10000000);			// PDM Register Target.FAST Sim
	WRITE_1(baApb_ch0+0x8eed0, 0x10000000);			// PDM Register Target.FAST Sim

	/* PHY1 Fastsim */
	WRITE_1(baApb_ch1+0x8c028, 0x00000001); 		// PDM Register Target.FAST Sim
	WRITE_1(baApb_ch1+0x8e8d0, 0x10000000); 		// PDM Register Target.FAST Sim
	WRITE_1(baApb_ch1+0x8ead0, 0x10000000); 		// PDM Register Target.FAST Sim
	WRITE_1(baApb_ch1+0x8ecd0, 0x10000000); 		// PDM Register Target.FAST Sim
	WRITE_1(baApb_ch1+0x8eed0, 0x10000000); 		// PDM Register Target.FAST Sim

	/* PHY0,1 Run Request */
	WRITE_1(baApb_ch0+0x0080C, 0x00040004);			// Start the CPU REQ

	mprintf("PHY0,1 WAIT_FOR_ACK\n");
	mask = 0x02000200;
	while ((REG_READ (baApb_ch0+0x0080C) & mask) != mask) {
		timeout--;
		if (timeout==0) {mprintf("** TIMEOUT\n"); break;}
	}
	timeout = 10000;
	mprintf("---\n");

	WRITE_0(baApb_ch0+0x0080C, 0x00040004);			// Stop the CPU REQ

	/* PHY0,1 wait rxX_ACK */
	mprintf("WAIT_ACK_DOWN_PHY0,1\n");
	mask = 0x00800000;
	timeout = 11;				// 11 * 10[us] = 110[us] wait
	rxack_flg = 0;

	while (rxack_flg != 0x3) {
		if ((rxack_flg & BIT0) == 0x0) {
			if ((REG_READ (baApb_ch0+0x8C6CC) & mask) == 0x0) {
				mprintf("->ACK_DOWN_PHY0\n");
				rxack_flg |= BIT0;
			}
		}

		if ((rxack_flg & BIT1) == 0x0) {
			if ((REG_READ (baApb_ch1+0x8C6CC) & mask) == 0x0) {
				mprintf("->ACK_DOWN_PHY1\n");
				rxack_flg |= BIT1;
			}
		}

		if (rxack_flg == 0x3)	break;

		// TODO: replace by freertos delay api.
		//wait_usec(1);			// 10[us] wait

		timeout--;
		if (timeout == 0)	break;
	}

	switch (rxack_flg) {
	case 0:		mprintf("PHY0,1 rxX_ACK TIMEOUT\n");	break;
	case BIT0:	mprintf("PHY1 rxX_ACK TIMEOUT\n");		break;
	case BIT1:	mprintf("PHY0 rxX_ACK TIMEOUT\n");		break;
	}

	timeout = 10000;
	mprintf("---\n");

	/*====================================*/
	/* AXI (ch0) */
	val = mem_read32(baAxi_ch0+0x00718) & ~(0x60000000);	// clear FAST_LINK_SCALING_FACTOR
	val |= scaling_factor;
	mem_write32(baAxi_ch0+0x00718, val);

	val = mem_read32(baAxi_ch0+0x00710) & ~(0x3F0000);		// clear PORT_LINK_CTRL.LINK_CAPABLE[21:16]
	val = val | 0x70000;									// set PORT_LINK_CTRL.LINK_CAPABLE[21:16] = 0x7
	val = val | 0x80;										// set FAST_LINK_MODE
	mem_write32(baAxi_ch0+0x00710, val);

	val = mem_read32(baAxi_ch0+0x0080C) & ~(0x00001F00);	// clear GEN2_CTRL.NUM_OF_LANES[12:8]
	val = val | 0x400;										// set GEN2_CTRL.NUM_OF_LANES[12:8] = 5'h04
	mem_write32(baAxi_ch0+0x0080C, val);

	WRITE_1(baAxi_ch0+0x00714, 0x40);						// set INSERT_LANE_SKEW

	/* AXI (ch1) */
	val = mem_read32(baAxi_ch1+0x00718) & ~(0x60000000);	// clear FAST_LINK_SCALING_FACTOR
	val |= scaling_factor;
	mem_write32(baAxi_ch1+0x00718, val);

	val = mem_read32(baAxi_ch1+0x00710) & ~(0x3F0000);		// clear PORT_LINK_CTRL.LINK_CAPABLE[21:16]
	val = val | 0x70000;									// set PORT_LINK_CTRL.LINK_CAPABLE[21:16] = 0x7
	val = val | 0x80;										// set FAST_LINK_MODE
	mem_write32(baAxi_ch1+0x00710, val);

	val = mem_read32(baAxi_ch1+0x0080C) & ~(0x00001F00);	// clear GEN2_CTRL.NUM_OF_LANES[12:8]
	val = val | 0x400;										// set GEN2_CTRL.NUM_OF_LANES[12:8] = 5'h04
	mem_write32(baAxi_ch1+0x0080C, val);

	WRITE_1(baAxi_ch0+0x00714, 0x40);						// set INSERT_LANE_SKEW

	/* AXI ch0,1 */
	WRITE_1(baAxi_ch0+0x008bc , 0x1);						// MISC_CONTROL_1_OFF DBI_RO_WR_EN=1
	WRITE_1(baAxi_ch1+0x008bc , 0x1);

	WRITE_1(baAxi_ch0+0x001cc, 0x00000002);					// PCIe Controller PL32G_CAP_REG
	WRITE_1(baAxi_ch1+0x001CC, 0x00000002);					// PCIe Controller PL32G_CAP_REG

	mprintf("[DBG]TIMER_CTRL_MAX_FUNC_NUM_OFF ch0=0x%x, ch1=0%x\n",
			mem_read32(baAxi_ch0+0x00718), mem_read32(baAxi_ch1+0x00718) );

	/*====================================*/
	// enable Interrupt smlh_link_up enable, Interrupt rdlh_link_up enable
	WRITE_1(baApb_ch0+0x002C0, 0x000000C0);
	WRITE_1(baApb_ch1+0x002C0, 0x000000C0);
	// enable app_ltssm_enable
	WRITE_1(baApb_ch0+0x0001C, 0x1);
	WRITE_1(baApb_ch1+0x0001C, 0x1);
	mprintf("ltssm enable\n");

	/*====================================*/
	// wait linkup here
	mprintf("WAIT_LINKUP\n");
	mask		= 0x1F8;				// BIT8:3 smlh_ltssm_state
	s_l0_state	= 0x11 << 3;			// 6'h11: S_L0
	timeout		= 10000;
	linkup_flg	= 0;

	// wait for S_L0 state (1st)
	while (linkup_flg != 0x3) {
		if ((mem_read32(baApb_ch0+0x00088) & mask) == s_l0_state) {
			linkup_flg |= BIT0;
		}

		if ((mem_read32(baApb_ch1+0x00088) & mask) == s_l0_state) {
			linkup_flg |= BIT1;
		}

		timeout--;
		if (timeout == 0)	break;
	}

	switch (linkup_flg) {
	case 0:		mprintf("ch0,1 S_L0 (1st) TIMEOUT\n");		break;
	case BIT0:	mprintf("ch1 S_L0 (1st) TIMEOUT\n");		break;
	case BIT1:	mprintf("ch0 S_L0 (1st) TIMEOUT\n");		break;
	case 0x3:	mprintf("ch0,1 S_L0 (1st) PASS\n");			break;
	}

	if (linkup_flg != 0x3) {	return ERROR_END;	}

	// wait for S_L0 state (2nd)
	timeout		= 10000;
	linkup_flg	= 0;

	while (linkup_flg != 0x3) {
		if ((mem_read32(baApb_ch0+0x00088) & mask) == s_l0_state) {
			linkup_flg |= BIT0;
		}

		if ((mem_read32(baApb_ch1+0x00088) & mask) == s_l0_state) {
			linkup_flg |= BIT1;
		}

		timeout--;
		if (timeout == 0)	break;
	}

	switch (linkup_flg) {
	case 0:		mprintf("ch0,1 S_L0 (2nd) TIMEOUT\n");		break;
	case BIT0:	mprintf("ch1 S_L0 (2nd) TIMEOUT\n");		break;
	case BIT1:	mprintf("ch0 S_L0 (2nd) TIMEOUT\n");		break;
	case 0x3:	mprintf("ch0,1 S_L0 (2nd) PASS\n");			break;
	}

	/*====================================*/
	/* PCIE6 ch0 (AXI) */
	Pcie_set_axireg_ch0();
	/* PCIE6 ch1 (AXI) */
	Pcie_set_axireg_ch1();

	return NORMAL_END;
}

/*******************************************************
 * NAME		: Pcie_set_axireg_ch0
 * FUNCTION	:
 * INPUT	: none
 * OUTPUT	: none
 * RETURN	: NORMAL_END
 * REMARKS	: base code (pci6_tests/pci6_rc_outbound_ch1_dma_read_test/pcie_data.s)
 ******************************************************/
uint32_t Pcie_set_axireg_ch0(void)
{
	uintptr_t baAxi_ch0 = PCIE60_AXI_BASE;

	/*==== main TM =======================*/
	REG_READ (baAxi_ch0+0x008bc);
	REG_WRITE(baAxi_ch0+0x008bc , 0x7ff49);
	REG_READ (baAxi_ch0+0x00000);
	REG_READ (baAxi_ch0+0x00040);
	REG_READ (baAxi_ch0+0x00050);
	REG_READ (baAxi_ch0+0x00070);
	REG_READ (baAxi_ch0+0x00100);
	REG_READ (baAxi_ch0+0x00148);
	REG_READ (baAxi_ch0+0x00158);
	REG_READ (baAxi_ch0+0x00178);
	REG_READ (baAxi_ch0+0x001a0);
	REG_READ (baAxi_ch0+0x001d8);
	REG_READ (baAxi_ch0+0x001fc);
	REG_READ (baAxi_ch0+0x00214);
	REG_READ (baAxi_ch0+0x00218);
	REG_READ (baAxi_ch0+0x00314);
	REG_READ (baAxi_ch0+0x00318);
	REG_READ (baAxi_ch0+0x0034c);
	REG_READ (baAxi_ch0+0x00358);
	REG_READ (baAxi_ch0+0x00364);
	REG_READ (baAxi_ch0+0x00368);
	REG_READ (baAxi_ch0+0x003cc);
	REG_READ (baAxi_ch0+0x003d0);
	REG_READ (baAxi_ch0+0x003e4);
	REG_READ (baAxi_ch0+0x000a0);
//	REG_WRITE(baAxi_ch0+0x000a0 , 0x3010004);
	REG_WRITE(baAxi_ch0+0x000a0 , 0x3010006);
	REG_READ (baAxi_ch0+0x0080c);
	REG_WRITE(baAxi_ch0+0x0080c , 0x1708ff);

	/* label_1 */
	REG_READ (baAxi_ch0+0x00890);
	REG_WRITE(baAxi_ch0+0x00890 , 0x2000);
	REG_READ (baAxi_ch0+0x00890);
	REG_WRITE(baAxi_ch0+0x00890 , 0x2800);
	REG_READ (baAxi_ch0+0x00164);
	REG_WRITE(baAxi_ch0+0x00164 , 0x28047470);
	REG_READ (baAxi_ch0+0x00168);
	REG_WRITE(baAxi_ch0+0x00168 , 0x603a2130);
	REG_READ (baAxi_ch0+0x0016c);
	REG_WRITE(baAxi_ch0+0x0016c , 0x20535067);
	REG_READ (baAxi_ch0+0x00170);
	REG_WRITE(baAxi_ch0+0x00170 , 0x4668430a);
	REG_READ (baAxi_ch0+0x008ac);
	REG_WRITE(baAxi_ch0+0x008ac , 0x0);
	REG_READ (baAxi_ch0+0x008a8);
	REG_WRITE(baAxi_ch0+0x008a8 , 0xc001071);
	REG_READ (baAxi_ch0+0x00890);
	REG_WRITE(baAxi_ch0+0x00890 , 0x1002800);
	REG_READ (baAxi_ch0+0x00890);
	REG_WRITE(baAxi_ch0+0x00890 , 0x1402800);
	REG_READ (baAxi_ch0+0x00198);
	REG_WRITE(baAxi_ch0+0x00198 , 0xa1397439);
	REG_READ (baAxi_ch0+0x0019c);
	REG_WRITE(baAxi_ch0+0x0019c , 0x36620207);
	REG_READ (baAxi_ch0+0x008ac);
	REG_WRITE(baAxi_ch0+0x008ac , 0x0);
	REG_READ (baAxi_ch0+0x008a8);
	REG_WRITE(baAxi_ch0+0x008a8 , 0x4004031);

	return NORMAL_END;
}

/*******************************************************
 * NAME		: Pcie_rc_ch0_inbound
 * FUNCTION	:
 * INPUT	: none
 * OUTPUT	: none
 * RETURN	: NORMAL_END
 * REMARKS	: base code (pci6_tests/pci6_rc_outbound_ch1_dma_read_test/pcie_data.s)
 ******************************************************/
uint32_t Pcie_rc_ch0_inbound(void)
{
	uintptr_t baAxi_ch0 = PCIE60_AXI_BASE;

	REG_READ (baAxi_ch0+0x00078);

	/* set max payload to 1024byte DWC_PCIE_DSP_PF0_PCIE_CAP - DEVICE_CONTROL_DEVICE_STATUS bit [7:5] = 3 */
	REG_WRITE(baAxi_ch0+0x00078 , 0x102970);

	/* PORT_LOGIC TRGT_MAP_CTRL_OFF */
	REG_READ  (baAxi_ch0+0x0081c);
	REG_WRITE (baAxi_ch0+0x0081c , 0x40);

	/*=============== iATU register configuration (INBOUND 0) ===============*/
	REG_WRITE(baAxi_ch0+0x300108, PCIE6_CXL_CH0_LOWER);				// IATU_LWR_BASE_ADDR_OFF_INBOUND_0
	REG_WRITE(baAxi_ch0+0x30010c, PCIE6_CXL_CH0_UPPER);				// IATU_UPPER_BASE_ADDR_OFF_INBOUND_0
	REG_WRITE(baAxi_ch0+0x300110, PCIE6_CXL_CH0_LOWER+0xffff);		// limit addr
	REG_WRITE(baAxi_ch0+0x300114, 0x64000000);						// lower target addr
	REG_WRITE(baAxi_ch0+0x300118, 0x00000000);						// upper target addr
	REG_WRITE(baAxi_ch0+0x300100, 0x00000000);						// tlp type Mwr
	REG_WRITE(baAxi_ch0+0x300104, 0x80000000);						// enable region
	/*===========================================================*/

	/* label_2 */
	/* bus master enable , memory space enable , IO space enable */
	REG_READ  (baAxi_ch0+0x00004);
	REG_WRITE (baAxi_ch0+0x00004 , 0x100007);
//  polling last memory address to check if data is stored

	return NORMAL_END;
}

/*******************************************************
 * NAME		: Pcie_set_axireg_ch1
 * FUNCTION	:
 * INPUT	: none
 * OUTPUT	: none
 * RETURN	: NORMAL_END
 * REMARKS	: base code (pci6_octa_ep_dma_test/pcie_data.s)
 ******************************************************/
uint32_t Pcie_set_axireg_ch1(void)
{
	uintptr_t baAxi_ch1 = PCIE61_AXI_BASE;

/*==== main TM ===================*/
	REG_READ (baAxi_ch1+0x008bc);
	REG_WRITE(baAxi_ch1+0x008bc , 0x7ff49);
	REG_READ (baAxi_ch1+0x00000);
	REG_READ (baAxi_ch1+0x00040);
	REG_READ (baAxi_ch1+0x00050);
	REG_READ (baAxi_ch1+0x00070);
	REG_READ (baAxi_ch1+0x00100);
	REG_READ (baAxi_ch1+0x00148);
	REG_READ (baAxi_ch1+0x00158);
	REG_READ (baAxi_ch1+0x00178);
	REG_READ (baAxi_ch1+0x001a0);
	REG_READ (baAxi_ch1+0x001d8);
	REG_READ (baAxi_ch1+0x001fc);
	REG_READ (baAxi_ch1+0x00214);
	REG_READ (baAxi_ch1+0x00218);
	REG_READ (baAxi_ch1+0x00314);
	REG_READ (baAxi_ch1+0x00318);
	REG_READ (baAxi_ch1+0x0034c);
	REG_READ (baAxi_ch1+0x00358);
	REG_READ (baAxi_ch1+0x00364);
	REG_READ (baAxi_ch1+0x00368);
	REG_READ (baAxi_ch1+0x003cc);
	REG_READ (baAxi_ch1+0x003d0);
	REG_READ (baAxi_ch1+0x003e4);
	REG_READ (baAxi_ch1+0x000a0);
	REG_WRITE(baAxi_ch1+0x000a0 , 0x3010006);
	REG_READ (baAxi_ch1+0x0080c);
	REG_WRITE(baAxi_ch1+0x0080c , 0x1708ff);

	/* label_1 */
	REG_READ (baAxi_ch1+0x00890);
	REG_WRITE(baAxi_ch1+0x00890 , 0x2000);
	REG_READ (baAxi_ch1+0x00890);
	REG_WRITE(baAxi_ch1+0x00890 , 0x2800);
	REG_READ (baAxi_ch1+0x00164);
	REG_WRITE(baAxi_ch1+0x00164 , 0x28047470);
	REG_READ (baAxi_ch1+0x00168);
	REG_WRITE(baAxi_ch1+0x00168 , 0x603a2130);
	REG_READ (baAxi_ch1+0x0016c);
	REG_WRITE(baAxi_ch1+0x0016c , 0x20535067);
	REG_READ (baAxi_ch1+0x00170);
	REG_WRITE(baAxi_ch1+0x00170 , 0x4668430a);
	REG_READ (baAxi_ch1+0x008ac);
	REG_WRITE(baAxi_ch1+0x008ac , 0x0);
	REG_READ (baAxi_ch1+0x008a8);
	REG_WRITE(baAxi_ch1+0x008a8 , 0xc001071);
	REG_READ (baAxi_ch1+0x00890);
	REG_WRITE(baAxi_ch1+0x00890 , 0x1002800);
	REG_READ (baAxi_ch1+0x00890);
	REG_WRITE(baAxi_ch1+0x00890 , 0x1402800);
	REG_READ (baAxi_ch1+0x00198);
	REG_WRITE(baAxi_ch1+0x00198 , 0xa1397439);
	REG_READ (baAxi_ch1+0x0019c);
	REG_WRITE(baAxi_ch1+0x0019c , 0x36620207);
	REG_READ (baAxi_ch1+0x008ac);
	REG_WRITE(baAxi_ch1+0x008ac , 0x0);
	REG_READ (baAxi_ch1+0x008a8);
	REG_WRITE(baAxi_ch1+0x008a8 , 0x4004031);

	return NORMAL_END;
}

/*******************************************************
 * NAME		: Pcie_ep_ch1_outbound
 * FUNCTION	:
 * INPUT	: none
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 * REMARKS	: pci6_octa_ep_dma_test/pcie_data.s
 ******************************************************/
uint32_t Pcie_ep_ch1_outbound(void)
{
	uintptr_t baAxi_ch1 = PCIE61_AXI_BASE;

	REG_READ  (baAxi_ch1+0x00078);

	/* set max payload to 1024byte DWC_PCIE_DSP_PF0_PCIE_CAP - DEVICE_CONTROL_DEVICE_STATUS bit [7:5] = 3 */
	REG_WRITE (baAxi_ch1+0x00078 , 0x102970);

	/* PORT_LOGIC TRGT_MAP_CTRL_OFF */
	REG_READ  (baAxi_ch1+0x0081c);
	REG_WRITE (baAxi_ch1+0x0081c , 0x40);

	/* TYPE1_HDR  SEC_STAT_IO_LIMIT_IO_BASE_REG */
	REG_WRITE (baAxi_ch1+0x0001c , 0x101);

	/* TYPE1_HDR  IO_LIMIT_UPPER_IO_BASE_UPPER_REG */
	REG_WRITE (baAxi_ch1+0x00030 , 0x0);
	REG_WRITE (baAxi_ch1+0x0001c , 0xf00);

	/* TYPE1_HDR  MEM_LIMIT_MEM_BASE_REG */
	REG_WRITE (baAxi_ch1+0x00020 , 0x223f2220);
	REG_WRITE (baAxi_ch1+0x00024 , 0xffff0001);
	REG_WRITE (baAxi_ch1+0x00028 , 0x612d3f00);
	REG_WRITE (baAxi_ch1+0x0002c , 0x612d3f0f);

	/*=============== iATU register configuration (OUTBOUND 0) ===============*/
	REG_WRITE(baAxi_ch1+0x300008, 0x62040000);								// IATU_LWR_BASE_ADDR_OFF_OUTBOUND_0
	REG_WRITE(baAxi_ch1+0x30000c, 0x0);										// IATU_UPPER_BASE_ADDR_OFF_OUTBOUND_0
	REG_WRITE(baAxi_ch1+0x300010, 0x62040000 + 0xffff);						// limit addr
	REG_WRITE(baAxi_ch1+0x300014, PCIE6_CXL_CH1_LOWER);						// lower target addr
	REG_WRITE(baAxi_ch1+0x300018, PCIE6_CXL_CH1_UPPER);						// upper target addr
	REG_WRITE(baAxi_ch1+0x300000, 0x00000000);								// tlp type Mwr
	REG_WRITE(baAxi_ch1+0x300004, 0x80000000);								// enable region

	/* label_2 */
	/* bus master enable , memory space enable , IO space enable */
	REG_READ  (baAxi_ch1+0x00004);
	REG_WRITE (baAxi_ch1+0x00004 , 0x100007);

	return NORMAL_END;
}

/*******************************************************
 * NAME		: Pcie_loopback_test_fromCh1
 * FUNCTION	: DMA Transfer (ch1 -> ch0 -> ch1)
 * INPUT	: none
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
uint32_t Pcie_loopback_test_fromCh1(void)
{
	uint32_t ret = NORMAL_END;
	uint32_t val, mask;
	uintptr_t baAxi_ch1 = PCIE61_AXI_BASE;
	uint32_t timeout = 10000;
	uint32_t dma_rd_dar, dma_wr_sar;
	uint32_t offset;
	uint32_t data1[] = { 0x12345678, 0xFEDCBA98, 0xA5A5A5A5, 0x5A5A5A5A,
						 0x11223344, 0x55667788, 0xAABBCCDD, 0xEEFF0011 };

	mprintf("PCIE6 Loopback Test(CXL)\n");

	/*====================================*/
	/* DMA SAR,DAR                        */
	/*====================================*/
	dma_wr_sar = 0x62000000;
	dma_rd_dar = 0x62080000;

	/*====================================*/
	/* Initialize SAR,DAR and dump        */
	/*====================================*/
	for (offset = 0; offset < 8; offset++) {
		mem_write32((dma_wr_sar + (offset * 0x4)), data1[offset]);

		mem_write32((dma_rd_dar + (offset * 0x4)), 0x0);
	}

	mprintf("[Test Data:SAR]\n");
	mprintf("0x%x | 0x%08x 0x%08x 0x%08x 0x%08x\n",
			dma_wr_sar,
			mem_read32(dma_wr_sar), mem_read32(dma_wr_sar + 0x4),
			mem_read32(dma_wr_sar + 0x8), mem_read32(dma_wr_sar + 0xC));
	mprintf("0x%x | 0x%08x 0x%08x 0x%08x 0x%08x\n",
			(dma_wr_sar + 0x10),
			mem_read32(dma_wr_sar + 0x10), mem_read32(dma_wr_sar + 0x14),
			mem_read32(dma_wr_sar + 0x18), mem_read32(dma_wr_sar + 0x1C));

	/*====================================*/
	/* Setup PCIE and Transfer            */
	/*====================================*/
	Pcie_setup();
	Pcie_rc_ch0_inbound();
	Pcie_ep_ch1_outbound();

	/*========== Write channel config =======================*/
	/* DMA Write channel 0 enable */
	REG_WRITE (baAxi_ch1+0x380000 , 0x1);

	/* DMA write channel interrupt setup */
	REG_WRITE (baAxi_ch1+0x380088 , 0x50);

	/* DMA write channel control setting 1 */
	REG_WRITE (baAxi_ch1+0x380034 , 0x2);

	/* DMA write channel function number */
	REG_WRITE (baAxi_ch1+0x380038 , 0x0);

	/* DMA write channel QOS setting */
	REG_WRITE (baAxi_ch1+0x38003c , 0x0);

	/* DMA write channel transfer size = 64KB */
	REG_WRITE (baAxi_ch1+0x38001c , 0x00001000);

	/* DMA write channel SAR low */
	REG_WRITE (baAxi_ch1+0x380020 , dma_wr_sar);

	/* DMA write channel SAR high  */
	REG_WRITE (baAxi_ch1+0x380024 , 0x00000000);

	/* DMA write channel DAR low */
	REG_WRITE (baAxi_ch1+0x380028 , PCIE6_CXL_CH0_LOWER);

	/* DMA write channel DAR high */
	REG_WRITE (baAxi_ch1+0x38002c , PCIE6_CXL_CH0_UPPER);

	/*==========================================*/
	/* DoorBell to start transfer */
	/* DMA write channel 0 doorbell */
	REG_WRITE (baAxi_ch1+0x380004 , 0x00000001);

	/* wait write channel */
	mprintf("WAIT_INT_WRITE\n");
	while ((REG_READ (baAxi_ch1+0x380084) & BIT0) == 0) {		/* Wait HDMA_INT_STATUS_OFF_WRCH_0.STOP = b'1 */
		timeout--;
		if (timeout==0) {mprintf("** TIMEOUT\n"); break;}
	}
	timeout = 10000;
	mprintf("---\n");

	/* Clear interrupt */
	REG_WRITE (baAxi_ch1+0x38008c , 0x1);

	//========== Read channel config =======================
	/* DMA Read channel 0 enable */
	REG_WRITE (baAxi_ch1+0x380400 , 0x1);

	/* DMA read channel interrupt setup */
	REG_WRITE (baAxi_ch1+0x380488 , 0x50);

	/* DMA read channel control setting 1 */
	REG_WRITE (baAxi_ch1+0x380434 , 0x2);

	/* DMA read channel function number */
	REG_WRITE (baAxi_ch1+0x380438 , 0x0);

	/* DMA read channel QOS setting */
	REG_WRITE (baAxi_ch1+0x38043c , 0x0);

	/* DMA read channel transfer size = 64KB */
	REG_WRITE (baAxi_ch1+0x38041c , 0x00001000);

	/* DMA read channel SAR low */
	REG_WRITE (baAxi_ch1+0x380420 , PCIE6_CXL_CH0_LOWER);

	/* DMA read channel SAR high  */
	REG_WRITE (baAxi_ch1+0x380424 , PCIE6_CXL_CH0_UPPER);

	/* DMA read channel DAR low */
	REG_WRITE (baAxi_ch1+0x380428 , dma_rd_dar);

	/* DMA read channel DAR high */
	REG_WRITE (baAxi_ch1+0x38042c , 0x00000000);

	//==========================================
	/* DoorBell to start transfer */
	/* DMA read channel 0 doorbell */
	REG_WRITE (baAxi_ch1+0x380404 , 0x00000001);

	/* wait read channel   */
	mprintf("WAIT_INT_WRITE\n");
	while ((REG_READ (baAxi_ch1+0x380484) & BIT0) == 0) {		/* Wait HDMA_INT_STATUS_OFF_WRCH_0.STOP = b'1 */
		timeout--;
		if (timeout==0) {mprintf("** TIMEOUT\n"); break;}
	}
	timeout = 10000;
	mprintf("---\n");

	/* Clear interrupt */
	REG_WRITE (baAxi_ch1+0x38048c , 0x1);

	//=========================================== End - pcie_data.s

	/*====================================*/
	/* Check Result                         */
	/*====================================*/
	mprintf("[Dump:DAR]\n");
	mprintf("0x%x | 0x%08x 0x%08x 0x%08x 0x%08x\n",
			dma_rd_dar,
			mem_read32(dma_rd_dar), mem_read32(dma_rd_dar + 0x4),
			mem_read32(dma_rd_dar + 0x8), mem_read32(dma_rd_dar + 0xC));
	mprintf("0x%x | 0x%08x 0x%08x 0x%08x 0x%08x\n",
			(dma_rd_dar + 0x10),
			mem_read32(dma_rd_dar + 0x10), mem_read32(dma_rd_dar + 0x14),
			mem_read32(dma_rd_dar + 0x18), mem_read32(dma_rd_dar + 0x1C));

	/* Verify: DMA Write Channel SAR -> Read Channel DAR */
	for (offset = 0; offset < 8; offset++) {
		if ((mem_read32(dma_wr_sar + (offset * 0x4)))
			!= (mem_read32(dma_rd_dar + (offset * 0x4)))) {
				mprintf("** Verify Error\n");
				ret = ERROR_END;
				break;
		}
	}

	if (ret == NORMAL_END)	mprintf("Success!\n");

	mprintf("Finish\n");
	while(1);

	//TODO: Remove unrelated function.
	//SetEndTP(0,0,0,0,0);
	return NORMAL_END;
}

/*******************************************************
 * NAME		: Pcie_TestMain
 * FUNCTION	: PCIE Test Main Function
 * INPUT	: none
 * OUTPUT	: none
 * RETURN	: NORMAL_END	Pass
 *			: ERROR_END		Fail
 ******************************************************/
uint32_t Pcie_TestMain(void)
{
	uint32_t index;
	uint32_t ret = NORMAL_END;
	char inBuf[64];
	char chCnt;
	long hexData;

	while (1) {
		/* Display Test Menu */
		mprintf("PCIE Test Menu: [ID] Title\n");
		mprintf("---------------------------\n");
		for (index = 0; index <= PCIE_EXIT; index++) {
			mprintf("[%d] %s\n", pcie_TestTbl[index].id, pcie_TestTbl[index].title);
		}
		mprintf("---------------------------\n");
		mprintf("> ");

#if 1
		// TODO: hard cpde to test first.
		ret = pcie_TestTbl[PCIE_SIM_UT].func();
		break;
#else
		/* Select Test */
		/*GetStr(inBuf,&chCnt);
		if (!DecAscii2Hex(inBuf, chCnt, &hexData)) {
			if (hexData == PCIE_EXIT) {
				break;
			} else if (0 <= hexData && hexData < PCIE_EXIT) {
				/* Exec Test */
				ret = pcie_TestTbl[hexData].func();
			} else {
				mprintf("Unknown ID!\n");
			}
		} else {
			mprintf("Invalid Input!\n");
		}
#endif
	}
	mprintf("Test Finished!\n");

	return ret;
}
