#include "public.h"

void _putchar(char ch)
{
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = ch;
}
int main(void)
{
	/* Replace with your application code */
	
	usart_init();
	DDRB =0xff;
	vm_init();
	timer0_init();
	while (1)
	{
		vm_run_command();
		printf("%10d\n",millis());
		_delay_ms(1000);
	}
}
