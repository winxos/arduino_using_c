#ifndef _VM_PUBLIC
#define _VM_PUBLIC

#include <stdio.h>
#include <avr/io.h>

#define F_CPU 16000000
#define BAUD 250000

#include <stdlib.h>
#include <string.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <assert.h>

#define VERSION "0.2"
#define BAUD 250000
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned char bool;

#define false (0)
#define true (!false)

#include "string_table.h"
#define GFS get_flash_str
extern char STR_MEM_BUF[80];
extern char *get_flash_str(const char *str);

/* public.c     */
extern int serial_putc( char c, struct __file * );
extern int split(char dst[][10], char *str, const char *spl);
extern int printf(const char *fmt, ...);
/*	vm_manager.c */
extern void vm_init(void);
extern void vm_run_command(void);

/* protocol.c   */
#define BUF_MAX_LEN 80

typedef struct FRAME {
	u8 len;
	u8 data[BUF_MAX_LEN];
}FRAME;
extern void receive_data(u8 data);
extern bool frame_ready;
extern FRAME frame_buf;
extern FRAME* get_frame(void);

/* shell.c    */
extern void shell(void);

/* vm.c       */
extern void dump(void);
extern void init_SML(void);
extern bool step_SML(void);
extern bool input_single_code(int code);
extern void init_shell(void);
extern unsigned long total_instructions;
extern bool RUN_VM;
extern void run_code(u16 *fbuf, u8 sz);
/* fs.c       */
enum RETURN_CODE {
	FILE_SUCCESS, FILE_NOT_EXIST = -1, FILE_ALREADY_EXIST = -2, FILE_REACH_MAX = -3, DISK_NOT_ENOUGH = -4, DISK_FULL = -5,
};
enum FILE_STATE {
	NORMAL = 0x01, READ_ONLY = 0x02, HIDE = 0x04, OPENING = 0x08, DELETED = 0x10
};
#define MAX_NAME_LEN 8
typedef struct FILE_INFO{
	char name[MAX_NAME_LEN];
	u16 start_address;
	u16 size;
	u8 state; //normal, read only, hide, opening delete?
	u8 xor_sum; //
}FILE_INFO;
extern int init_file_system(void);
extern int get_file(const char name[], struct FILE_INFO *fout);
extern int read_file(struct FILE_INFO FI, u8 buf[]);
extern int write_file(const char name[], u8 buf[], u8 len);
extern int delete_file(const char name[]);
extern void test_fs(void);
extern int get_file_by_index(u8 index,struct FILE_INFO *out);
extern u8 max_file_number;

/* port*/
extern bool digitalRead(u8 p);
#endif
