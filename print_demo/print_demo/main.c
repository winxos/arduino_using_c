/*
 * self format print using atmel studio7
 *
 * Created: 2017-1-3 19:53:08
 * Author : winxos
 */ 

#define F_CPU 16000000
#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#include <avr/io.h>
#include <util/delay.h>
#include "print.h"

void put_char(char ch)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = ch;
}
void usart_init()
{
	// Set baud rate
	UBRR0H = (uint8_t)(UBRR_VALUE>>8);
	UBRR0L = (uint8_t)UBRR_VALUE;
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	//enable transmission and reception
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}
int main(void)
{
	/* Replace with your application code */
	usart_init();
	DDRB =0xff;
	int i=-20;
	while (1)
	{
		PORTB|=0b00100000;
		_delay_ms(500);
		PORTB&=~0b00100000;
		_delay_ms(50);
		print("%10x %30d\n",i,i++);
	}
}
