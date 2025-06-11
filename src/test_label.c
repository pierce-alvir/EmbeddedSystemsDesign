#include "lcd.h"
#include "systick.h"
#include "watchdog.h"
#include "interrupt.h"


#define LCD_ROW0_COL0 0x80
#define LCD_ROW1_COL0 0xA8
#define LCD_ROW1_COL6 0xAE

void lcd_label();

int main()
{
	disable_irq();

	systick_register_callback(lcd_fsm);

	configure_lcd();
	configure_systick();
	configure_watchdog(1500);
	lcd_label();

	enable_irq();

	while(1)
	{
		if (! systick_has_fired())
			continue;
		feed_the_watchdog();	
	}
}

void lcd_label()
{
	lcd_command(LCD_ROW0_COL0);
	lcd_putchar('J');
	lcd_putchar('U');
	lcd_putchar('M');
	lcd_putchar('P');
	lcd_command(LCD_ROW1_COL0);
	lcd_putchar('C');
	lcd_putchar('O');
	lcd_putchar('U');
	lcd_putchar('N');
	lcd_putchar('T');
	lcd_putchar(':');
	lcd_command(LCD_ROW1_COL6);
	lcd_putchar('1');
}
