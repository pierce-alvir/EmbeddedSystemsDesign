#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include <stdbool.h>

void configure_keypad();
void keypad_scan_column(uint8_t col);
uint8_t keypad_get_row();
_Bool keypad_fsm(uint8_t * keypad_return);

#define ALL_KEYPAD_COLUMNS 0xff

#endif
