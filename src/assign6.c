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
#include "i2c0.h"
#include <stdbool.h>
#include <rp2040/i2c.h>
#include <stdint.h>
#include <stdbool.h>

#define I2C_AUTO_INC 0x80
#define I2C_X_DATA 0x28
#define I2C_Y_DATA 0x2A
#define I2C_Z_DATA 0x2C
#define FORMAT_LEN 5

void xyz_fsm();
char print_decimal_format[5] = "0.000";

int main()
{
	disable_irq();

	systick_register_callback(xyz_fsm);
	
	configure_i2c0();
	configure_usbcdc();
	configure_systick();
	configure_watchdog(1500);
	
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
	static enum {CONFIG, READ_PRESS, PRINT_RESULT} state=CONFIG;
	uint16_t trans[8];
	uint16_t data;
	uint8_t temp;
	char c;
	char c_hold;
	int16_t neg_check;
	uint16_t value;

	switch( state )
	{
		case CONFIG:
			trans[0]=0x80|0x20;
			trans[1]=0x17;
			trans[2]=0x00;
			trans[3]=0x00;
			trans[4]=0x88;
			trans[5]=0x00;
			trans[6]=0x00|I2C_STOP;
			if (i2c0_do_transaction(0x19,7,trans) )
				print_string("Sent Config\n\r");
			else
				print_string("Config failed\n\r");
			state=READ_PRESS;
			break;
		case READ_PRESS:
			if (usbcdc_getchar(&c) )
			{
				switch(c)
				{
					case 'x':
						trans[0]=I2C_AUTO_INC|I2C_X_DATA;
						trans[1]=I2C_READ;
						trans[2]=I2C_READ|I2C_STOP;
						if (i2c0_do_transaction(0x19,3,trans) )
							print_string("Requested X Accel Data\n\r");
						state=PRINT_RESULT;
						break;
					case 'y':
						trans[0]=I2C_AUTO_INC|I2C_Y_DATA;
						trans[1]=I2C_READ;
						trans[2]=I2C_READ|I2C_STOP;
						if (i2c0_do_transaction(0x19,3,trans) )
							print_string("Requested Y Accel Data\n\r");
						state=PRINT_RESULT;
						break;
					case 'z':
						trans[0]=I2C_AUTO_INC|I2C_Z_DATA;
						trans[1]=I2C_READ;
						trans[2]=I2C_READ|I2C_STOP;
						if (i2c0_do_transaction(0x19,3,trans) )
							print_string("Requested Z Accel Data\n\r");
						state=PRINT_RESULT;
						break;
				}
			}
			break;
		case PRINT_RESULT:
			i2c0_get_data(&temp);
			i2c0_get_data(&data);
			data = ((data<<8)|temp);
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
			state=READ_PRESS;
			break;
	}
}
