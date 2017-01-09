#ifndef _VM_STR_TABLE
#define _VM_STR_TABLE
#define MSG_COMMAND_NOT_EXIST_FMT PSTR("'%s' NOT EXIST, TYPE help FOR SUPPORTED COMMANDS\n")
#define MSG_NOT_FINISHED PSTR("NOT FINISHED")
#define MSG_RM_RETURN_CODE_FMT PSTR("rm '%s' RETURN CODE %d\n")
#define MSG_VM_STOPPED PSTR("VM STOPPD")
#define MSG_VM_START PSTR("VM START")
#define MSG_LS_HEADER PSTR("    NAME\t\tSIZE")
#define MSG_LS_ITEMS_FMT PSTR("%8s\t\t%d\n")
#define MSG_LS_SUMMARY_FMT PSTR("\t\t%d FILES\t\t%d BYTES\n")

#define MSG_INPUT_HINT PSTR("\nEnter -1 to end input.")
#define MSG_HELP_HEADER_FMT PSTR("COMMANDS COUNT (%d)\n")
#define MSG_HELP_ITMES_FMT PSTR("%s\t\t\t%s\n")

#define MSG_VM_DUMP_HEADER_FMT PSTR("\nSystem started %ld ms ago, runned %ld Instructions\n")
#define MSG_VM_DUMP_HEADER2_FMT PSTR("ADDER:%4d, MEMORY:\n%3s")
#define MSG_VM_HELP_HEADER_FMT PSTR("THIS MACHINE HAVE %d(int)MEMORY, %d(int)STACK\n")
#define MSG_VM_HELP_CONTENT PSTR("INSTRUCTION IS 4 DECIMAL DIGITS, FORMAT: AABB")
#define MSG_VM_WARNING_STACK_OVERFLOW PSTR("[warning] STACK OVERFLOW.")
#define MSG_VM_WARNING_STACK_EMPTY PSTR("[warning] STACK EMPTY.")

#define MSG_VM_MANAGER_FINISHED_SUMMARY_FMT PSTR("\nTotal run %ld instructions. used %ld ms.\n")
#define MSG_VM_MANAGER_DEFAULT_WRITE_BLINK PSTR("default write blink code")
#define MSG_VM_MANAGER_LOADED_CODE_FMT PSTR("default running code '%s' size:%d\n")
#define MSG_VM_MANAGER_WELCOME_FMT PSTR("AISTLAB VM v%s\nBAUD:%ld\n")
#endif

