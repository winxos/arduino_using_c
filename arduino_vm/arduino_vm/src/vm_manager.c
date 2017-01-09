#include "public.h"
void startup_code()
{
	FILE_INFO f;
	if (FILE_SUCCESS == get_file_by_index(0, &f))
	{
		u8 *fbuf = malloc(f.size);
		read_file(f, fbuf);
		//printf(GFS(MSG_VM_MANAGER_LOADED_CODE_FMT), f.name, f.size);
		run_code((u16*)fbuf, f.size / sizeof(u16));
	}
}
void vm_init(void)
{
	//fdevopen( &serial_putc, 0 );
	//Serial.begin(BAUD);
	printf(GFS(MSG_VM_MANAGER_WELCOME_FMT), VERSION, BAUD);
	init_shell();
	init_file_system();
	int buf[13] = {0x2201, 0x6200, 0x700d, 0x2200, 0x6200, 0x710d, 0x8050, 0x2201, 0x6200, 0x710d, 0x8050, 0x4003, 0x4300};
	int c = write_file("blink", (u8*)buf, 26);
	if (c == FILE_SUCCESS)
	{
		puts(GFS(MSG_VM_MANAGER_DEFAULT_WRITE_BLINK));
	}
	startup_code();
}

unsigned long st = 0;
void vm_run_command()
{
	if (frame_ready)
	{
		FRAME *fbuf = get_frame();
		if (fbuf->data[0] == 'b' && fbuf->data[1] == ' ')
		{
			char fname[8] = {0};
			char *p = &fbuf->data[2];
			u8 i = 0;
			while (*p)
			{
				fname[i++] = *p;
				p++;
			}
			int skip = 2 + i + 1;
			int c = write_file(fname, (u8*)&fbuf->data[skip], fbuf->len - skip);
			printf(GFS(PSTR("Write '%s', ret %d\n")), fname, c);
		}
		else
		{
			shell();
		}
	}
	if (RUN_VM)
	{
		if (st == 0)
		{
			//st = millis();
		}
		if (!step_SML())
		{
			RUN_VM = false;
			//printf(GFS(MSG_VM_MANAGER_FINISHED_SUMMARY_FMT), total_instructions, millis() - st);
			st = 0;
			dump();
		}
	}
}
