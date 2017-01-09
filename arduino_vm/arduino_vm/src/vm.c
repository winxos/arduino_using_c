/*
  SIMPLE MACHINE LANGUAGE v5
  This version added timer
  winxos 2016-11-14
*/
#include "public.h"

#define MAX_MEM 100
int MEM[MAX_MEM] = {0};
long ADDER = 0; //ONLY MAX_MEN MEMORY, ONE REGISTER ADDER
u8 rega, regb, regc, regd;
#define MAX_STACK 30
int STACK[MAX_STACK] = {0};
bool RUN_VM = false;

int pcode, pstack;
unsigned long total_instructions;
long slp_ct = 0;

enum
{
  _INPUT = 0x10, PRINT,
  LOAD = 0x20, STORE, SET,
  ADD = 0x30, SUB, MUL, DIV, MOD, INC, _DEC, NEG,
  JMP = 0x40, JMPN, JMPZ, HALT,
  AND = 0x50, OR, XOR,
  PUSH = 0x60, POP, SREGA, SREGB, SREGC, SREGD,
  PMOD = 0x70, DWP, DRP, AWP, ARP,
  SLP = 0x80,
};

#define COLS 10
#define TITLE_FORMAT "%6X"
#define DATA_FORMAT "  %04x"
#define COL_HEADER_FORMAT "\n%3d"

void dump() //Pretty Show Memory For Debugging
{
  int i;
  //printf(GFS(MSG_VM_DUMP_HEADER_FMT), millis(), total_instructions);
  printf(GFS(MSG_VM_DUMP_HEADER2_FMT), ADDER, "");
  for (i = 0; i < COLS; i++)
  {
    printf(TITLE_FORMAT, i);
  }
  for (i = 0; i < MAX_MEM; i++)
  {
    if (i % COLS == 0)
    {
      printf(COL_HEADER_FORMAT, i / COLS);
    }
    printf(DATA_FORMAT, MEM[i]);
  }
  puts("\n");
}


void init_SML()
{
  pcode = 0, pstack = 0, total_instructions = 0;
  memset(MEM, 0, sizeof(int) * MAX_MEM);
}
bool START_CLOCK = false;
bool step_SML() //
{
  total_instructions++;
  u8 op = MEM[pcode] >> 8;  //xxbb
  u8 operand = MEM[pcode] & 0x00FF;
  switch (op)
  {
    case JMP: pcode = operand - 1;
      break;
    case LOAD: ADDER = MEM[operand];
      break;
    case STORE: MEM[operand] = ADDER;
      break;
    case SET: ADDER = operand;
      break;
    case JMPN: ADDER < 0 ? pcode = operand - 1 : 0;
      break;
    case JMPZ: ADDER == 0 ? pcode = operand - 1 : 0;
      break;
    case ADD: ADDER += MEM[operand];
      break;
    case SUB: ADDER -= MEM[operand];
      break;
    case MUL: ADDER *= MEM[operand];
      break;
    case DIV: ADDER /= MEM[operand];
      break;
    case MOD: ADDER %= MEM[operand];
      break;
    case SREGA: rega = ADDER;
      break;
    case SREGB: regb = ADDER;
      break;
    case SREGC: regc = ADDER;
      break;
    case SREGD: regd = ADDER;
      break;
    case PMOD:
      //pinMode(operand, ADDER);
      break;
    case DWP:
      //digitalWrite(operand, ADDER);
      break;
    case DRP:
      //ADDER = digitalRead(operand);
      break;
    case AWP:
     // analogWrite(operand, ADDER);
      break;
    case ARP:
      //ADDER = analogRead(operand);
      break;
    case SLP://sleep
      if (!START_CLOCK)
      {
        START_CLOCK = true;
        //slp_ct = millis();
        total_instructions--; //not finished , so can't caculate the count.
        return true;//
      }
      else
      {
        //if (millis() - slp_ct < operand * 10)
        {
          total_instructions--; //not finished , so can't caculate the count.
          return true;
        }
        //else //sleep finished
        {
          START_CLOCK = false;
        }
      }
      break;
    case INC: MEM[operand]++;
      break;
    case _DEC: MEM[operand]--;
      break;
    case NEG: MEM[operand] = -MEM[operand];
      break;
    case AND: ADDER = ADDER & MEM[operand];
      break;
    case OR: ADDER = ADDER | MEM[operand];
      break;
    case XOR: ADDER = ADDER ^ MEM[operand];
      break;
    case PUSH: pstack < MAX_STACK ? STACK[pstack++] = ADDER : puts(GFS(MSG_VM_WARNING_STACK_OVERFLOW));
      break;
    case POP: pstack > 0 ? ADDER = STACK[--pstack] : puts(GFS(MSG_VM_WARNING_STACK_EMPTY));
      break;
    case HALT: return false;
      break;
    case _INPUT:
      break;
    case PRINT: printf("%d ", MEM[operand]);
      break;
    default: break;
  }
  if (++pcode >= MAX_MEM)
    return false;
  return true;
}
void run_code(u16 *fbuf, u8 sz)
{
  init_SML();
  memcpy(MEM, fbuf, sz * sizeof(u16));
  //  puts("run code:");
  //  for (int i = 0; i < sz; i++)
  //  {
  //    printf("%2d:%04x\n", i, MEM[i]);
  //  }
  //  puts("");
  RUN_VM = true;
}
int li = 0;
bool input_single_code(int code)
{
  MEM[li++] = code;
  printf("%04x\n%02d ? ", code, li);
  return true;
}
