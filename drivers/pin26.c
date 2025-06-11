#include "pin26.h"
#include <rp2040/resets.h>
#include <rp2040/sio.h>
#include <rp2040/io_bank0.h>
#include <rp2040/pads_bank0.h>
#include "gpio_irq.h"
#include <stdbool.h>

#define CONCAT2(X,Y) X ## Y
#define CONCAT3(X,Y,Z) X ## Y ## Z
#define GPIO(X) CONCAT2(gpio,X)
#define GPIO_CTRL(X) CONCAT3(gpio,X,_ctrl)

#define GPIO_LOC 26

#define PIN26_RESETS  ( RESETS_RESET_IO_BANK0_MASK \
				| RESETS_RESET_PADS_BANK0_MASK)

static void callback();
static _Bool pin_state;

_Bool get_pin26();

void configure_pin26()
{
	resets -> clr_reset = PIN26_RESETS;
	while( (resets -> reset_done & PIN26_RESETS) != PIN26_RESETS);
	
	pads_bank0 -> GPIO( GPIO_LOC ) = 
		PADS_BANK0_GPIO25_OD(0)
		| PADS_BANK0_GPIO25_IE(1)
		| PADS_BANK0_GPIO25_DRIVE(0)
		| PADS_BANK0_GPIO25_PUE(1)
		| PADS_BANK0_GPIO25_PDE(0)
		| PADS_BANK0_GPIO25_SCHMITT(0)
		| PADS_BANK0_GPIO25_SLEWFAST(0);
	
	io_bank0 -> GPIO_CTRL( GPIO_LOC ) = 
		IO_BANK0_GPIO25_CTRL_IRQOVER(0) |
		IO_BANK0_GPIO25_CTRL_INOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OEOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OUTOVER(0) |
		IO_BANK0_GPIO25_CTRL_FUNCSEL(5);
	
	register_gpio_irq_callback(GPIO_LOC, callback);
	io_bank0 -> proc0_inte3 |= ( IO_BANK0_PROC0_INTE3_GPIO26_EDGE_HIGH_MASK
					| IO_BANK0_PROC0_INTE3_GPIO26_EDGE_LOW_MASK);
}


_Bool get_pin26()
{
	return pin_state;
}

static void callback()
{
	if( io_bank0 -> proc0_ints3 & IO_BANK0_PROC0_INTS3_GPIO26_EDGE_HIGH_MASK)
		pin_state=true;
	if( io_bank0 -> proc0_ints3 & IO_BANK0_PROC0_INTS3_GPIO26_EDGE_LOW_MASK)
		pin_state=false;
}
