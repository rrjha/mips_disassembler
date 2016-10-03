#include <stdio.h>
#include <string.h>
#include "mipssim.h"
#include "rformat.h"

extern Instruction_Table_Entry instr_tab[MAX_INSTR];


int handle_rtype_opcode0 (FILE *fp, const unsigned int instruction, unsigned int pc) {
        RFormat_Instr instr;
	char instr_str[50] = "";
	int retval = 0;

        instr.funct = (instruction & 0x0000003F);
        instr.sa = ((instruction >> 6) & 0x0000001F);
        instr.rd = ((instruction >> 11) & 0x0000001F);
        instr.rt = ((instruction >> 16) & 0x0000001F);
        instr.rs = ((instruction >> 21) & 0x0000001F);
        instr.opcode = ((instruction >> 26) & 0x0000003F);


        switch(instr.funct) {
                case 0:
                        /* SLL/NOP */
			if((instr.rd == 0) && (instr.rt == 0) && (instr.sa == 0))
				sprintf(instr_str,"%s", "NOP");
			else
                        	sprintf(instr_str, "%s R%u, R%u, #%u", "SLL", instr.rd, instr.rt, instr.sa);
                        break;
                case 2:
                        /* SRL */
                        sprintf(instr_str, "%s R%u, R%u, #%u", "SRL", instr.rd, instr.rt, instr.sa);
                        break;
                case 3:
                        /* SRA */
                        sprintf(instr_str, "%s R%u, R%u, #%u", "SRA", instr.rd, instr.rt, instr.sa);
                        break;
                case 13:
                        /* BREAK */
			sprintf(instr_str,"%s", "BREAK");
			/* Break found - instruction processing need to stop */
			retval = 1;
                        break;
                case 32:
                        /* ADD */
                        sprintf(instr_str, "%s R%u, R%u, R%u", "ADD", instr.rd, instr.rs, instr.rt);
                        break;
                case 33:
                        /* ADDU */
                        sprintf(instr_str, "%s R%u, R%u, R%u", "ADDU", instr.rd, instr.rs, instr.rt);
                        break;
                case 34:
                        /* SUB */
                        sprintf(instr_str, "%s R%u, R%u, R%u", "SUB", instr.rd, instr.rs, instr.rt);
                        break;
                case 35:
                        /* SUBU */
                        sprintf(instr_str, "%s R%u, R%u, R%u", "SUBU", instr.rd, instr.rs, instr.rt);
                        break;
                case 36:
                        /* AND */
                        sprintf(instr_str, "%s R%u, R%u, R%u", "AND", instr.rd, instr.rs, instr.rt);
                        break;
                case 37:
                        /* OR */
                        sprintf(instr_str, "%s R%u, R%u, R%u", "OR", instr.rd, instr.rs, instr.rt);
                        break;
                case 38:
                        /* XOR */
                        sprintf(instr_str, "%s R%u, R%u, R%u", "XOR", instr.rd, instr.rs, instr.rt);
                        break;
                case 39:
                        /* NOR */
                        sprintf(instr_str, "%s R%u, R%u, R%u", "NOR", instr.rd, instr.rs, instr.rt);
                        break;
                case 42:
                        /* SLT */
                        sprintf(instr_str, "%s R%u, R%u, R%u", "SLT", instr.rd, instr.rs, instr.rt);
                        break;
                case 43:
                        /* SLTU */
                        sprintf(instr_str, "%s R%u, R%u, R%u", "SLTU", instr.rd, instr.rs, instr.rt);
                        break;
                default:
                        /* We should not be here - log it as error */
                        printf("Unhandled R-Format Opcode - %u - %u\n", instr.opcode, instr.funct);
        }

        fwrite(instr_str, 1, strlen(instr_str), fp);

        return retval;
}
