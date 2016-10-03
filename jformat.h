#ifndef JFORMAT_H
#define JFORMAT_H

typedef struct jformat_instr {
        int jump_addr:26;
        unsigned char opcode:6;
}  __attribute__ ((__packed__)) JFormat_Instr;


int default_handle_jtype (FILE *fp, const unsigned int instruction, unsigned int pc);

#endif
