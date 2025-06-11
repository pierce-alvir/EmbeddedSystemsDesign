#include "usbcdc.h"
#include "uclib.h"
#include "systick.h"
#include "spi0.h"
#include "rp2040/spi.h"


int main()
{
	char c;
	configure_systick();
	configure_spi0();
	configure_usbcdc();
	while(1)
	{
		if (!systick_has_fired())
			continue;

		if( usbcdc_getchar(&c))
		{
			switch(c)
			{
			case 'w':
				spi0_tx(0x8F00);
				break;				
			case 's':
				spi0_tx(0xA700);
				break;
			case 'p':
				usbcdc_putchar(':');
				print_hex(4,spi0_rx());
				usbcdc_putchar(':');
				break;
			case 'r':
				spi0_rx();
			case 'a':
				spi0_tx(0xA800);
				spi0_tx(0xA900);
				break;

			case 'c':
				spi0_tx(0x2017);
				//spi0_tx(0x2388);
				break;		
			}
		}
	}
}
