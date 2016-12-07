/*
* shell.c
*
* Created: 2016-12-7 9:54:53
*  Author: winxos
*/
#include "public.h"


static void help(void)
{
	uart_send_str("AISTLAB ARDUINO VM 1.0\n");
	uart_send_str("ls [list inner files]\n");
	uart_send_str("load xx[load and run inner file]\n");
	uart_send_str("code xx[run machine code xx]\n");
	uart_send_str("input [interactive mode]\n");
	uart_send_str("dump [show vm memory]\n");
}

void shell(void)
{
	u8 *buf=frame_buf.data;
	char dst[3][80] = {{0},{0},{0}};
	split(dst,(char *) buf, " ");
	uart_send_str(buf);
	if (0 == strcmp(dst[0], "ls"))
	{
		
	}
	else if (0 == strcmp(dst[0], "help"))
	{
		help();
	}
	else if (0 == strcmp(dst[0], "load"))
	{

	}
	else if (0 == strcmp(dst[0], "code"))
	{

	}
	else if (0 == strcmp(dst[0], "input"))
	{

	}
	else if (0 == strcmp(dst[0], "dump"))
	{
		dump();
	}
	else if (0 == strcmp(dst[0], "p")) //protocol
	{
	}
}