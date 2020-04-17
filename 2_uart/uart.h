#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "gpio.h"

void uart_init();
void uart_send(uint8_t byte);
//char uart_read();
uint8_t uart_receive(uint8_t* p_byte);

#endif