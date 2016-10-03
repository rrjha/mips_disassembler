#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "mipssim.h"
#include "iformat.h"
#include "jformat.h"
#include "rformat.h"

#define DATA_BASE_ADDR 716
#define CNV_ENDIAN(num) \
		(((num>>24)&0x000000FF) | \
                ((num>>8)&0x0000FF00)   | \
		((num<<8)&0x00FF0000)   | \
		((num<<24)&0xFF000000));  \

const char *nibble_bit_rep[16] = {
	[ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
	[ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
	[ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
	[12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};

/*	List of all instruction strings indexed by their opcode. When multiple	*/
/*	instruction grouped the string is the class. Placeholder "TBD" for	*/
/*	instructions that are currently not classified.				*/

Instruction_Table_Entry instr_tab[MAX_INSTR] = {
/* SL */        /* Instruction Name */          /* Instruction Type */
{/* 00 */        "RTYPE",                       handle_rtype_opcode0    },
{/* 01 */        "ITYPE",                       handle_itype_opcode1    },
{/* 02 */        "J"    ,                       default_handle_jtype    },
{/* 03 */        "JAL"  ,                       NULL    		},
{/* 04 */        "BEQ"  ,                       default_handle_itype    },
{/* 05 */        "BNE"  ,                       default_handle_itype    },
{/* 06 */        "BLEZ" ,                       default_handle_itype    },
{/* 07 */        "BGTZ" ,                       default_handle_itype    },
{/* 08 */        "ADDI" ,                       default_handle_itype    },
{/* 09 */        "ADDIU",                       default_handle_itype    },
{/* 10 */        "SLTI" ,                       default_handle_itype    },
{/* 11 */        "SLTIU",                       default_handle_itype    },
{/* 12 */        "ANDI" ,                       NULL    		},
{/* 13 */        "ORI"  ,                       NULL    		},
{/* 14 */        "XORI" ,                       NULL    		},
{/* 15 */        "LUI"  ,                       NULL    		},
{/* 16 */        "TBD",                       	NULL    		},
{/* 17 */        "TBD",                       	NULL    		},
{/* 18 */        "TBD",                       	NULL    		},
{/* 19 */        "TBD",                       	NULL    		},
{/* 20 */        "BEQL" ,                       NULL    		},
{/* 21 */        "BNEL" ,                       NULL    		},
{/* 22 */        "BLEZL",                       NULL    		},
{/* 23 */        "BGTZL",                       NULL    		},
{/* 24 */        "TBD",                       	NULL    		},
{/* 25 */        "TBD",                       	NULL    		},
{/* 26 */        "TBD",                       	NULL    		},
{/* 27 */        "TBD",                       	NULL    		},
{/* 28 */        "TBD",                       	NULL    		},
{/* 29 */        "TBD",                       	NULL    		},
{/* 30 */        "TBD",                       	NULL    		},
{/* 31 */        "TBD",                       	NULL    		},
{/* 32 */        "LB"   ,                       NULL    		},
{/* 33 */        "LH"   ,                       NULL    		},
{/* 34 */        "LWL"  ,                       NULL    		},
{/* 35 */        "LW"   ,                       default_handle_itype    },
{/* 36 */        "LBU"  ,                       NULL    		},
{/* 37 */        "LHU"  ,                       NULL    		},
{/* 38 */        "LWR"  ,                       NULL    		},
{/* 39 */        "TBD",                       	NULL    		},
{/* 40 */        "SB"   ,                       NULL    		},
{/* 41 */        "SH"   ,                       NULL    		},
{/* 42 */        "SWL"  ,                       NULL    		},
{/* 43 */        "SW"   ,                       default_handle_itype    },
{/* 44 */        "TBD",                       	NULL    		},
{/* 45 */        "TBD",                       	NULL    		},
{/* 46 */        "SWR"  ,                       NULL    		},
{/* 47 */        "CACHE",                       NULL    		},
{/* 48 */        "LL"   ,                       NULL    		},
{/* 49 */        "LWC1" ,                       NULL    		},
{/* 50 */        "LWC2" ,                       NULL    		},
{/* 51 */        "PREF" ,                       NULL    		},
{/* 52 */        "TBD",                       	NULL    		},
{/* 53 */        "LDC1" ,                       NULL    		},
{/* 54 */        "LDC2" ,                       NULL    		},
{/* 55 */        "TBD",                       	NULL    		},
{/* 56 */        "SC"   ,                       NULL    		},
{/* 57 */        "SWC1" ,                       NULL    		},
{/* 58 */        "SWC2" ,                       NULL    		},
{/* 59 */        "TBD",                       	NULL    		},
{/* 60 */        "TBD",                       	NULL    		},
{/* 61 */        "SDC1" ,                       NULL    		},
{/* 62 */        "SDC2" ,                       NULL    		},
{/* 63 */        "TBD",                       	NULL    		},
};

void showusage () {
	printf("Usage: mipssim inputfilename outputfilename operation [-Tm:n]\n\n");
        printf("Inputfilename - The file name of the binary input file.\n\
Outputfilename - The file name to which to print the output.\n\
Operation - Either\"dis\" or\"sim\" to specify disassembly or simulation.\n\
-Tm:n - Optional argument to specify the start (m) and end (n) cycles of simulation tracing output.\n\n");
}


int main (int argc, char *argv[]) {

	if ((argc < 4) || (argc > 5)) {
		showusage();
	}
	else {
		if(!strcmp(argv[3], "dis")) {
			if (argc > 4) {
				printf("Too many arguments for \"dis\" operation\n");
				showusage();
				return 1;
			}
			/* Setup for disassembly */
			FILE *fin, *fout;
			unsigned int rbuff = 0;
			int filesz=0, testval = 1;
			unsigned int pc = BASE_ADDRESS, i=7, instr_complete = 0;
			char *wbuff, *temp;
			instr_handle_func handle = NULL;

			fin = fopen (argv[1], "rb");
                	fout = fopen (argv[2], "w");

                	if ((NULL == fin) || (NULL == fout)) {
                        	perror("Program encountered error exiting..\n");
				return 1;
                	}
			fseek(fin, 0L, SEEK_END);
			filesz = ftell (fin);
			if ((filesz <= 0) || ((filesz % 4) != 0)) {
				printf("The size of input file %s is not multiple of 4 bytes. MIPS instruction must be exactly 32 bit.\n", argv[1]);
				fclose(fout);
				fclose(fin);
				return 1;
			}
			rewind(fin);

			wbuff = malloc(50);
			while ((pc-BASE_ADDRESS) < filesz) {
				/* Now read instructions or address from the stream into 32 bit buffer */
				fread(&rbuff, 4, 1, fin);
				if (*(char*)&testval)
					rbuff = CNV_ENDIAN(rbuff);
				for (i=7; i>0; i--)
					strncpy((wbuff+((7-i)*4)), nibble_bit_rep[((rbuff>>(4*i))&0xF)], 4);
				strncpy(wbuff+28, nibble_bit_rep[(rbuff&0xF)], 4);

				if(!instr_complete) {
					/* First write 6-bit instruction */
					fwrite(wbuff, 1, 6, fout);
					fwrite(BLANK, 1, 1, fout);

					/* Next 4 5-bit fields with spaces */
					for(i=0; i<4; i++) {
						fwrite(wbuff+6+i*5, 1, 5, fout);
						fwrite(BLANK, 1, 1, fout);
					}

					/* Now write last 6-bit field */
					fwrite(wbuff+6+i*5, 1, 6, fout);

					/* Echo pc */
					sprintf(wbuff, " %u ", pc);
					fwrite(wbuff, 1, strlen(wbuff), fout);

					/* Now call instruction processing handle */
					handle = (instr_tab[((rbuff>>26)&0xFF)].instr_handle);
					if(NULL != handle) {
						instr_complete = handle(fout, rbuff, pc);
					}
				}
				else {
					/* Instruction processing finished */
					fwrite(wbuff, 1, strlen(wbuff), fout);
                                        /* Echo addresses and value */
					if(pc < DATA_BASE_ADDR)
						sprintf(wbuff, " %u %u", pc, rbuff);
					else
						sprintf(wbuff, " %u %d", pc, rbuff);
                                        fwrite(wbuff, 1, strlen(wbuff), fout);

				}
				pc+=4;
				fwrite(NEWLINE, 1, 1, fout);
			}
			free(wbuff);
			fclose(fout);
			fclose(fin);

		}
		else if (!strcmp(argv[3], "sim")) {
			/* Setup for simulation - TBD */
			printf("Operation %s is not supported yet\n", argv[3]);
		}
		else {
			printf("%s is not a valid operation\n", argv[3]);
			showusage();
		}
	}

	return 0;
}
