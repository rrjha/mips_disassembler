#ifndef RFORMAT_H
#define RFORMAT_H

typedef struct rformat_instr {
        unsigned int funct:6;
        unsigned char sa:5;
        unsigned char rd:5;
        unsigned char rt:5;
        unsigned char rs:5;
        unsigned char opcode:6;
}  __attribute__ ((__packed__)) RFormat_Instr;


int handle_rtype_opcode0 (FILE *fp, const unsigned int instruction, unsigned int pc);

#endif
