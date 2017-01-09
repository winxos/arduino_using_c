#include "public.h"
typedef enum {SHELL, INPUT_CODE}INPUT_STATE;
INPUT_STATE input_state = SHELL;
u8 cmd_count = 0;
#define MAX_CMD_COUNT 16

static struct CMD {
	char *_name;
	const char *_help;
	void (*execute)(char args[2][10]);
} CMDS[MAX_CMD_COUNT];
char hbuf[80];

static void _help(char args[2][10])
{
	printf(GFS(MSG_HELP_HEADER_FMT), cmd_count);
	char hbuf[80], *p;
	for (int i = 0; i < cmd_count; i++)
	{
		p = GFS(CMDS[i]._help);
		memcpy(hbuf, p, strlen(p) + 1);
		printf(GFS(MSG_HELP_ITMES_FMT), CMDS[i]._name, hbuf);
	}
}
static void _input(char args[2][10])
{
	puts(GFS(MSG_INPUT_HINT));
	RUN_VM = false;
	init_SML();
	puts("00 ? ");
	input_state = INPUT_CODE;
}
static void _dump(char args[2][10])
{
	dump();
}
static void _start(char args[2][10])
{
	RUN_VM = true;
	puts(GFS(MSG_VM_START));
}
static void _stop(char args[2][10])
{
	RUN_VM = false;
	puts(GFS(MSG_VM_STOPPED));
}
static void _ls(char args[2][10])
{
	struct FILE_INFO tmp;
	puts(GFS(MSG_LS_HEADER));
	int i, total_size = 0;
	for (i = 0; i < max_file_number; i++)
	{
		if (get_file_by_index(i, &tmp) == FILE_SUCCESS)
		{
			printf(GFS(MSG_LS_ITEMS_FMT), tmp.name, tmp.size);
			total_size += tmp.size;
		}
		else
		{
			break;
		}
	}
	printf(GFS(MSG_LS_SUMMARY_FMT), i, total_size);
}
static void _cat(char args[2][10])
{
	struct FILE_INFO tmp;
	if (FILE_SUCCESS == get_file(args[0], &tmp))
	{
		u16 *sbuf = malloc(tmp.size);
		read_file(tmp, (u8*)sbuf);
		printf(GFS(PSTR("'%s' content:\n")), tmp.name);
		for (int i = 0; i < tmp.size / 2; i++)
		{
			printf("%5x", sbuf[i]);
		}
		puts("");
		free(sbuf);
	}
}
static void _rm(char args[2][10])
{
	int ret = delete_file(args[0]);
	printf(GFS(MSG_RM_RETURN_CODE_FMT), args[0], ret);
}
static void _ps(char args[2][10])
{
	puts(GFS(MSG_NOT_FINISHED));
}
static void _kill(char args[2][10])
{
	puts(GFS(MSG_NOT_FINISHED));
}
static void _load(char args[2][10])
{
	struct FILE_INFO f;
	if (FILE_SUCCESS == get_file(args[0], &f))
	{
		u8 *fbuf = malloc(f.size);
		read_file(f, fbuf);
		//printf(GFS(MSG_VM_MANAGER_LOADED_CODE_FMT), f.name, f.size);
		run_code((u16*)fbuf, f.size / sizeof(u16));
	}
}

static void _pmode(char args[2][10])
{
	u8 p = atoi(args[0]), m = atoi(args[1]);
	//pinMode(p, m);
	printf(GFS(PSTR("SET PIN %d MODE %d\n")), p, m);
}
static void _dread(char args[2][10])
{
	u8 p = atoi(args[0]);
	printf(GFS(PSTR("(PIN %d) = %d\n")), p, digitalRead(p));
}
static void _dwrite(char args[2][10])
{
	u8 p = atoi(args[0]), m = atoi(args[1]);
	//digitalWrite(p, m);
	printf(GFS(PSTR("SET (PIN %d) = %d\n")), p, m);
}
static void _aread(char args[2][10])
{
	u8 p = atoi(args[0]);
	//printf(GFS(PSTR("(PIN %d) = %d\n")), p, analogRead(p));
}
static void _awrite(char args[2][10])
{
	u8 p = atoi(args[0]), m = atoi(args[1]);
	//analogWrite(p, m);
	printf(GFS(PSTR("SET (PIN %d) = %d\n")), p, m);
}
static int cmd_index(char *s)
{
	for (int i = 0; i < cmd_count; i++)
	{
		if (strcmp(CMDS[i]._name, s) == 0)
		{
			return i;
		}
	}
	return -1;
}
void shell(void)
{
	struct FRAME *fbuf = get_frame();
	if (input_state == SHELL)
	{
		char *buf = fbuf->data;
		char dst[3][10] = {{0}, {0}, {0}};
		split(dst, (char *) buf, " ");
		int cmdi = cmd_index(dst[0]);
		if (cmdi >= 0)
		{
			CMDS[cmdi].execute(&dst[1]);
		}
		else
		{
			printf(GFS(MSG_COMMAND_NOT_EXIST_FMT), buf);
		}
	}
	else if (input_state == INPUT_CODE)
	{
		if (atoi(fbuf->data) == -1)
		{
			puts("-1");
			RUN_VM = true;
			input_state = SHELL;
		}
		else
		{
			char tmp[7] = "0x";
			memcpy(&tmp[2], fbuf->data, 4);
			tmp[6] = 0;
			if (!input_single_code(strtoul(tmp, 0, 16)))
			{
				input_state = SHELL;
			}
		}
	}
}
static void add_command(char *cname, void(*f)(char args[2][10]), const char *chelp)
{
	assert(chelp != NULL);
	CMDS[cmd_count]._name = cname;
	CMDS[cmd_count].execute = f;
	CMDS[cmd_count]._help = chelp;
	cmd_count++;
}
void init_shell()
{
	add_command("ls", _ls, PSTR("LIST FILES"));
	add_command("cat", _cat, PSTR("SHOW FILE CONTENT"));
	add_command("rm", _rm, PSTR("REMOVE FILE"));
	add_command("ps", _ps, PSTR("PROCESS STATE"));
	add_command("kill", _kill, PSTR("KILL ID"));
	add_command("dump", _dump, PSTR("SHOW MEMERY"));
	add_command("start", _start, PSTR("START VM"));
	add_command("stop", _stop, PSTR("STOP VM"));
	add_command("input", _input, PSTR("INPUT CODE MODE"));
	add_command("load", _load, PSTR("LOAD CODE"));
	add_command("pmode", _pmode, PSTR("PMOD PIN 0/1/2"));
	add_command("dread", _dread, PSTR("DIGITAL READ PIN"));
	add_command("dwrite", _dwrite, PSTR("DIGITAL WRITE PIN 0/1"));
	add_command("aread", _aread, PSTR("ANALOG READ"));
	add_command("awrite", _awrite, PSTR("ANALOG WRITE PIN 0-255"));
	add_command("help", _help, PSTR("HELP"));
}
