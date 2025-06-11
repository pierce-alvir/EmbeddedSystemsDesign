/************************************************************************

Author - Pierce Alvir

Description - 
	Source File implementation that uses keypad driver to take in keypad inputs and then utilizies the lcd driver to display the inputs as decimal in the x row and hexadecimal in the y row. The file implements a systick callback to interrupt the program for the lcd fsm and features multiple helper functions for formatting the lcd when it is either in display or entry mode in the lcd mode fsm.

*************************************************************************/
#include "keypad.h"
#include "lcd.h"
#include "watchdog.h"
#include "systick.h"
#include "interrupt.h"
#include <stdint.h>
#include <stdbool.h>

#define LCD_ROW0_COL0 0x80
#define LCD_ROW0_COL3 0x83
#define LCD_ROW1_COL0 0xA8
#define LCD_ROW1_COL3 0xAB
#define LCD_CURSOR_OFF 0x0C
#define LCD_CURSOR_ON 0x0F
#define ROW_SIZE 8

char keypad_char[16] = {'1', '4', '7', '*', '2', '5', '8', '0', '3', '6', '9', '#'};

void lcd_label_rows();
void lcd_start_up();
void lcd_xrow( uint8_t xrow_input[]);
void lcd_yrow_hex_conversion( uint8_t xrow_input[] );
void lcd_mode_fsm(uint8_t keypad_input);

int main()
{
	uint8_t keypad_return;

	disable_irq();
	
	systick_register_callback(lcd_fsm);
	
	configure_keypad();
	configure_lcd();
	configure_watchdog(1500);
	configure_systick();
	lcd_start_up();

	enable_irq();

	while(1)
	{
		if (! systick_has_fired())
			continue;
		if (keypad_fsm(&keypad_return))
			lcd_mode_fsm(keypad_return);
		feed_the_watchdog();
	}
}

void lcd_label_rows()
{
	lcd_command(LCD_ROW0_COL0);
	lcd_putchar('X');
	lcd_putchar(':');
	lcd_putchar(' ');
	lcd_command(LCD_ROW1_COL0);
	lcd_putchar('Y');
	lcd_putchar(':');
	lcd_putchar(' ');
}

void lcd_start_up()
{
	lcd_label_rows();
	lcd_command(LCD_ROW0_COL3);
	for (uint8_t i=0;i<ROW_SIZE;i++)
		lcd_putchar('0');
	lcd_command(LCD_ROW1_COL3);
	for (uint8_t i=0;i<ROW_SIZE;i++)
		lcd_putchar('0');
	lcd_command(LCD_CURSOR_OFF);
}

void lcd_xrow( uint8_t xrow_input[])
{
	lcd_command(LCD_ROW0_COL3);
	for( uint8_t i=0;i<ROW_SIZE;i++)
	{
		char number_convert = xrow_input[i] + '0';
		lcd_putchar(number_convert);
	}
}

void lcd_yrow_hex_conversion( uint8_t xrow_input[] )
{
	uint32_t value=0;
	for (uint8_t i=0;i<ROW_SIZE;i++)
	{
		value = value*10 + xrow_input[i];
	}	
	lcd_command(LCD_ROW1_COL3);
	int8_t j;
	for (j=ROW_SIZE-1; j>=0; j--) 
	{
		uint8_t hex_digit= (value>>4*j)&0xf;
		lcd_putchar(hex_digit+((hex_digit < 10)?'0':'A'-10));
	}
}


void lcd_mode_fsm(uint8_t keypad_input)
{
	static enum {DISPLAY,ENTRY} state=DISPLAY;
	static uint8_t xrow_saved_input[ROW_SIZE] = {0,0,0,0,0,0,0,0};
	static uint8_t xrow_input[ROW_SIZE] = {0,0,0,0,0,0,0,0};
	static uint8_t cnt = 0;
	switch( state )
	{
		case DISPLAY:
			if( keypad_char[keypad_input] == '*' )
			{
				state=ENTRY;
				lcd_command(LCD_CURSOR_ON);
				lcd_command(LCD_ROW0_COL3);
				for (uint8_t i=0;i<ROW_SIZE;i++)
					lcd_putchar(' ');
				lcd_command(LCD_ROW0_COL3);
			}
			break;
		case ENTRY:
			if( keypad_char[keypad_input] == '*')
			{
				state=DISPLAY;
				lcd_xrow(xrow_saved_input);
				lcd_command(LCD_CURSOR_OFF);
				for (uint8_t i=0;i<ROW_SIZE;i++)
					xrow_input[i]=0;
			}
			else if( keypad_char[keypad_input] == '#')
			{
				state = DISPLAY;
				for (uint8_t i=0;i<ROW_SIZE;i++)
					xrow_saved_input[i]=xrow_input[i];
				lcd_xrow(xrow_saved_input);
				lcd_yrow_hex_conversion(xrow_saved_input);
				lcd_command(LCD_CURSOR_OFF);
				for (uint8_t i=0;i<ROW_SIZE;i++)
					xrow_input[i]=0;
			}
			else
			{			
				xrow_input[cnt] = (uint8_t)(keypad_char[keypad_input] - '0');
				lcd_putchar(keypad_char[keypad_input]);	
				if ( ++cnt == ROW_SIZE )
				{
					lcd_command(LCD_ROW0_COL3);
					cnt=0;
				}
			}
			break;
	}
}
