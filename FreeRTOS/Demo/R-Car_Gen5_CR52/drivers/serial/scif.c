#include "drivers/serial/scif.h"
#include "CMSIS_5/cmsis_rcar_gen5.h"


#if SCIF==1
#define SCIF0_BASE 0xc0700000
#define SCIF1_BASE 0xc0704000
#define SCIF3_BASE 0xc0708000
#define SCIF4_BASE 0xc070C000
#else // HSCIF
#define SCIF0_BASE 0xc0710000
#define SCIF1_BASE 0xc0714000
#define SCIF3_BASE 0xc0718000
#define SCIF4_BASE 0xc071C000
#endif
static uint32_t scif_base = SCIF0_BASE;


/* Registers */
#define SCSMR           0x00    /* Serial Mode Register */
#define SCBRR           0x04    /* Bit Rate Register */
#define SCSCR           0x08    /* Serial Control Register */
#define SCFTDR          0x0c    /* Transmit FIFO Data Register */
#define SCFSR           0x10    /* Serial Status Register */
#define SCFRDR          0x14    /* Receive FIFO Data Register */
#define SCFCR           0x18    /* FIFO Control Register */
#define SCFDR           0x1c    /* FIFO Data Count Register */
#define SCSPTR          0x20    /* Serial Port Register */
#define SCLSR           0x24    /* Line Status Register */
#define DL              0x30    /* Frequency Division Register */
#define CKS             0x34    /* Clock Select Register */

/* SCSMR (Serial Mode Register) */
#define SCSMR_C_A       BIT(7)  /* Communication Mode */
#define SCSMR_CHR       BIT(6)  /* 7-bit Character Length */
#define SCSMR_PE        BIT(5)  /* Parity Enable */
#define SCSMR_O_E       BIT(4)  /* Odd Parity */
#define SCSMR_STOP      BIT(3)  /* Stop Bit Length */
#define SCSMR_CKS1      BIT(1)  /* Clock Select 1 */
#define SCSMR_CKS0      BIT(0)  /* Clock Select 0 */

/* SCSCR (Serial Control Register) */
#define SCSCR_TEIE      BIT(11) /* Transmit End Interrupt Enable */
#define SCSCR_TIE       BIT(7)  /* Transmit Interrupt Enable */
#define SCSCR_RIE       BIT(6)  /* Receive Interrupt Enable */
#define SCSCR_TE        BIT(5)  /* Transmit Enable */
#define SCSCR_RE        BIT(4)  /* Receive Enable */
#define SCSCR_REIE      BIT(3)  /* Receive Error Interrupt Enable */
#define SCSCR_TOIE      BIT(2)  /* Timeout Interrupt Enable */
#define SCSCR_CKE1      BIT(1)  /* Clock Enable 1 */
#define SCSCR_CKE0      BIT(0)  /* Clock Enable 0 */

/* SCFCR (FIFO Control Register) */
#define SCFCR_RTRG1     BIT(7)  /* Receive FIFO Data Count Trigger 1 */
#define SCFCR_RTRG0     BIT(6)  /* Receive FIFO Data Count Trigger 0 */
#define SCFCR_TTRG1     BIT(5)  /* Transmit FIFO Data Count Trigger 1 */
#define SCFCR_TTRG0     BIT(4)  /* Transmit FIFO Data Count Trigger 0 */
#define SCFCR_MCE       BIT(3)  /* Modem Control Enable */
#define SCFCR_TFRST     BIT(2)  /* Transmit FIFO Data Register Reset */
#define SCFCR_RFRST     BIT(1)  /* Receive FIFO Data Register Reset */
#define SCFCR_LOOP      BIT(0)  /* Loopback Test */

/* SCFSR (Serial Status Register) */
#define SCFSR_PER3      BIT(15) /* Parity Error Count 3 */
#define SCFSR_PER2      BIT(14) /* Parity Error Count 2 */
#define SCFSR_PER1      BIT(13) /* Parity Error Count 1 */
#define SCFSR_PER0      BIT(12) /* Parity Error Count 0 */
#define SCFSR_FER3      BIT(11) /* Framing Error Count 3 */
#define SCFSR_FER2      BIT(10) /* Framing Error Count 2 */
#define SCFSR_FER_1     BIT(9)  /* Framing Error Count 1 */
#define SCFSR_FER0      BIT(8)  /* Framing Error Count 0 */
#define SCFSR_ER        BIT(7)  /* Receive Error */
#define SCFSR_TEND      BIT(6)  /* Transmission ended */
#define SCFSR_TDFE      BIT(5)  /* Transmit FIFO Data Empty */
#define SCFSR_BRK       BIT(4)  /* Break Detect */
#define SCFSR_FER       BIT(3)  /* Framing Error */
#define SCFSR_PER       BIT(2)  /* Parity Error */
#define SCFSR_RDF       BIT(1)  /* Receive FIFO Data Full */
#define SCFSR_DR        BIT(0)  /* Receive Data Ready */

