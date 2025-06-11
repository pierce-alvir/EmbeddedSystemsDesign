/************************************************************************

Author - Pierce Alvir

Description - 
	Interface file for Systick Functions, which includes the configuration for the systick and the systick has fired function sets a flag once every 1 ms. 	
	
*************************************************************************/

#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdbool.h>

void configure_systick();
_Bool systick_has_fired();
_Bool systick_register_callback( void (*p_fn)() );

#endif
