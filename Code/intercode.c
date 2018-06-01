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
    case O_ADDRESS:
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
        fprintf(fout,"NULL");
        break;
    }
}

void printCode(FILE* fout,InterCode* ic){
    switch (ic->kind) {
    case I_LABEL:
        fprintf(fout,"LABEL ");
        printOp(fout,&ic->u.sinop.x);
        fprintf(fout," : \n");
        break;
    case I_FUNCTION:
        fprintf(fout,"FUNCTION ");
        printOp(fout,&ic->u.sinop.x);
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
        printOp(fout,&ic->u.sinop.x);
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
        printOp(fout,&ic->u.sinop.x);
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
        printOp(fout,&ic->u.sinop.x);
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
        printOp(fout,&ic->u.sinop.x);
        fprintf(fout,"\n");
        break;
    case I_READ:
        fprintf(fout,"READ ");
        printOp(fout,&ic->u.sinop.x);
        fprintf(fout,"\n");
        break;
    case I_WRITE:
        fprintf(fout,"WRITE ");
        printOp(fout,&ic->u.sinop.x);
        fprintf(fout,"\n");
        break;

    default:
        break;
    }
}

int checkStruct(Node *node){
    if(node == NULL)
        return 0;
    
    if(node->type == TR_STRUCT)
        return 1;

    for(int i = 0; i < node->length; i++)
    {
        if(checkStruct(node->childNodes[i]))
            return 1;
    }
    return 0;
    
}

void optimizeCode(){
    //去掉所有NULL的指令
    for(InterCode* i = headCode; i != NULL;)
    {
        if(i->u.sinop.x.kind == O_NULL ||
        i->u.binop.x.kind == O_NULL ||
        i->u.triop.x.kind == O_NULL ||
        i->u.forop.x.kind == O_NULL ||
        i->u.dec.op.kind == O_NULL){
            InterCode* p = i;
            i = i->next;
            deleteCode(p);
        }
        else{
            i = i->next;
        }
    }
    int flag = 0;
    while(1){
        Operand **override = malloc((tCount+1)*sizeof(Operand *));
        for(InterCode* i = headCode; i != NULL;)
        {
            if(i->kind == I_ASSIGN 
                && (i->u.binop.y.kind == O_CONSTANT || i->u.binop.y.kind == O_VARIABLE)
                && i->u.binop.x.kind == O_TEMPVAR){
                flag = 1;
                override[i->u.binop.x.u.var_no] = malloc(sizeof(Operand));
                *(override[i->u.binop.x.u.var_no]) = i->u.binop.y;
                InterCode* p = i;
                i = i->next;
                deleteCode(p);
            }
            else{
                i = i->next;
            }
        }
        for(InterCode* ic = headCode; ic != NULL;ic = ic->next)
        {
            switch (ic->kind) {
            case I_ASSIGN:
            case I_PUTVALUE:
            case I_GETVALUE:
            case I_GETADDR:
                if(ic->u.binop.x.kind == O_TEMPVAR && override[ic->u.binop.x.u.var_no] != NULL){
                    ic->u.binop.x = *(override[ic->u.binop.x.u.var_no]);
                }
                if(ic->u.binop.y.kind == O_TEMPVAR && override[ic->u.binop.y.u.var_no] != NULL){
                    ic->u.binop.y = *(override[ic->u.binop.y.u.var_no]);
                }
                break;
            case I_ADD:
            case I_MINUS:
            case I_STAR:
            case I_DIV:
                if(ic->u.triop.x.kind == O_TEMPVAR && override[ic->u.triop.x.u.var_no] != NULL){
                    ic->u.triop.x = *(override[ic->u.triop.x.u.var_no]);
                }
                if(ic->u.triop.y.kind == O_TEMPVAR && override[ic->u.triop.y.u.var_no] != NULL){
                    ic->u.triop.y = *(override[ic->u.triop.y.u.var_no]);
                }
                if(ic->u.triop.z.kind == O_TEMPVAR && override[ic->u.triop.z.u.var_no] != NULL){
                    ic->u.triop.z = *(override[ic->u.triop.z.u.var_no]);
                }

                if(ic->u.triop.y.kind == O_CONSTANT && ic->u.triop.z.kind == O_CONSTANT){
                    if(ic->kind == I_ADD){
                        ic->kind = I_ASSIGN;
                        ic->u.binop.x = ic->u.triop.x;
                        ic->u.binop.y.kind = O_CONSTANT;
                        ic->u.binop.y.u.value = ic->u.triop.y.u.value + ic->u.triop.z.u.value;
                    }
                    if(ic->kind == I_MINUS){
                        ic->kind = I_ASSIGN;
                        ic->u.binop.x = ic->u.triop.x;
                        ic->u.binop.y.kind = O_CONSTANT;
                        ic->u.binop.y.u.value = ic->u.triop.y.u.value - ic->u.triop.z.u.value;
                    }
                    if(ic->kind == I_STAR){
                        ic->kind = I_ASSIGN;
                        ic->u.binop.x = ic->u.triop.x;
                        ic->u.binop.y.kind = O_CONSTANT;
                        ic->u.binop.y.u.value = ic->u.triop.y.u.value * ic->u.triop.z.u.value;
                    }
                    if(ic->kind == I_DIV){
                        ic->kind = I_ASSIGN;
                        ic->u.binop.x = ic->u.triop.x;
                        ic->u.binop.y.kind = O_CONSTANT;
                        ic->u.binop.y.u.value = ic->u.triop.y.u.value / ic->u.triop.z.u.value;
                    }
                }
                break;
            case I_IFGOTO:
                if(ic->u.forop.x.kind == O_TEMPVAR && override[ic->u.forop.x.u.var_no] != NULL){
                    ic->u.forop.x = *(override[ic->u.forop.x.u.var_no]);
                }
                if(ic->u.forop.y.kind == O_TEMPVAR && override[ic->u.forop.y.u.var_no] != NULL){
                    ic->u.forop.y = *(override[ic->u.forop.y.u.var_no]);
                }
                break;

            case I_CALL:
                if(ic->u.binop.x.kind == O_TEMPVAR && override[ic->u.binop.x.u.var_no] != NULL){
                    ic->u.binop.x = *(override[ic->u.binop.x.u.var_no]);
                }
                break;
            case I_RETURN:
            case I_ARG:
            case I_READ:
            case I_WRITE:
                if(ic->u.sinop.x.kind == O_TEMPVAR && override[ic->u.sinop.x.u.var_no] != NULL){
                    ic->u.sinop.x = *(override[ic->u.sinop.x.u.var_no]);
                }
                break;

            default:
                break;
            }
        }
        if(flag == 0 )
            break;
        flag = 0;
    }
}

Operand* newTemp(){
    Operand *ret = malloc(sizeof(Operand));
    ret->kind = O_TEMPVAR;
    ret->u.var_no = tCount;
    tCount++;
    return ret;
}
Operand* newLabel(){
    Operand *ret = malloc(sizeof(Operand));
    ret->kind = O_LABEL;
    ret->u.var_no = lCount;
    lCount++;
    return ret;
}
Operand* newAddr(){
    Operand *ret = malloc(sizeof(Operand));
    ret->kind = O_ADDRESS;
    ret->u.var_no = tCount;
    tCount++;
    return ret;
}