/************************************************************************

Author - Pierce Alvir

Description - 
	Source File implementation that prints the memory region of the hex dump function when a character is input to the USB serial stream.
	
*************************************************************************/

#include "usbcdc.h"
#include "uclib.h"
#include <stdint.h>

int main()
{
	char c;
	configure_usbcdc();
	uint32_t start_addr = 0x20000310;
        uint32_t end_addr   = 0x2000039c;
	uint8_t byte_size = end_addr - start_addr;	
	while(1)
	{
		if ( usbcdc_getchar(&c))
		{
			hex_dump(0x20000000, 0);
			hex_dump(start_addr, byte_size);
		}
	}
	return 0;
}
