/*
 * public.h
 *
 * Created: 2016-12-7 8:30:21
 *  Author: winxo
 */ 


#ifndef PUBLIC_H_
#define PUBLIC_H_

#include "asf.h"
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

typedef uint8_t u8;

/* public function	*/

extern int split(char dst[][80], char *str, const char *spl);

/* protocol.c		*/
#define BUF_MAX_LEN 0x50

struct FRAME {
	u8 ip;
	u8 len;
	u8 counter;
	u8 data[BUF_MAX_LEN];
};
extern struct FRAME frame_buf;
extern void receive_data(u8 data);

/* vm.c				*/
extern void dump(void);

/* uart.h			*/
#include "uart.h"

/* shell.c			*/
extern void shell(void);

#endif /* PUBLIC_H_ */