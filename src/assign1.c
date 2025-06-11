/************************************************************************

Author - Pierce Alvir

Description - 
	Main program for led and push button driver that will turn on the led when the push 	button is pressed and turn off the led otherwise.
	
*************************************************************************/

#include "led0.h"
#include "pb0.h"

int main()
{
	configure_led0();
	configure_pb0();
	while(1)
	{
		if (read_pb0())
			turn_off_led0();
		else
			turn_on_led0();	
	}
}

