/************************************************************************

Author - Pierce Alvir

Description - 
	Source File implementation that uses systick to synchronize the main loop at 1 ms and has the watchdog timeout at 1.5 ms. The main runs a LED FSM that flashes the LED at .5 Hz and goes into a blocking loop when the push button is pressed. 
	
*************************************************************************/

#include "watchdog.h"
#include "systick.h"
#include "led.h"
#include "pb0.h"
#include <stdint.h>

void led_fsm();

int main(void)
{
	configure_led();
	configure_pb0();
	configure_systick();
	configure_watchdog(1500);
	while(1)
	{
		if ( !systick_has_fired() )
			continue;
		led_fsm();
		if (pb0_is_pressed())
			while(1);
		feed_the_watchdog();
	}
	return(0);
}

void led_fsm()
{
	static enum {LED_OFF, LED_ON} state = LED_OFF;
	static uint16_t cnt;
	switch(state){
		case LED_OFF:
			turn_off_led();
			if (++cnt >= 999)
				state = LED_ON;
			else
				state = LED_OFF;
		break;
		case LED_ON:
			turn_on_led();
			if (++cnt >= 1999)
			{
				cnt = 0;
				state = LED_OFF;
			}
			else
				state = LED_ON;
		break;
	}
}
