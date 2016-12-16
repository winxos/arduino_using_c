/**
* arduino minimize uart framework.
*/
#include "public.h"

const char test_string[] = "Hello, AISTLAB!\n";

int main(void)
{
	uart_init();
	print_str(test_string);
	print_number(-123456789);
	while (true)
	{
		if (uart_char_waiting())
		{
			receive_data(uart_getchar());
		}
	}
	return 0;
}

