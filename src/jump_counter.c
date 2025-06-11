#include "lcd.h"
#include "spi0.h"
#include "pin26.h"
#include "systick.h"
#include "watchdog.h"
#include "interrupt.h"
#include <stdint.h>
#include <stdbool.h>

#define LCD_ROW0_COL0 0x80
#define LCD_ROW1_COL0 0xA8
#define LCD_ROW1_COL6 0xAE

#define SPI_Z_DATA_LSB 0xAC00
#define SPI_Z_DATA_MSB 0xAD00

#define JUMP_THRESHOLD 0x4A00

void lcd_label();
uint16_t div10( uint16_t x );
uint16_t mod10( uint16_t x );
void jump_fsm();

int main()
{
	disable_irq();

	systick_register_callback(jump_fsm);

	configure_spi0();
	configure_lcd();
	configure_pin26();
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
	lcd_putchar('0');
	lcd_putchar('0');
	lcd_putchar('0');
}

uint16_t div10( uint16_t x )
{
	return ( (x*0x199a) >> 16);
}
uint16_t mod10( uint16_t x )
{
	return ( x - (div10(x) * 10) );
}

void  jump_fsm()
{
	static enum {CONFIG_WRITE, CONFIG_READ, TRANSMIT_DATA, WAIT_FOR_JUMP, JUMP_COOLDOWN, BUTTON_RESET, PRINT_JUMP} state=CONFIG_WRITE;

	static uint8_t jump_cnt = 0;
	static uint16_t data;
	static _Bool jump_flag = false;
	uint8_t temp;
	uint8_t count_hold;
	uint8_t ones_place;
	uint8_t tenths_place;
	uint8_t hundreths_place;
	char c_temp;

	switch( state )
	{
		case CONFIG_WRITE:
			spi0_tx(0x2017);
			state=CONFIG_READ;
			break;
		case CONFIG_READ:
			spi0_rx();
			state=TRANSMIT_DATA;
			break;
		case TRANSMIT_DATA:
			spi0_tx(SPI_Z_DATA_LSB);
			spi0_tx(SPI_Z_DATA_MSB);
			state=WAIT_FOR_JUMP;
			break;
		case WAIT_FOR_JUMP:
			temp = spi0_rx();
			data = spi0_rx();
			data = ((data<<8)|temp);
			
			if ( !get_pin26() )
				state = BUTTON_RESET;
			else if ( data > JUMP_THRESHOLD)
			{
				jump_flag = true;
				state = JUMP_COOLDOWN;
			}
			else if ( jump_flag )
				state = JUMP_COOLDOWN;
			else
				state = TRANSMIT_DATA;
			break;
		case JUMP_COOLDOWN:
			if ( data < JUMP_THRESHOLD )
			{
				if (jump_cnt >= 100)
					jump_cnt = 100;
				else
					jump_cnt++;
				jump_flag = false;
				state = PRINT_JUMP;
			}
			else if ( data > JUMP_THRESHOLD )
				state = TRANSMIT_DATA;
			else
				state = JUMP_COOLDOWN;
			break; 
		case BUTTON_RESET:
			jump_cnt = 0;
			state = PRINT_JUMP;
			break;
		case PRINT_JUMP:
			count_hold = jump_cnt;
			ones_place = mod10(count_hold);
			count_hold = div10(count_hold);
			tenths_place = mod10(count_hold);
			count_hold = div10(count_hold);
			hundreths_place = mod10(count_hold);
			lcd_command(LCD_ROW1_COL6);
			c_temp = hundreths_place + '0';
			lcd_putchar(c_temp);
			c_temp = tenths_place + '0';
			lcd_putchar(c_temp);
			c_temp = ones_place + '0';
			lcd_putchar(c_temp);
			state = TRANSMIT_DATA;
			break;
	}
}


