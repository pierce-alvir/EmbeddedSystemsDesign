/************************************************************************

Author - Pierce Alvir

Description - 
	Source File implementation that uses keypad driver to print the character of the keypad button pressed by utilizing a lookup table with all the keypad button options and a FSM to determine the selected keypad input. Also includes the usbcdc driver to print to terminal, systick driver to align the debounce count to 10 ms, and watchdog driver for a 1.5ms timeout.
	
*************************************************************************/
#include "keypad.h"
#include "usbcdc.h"
#include "watchdog.h"
#include "systick.h"
#include <stdint.h>
#include <stdbool.h>

_Bool keypad_fsm(uint8_t * keypad_return);
char keypad_char[16] = {'1', '4', '7', '*', '2', '5', '8', '0', '3', '6', '9', '#'};

int main()
{
	uint8_t keypad_lookup;
	configure_keypad();
	configure_usbcdc();
	configure_watchdog(1500);
	configure_systick();

	while(1)
	{
		if (! systick_has_fired())
			continue;
		if (keypad_fsm(&keypad_lookup))
			usbcdc_putchar(keypad_char[keypad_lookup]);
		feed_the_watchdog();
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
