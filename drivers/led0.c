/************************************************************************

Author - Pierce Alvir

Description - 
	Driver file for an active-low led connected to gpio 28.
	
*************************************************************************/

#include "led0.h"
#include <rp2040/resets.h>
#include <rp2040/sio.h>
#include <rp2040/io_bank0.h>
#include <rp2040/pads_bank0.h>

void configure_led0( void )
{
    resets -> clr_reset = 
	    RESETS_RESET_IO_BANK0_MASK
	    | RESETS_RESET_PADS_BANK0_MASK;

    while(! (resets -> reset_done & RESETS_RESET_IO_BANK0_MASK))
    	continue;

    while(! (resets -> reset_done & RESETS_RESET_PADS_BANK0_MASK))
    	continue;


    pads_bank0 -> gpio28 = 
	PADS_BANK0_GPIO28_OD(0) 
	| PADS_BANK0_GPIO28_IE(1) 
	| PADS_BANK0_GPIO28_DRIVE(0) 
	| PADS_BANK0_GPIO28_PUE(0) 
	| PADS_BANK0_GPIO28_PDE(0) 
	| PADS_BANK0_GPIO28_SCHMITT(0) 
	| PADS_BANK0_GPIO28_SLEWFAST(0);

    io_bank0 -> gpio28_ctrl = 
	IO_BANK0_GPIO28_CTRL_IRQOVER(0) |
	IO_BANK0_GPIO28_CTRL_INOVER(0)  |
	IO_BANK0_GPIO28_CTRL_OEOVER(0)  |
	IO_BANK0_GPIO28_CTRL_OUTOVER(0) |
	IO_BANK0_GPIO28_CTRL_FUNCSEL(5);

    sio->gpio_oe_set = (1<<28);
}
void turn_on_led0()
{
    sio -> gpio_out_clr = (1<<28);
}
void turn_off_led0()
{
    sio -> gpio_out_set = (1<<28);
}

