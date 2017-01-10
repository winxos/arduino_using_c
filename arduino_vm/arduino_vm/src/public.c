#include "public.h"
volatile uint32_t _MILLIS=0;
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

static int width=0;
static char state=0;
static va_list ap;
#define IS_FMT_BASE(X) (X=='x' || X=='d' ||X=='s' ||X=='c')
#define IS_DIGITS(X) (X>='0' && X<='9')

static void print_str(char* str)
{
	while(*str)
	{
		_putchar(*str++);
	}
}

inline void _print_nchar(char ch,int n)
{
	for(int i=0;i<n;i++)
	{
		_putchar(ch);
	}
}
static void _deal_fmt(const char* fmt)
{
	long d;
	char buf[16];
	switch (*fmt) //%  next
	{
		case 's':
		print_str(va_arg(ap,char*));
		break;
		case 'x':
		d = va_arg(ap,long);     //
		ltoa(d,buf,16);         //
		_print_nchar(' ',width-strlen(buf));
		print_str(buf);
		break;
		case 'd':
		d = va_arg(ap,long);
		ltoa(d,buf,10);         //
		_print_nchar(' ',width-strlen(buf));
		print_str(buf);
		break;
		case 'c':
		_putchar(va_arg(ap,int));
		break;
	}
}
enum FMT_STATE{
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
				_putchar(*fmt);
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
u8 _state=0b00100000;
ISR(TIMER0_OVF_vect)
{
	_MILLIS++;
	// 	if (_MILLIS%100==0)
	// 	{
	// 		_state=~_state;
	// 		PORTB=_state;
	// 	}
	_state=~_state;
	PORTB=_state;
}
void timer0_init(void)
{
	cli();
	TCCR0B =0x3;
	TCNT0 = 256-250; //1ms
	TIMSK0 |= (1 << TOIE0);
	sei();
}
void usart_init(void)
{
	// Set baud rate
	UBRR0H = (uint8_t)(UBRR_VALUE>>8);
	UBRR0L = (uint8_t)UBRR_VALUE;
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	//enable transmission and reception
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}
void pinMode(u8 p,u8 v)
{
	
}
void digitalWrite(u8 p,u8 v)
{
	
}
u8 digitalRead(u8 p)
{
	return 0;
}
u16 analogRead(u8 p)
{
	return 0;
}
void analogWrite(u8 p,u8 v)
{
	
}
uint32_t millis(void)
{
	return _MILLIS;
}