/* SCLSR (Line Status Register) on (H)SCIF */
#define SCLSR_TO        BIT(2)  /* Timeout */
#define SCLSR_ORER      BIT(0)  /* Overrun Error */

// CPG Registers



static void uart_rcar_write_8(uint32_t offs, uint8_t value)
{
	sys_write8(value, scif_base + offs);
}

static uint16_t uart_rcar_read_16(uint32_t offs)
{
	return sys_read16(scif_base + offs);
}

static void uart_rcar_write_16(uint32_t offs, uint16_t value)
{
	sys_write16(value, scif_base + offs);
}

static void uart_rcar_set_baudrate(uint32_t baud_rate)
{
	uint8_t reg_val;
    const uint32_t clock_rate = 66660000u; // S0D12 Clock rate
    
	reg_val = ((clock_rate + 16 * baud_rate) / (32 * baud_rate) - 1);
	uart_rcar_write_8(SCBRR, reg_val);
}

uint32_t console_init(uint32_t port) {

	uint16_t reg_val;

	switch (port) {
		case 0:
			scif_base = SCIF0_BASE;
			break;
		case 1:
			scif_base = SCIF1_BASE;
			break;
		case 3:
			scif_base = SCIF3_BASE;
			break;
		case 4:
			scif_base = SCIF4_BASE;
			break;
		default:
			return 0;
	}

	/* Disable Transmit and Receive */
	reg_val = uart_rcar_read_16(SCSCR);
	reg_val &= ~(SCSCR_TE | SCSCR_RE);
	uart_rcar_write_16(SCSCR, reg_val);

	/* Emptying Transmit and Receive FIFO */
	reg_val = uart_rcar_read_16(SCFCR);
	reg_val |= (SCFCR_TFRST | SCFCR_RFRST);
	uart_rcar_write_16(SCFCR, reg_val);

	/* Resetting Errors Registers */
	reg_val = uart_rcar_read_16(SCFSR);
	reg_val &= ~(SCFSR_ER | SCFSR_DR | SCFSR_BRK | SCFSR_RDF);
	uart_rcar_write_16(SCFSR, reg_val);

	reg_val = uart_rcar_read_16(SCLSR);
	reg_val &= ~(SCLSR_TO | SCLSR_ORER);
	uart_rcar_write_16(SCLSR, reg_val);

	/* Select internal clock */
	reg_val = uart_rcar_read_16(SCSCR);
	reg_val &= ~(SCSCR_CKE1 | SCSCR_CKE0);
	uart_rcar_write_16(SCSCR, reg_val);

	/* Serial Configuration (8N1) & Clock divider selection */
	reg_val = uart_rcar_read_16( SCSMR);
	reg_val &= ~(SCSMR_C_A | SCSMR_CHR | SCSMR_PE | SCSMR_O_E | SCSMR_STOP |
		     SCSMR_CKS1 | SCSMR_CKS0);
	uart_rcar_write_16(SCSMR, reg_val);

	/* Set baudrate */
	uart_rcar_set_baudrate(115200);

	/* FIFOs data count trigger configuration */
	reg_val = uart_rcar_read_16(SCFCR);
	reg_val &= ~(SCFCR_RTRG1 | SCFCR_RTRG0 | SCFCR_TTRG1 | SCFCR_TTRG0 |
		     SCFCR_MCE | SCFCR_TFRST | SCFCR_RFRST);
	uart_rcar_write_16( SCFCR, reg_val);

	/* Enable Transmit & Receive + disable Interrupts */
	reg_val = uart_rcar_read_16(SCSCR);
	reg_val |= (SCSCR_TE | SCSCR_RE);
	reg_val &= ~(SCSCR_TIE | SCSCR_RIE | SCSCR_TEIE | SCSCR_REIE |
		     SCSCR_TOIE);
	uart_rcar_write_16(SCSCR, reg_val);

	return 1;
}

void console_putc(char c) {
    uint16_t reg_val;

	/* Wait for empty space in transmit FIFO */
	while (!(uart_rcar_read_16(SCFSR) & SCFSR_TDFE)) {
	}

	/* Send current byte */
	uart_rcar_write_8(SCFTDR, c);

	reg_val = uart_rcar_read_16(SCFSR);
	reg_val &= ~(SCFSR_TDFE | SCFSR_TEND);
	uart_rcar_write_16(SCFSR, reg_val);
}
