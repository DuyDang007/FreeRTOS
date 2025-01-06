/*************************************************************************************************************
* Copyright (c) 2024 Renesas Electronics Corporation
* This software is released under the MIT License
* http://opensource.org/licenses/mit-license.php
*************************************************************************************************************/

#include "stdio.h"
#include "r_gpio_api.h"

#define BASE_ADDR_ERR			0xABCD

/* GPIO Bus Domain:
 * By default: use Bus Domain 0.
 */
#define	BUS_DOMAIN_0			0x0
#define	BUS_DOMAIN_1			0x2000
#define	BUS_DOMAIN_2			0x4000
#define	BUS_DOMAIN_3			0x6000

/* GPIO Registers Area:
 * By default: use R/W area.
 */
#define	RW_AREA				0x0
#define	SET_AREA			0x200
#define	CLR_AREA			0x400

/* GPIO base adrress */
#define	GPIO_GR_0			0xC1080000
#define	GPIO_GR_1			0xC1080800
#define	GPIO_GR_2			0xC1081000
#define	GPIO_GR_3			0xC0800000
#define	GPIO_GR_4			0xC0800800
#define	GPIO_GR_5			0xC0400000
#define	GPIO_GR_6			0xC0400800
#define	GPIO_GR_7			0xC0401000
#define	GPIO_GR_8			0xC0401800
#define	GPIO_GR_9			0xC9B00000
#define	GPIO_GR_10			0xC9B00800

#define BIT(nr)				(1UL << (nr))

/* GPIO register: offset address */
#define	GP_PMMR				0x000
#define	GP_PMMER			0x004
#define	GP_PSER				0x008
#define	GP_PSSR0			0x00C
#define	GP_PSSR1			0x010
#define	GP_DMPR0			0x020
#define	GP_DMPR1			0x024
#define	GP_DMPR2			0x028
#define	GP_DMPR3			0x02C
#define	GP_GPSR				0x040
#define	GP_ALTSEL0			0x060
#define	GP_ALTSEL1			0x064
#define	GP_ALTSEL2			0x068
#define	GP_ALTSEL3			0x06C
#define	GP_DRVCTRL0			0x080
#define	GP_DRVCTRL1			0x084
#define	GP_DRVCTRL2			0x088
#define	GP_TDSEL0			0x094
#define	GP_TDSEL1			0x098
#define	GP_PULLEN			0x0C0
#define	GP_PUDSEL			0x0C4
#define	GP_MODSEL			0x100
#define	GP_IOINTSEL			0x110
#define	GP_INOUTSEL			0x114
#define	GP_OUTDT			0x118
#define	GP_OUTDTSEL			0x11C
#define	GP_OUTDTH			0x120
#define	GP_OUTDTL			0x124
#define	GP_INEN				0x128
#define	GP_INDT				0x12C
#define	GP_INTDT			0x190
#define	GP_INTCLR			0x194
#define	GP_INTMSK			0x198
#define	GP_MSKCLR			0x19C
#define	GP_POSNEG			0x1A0
#define	GP_EDGLEVEL			0x1A4
#define	GP_FILONOFF			0x1A8
#define	GP_FILCLKSEL			0x1AC
#define	GP_BOTHEDGE			0x1CC

static void writel(const uint32_t Value, const uintptr_t Address)
{
       *((volatile unsigned int*) Address)  = Value;
}

static uint32_t readl(const uintptr_t Address)
{
	return *((volatile unsigned int*)Address);
}

static void setbit_l(uint32_t addr, uint32_t pos)
{
	uint32_t val = readl(addr);

	writel(val |= BIT(pos), addr);
}

static uint32_t getbit_l(uint32_t addr, uint32_t pos)
{
	return !!(readl(addr) & BIT(pos));
}

static void clearbit_l(uint32_t addr, uint32_t pos)
{
	uint32_t val = readl(addr);

	writel(val &= ~BIT(pos), addr);
}

