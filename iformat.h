#ifndef IFORMAT_H
#define IFORMAT_H

typedef struct iformat_instr {
        int imm:16;
        unsigned char rt:5;
        unsigned char rs:5;
        unsigned char opcode:6;
}  __attribute__ ((__packed__)) IFormat_Instr;


int default_handle_itype (FILE *fp, const unsigned int instruction, unsigned int pc);

int handle_itype_opcode1 (FILE *fp, const unsigned int instruction, unsigned int pc);

#endif
