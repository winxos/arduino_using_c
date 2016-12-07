/**
* arduino minimize uart framework.
*/
#include "public.h"
#include "uart.h"

const char test_string[] = "Hello, AISTLAB!\n";

int main(void)
{
	uart_init();
	uart_send_str(test_string);
	while (true)
	{
		while (uart_char_waiting())
		{
			uart_putchar(uart_getchar());
		}
		_delay_ms(200);
	}
	return 0;
}

