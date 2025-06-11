/************************************************************************

Author - Pierce Alvir

Description - 
	Header file for an active-low pushbutton driver, which contains a configure and read 	function.
	
*************************************************************************/

#ifndef PB0_H
#define PB0_H

void configure_pb0( void );
_Bool pb0_is_pressed();

#endif
