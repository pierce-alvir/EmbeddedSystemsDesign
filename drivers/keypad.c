#include "keypad.h"
#include <stdint.h>
#include <stdbool.h>
#include <rp2040/resets.h>
#include <rp2040/sio.h>
#include <rp2040/io_bank0.h>
#include <rp2040/pads_bank0.h>

void configure_keypad()
{
	resets -> clr_reset = 
		RESETS_RESET_IO_BANK0_MASK
		| RESETS_RESET_PADS_BANK0_MASK;

	while(! (resets -> reset_done & RESETS_RESET_IO_BANK0_MASK))
		continue;

	while(! (resets -> reset_done & RESETS_RESET_PADS_BANK0_MASK))
		continue;

	uint32_t const out_pads_bank0_gpio = 
		PADS_BANK0_GPIO25_OD(0)
		| PADS_BANK0_GPIO25_IE(0)
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
		IO_BANK0_GPIO25_CTRL_FUNCSEL(5);

	pads_bank0 -> gpio15 = out_pads_bank0_gpio;
	pads_bank0 -> gpio16 = out_pads_bank0_gpio;
	pads_bank0 -> gpio17 = out_pads_bank0_gpio;
	io_bank0 -> gpio15_ctrl = out_io_bank0_gpio_ctrl;
	io_bank0 -> gpio16_ctrl = out_io_bank0_gpio_ctrl;
	io_bank0 -> gpio17_ctrl = out_io_bank0_gpio_ctrl;

	uint32_t const in_pads_bank0_gpio = 
		PADS_BANK0_GPIO25_OD(0)
		| PADS_BANK0_GPIO25_IE(1)
		| PADS_BANK0_GPIO25_DRIVE(0)
		| PADS_BANK0_GPIO25_PUE(1)
		| PADS_BANK0_GPIO25_PDE(0)
		| PADS_BANK0_GPIO25_SCHMITT(0)
		| PADS_BANK0_GPIO25_SLEWFAST(0);

	uint32_t const in_io_bank0_gpio_ctrl = 
		IO_BANK0_GPIO25_CTRL_IRQOVER(0) |
		IO_BANK0_GPIO25_CTRL_INOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OEOVER(0)  |
		IO_BANK0_GPIO25_CTRL_OUTOVER(0) |
		IO_BANK0_GPIO25_CTRL_FUNCSEL(5);

	pads_bank0 -> gpio18 = in_pads_bank0_gpio;
	pads_bank0 -> gpio19 = in_pads_bank0_gpio;
	pads_bank0 -> gpio20 = in_pads_bank0_gpio;
	pads_bank0 -> gpio21 = in_pads_bank0_gpio;
	io_bank0 -> gpio18_ctrl = in_io_bank0_gpio_ctrl;
	io_bank0 -> gpio19_ctrl = in_io_bank0_gpio_ctrl;
	io_bank0 -> gpio20_ctrl = in_io_bank0_gpio_ctrl;
	io_bank0 -> gpio21_ctrl = in_io_bank0_gpio_ctrl;

	keypad_scan_column(ALL_KEYPAD_COLUMNS);
}

void keypad_scan_column(uint8_t col)
{
	sio -> gpio_oe_clr = (1<<15)|(1<<16)|(1<<17);
	sio -> gpio_out_clr = (1<<15)|(1<<16)|(1<<17);
	switch(col)
	{
		case 0:
			sio -> gpio_oe_set = (1<<15);
			break;
		case 1:
			sio -> gpio_oe_set = (1<<16);
			break;
		case 2:
			sio -> gpio_oe_set = (1<<17);
			break;
		default:
			sio -> gpio_oe_set = (1<<15)|(1<<16)|(1<<17);
			break;
	}
}

uint8_t keypad_get_row()
{
	uint32_t row = sio->gpio_in & ((1<<18)|(1<<19)|(1<<20)|(1<<21));
	switch(row)
	{
		case 0xE<<18:
			return 0;
		case 0xD<<18:
			return 1;
		case 0xB<<18:
			return 2;
		case 0x7<<18:
			return 3;
		default:
			return 0xf;
	}
}

_Bool keypad_fsm(uint8_t * keypad_return)
{
	static enum {WAIT_PRESS, DEBOUNCE, CHECK_C0, CHECK_C1, CHECK_C2, RELEASE} state = WAIT_PRESS;
	static uint8_t cnt;
	_Bool retval = false;
	switch(state)
	{
		case WAIT_PRESS:
			if (keypad_get_row() == 0xf)
				state = WAIT_PRESS;
			else
			{
				cnt = 0;
				state = DEBOUNCE;
			}
			break;
		case DEBOUNCE:
			if (keypad_get_row() != 0xf && cnt >= 10)
			{
				keypad_scan_column(0);
				state = CHECK_C0;
			}
			else if (keypad_get_row() != 0xf && cnt < 10)
			{
				cnt++;
				state = DEBOUNCE;
			}
			else if (keypad_get_row() == 0xf)
				state = WAIT_PRESS;
			else
				state = DEBOUNCE;
			break;
		case CHECK_C0:
			if (keypad_get_row() == 0xf)
			{
				keypad_scan_column(1);
				state = CHECK_C1;
			}
			else
			{
				*keypad_return = 4*0+keypad_get_row();
				retval = true;
				state = RELEASE;
			}
			break;
		case CHECK_C1:
			if (keypad_get_row() == 0xf)
			{
				keypad_scan_column(2);
				state = CHECK_C2;
			}
			else
			{
				*keypad_return = 4*1+keypad_get_row();
				retval = true;
				state = RELEASE;
			}
			break;
		case CHECK_C2:
			if (keypad_get_row() == 0xf)
			{
				keypad_scan_column(3);
				state = WAIT_PRESS;
			}
			else
			{
				*keypad_return = 4*2+keypad_get_row();
				retval = true;
				state = RELEASE;
			}
			break;
		case RELEASE:
			if (keypad_get_row() == 0xf)
			{
				keypad_scan_column(3);
				retval = false;
				state = WAIT_PRESS;
			}
			else
				state = RELEASE;
	}
	return retval;
}