/*
* vm.c
*
* Created: 2016-12-7 9:54:41
*  Author: winxos
*/
#include "public.h"

#define MAX_MEM 500
uint16_t MEM[MAX_MEM] = {0};
int32_t ADDER = 0; //ONLY MAX_MEN MEMORY, ONE REGISTER ADDER
#define MAX_STACK 300
uint8_t STACK[MAX_STACK] = {0};

enum
{
	_INPUT = 10, PRINT, LOAD = 20, STORE, SET,
	ADD = 30, SUB, MUL, DIV, MOD, INC, DEC, NEG,
	JMP = 40, JMPN, JMPZ, HALT,
	AND = 50, OR, XOR,
	PUSH = 60, POP
};

#define OPERAND_BIT 10

#define COLS 10
#define TITLE_FORMAT "%6X"
#define DATA_FORMAT "%6d"
#define COL_HEADER_FORMAT "\n%3d"

void dump() //Pretty Show Memory For Debugging
{
	//int i;
	//printf("\nADDER:%4d, MEMORY:\n%3s", ADDER, "");
	//for (i = 0; i < COLS; i++)
	//{
	//printf(TITLE_FORMAT, i);
	//}
	//for (i = 0; i < MAX_MEM; i++)
	//{
	//if (i % COLS == 0)
	//{
	//printf(COL_HEADER_FORMAT, i / COLS);
	//}
	//printf(DATA_FORMAT, MEM[i]);
	//}
	//puts("\n");
}

uint16_t op,operand, pcode, pstack;
uint32_t total_instructions;

void init_SML()
{
	operand = 0, pcode = 0, pstack = 0, total_instructions = 0;
	memset(MEM, 0, sizeof(int) * MAX_MEM);
}

static int step_SML() //
{
	if (pcode < MAX_MEM)
	{
		op = MEM[pcode]>>OPERAND_BIT;
		operand =MEM[pcode] & ((2<<OPERAND_BIT)-1) ;
		switch (op)
		{
			case PRINT:printf("%d ", MEM[operand]);
			break;
			case LOAD:ADDER = MEM[operand];
			break;
			case STORE:MEM[operand] = ADDER;
			break;
			case SET:ADDER = operand;
			break;
			case ADD:ADDER += MEM[operand];
			break;
			case SUB:ADDER -= MEM[operand];
			break;
			case MUL:ADDER *= MEM[operand];
			break;
			case DIV:ADDER /= MEM[operand];
			break;
			case MOD:ADDER %= MEM[operand];
			break;
			case INC:MEM[operand]++;
			break;
			case DEC:MEM[operand]--;
			break;
			case NEG:MEM[operand] = -MEM[operand];
			break;
			case JMP:pcode = operand - 1;
			break;
			case JMPN:ADDER < 0 ? pcode = operand - 1 : 0;
			break;
			case JMPZ:ADDER == 0 ? pcode = operand - 1 : 0;
			break;
			case AND:ADDER = ADDER & MEM[operand];
			break;
			case OR:ADDER = ADDER | MEM[operand];
			break;
			case XOR:ADDER = ADDER ^ MEM[operand];
			break;
			case PUSH:pstack < MAX_STACK ? STACK[pstack++] = ADDER : puts("[warning] STACK OVERFLOW.");
			break;
			case POP:pstack > 0 ? ADDER = STACK[--pstack] : puts("[warning] STACK EMPTY.");
			break;
			case HALT: pcode = MAX_MEM;
			break;
			default:break;
		}
		pcode++;
	}
	else
	{
		puts("HALT.");
		return 1;
	}
	return 0;
}
void runSML()
{
	while (1)
	{
		total_instructions++;
		if (step_SML())
		break;
	}
}