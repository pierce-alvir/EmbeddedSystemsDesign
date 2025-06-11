/************************************************************************

Author - Pierce Alvir

Description - 
	Driver file for the active-low pushbutton connected to gpio 26. 
	
*************************************************************************/

#include "pb0.h"
#include <rp2040/resets.h>
#include <rp2040/sio.h>
#include <rp2040/io_bank0.h>
#include <rp2040/pads_bank0.h>
#include <stdbool.h>

void configure_pb0( void )
{
    resets -> clr_reset = 
	    RESETS_RESET_IO_BANK0_MASK
	    | RESETS_RESET_PADS_BANK0_MASK;

    while(! (resets -> reset_done & RESETS_RESET_IO_BANK0_MASK))
    	continue;

    while(! (resets -> reset_done & RESETS_RESET_PADS_BANK0_MASK))
    	continue;

    pads_bank0 -> gpio26 = 
	PADS_BANK0_GPIO12_OD(0) 
	| PADS_BANK0_GPIO12_IE(1) 
	| PADS_BANK0_GPIO12_DRIVE(0) 
	| PADS_BANK0_GPIO12_PUE(1) 
	| PADS_BANK0_GPIO12_PDE(0) 
	| PADS_BANK0_GPIO12_SCHMITT(0) 
	| PADS_BANK0_GPIO12_SLEWFAST(0);

    io_bank0 -> gpio26_ctrl = 
	IO_BANK0_GPIO12_CTRL_IRQOVER(0) |
	IO_BANK0_GPIO12_CTRL_INOVER(0)  |
	IO_BANK0_GPIO12_CTRL_OEOVER(0)  |
	IO_BANK0_GPIO12_CTRL_OUTOVER(0) |
	IO_BANK0_GPIO12_CTRL_FUNCSEL(5);


}

_Bool pb0_is_pressed()
{
	if ( (sio->gpio_in) & (1<<12))
		return 0;
	else
		return 1;
}
