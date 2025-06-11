#include "systick.h"
#include "led.h"
#include <stdint.h>
#include "interrupt.h"

void foo();

int main (void)
{
	disable_irq();
	
	systick_register_callback(foo);

	configure_systick();
	configure_led();
	enable_irq();
	while(1)
	{
		asm("WFI");
		if ( !systick_has_fired() )
			continue;
	}
	return(0);
}

void foo()
{
	static uint16_t cnt=0;
	if ( ++cnt==500)
	{
		toggle_led();
		cnt=0;
	}
}
