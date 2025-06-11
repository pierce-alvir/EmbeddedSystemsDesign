#ifndef GPIO_IRQ_H
#define GPIO_IRQ_H

#include <stdint.h>

void register_gpio_irq_callback( uint8_t gpio, void (*pf)() );

#endif