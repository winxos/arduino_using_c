/*
 * print.c
 *
 * Created: 2017-1-3 19:53:36
 *  Author: winxos
 */ 
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "print.h"

extern void put_char(char ch);

static void print_str(char* str)
{
	while(*str)
	{
		put_char(*str++);
	}
}
void print(char* fmt, ...)
{
	char* s;
	int d;
	char buf[16];
	va_list ap;
	va_start(ap,fmt);
	while (*fmt)
	{
		if (*fmt != '%')
		{
			put_char(*fmt++);
			continue;
		}
		switch (*++fmt) // next %
		{
			case 's':
			s = va_arg(ap,char*); //
			print_str(s);
			break;
			case 'x':
			d = va_arg(ap,int);     //
			itoa(d,buf,16);         //
			print_str(buf);
			break;
			case 'd':
			d = va_arg(ap,int);
			itoa(d,buf,10);         //
			print_str(buf);
			break;
			default:
			put_char(*fmt);
			break;
		}
		fmt++;
	}
	va_end(ap);
}
