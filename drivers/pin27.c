#include "pin27.h"
#include <rp2040/resets.h>
#include <rp2040/sio.h>
#include <rp2040/io_bank0.h>
#include <rp2040/pads_bank0.h>
#include "gpio_irq.h"
#include <stdbool.h>

#define PIN27_RESETS  ( RESETS_RESET_IO_BANK0_MASK \
			| RESETS_RESET_PADS_BANK0_MASK)

static void callback();
static _Bool pin_state;

_Bool get_pin27();

void configure_pin27()
{
	resets -> clr_reset = PIN27_RESETS;
	while( (resets -> reset_done & PIN27_RESETS) != PIN27_RESETS);
	
	pads_bank0 -> gpio27 = 
		PADS_BANK0_GPIO25_OD(0)
		| PADS_BANK0_GPIO25_IE(1)
		| PADS_BANK0_GPIO25_DRIVE(0)
		| PADS_BANK0_GPIO25_PUE(1)
		| PADS_BANK0_GPIO25_PDE(0)
		| PADS_BANK0_GPIO25_SCHMITT(0)
		| PADS_BANK0_GPIO25_SLEWFAST(0);
	
	io_bank0 -> gpio27_ctrl = 
		IO_BANK0_GPIO25_CTRL_IRQOVER(0) |
		IO_BANK0_GPIO25_CTRL_INOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OEOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OUTOVER(0) |
		IO_BANK0_GPIO25_CTRL_FUNCSEL(5);
	
	register_gpio_irq_callback(27, callback);
	io_bank0 -> proc0_inte3 |= ( IO_BANK0_PROC0_INTE3_GPIO27_EDGE_HIGH_MASK
					| IO_BANK0_PROC0_INTE3_GPIO27_EDGE_LOW_MASK);
}


_Bool get_pin27()
{
	return pin_state;
}

static void callback()
{
	if( io_bank0 -> proc0_ints3 & IO_BANK0_PROC0_INTS3_GPIO27_EDGE_HIGH_MASK)
		pin_state=true;
	if( io_bank0 -> proc0_ints3 & IO_BANK0_PROC0_INTS3_GPIO27_EDGE_LOW_MASK)
		pin_state=false;
}
