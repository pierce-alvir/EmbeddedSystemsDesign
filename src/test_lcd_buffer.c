#include "systick.h"
#include "lcd.h"
#include "led.h"
#include "watchdog.h"
#include "interrupt.h"
#include <stdbool.h>
#include <stdint.h>

int main()
{
	uint16_t cnt=0;
	disable_irq();

	systick_register_callback(lcd_fsm);

	configure_led();
	configure_systick();
	configure_watchdog(1500);
	configure_lcd();
	
	enable_irq();

	lcd_putchar('H');
	lcd_putchar('e');
	lcd_putchar('l');
	lcd_putchar('l');
	lcd_putchar('o');
	lcd_command(0x02);

	while(1)
	{
		if ( !systick_has_fired() )
			continue;
		if ( ++cnt == 500)
		{
			toggle_led();
			cnt=0;
		}
	
		feed_the_watchdog();
	}
	return 0;
}