static uint32_t getGpioResgister(uint8_t group, uint32_t offset)
{
	uint32_t base_addr;
	uint32_t reg_addr;

	switch (group) {
	case 0:
		base_addr = GPIO_GR_0;
		break;
	case 1:
		base_addr = GPIO_GR_1;
		break;
	case 2:
		base_addr = GPIO_GR_2;
		break;
	case 3:
		base_addr = GPIO_GR_3;
		break;
	case 4:
		base_addr = GPIO_GR_4;
		break;
	case 5:
		base_addr = GPIO_GR_5;
		break;
	case 6:
		base_addr = GPIO_GR_6;
		break;
	case 7:
		base_addr = GPIO_GR_7;
		break;
	case 8:
		base_addr = GPIO_GR_8;
		break;
	case 9:
		base_addr = GPIO_GR_9;
		break;
	case 10:
		base_addr = GPIO_GR_10;
		break;
	default:
		base_addr = BASE_ADDR_ERR;
		goto hang_drive;
	}

	reg_addr = base_addr + offset;

	return reg_addr;

hang_drive:
	printf("GPIO group %d not exist!\n", group);
	while(1);
}

static void pfcWrite(uint8_t group, uint32_t addr, uint32_t val)
{
	writel(~val, getGpioResgister(group, GP_PMMR));
	writel(val, addr);
}

static void pfcSetGPSR(uint8_t gpio, uint8_t group, uint8_t pin)
{
	uint32_t val, reg_addr;

	reg_addr = getGpioResgister(group, GP_GPSR);
	val = readl(reg_addr);
	val = gpio ? val & ~BIT(pin) : val | BIT(pin);
	pfcWrite(group, reg_addr, val);
}

static void pfcSetPeripheral(uint8_t group, uint8_t pin)
{
	pfcSetGPSR(0, group, pin);
}

int gpioSetPinOutput(uint8_t group, uint8_t pin, bool lvl)
{
	uint32_t reg_addr;

	reg_addr = getGpioResgister(group, GP_OUTDT);
	if (lvl)
		setbit_l(reg_addr, pin);
	else
		clearbit_l(reg_addr, pin);

	//printf("GP_OUTDT=0x%x\n", readl(getGpioResgister(0, GP_OUTDT)));
	return 0;
}

int gpioSetPortOutput(uint8_t group, uint32_t mask_levels,
		       uint32_t mask_pins)
{
	uint32_t pin_num, mask_pos;
	bool pin_level;

	for (pin_num = 0U; pin_num < PINS_EACH_GROUP; pin_num++) {
		mask_pos = 1U << pin_num;
		if (mask_pins & mask_pos) {
			pin_level = (mask_levels & mask_pos) >> pin_num;
			//printf("pin_num=%d\n", pin_num);
			//printf("pin_level=%d\n", pin_level);
			gpioSetPinOutput(group, pin_num, pin_level);
		}
	}

	return 0;
}

bool gpioReadPin(uint8_t group, uint8_t pin)
{
	uint32_t bit = BIT(pin);
	bool pin_val;

	if (readl(getGpioResgister(group, GP_INOUTSEL)) & bit)
    		pin_val = !!(readl(getGpioResgister(group, GP_OUTDT)) & bit);
	else
    		pin_val = !!(readl(getGpioResgister(group, GP_INDT)) & bit);

	return pin_val;
}

uint32_t gpioReadPort(uint8_t group)
{
	uint32_t pin_val;

    	pin_val = readl(getGpioResgister(group, GP_OUTDT));
    	pin_val |= readl(getGpioResgister(group, GP_INDT));

	return pin_val;
}

