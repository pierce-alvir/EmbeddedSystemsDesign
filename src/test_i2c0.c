#include "usbcdc.h"
#include "uclib.h"
#include "i2c0.h"
#include <stdbool.h>
#include <rp2040/i2c.h>

// put on fsm 1. ask for x y or z data and then send either x y or z data back
// use minicom -D /dev/ttyACM0
int main()
{
	char c;
	configure_i2c0();
	configure_usbcdc();
	while(1)
	{
		if( usbcdc_getchar(&c))
		{
			uint16_t data;
			uint16_t trans[8];
			switch(c)
			{
			_Bool flag;
			case 'w':
				trans[0]=0x0f;
				trans[1]=I2C_READ|I2C_STOP;
				if( i2c0_do_transaction(0x19,2,trans) )
					print_string("WhoamI sent \n\r");
				else
					print_string("WhoamI not sent \n\r");
				break;
			case 's':
				trans[0]=0x27;
				trans[1]=I2C_READ|I2C_RESTART|I2C_STOP;
				if( i2c0_do_transaction(0x19,2,trans) )
					print_string("Status sent \n\r");
				else
					print_string("Status not sent \n\r");
				break;
			case 'r':
				do
				{
					if( (flag=(i2c0_get_data(&data))) )
					{
						print_string("\nRead Byte: 0x");
						print_hex(2,data);
						print_string("\n\r");
					}
					else
						print_string("No More Data\n\r");
				}while(flag);
				break;
			case 'a':
				trans[0]=0xAB; //address 8 auto inc
				trans[1]=I2C_READ|I2C_RESTART;
				trans[2]=I2C_READ;
				trans[3]=I2C_READ;
				trans[4]=I2C_READ;
				trans[5]=I2C_READ;
				trans[6]=I2C_READ|I2C_STOP;
				if (i2c0_do_transaction(0x19,7,trans) )
					print_string("Requested 6B Accel Data \n\r");
				break;
			case 'p':
				print_hex(8,i2c0->ic_raw_intr_stat);
				print_string("\n\r");
				break;
			case 'c':
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
				break;		
			}
		}
	}
}
