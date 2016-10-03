#include <string.h>
#include "mipssim.h"
#include "iformat.h"

extern Instruction_Table_Entry instr_tab[MAX_INSTR];

int default_handle_itype (FILE *fp, const unsigned int instruction, unsigned int pc) {
	char opr[50] = "";
        IFormat_Instr instr;

	instr.imm = (instruction & 0x0000FFFF);
	instr.rt = ((instruction >> 16) & 0x0000001F);
	instr.rs = ((instruction >> 21) & 0x0000001F);
	instr.opcode = ((instruction >> 26) & 0x0000003F);

        char *str = instr_tab[((instr.opcode)&0xFF)].instr_str;

        fwrite(str, 1, strlen(str), fp);

        fwrite(BLANK, 1, 1, fp);

        switch(instr.opcode) {
                case 4:
                        /* BEQ */
                case 5:
                        /* BNE */
			sprintf(opr, "R%u, R%u, #%d", instr.rs, instr.rt, (int)((int)(instr.imm) << 2));
                        break;
                case 6:
                        /* BLEZ */
                case 7:
                        /* BGTZ */
			sprintf(opr, "R%u, #%d", instr.rs, (int)((int)(instr.imm) << 2));
                        break;
                case 8:
                        /* ADDI */
                case 9:
                        /* ADDIU */
                case 10:
                        /* SLTI */
                case 11:
                        /* SLTIU */
			sprintf(opr, "R%u, R%u, #%d", instr.rt, instr.rs, (int)(instr.imm));
                        break;
                case 35:
                        /* LW */
                case 43:
                        /* SW */
			sprintf(opr, "R%u, %d(R%u)", instr.rt, (int)(instr.imm), instr.rs);
                        break;
                default:
                        /* We should not be here - log it as error */
                        printf("Unhandled I-Format Opcode - %s - %u\n", str, instr.opcode);
        }

        fwrite(opr, 1, strlen(opr), fp);

        return 0;
}

int handle_itype_opcode1 (FILE *fp, const unsigned int instruction, unsigned int pc) {
        IFormat_Instr instr;
        char instr_str[50] = "";

        instr.imm = (instruction & 0x0000FFFF);
        instr.rt = ((instruction >> 16) & 0x0000001F);
        instr.rs = ((instruction >> 21) & 0x0000001F);
        instr.opcode = ((instruction >> 26) & 0x0000003F);


        switch(instr.rt) {
                case 0:
                        /* BLTZ */
			sprintf(instr_str, "%s R%u, #%d", "BLTZ", instr.rs, (int)((int)(instr.imm) << 2));
			break;
		case 1:
			/* BGEZ */
			sprintf(instr_str, "%s R%u, #%d", "BGEZ", instr.rs, (int)((int)(instr.imm) << 2));
			break;
                default:
                        /* We should not be here - log it as error */
                        printf("Unhandled I-Format Opcode - %u - %u\n", instr.opcode, instr.rt);
        }

        fwrite(instr_str, 1, strlen(instr_str), fp);

        return 0;
}

