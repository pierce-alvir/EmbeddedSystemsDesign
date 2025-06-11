/************************************************************************

Author - Pierce Alvir

Description - 
	Source File implementation that uses I2C driver to interact with the accelerometer through the serial terminal using the usbcdc driver. The file implements a systick callback to interrupt the program for the fsm that prints the unit acceleration vector as a fixed point decimal number when x,y, or z keys are pressed. 

*************************************************************************/
#include "systick.h"
#include "watchdog.h"
#include "interrupt.h"
#include "usbcdc.h"
#include "uclib.h"
#include "spi0.h"
#include "pin_irq_26.h"
#include <stdbool.h>
#include <rp2040/spi.h>
#include <stdint.h>
#include <stdbool.h>

#define SPI_X_DATA_LSB 0xA800
#define SPI_X_DATA_MSB 0xA900
#define SPI_Y_DATA_LSB 0xAA00
#define SPI_Y_DATA_MSB 0xAB00
#define SPI_Z_DATA_LSB 0xAC00
#define SPI_Z_DATA_MSB 0xAD00
#define FORMAT_LEN 5

void xyz_fsm();
char print_decimal_format[5] = "0.000";

int main()
{
	disable_irq();

	systick_register_callback(xyz_fsm);
	
	configure_spi0();
	configure_usbcdc();
	configure_systick();
	configure_watchdog(1500);
	configure_pin_irq_26();

	enable_irq();
	while(1)
	{
		asm("WFI");
		if (!systick_has_fired())
			continue;
		feed_the_watchdog();
	}
}

void  xyz_fsm()
{
	static enum {CONFIG_WRITE, CONFIG_READ, READ_PRESS, PRINT_RESULT} state=CONFIG_WRITE;

	uint16_t data;
	uint8_t temp;
	char c;
	char c_hold;
	int16_t neg_check;
	uint16_t value;

	switch( state )
	{
		case CONFIG_WRITE:
			spi0_tx(0x2017);
			state=CONFIG_READ;
			break;
		case CONFIG_READ:
			spi0_rx();
			state=READ_PRESS;
			break;
		case READ_PRESS:
			//if (usbcdc_getchar(&c) )
			if ( !get_pin26() )
			{
				/*
				switch(c)
				{
					case 'x':
						spi0_tx(SPI_X_DATA_LSB);
						spi0_tx(SPI_X_DATA_MSB);
						state=PRINT_RESULT;
						break;
					case 'y':
						spi0_tx(SPI_Y_DATA_LSB);
						spi0_tx(SPI_Y_DATA_MSB);
						state=PRINT_RESULT;
						break;
					case 'z': */
						spi0_tx(SPI_Z_DATA_LSB);
						spi0_tx(SPI_Z_DATA_MSB);
						state=PRINT_RESULT;
						//break;
				//}
			}
			break;
		case PRINT_RESULT:
			temp = spi0_rx();
			data = spi0_rx();
			data = ((data<<8)|temp);
			print_hex(4,data);
			print_string("\n\r");
			/*
			neg_check = (int16_t)data;
			if( neg_check < 0)
			{
			 	neg_check = neg_check * (-1);
				usbcdc_putchar('-');
			}	
			else
				usbcdc_putchar('+');
			value = div10((( neg_check *10000+5) >> 14));
			for(int8_t i=FORMAT_LEN-1;i>=0;i--)
			{
				if (print_decimal_format[i]=='.')
					continue;
				c_hold = (mod10(value)) + '0';
				print_decimal_format[i]=c_hold;
				value = div10(value);
			}
			print_string(print_decimal_format);
			print_string("g\n\r");
			*/
			state=READ_PRESS;
			break;
	}
}
