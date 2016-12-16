/*
* uart.c
*
* Created: 2016-12-7 10:22:42
*  Author: winxos
*/
#include "uart.h"

ISR(UART0_DATA_EMPTY_IRQ)
{
	// if there is data in the ring buffer, fetch it and send it
	if (!ring_buffer_is_empty(&ring_buffer_out)) {
		UDR0 = ring_buffer_get(&ring_buffer_out);
	}
	else {
		// no more data to send, turn off data ready interrupt
		UCSR0B &= ~(1 << UDRIE0);
	}
}

ISR(UART0_RX_IRQ)
{
	ring_buffer_put(&ring_buffer_in, UDR0);
}

void uart_init()
{
	cli();
	#if defined UBRR0H
	// get the values from the setbaud tool
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	#else
	#error "Device is not supported by the driver"
	#endif

	#if USE_2X
	UCSR0A |= (1 << U2X0);
	#endif

	// enable RX and TX and set interrupts on rx complete
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

	// 8-bit, 1 stop bit, no parity, asynchronous UART
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (0 << USBS0) |
	(0 << UPM01) | (0 << UPM00) | (0 << UMSEL01) |
	(0 << UMSEL00);

	// initialize the in and out buffer for the UART
	ring_buffer_out = ring_buffer_init(out_buffer, BUFFER_SIZE);
	ring_buffer_in = ring_buffer_init(in_buffer, BUFFER_SIZE);
	sei();
}


void uart_putchar(uint8_t data)
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
void print_str(const char *data)
{
	uint8_t i=0;
	while (i<strlen(data))
	{
		uart_putchar(data[i++]);
	}
	_delay_ms(1);
}
void print_number(int32_t n)
{
	char buf[12]={0},offset=0;
	if (n<0)
	{
		buf[0]='-';
		n=-n;
		offset=1;
	}
	ltoa(n,buf+offset,10);
	print_str(buf);
}
uint8_t uart_getchar(void)
{
	return ring_buffer_get(&ring_buffer_in);
}
bool uart_char_waiting(void)
{
	return !ring_buffer_is_empty(&ring_buffer_in);
}