//void pfcSetBusDomainProtection()
//{
//	/* NOTE: At the moment, only set Bus domain 0 */
//
//	/* Set GPn_PMMER[p] = 1'b0 */
//	clearbit_l(getGpioResgister(0, GP_PMMER), 0);
//	//printf("GP_PMMER=0x%x\n", readl(getGpioResgister(0, GP_PMMER)));
//
//	/* Set GPn_DMCTRLm[p]: default */
//	//writel(0xFFFFFFFF, getGpioResgister(0, GP_DMPR0));
//	//writel(0x00000000, getGpioResgister(0, GP_DMPR1));
//	//writel(0x00000000, getGpioResgister(0, GP_DMPR2));
//	//writel(0x00000000, getGpioResgister(0, GP_DMPR3));
//
//	/* Set GPn_PMMER[p] = 1'b1 */
//	setbit_l(getGpioResgister(0, GP_PMMER), 0);
//	//printf("GP_PMMER=0x%x\n", readl(getGpioResgister(0, GP_PMMER)));
//}
//
//void pfcSetPortSafeState()
//{
//	/* NOTE: At the moment, only set Bus domain 0 */
//
//	/* Set GPn_PMMER[p] = 1'b0 */
//	clearbit_l(getGpioResgister(0, GP_PMMER), 0);
//	//printf("GP_PMMER=0x%x\n", readl(getGpioResgister(0, GP_PMMER)));
//
//	/* Set value of User's configuration for GPIO or Peripheral Function: default */
//
//	/* Set GPn_PSSRm: default */
//
//	/* Set GPn_PSER: default */
//
//	/* Set GPn_PMMER[p] = 1'b1 */
//	setbit_l(getGpioResgister(0, GP_PMMER), 0);
//	//printf("GP_PMMER=0x%x\n", readl(getGpioResgister(0, GP_PMMER)));
//}

static void pfcSetGPIO(uint8_t group, uint8_t pin)
{
	pfcSetGPSR(1, group, pin);
}

static void gpioSetGeneralOutputMode(uint8_t group, uint8_t pin, uint8_t lvl)
{
	/* Set Peripheral Function to GPIO */
	pfcSetGPIO(group, pin);

	/* (1) Set the initial values of the output ports in OUTDT.  
	 *     Set the positive or negative logic in POSNEG.
	 */
	gpioSetPinOutput(group, pin, lvl);
	clearbit_l(getGpioResgister(group, GP_POSNEG), pin);
	//printf("GP_POSNEG=0x%x\n", readl(getGpioResgister(group, GP_POSNEG)));

	/* Select "Input Disable" in INEN */
	clearbit_l(getGpioResgister(group, GP_INEN), pin);
	//printf("GP_INEN=0x%x\n", readl(getGpioResgister(group, GP_INEN)));

	/* (2) Set general input/output mode in IOINTSEL. */
	clearbit_l(getGpioResgister(group, GP_IOINTSEL), pin);
	//printf("GP_IOINTSEL=0x%x\n", readl(getGpioResgister(group, GP_IOINTSEL)));

	/* (3) Set general output mode in INOUTSEL, which allows
	 * the ports to output the signal of the specified level.
	 */
	setbit_l(getGpioResgister(group, GP_INOUTSEL), pin);
	//printf("GP_INOUTSEL=0x%x\n", readl(getGpioResgister(group, GP_INOUTSEL)));

	/* Output mode by configuring OUTDTSEL register to change output by OUTDT */
	clearbit_l(getGpioResgister(group, GP_OUTDTSEL), pin);
	//printf("GP_OUTDTSEL=0x%x\n", readl(getGpioResgister(group, GP_OUTDTSEL)));
}

static void gpioSetGeneralInputMode(uint8_t group, uint8_t pin)
{
	/* Set Peripheral Function to GPIO */
	pfcSetGPIO(group, pin);

	/* (1) Set the positive (not inverted) or negative (inverted)
	 *     logic for processing the input signals in POSNEG.
	 */
	clearbit_l(getGpioResgister(group, GP_POSNEG), pin);
	//printf("GP_POSNEG=0x%x\n", readl(getGpioResgister(group, GP_POSNEG)));

	/* Select "Input Enable" in INEN */
	setbit_l(getGpioResgister(group, GP_INEN), pin);
	//printf("GP_INEN=0x%x\n", readl(getGpioResgister(group, GP_INEN)));

	/* (2) Set general input/output mode in IOINTSEL. */
	clearbit_l(getGpioResgister(group, GP_IOINTSEL), pin);
	//printf("GP_IOINTSEL=0x%x\n", readl(getGpioResgister(group, GP_IOINTSEL)));

	/* (3) Set general input mode in INOUTSEL. */
	clearbit_l(getGpioResgister(group, GP_INOUTSEL), pin);
	//printf("GP_INOUTSEL=0x%x\n", readl(getGpioResgister(group, GP_INOUTSEL)));
}

