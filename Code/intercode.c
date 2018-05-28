#include "intercode.h"

extern char OutTempVar = 't';
extern char OutLabel = 'l';

extern int tCount = 0;
extern int lCount = 0;

extern InterCode* headCode = NULL;
extern InterCode* tailCode = NULL;

void addCode(InterCode *ic)
{
    if(headCode == NULL){
        headCode = ic;
        tailCode = ic;
    }
    else{
        tailCode->next = ic;
        ic->pre = tailCode;
    }
    ic->next = NULL;
    tailCode = ic;
}

void deleteCode(InterCode *ic)
{
    if(ic->pre != NULL)
        ic->pre->next = ic->next;
    if(ic->next != NULL)
        ic->next->pre = ic->pre;
    free(ic);
}

void printCodes(char *filename)
{
    FILE *fout = fopen(filename,"w");
    if(headCode == NULL)
        printf("ERROR: no interCode stored!");
    else{
        InterCode *p = headCode;
        while (p != NULL) {
            printCode(fout,p);
            p = p->next;
        }
    }

}

void printOp(FILE *fout, Operand *op)
{
    switch (op->kind) {
    case O_TEMPVAR:
        fprintf(fout,"%c%d",OutTempVar,op->u.var_no);
        break;
    case O_VARIABLE:
        fprintf(fout,"%s",op->u.name);
        break;
    case O_CONSTANT:
        fprintf(fout,"#%d",op->u.value);
        break;
    case O_LABEL:
        fprintf(fout,"%c%d",OutLabel,op->u.var_no);
        break;
    case O_FUNCTION:
        fprintf(fout,"%s",op->u.name);
        break;
    default:
        break;
    }
}

void printCode(FILE* fout,InterCode* ic){
    switch (ic->kind) {
    case I_LABEL:
        fprintf(fout,"LABEL ");
        printOp(fout,&ic->u.sinop);
        fprintf(fout," : \n");
        break;
    case I_FUNCTION:
        fprintf(fout,"FUNCTION ");
        printOp(fout,&ic->u.sinop);
        fprintf(fout," : \n");
        break;
    case I_ASSIGN:
        printOp(fout,&ic->u.binop.x);
        fprintf(fout," := ");
        printOp(fout,&ic->u.binop.y);
        fprintf(fout,"\n");
        break;
    case I_ADD:
        printOp(fout,&ic->u.triop.x);
        fprintf(fout," := ");
        printOp(fout,&ic->u.triop.y);
        fprintf(fout," + ");
        printOp(fout,&ic->u.triop.z);
        fprintf(fout,"\n");
        break;
    case I_MINUS:
        printOp(fout,&ic->u.triop.x);
        fprintf(fout," := ");
        printOp(fout,&ic->u.triop.y);
        fprintf(fout," - ");
        printOp(fout,&ic->u.triop.z);
        fprintf(fout,"\n");
        break;
    case I_STAR:
        printOp(fout,&ic->u.triop.x);
        fprintf(fout," := ");
        printOp(fout,&ic->u.triop.y);
        fprintf(fout," * ");
        printOp(fout,&ic->u.triop.z);
        fprintf(fout,"\n");
        break;
    case I_DIV:
        printOp(fout,&ic->u.triop.x);
        fprintf(fout," := ");
        printOp(fout,&ic->u.triop.y);
        fprintf(fout," / ");
        printOp(fout,&ic->u.triop.z);
        fprintf(fout,"\n");
        break;
    case I_GETADDR:
        printOp(fout,&ic->u.binop.x);
        fprintf(fout," := &");
        printOp(fout,&ic->u.binop.y);
        fprintf(fout,"\n");
        break;
    case I_GETVALUE:
        printOp(fout,&ic->u.binop.x);
        fprintf(fout," := *");
        printOp(fout,&ic->u.binop.y);
        fprintf(fout,"\n");
        break;
    case I_PUTVALUE:
        fprintf(fout,"*");
        printOp(fout,&ic->u.binop.x);
        fprintf(fout," := ");
        printOp(fout,&ic->u.binop.y);
        fprintf(fout,"\n");
        break;
    case I_GOTO:
        fprintf(fout,"GOTO ");
        printOp(fout,&ic->u.sinop);
        fprintf(fout,"\n");
        break;
    case I_IFGOTO:
        fprintf(fout,"IF ");
        printOp(fout,&ic->u.forop.x);
        // > 1 ,< 2 ,>= 3, <= 4, == 5 ,!= 6
        switch (ic->u.forop.relop) {
        case 1:
            fprintf(fout," > ");
            break;
        case 2:
            fprintf(fout," < ");
            break;
        case 3:
            fprintf(fout," >= ");
            break;
        case 4:
            fprintf(fout," <= ");
            break;
        case 5:
            fprintf(fout," == ");
            break;
        case 6:
            fprintf(fout," != ");
            break;
        default:
            break;
        }
        printOp(fout,&ic->u.forop.y);
        fprintf(fout," GOTO ");
        printOp(fout,&ic->u.forop.z);
        fprintf(fout,"\n");
        break;
    case I_RETURN:
        fprintf(fout,"RETURN ");
        printOp(fout,&ic->u.sinop);
        fprintf(fout,"\n");
        break;
    case I_DEC:
        fprintf(fout,"DEC ");
        printOp(fout,&ic->u.dec.op);
        fprintf(fout," %d",ic->u.dec.size);
        fprintf(fout,"\n");
        break;
    case I_ARG:
        fprintf(fout,"ARG ");
        printOp(fout,&ic->u.sinop);
        fprintf(fout,"\n");
        break;
    case I_CALL:
        printOp(fout,&ic->u.binop.x);
        fprintf(fout," := CALL ");
        printOp(fout,&ic->u.binop.y);
        fprintf(fout,"\n");
        break;
    case I_PARAM:
        fprintf(fout,"PARAM ");
        printOp(fout,&ic->u.sinop);
        fprintf(fout,"\n");
        break;
    case I_READ:
        fprintf(fout,"READ ");
        printOp(fout,&ic->u.sinop);
        fprintf(fout,"\n");
        break;
    case I_WRITE:
        fprintf(fout,"WRITE ");
        printOp(fout,&ic->u.sinop);
        fprintf(fout,"\n");
        break;

    default:
        break;
    }
}
