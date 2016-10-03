#include <stdio.h>
#include <string.h>
#include "mipssim.h"
#include "jformat.h"

extern Instruction_Table_Entry instr_tab[MAX_INSTR];

int default_handle_jtype (FILE *fp, const unsigned int instruction, unsigned int pc) {
        char bcd_addr[13]="";
	JFormat_Instr instr;

	instr.jump_addr = (instruction & 0x03FFFFFF);
	instr.opcode = ((instruction >> 26) & 0x0000003F);
	
        char *str = instr_tab[((instr.opcode)&0xFF)].instr_str;

        fwrite(str, 1, strlen(str), fp);
        if(instr.opcode == 2)
                sprintf(bcd_addr, " #%u", ((pc & 0xF0000000) | (((int)(instr.jump_addr)) << 2)) & 0x0FFFFFFF);
        else
                printf("Unhandled J-Format Opcode - %s - %u\n", str, instr.opcode);


        fwrite(bcd_addr, 1, strlen(bcd_addr), fp);

        return 0;
}