int gpioSetPinGeneralMode(uint8_t group, uint8_t pin,
			  rcar_io_direction_t option)
{
	switch (option) {
	case RCAR_IO_DIRECTION_OUTPUT:
		gpioSetGeneralOutputMode(group, pin, 0);
		break;
	case RCAR_IO_DIRECTION_INPUT:
		gpioSetGeneralInputMode(group, pin);
		break;
	default:
		return -1;
	}
	return 0;
}

int gpioSetPortGeneralMode(uint8_t group, uint32_t mask_directions,
			   uint32_t mask_pins)
{
	uint32_t pin_num, mask_pos;
	rcar_io_direction_t pin_option;

	for (pin_num = 0U; pin_num < PINS_EACH_GROUP; pin_num++) {
		mask_pos = 1U << pin_num;
		if (mask_pins & mask_pos) {
			pin_option = (mask_directions & mask_pos) >> pin_num;
			//printf("pin_num=%d\n", pin_num);
			//printf("pin_option=%d\n", pin_option);
			(void) gpioSetPinGeneralMode(group, pin_num, pin_option);
		}
	}
}

void gpioSetEdgeSensitiveInterruptInputMode(uint8_t group, uint8_t pin,
					    bool active_high_rising_edge,
					    bool both)
{
	/* Set Peripheral Function to GPIO */
	pfcSetGPIO(group, pin);

	/* (1) Set the positive or negative logic as the interrupt
	 *     input condition in POSNEG.
	 */
	if (active_high_rising_edge)
		clearbit_l(getGpioResgister(group, GP_POSNEG), pin);
	else
		setbit_l(getGpioResgister(group, GP_POSNEG), pin);
	//printf("GP_POSNEG=0x%x\n", readl(getGpioResgister(group, GP_POSNEG)));

	/* (2) Set the edge (set to 1) as the interrupt input
	 *     condition in EDGLEVEL.
	 */
	setbit_l(getGpioResgister(group, GP_EDGLEVEL), pin);
	//printf("GP_EDGLEVEL=0x%x\n", readl(getGpioResgister(group, GP_EDGLEVEL)));

	/* (3) Set the one edge/both edge as the interrupt input
	 *     condition in BOTHEDGE.
	 */
	if (both)
		setbit_l(getGpioResgister(group, GP_BOTHEDGE), pin);
	else
		clearbit_l(getGpioResgister(group, GP_BOTHEDGE), pin);
	//printf("GP_BOTHEDGE=0x%x\n", readl(getGpioResgister(group, GP_BOTHEDGE)));

	/* Select "Input Enable" in INEN */
	setbit_l(getGpioResgister(group, GP_INEN), pin);
	//printf("GP_INEN=0x%x\n", readl(getGpioResgister(group, GP_INEN)));

	/* (4) Set interrupt input mode in IOINTSEL. */
	setbit_l(getGpioResgister(group, GP_IOINTSEL), pin);
	//printf("GP_IOINTSEL=0x%x\n", readl(getGpioResgister(group, GP_IOINTSEL)));

	/* (5) Write 1 to INTCLR to clear (initialize) the interrupt
	 *     display bit in edge-sensitive interrupt mode.
	 */
	setbit_l(getGpioResgister(group, GP_INTCLR), pin);
	//printf("GP_INTCLR=0x%x\n", readl(getGpioResgister(group, GP_INTCLR)));

	/* (6) Write 1 to MSKCLR (or MSKCLRS depending on
	 *     interrupt or alternative interrupt is used) cancel the
	 *     interrupt mask.
	 */
	setbit_l(getGpioResgister(group, GP_MSKCLR), pin);
	//printf("GP_MSKCLR=0x%x\n", readl(getGpioResgister(group, GP_MSKCLR)));
}

int gpioSetPinInterruptInputMode(uint8_t group, uint8_t pin,
				 rcar_interrupt_input_t option)
{
	switch (option) {
	case RCAR_INTERRUPT_INPUT_RISING_EDGE:
		gpioSetEdgeSensitiveInterruptInputMode(group, pin, true, false);
		break;
	case RCAR_INTERRUPT_INPUT_FALLING_EDGE:
		gpioSetEdgeSensitiveInterruptInputMode(group, pin, false, false);
		break;
	case RCAR_INTERRUPT_INPUT_BOTH_EDGE:
		gpioSetEdgeSensitiveInterruptInputMode(group, pin, true, true);
		break;
	default:
		return -1;
	}
	return 0;
}

