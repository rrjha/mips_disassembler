#ifndef MIPSSIM_H
#define MIPSSIM_H
#include <stdio.h>
#include "mipssim.h"

#define MAX_INSTR 64
#define BASE_ADDRESS 600
#define BLANK " "
#define NEWLINE "\n"


/* Handler format */
typedef int (*instr_handle_func) (FILE* fp, const unsigned int instruction, unsigned int pc);

/* Instruction Table entry */
typedef struct instr_tab_entry {
        char instr_str[10];
        instr_handle_func instr_handle;
} Instruction_Table_Entry;

#endif
