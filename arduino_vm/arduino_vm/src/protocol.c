/*
PROTOCOL finite machine
MARK_START LEN CMD CRC

WINXOS 2014-05-05
*/
#include "public.h"
#include "uart.h"

#define RAW_INPUT false
#define ENABLE_CHECKSUM false

#define FRAME_START 0xE5
#define CHECK_SUM_OFFSET 0xD3

enum {NOT_READY, SUCCESS}RECEIVE_STATE;
enum {S_READY, S_LEN, S_DATA, S_VERIFY}STATE;

struct FRAME frame_buf;
u8 receive_state = S_READY;
extern u8 state;

//validate
static bool checksum(u8 n)
{
	u8 sum = CHECK_SUM_OFFSET;
	for (u8 i = 0; i < frame_buf.len; i++)
	{
		sum ^= frame_buf.data[i];
	}
	return (n == sum);
}

//receive the frame
void receive_data(u8 data)
{
	if(RAW_INPUT)
	{
		//uart_putchar(data);
		if (data==';')
		{
			print_str("get:");
			print_str((char*)frame_buf.data);
			shell();
			frame_buf.counter=0;	
			memset(frame_buf.data,0,BUF_MAX_LEN);
		}
		else
		{
			frame_buf.data[frame_buf.counter++] = data;
		}
	}
	else
	{
		switch (receive_state)
		{
			case S_READY:
			if (data == FRAME_START)
			receive_state = S_LEN;
			break;
			case S_LEN:
			frame_buf.len = data;
			frame_buf.counter = 0;
			receive_state = S_DATA;
			break;
			case S_DATA:
			frame_buf.data[frame_buf.counter++] = data;
			if (frame_buf.counter == frame_buf.len)
			receive_state = S_VERIFY;
			break;
			case S_VERIFY:
			if (ENABLE_CHECKSUM)
			{
				if (checksum(data) == true)
				{
					shell();
				}
			}
			else
			{
				shell();
			}
			receive_state =S_READY;
			break;
			default: break;
		}
	}

}