void gpioSetLevelSensitiveInterruptInputMode(uint8_t group, uint8_t pin,
					    bool active_high_rising_edge,
					    bool both)
{
	/* Set Peripheral Function to GPIO */
	pfcSetGPIO(group, pin);

	/* (1) Set the positive or negative logic as the interrupt
	 *     input condition in POSNEG.
	 */
	if (active_high_rising_edge)
		clearbit_l(getGpioResgister(group, GP_POSNEG), pin);
	else
		setbit_l(getGpioResgister(group, GP_POSNEG), pin);
	//printf("GP_POSNEG=0x%x\n", readl(getGpioResgister(group, GP_POSNEG)));

	/* (2) Set the level (set to 0) as the interrupt input
	 *     condition in EDGLEVEL.
	 */
	clearbit_l(getGpioResgister(group, GP_EDGLEVEL), pin);
	//printf("GP_EDGLEVEL=0x%x\n", readl(getGpioResgister(group, GP_EDGLEVEL)));

	/* Select "Input Enable" in INEN */
	setbit_l(getGpioResgister(group, GP_INEN), pin);
	//printf("GP_INEN=0x%x\n", readl(getGpioResgister(group, GP_INEN)));

	/* (3) Set interrupt input mode in IOINTSEL. */
	setbit_l(getGpioResgister(group, GP_IOINTSEL), pin);
	//printf("GP_IOINTSEL=0x%x\n", readl(getGpioResgister(group, GP_IOINTSEL)));

	/* (4) Write 1 to MSKCLR to cancel the interrupt mask. */
	setbit_l(getGpioResgister(group, GP_MSKCLR), pin);
	//printf("GP_MSKCLR=0x%x\n", readl(getGpioResgister(group, GP_MSKCLR)));
	
}

void gpioClearInterruptHandler(uint32_t group_pin)
{
	uint32_t group = (IOPORT_PRV_PORT_BITS & group_pin) >> IOPORT_PRV_PORT_OFFSET;
	uint32_t pin = (IOPORT_PRV_PIN_BITS & group_pin);

	//printf("pin_cfg=%d\n", p_instance_ctrl->p_cfg->p_pin_cfg_data->pin_cfg);
	//printf("pin=%d\n", p_instance_ctrl->p_cfg->p_pin_cfg_data->pin);
	//printf("group=%d\n", group); 
	//printf("pin=%d\n", pin);

	//printf("GPIO IRQ!\n");

	/* 2. Clear interrupt status flag */
	setbit_l(getGpioResgister(group, GP_INTCLR), pin);

	/* 4. Use Semaphore to trigger other task if needed */
	//BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	//xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
	//portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

int gpioSetupIRQ(uint8_t group, IrqHandlerFn handler, void *data)
{
	uint32_t int_id;

	switch (group) {
	case 0:
		int_id = INTID_GPIO_GRP0;
		break;
	case 1:
		int_id = INTID_GPIO_GRP1;
		break;
	case 2:
		int_id = INTID_GPIO_GRP2;
		break;
	case 3:
		int_id = INTID_GPIO_GRP3;
		break;
	case 4:
		int_id = INTID_GPIO_GRP4;
		break;
	case 5:
		int_id = INTID_GPIO_GRP5;
		break;
	case 6:
		int_id = INTID_GPIO_GRP6;
		break;
	case 7:
		int_id = INTID_GPIO_GRP7;
		break;
	case 8:
		int_id = INTID_GPIO_GRP8;
		break;
	case 9:
		int_id = INTID_GPIO_GRP9;
		break;
	case 10:
		int_id = INTID_GPIO_GRP10;
		break;
	default:
		int_id = INTID_GPIO_NO_EXIST;
		goto setup_irq_fail;
	}

	/* Set Handler for Irq */
	Irq_SetupEntry(int_id, handler, data);

	/* Set priority for Irq */
	Irq_SetPriority(int_id, IPRIORITY(3));
	
	/* Enable Irq */
	Irq_Enable(int_id);

	return 0;

setup_irq_fail:
	printf("IRQ FAILED: group no exist!\n");
	return -1;
}

