#ifndef SPI0_H
#define SPI0_H

#include <stdint.h>

void configure_spi0();
uint16_t spi0_rx();
void spi0_tx(uint16_t data);

#endif
