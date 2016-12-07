/**
* arduino minimize uart framework.
*/
#include "public.h"

const char test_string[] = "Hello, AISTLAB!\n";

int main(void)
{
	uart_init();
	uart_send_str(test_string);
	while (true)
	{
		if (uart_char_waiting())
		{
			receive_data(uart_getchar());
		}
	}
	return 0;
}

