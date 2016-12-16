/*
 * uart.h
 *
 * Created: 2016-12-6 22:21:49
 *  Author: winxos
 */ 


#ifndef UART_H_
#define UART_H_

#include "ring_buffer.h"
#include <util/setbaud.h>
#include <avr/interrupt.h>

extern struct ring_buffer ring_buffer_out;
extern struct ring_buffer ring_buffer_in;

extern void uart_init(void);
extern void uart_putchar(uint8_t data);
extern void print_str(const char *data);
extern void print_number(int32_t n);
extern uint8_t uart_getchar(void);
extern bool uart_char_waiting(void);
#endif /* UART_H_ */