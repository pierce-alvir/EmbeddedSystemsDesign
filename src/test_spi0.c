#include "usbcdc.h"
#include "uclib.h"
#include "spi0.h"
#include "rp2040/spi.h"

int main()
{
	char c;
	configure_spi0();
	configure_usbcdc();
	while(1)
	{
		if (usbcdc_getchar(&c))
		{
			switch(c)
			{
				case 'w':
					spi0_tx(0x8F00);
					break;
				case 'r':
					usbcdc_putchar(':');
					print_hex(2,spi0_rx());
					break;
			}
		}
	}
	return 0;
}
