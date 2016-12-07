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

extern void uart_init(void);

static inline void uart_putchar(uint8_t data)
{
	// Disable interrupts to get exclusive access to ring_buffer_out.
	cli();
	if (ring_buffer_is_empty(&ring_buffer_out)) {
		// First data in buffer, enable data ready interrupt
		UCSR0B |=  (1 << UDRIE0);
	}
	// Put data in buffer
	ring_buffer_put(&ring_buffer_out, data);

	// Re-enable interrupts
	sei();
}
static inline void uart_send_str(const char *data)
{
	uint8_t i=0;
	while (i<strlen(data))
	{
		uart_putchar(data[i++]);
	}
	_delay_ms(1);
}
static inline uint8_t uart_getchar(void)
{
	return ring_buffer_get(&ring_buffer_in);
}
static inline bool uart_char_waiting(void)
{
	return !ring_buffer_is_empty(&ring_buffer_in);
}


#endif /* UART_H_ */