#include "public.h"

char STR_MEM_BUF[80] = {0};
char *get_flash_str(const char *str)
{
	strcpy_P(STR_MEM_BUF, str);
	return STR_MEM_BUF;
}

int split(char dst[][10], char *str, const char *spl)//for single line chars split to words
{
	int n = 0;
	char *result = NULL;
	result = strtok(str, spl);
	while (result != NULL)
	{
		strcpy(dst[n++], result);
		result = strtok(NULL, spl);
	}
	return n;
}
extern void put_char(char ch);

static int width=0;
static char state=0;
static va_list ap;
#define IS_FMT_BASE(X) (X=='x' || X=='d' ||X=='s' ||X=='c')
#define IS_DIGITS(X) (X>='0' && X<='9')

static void print_str(char* str)
{
	while(*str)
	{
		put_char(*str++);
	}
}
extern void put_char(char ch);
inline void _print_nchar(char ch,int n)
{
	for(int i=0;i<n;i++)
	{
		put_char(ch);
	}
}
static void _deal_fmt(const char* fmt)
{
	int d;
	char buf[16];
	switch (*fmt) //%  next
	{
		case 's':
		print_str(va_arg(ap,char*));
		break;
		case 'x':
		d = va_arg(ap,int);     //
		itoa(d,buf,16);         //
		_print_nchar(' ',width-strlen(buf));
		print_str(buf);
		break;
		case 'd':
		d = va_arg(ap,int);
		itoa(d,buf,10);         //
		_print_nchar(' ',width-strlen(buf));
		print_str(buf);
		break;
		case 'c':
		put_char(va_arg(ap,int));
		break;
	}
}
static enum FMT_STATE{
	FMT_NORMAL,FMT_BEGIN,FMT_WIDTH
};
int printf(const char* fmt, ...)
{
	va_start(ap,fmt);
	while (*fmt)
	{
		switch(state)
		{
			case FMT_NORMAL:
			if (*fmt=='%')
			{
				state=FMT_BEGIN;
			}
			else
			{
				put_char(*fmt);
			}
			break;
			case FMT_BEGIN:
			if (IS_DIGITS(*fmt))
			{
				width=*fmt-'0';
				state=FMT_WIDTH;
			}
			else if (IS_FMT_BASE(*fmt))
			{
				_deal_fmt(fmt);
				state=FMT_NORMAL;
			}
			break;
			case FMT_WIDTH:
			if (IS_DIGITS(*fmt))
			{
				width=width*10+*fmt-'0';
			}
			else if(IS_FMT_BASE(*fmt))
			{
				_deal_fmt(fmt);
				state=FMT_NORMAL;
			}
			else
			{
				state=FMT_NORMAL;
			}
			break;
		}
		fmt++;
	}
	va_end(ap);
	return 0;
}

bool digitalRead(u8 p)
{
	return true;
}