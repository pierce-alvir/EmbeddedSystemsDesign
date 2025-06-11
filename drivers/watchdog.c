/************************************************************************

Author - Pierce Alvir

Description - 
	Implementation file for Watchdog Functions, which includes the configuration for the watchdog and the feeding watchdog function to reset the timeout.
	
*************************************************************************/

#include "watchdog.h"
#include <rp2040/watchdog.h>
#include <rp2040/psm.h>
#include <stdint.h>

static uint32_t reload_value;

void configure_watchdog(uint32_t reload)
{
	//watchdog->ctrl &= ~(1<<30);
	watchdog->ctrl &= WATCHDOG_CTRL_ENABLE(0);
	//psm->wdsel = 0x0000FFFC;
	psm->wdsel = (PSM_WDSEL_PROC1(0)
					|PSM_WDSEL_PROC0_MASK
					|PSM_WDSEL_SIO_MASK
					|PSM_WDSEL_VREG_AND_CHIP_RESET_MASK
					|PSM_WDSEL_XIP_MASK
					|PSM_WDSEL_SRAM5_MASK
					|PSM_WDSEL_SRAM4_MASK
					|PSM_WDSEL_SRAM3_MASK
					|PSM_WDSEL_SRAM2_MASK
					|PSM_WDSEL_SRAM1_MASK
					|PSM_WDSEL_SRAM0_MASK
					|PSM_WDSEL_ROM_MASK
					|PSM_WDSEL_BUSFABRIC_MASK
					|PSM_WDSEL_RESETS_MASK
					|PSM_WDSEL_CLOCKS_MASK
					|PSM_WDSEL_XOSC(0)
					|PSM_WDSEL_ROSC(0));
	
	//watchdog->tick |= 0xC;
	watchdog->tick |= WATCHDOG_TICK_CYCLES(0xC);	
	reload_value = reload*2;
	feed_the_watchdog();
	//watchdog->ctrl |= (1<<30);
	watchdog->ctrl |= WATCHDOG_CTRL_ENABLE(1);
}

void feed_the_watchdog()
{
	watchdog->load = reload_value;
}
