#include "systick.h"
#include "led.h"
#include <stdint.h>
#include "interrupt.h"
#include "pin_irq_26.h"

int main (void)
{
	disable_irq();
	configure_pin_irq_26();
	configure_systick();
	configure_led();
	enable_irq();

	while(1)
	{
		asm("WFI");
		if ( !systick_has_fired() )
			continue;
		if ( get_pin26() )
			turn_on_led();
		else
			turn_off_led();
	}
	return(0);
}

