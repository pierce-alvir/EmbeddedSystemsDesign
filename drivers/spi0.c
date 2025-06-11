#include "spi0.h"
#include <rp2040/resets.h>
#include <rp2040/sio.h>
#include <rp2040/io_bank0.h>
#include <rp2040/pads_bank0.h>
#include <rp2040/spi.h>
#include <rp2040/clocks.h>
#include <stdint.h>

#define CONCAT2(X,Y) X ## Y
#define CONCAT3(X,Y,Z) X ## Y ## Z
#define GPIO(X) CONCAT2(gpio,X)
#define GPIO_CTRL(X) CONCAT3(gpio,X,_ctrl)

#define SPI0_SCK_LOC 2
#define SPI0_TX_LOC 3
#define SPI0_RX_LOC 4
#define SPI0_CS_LOC 5

#define SPI0_RESETS ( RESETS_RESET_SPI0_MASK \
		| RESETS_RESET_IO_BANK0_MASK \
		| RESETS_RESET_PADS_BANK0_MASK )

void configure_spi0()
{
	clocks -> clk_peri_ctrl = (CLOCKS_CLK_PERI_CTRL_ENABLE_MASK|CLOCKS_CLK_PERI_CTRL_AUXSRC(0));

	resets -> clr_reset = SPI0_RESETS;
	while( (resets->reset_done & SPI0_RESETS) != SPI0_RESETS);

	uint32_t const out_pads_bank0_gpio = 
	PADS_BANK0_GPIO25_OD(0) 
	| PADS_BANK0_GPIO25_IE(1) 
	| PADS_BANK0_GPIO25_DRIVE(0) 
	| PADS_BANK0_GPIO25_PUE(0) 
	| PADS_BANK0_GPIO25_PDE(0) 
	| PADS_BANK0_GPIO25_SCHMITT(0) 
	| PADS_BANK0_GPIO25_SLEWFAST(0);

    uint32_t const out_io_bank0_gpio_ctrl = 
	IO_BANK0_GPIO25_CTRL_IRQOVER(0) |
	IO_BANK0_GPIO25_CTRL_INOVER(0)  |
	IO_BANK0_GPIO25_CTRL_OEOVER(0)  |
	IO_BANK0_GPIO25_CTRL_OUTOVER(0) |
	IO_BANK0_GPIO25_CTRL_FUNCSEL(1);
	
	/*
	pads_bank0 -> GPIO( SPI0_SCK_LOC ) = 
		PADS_BANK0_GPIO25_OD(0)
		| PADS_BANK0_GPIO25_IE(1)
		| PADS_BANK0_GPIO25_DRIVE(0)
		| PADS_BANK0_GPIO25_PUE(0)
		| PADS_BANK0_GPIO25_PDE(0)
		| PADS_BANK0_GPIO25_SCHMITT(0)
		| PADS_BANK0_GPIO25_SLEWFAST(0);
	pads_bank0 -> GPIO( SPI0_TX_LOC ) = 
		PADS_BANK0_GPIO25_OD(0)
		| PADS_BANK0_GPIO25_IE(1)
		| PADS_BANK0_GPIO25_DRIVE(0)
		| PADS_BANK0_GPIO25_PUE(0)
		| PADS_BANK0_GPIO25_PDE(0)
		| PADS_BANK0_GPIO25_SCHMITT(0)
		| PADS_BANK0_GPIO25_SLEWFAST(0);
	pads_bank0 -> GPIO( SPI0_RX_LOC ) = 
		PADS_BANK0_GPIO25_OD(0)
		| PADS_BANK0_GPIO25_IE(1)
		| PADS_BANK0_GPIO25_DRIVE(0)
		| PADS_BANK0_GPIO25_PUE(0)
		| PADS_BANK0_GPIO25_PDE(0)
		| PADS_BANK0_GPIO25_SCHMITT(0)
		| PADS_BANK0_GPIO25_SLEWFAST(0);
	pads_bank0 -> GPIO( SPI0_CS_LOC ) = 
		PADS_BANK0_GPIO25_OD(0)
		| PADS_BANK0_GPIO25_IE(1)
		| PADS_BANK0_GPIO25_DRIVE(0)
		| PADS_BANK0_GPIO25_PUE(0)
		| PADS_BANK0_GPIO25_PDE(0)
		| PADS_BANK0_GPIO25_SCHMITT(0)
		| PADS_BANK0_GPIO25_SLEWFAST(0);
	
	io_bank0 -> GPIO_CTRL( SPI0_SCK_LOC ) = 
		IO_BANK0_GPIO25_CTRL_IRQOVER(0) |
		IO_BANK0_GPIO25_CTRL_INOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OEOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OUTOVER(0) |
		IO_BANK0_GPIO25_CTRL_FUNCSEL(1);
	io_bank0 -> GPIO_CTRL( SPI0_TX_LOC ) = 
		IO_BANK0_GPIO25_CTRL_IRQOVER(0) |
		IO_BANK0_GPIO25_CTRL_INOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OEOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OUTOVER(0) |
		IO_BANK0_GPIO25_CTRL_FUNCSEL(1);
	io_bank0 -> GPIO_CTRL( SPI0_RX_LOC ) = 
		IO_BANK0_GPIO25_CTRL_IRQOVER(0) |
		IO_BANK0_GPIO25_CTRL_INOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OEOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OUTOVER(0) |
		IO_BANK0_GPIO25_CTRL_FUNCSEL(1);
	io_bank0 -> GPIO_CTRL( SPI0_CS_LOC ) = 
		IO_BANK0_GPIO25_CTRL_IRQOVER(0) |
		IO_BANK0_GPIO25_CTRL_INOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OEOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OUTOVER(0) |
		IO_BANK0_GPIO25_CTRL_FUNCSEL(1);
	*/
	pads_bank0 -> GPIO(SPI0_SCK_LOC) = out_pads_bank0_gpio;
	pads_bank0 -> GPIO(SPI0_TX_LOC) = out_pads_bank0_gpio;
	pads_bank0 -> GPIO(SPI0_RX_LOC) = out_pads_bank0_gpio;
	pads_bank0 -> GPIO(SPI0_CS_LOC) = out_pads_bank0_gpio;
	
	io_bank0 -> GPIO_CTRL(SPI0_SCK_LOC) = out_io_bank0_gpio_ctrl;
	io_bank0 -> GPIO_CTRL(SPI0_TX_LOC) = out_io_bank0_gpio_ctrl;
	io_bank0 -> GPIO_CTRL(SPI0_RX_LOC) = out_io_bank0_gpio_ctrl;
	io_bank0 -> GPIO_CTRL(SPI0_CS_LOC) = out_io_bank0_gpio_ctrl;
	
	spi0 -> sspcr0 = (SPI0_SSPCR0_SCR(0)
				|SPI0_SSPCR0_SPH(0)
				|SPI0_SSPCR0_SPO(0)
				|SPI0_SSPCR0_FRF(0x0)
				|SPI0_SSPCR0_DSS(0xf));
	spi0 -> sspcpsr = SPI0_SSPCPSR_CPSDVSR(125);
	spi0 -> sspcr1 = SPI0_SSPCR1_SSE_MASK;	
}

uint16_t spi0_rx()
{
	return spi0 -> sspdr;
}

void spi0_tx(uint16_t data)
{
	spi0 -> sspdr = data;
}