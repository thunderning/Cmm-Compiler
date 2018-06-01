#ifndef INTERCODE_H
#define INTERCODE_H

#include <stdio.h>

typedef struct Operand_ Operand;
typedef struct InterCode_ InterCode;

char OutTempVar;
char OutLabel;

int tCount;
int lCount;

InterCode* headCode;
InterCode* tailCode;

typedef struct Operand_{
    enum{O_TEMPVAR,O_VARIABLE,O_CONSTANT,O_LABEL,O_FUNCTION,O_ADDRESS,O_NULL } kind;

    union{
        int var_no;
        int value;
        char name[32];
    }u;

    Operand *next;
} Operand_;

typedef struct InterCode_{
    enum{I_LABEL,I_FUNCTION,I_ASSIGN,I_ADD,I_MINUS,I_STAR,I_DIV,I_GETADDR,I_GETVALUE,I_PUTVALUE,
         I_GOTO,I_IFGOTO,I_RETURN,I_DEC,I_ARG,I_CALL,I_PARAM,I_READ,I_WRITE}kind;

    union{
        struct{	Operand x;	}sinop;

        struct{ Operand x,y; }binop;

        struct{ Operand x; Operand y; Operand z; }triop;

        struct{ Operand x; Operand y; Operand z; int relop;} forop;

        struct{ Operand op; int size; }dec;
    }u;

    InterCode* pre;
    InterCode* next;
} InterCode_;

void addCode(InterCode* ic);
void deleteCode(InterCode* ic);
void printCodes(char* filename);
void printOp(FILE* fout,Operand* op);
void printCode(FILE* fout,InterCode* ic);
void optimizeCode();
Operand* newTemp();
Operand* newLabel();
Operand* newAddr();
#endif // INTERCODE_H
