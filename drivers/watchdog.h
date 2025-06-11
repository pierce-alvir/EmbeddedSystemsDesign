/************************************************************************

Author - Pierce Alvir

Description - 
	Interface file for Watchdog Functions, which includes the configuration for the watchdog and the feeding watchdog function to reset the timeout.	
	
*************************************************************************/

#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stdint.h>

void configure_watchdog( uint32_t reload);
void feed_the_watchdog();

#endif
