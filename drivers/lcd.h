/************************************************************************

Author - Pierce Alvir

Description - 
	Interface file for LCD functions, which includes the configuration for start-up, command for sending LCD commands, putchar for displaying one character, and fsm for taking in commands or data through a FIFO buffer when displaying to the LCD for a LCD configured for a 4-Bit mode.	
	
*************************************************************************/

#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <stdbool.h>

void configure_lcd();
_Bool lcd_command( uint8_t command );
_Bool lcd_putchar ( char data );
void lcd_fsm();

#endif

