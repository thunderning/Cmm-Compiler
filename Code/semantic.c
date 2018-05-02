#include "semantic.h"

void Program(Node* start){
    ExtDefList(start->childNodes[0]);
}

void ExtDefList(Node* node){
    if(NULL == node)
        return;

    // ExtDefList -> NULL
    if(0 == node->length)
        return;

    // ExtDefList -> ExtDef ExtDefList
    ExtDef(node->childNodes[0]);
    ExtDefList(node->childNodes[1]);
}
void ExtDef(Node *node)
{
    if(NULL == node)
        return;
    //ExtDef -> Specifier ExtDecList SEMI

    //ExtDef -> Specifier SEMI

    //ExtDef -> Specifier FunDec CompSt
}

void ExtDecList(Node *node, Type * type)
{
    if(NULL == node)
        return;
    //ExtDecList -> VarDec

    //ExtDecList -> VarDec COMMA ExtDecList
}

Type* Specifier(Node *node)
{
    if(NULL == node)
        return NULL;

    //Specifier -> TYPE

    //Specifier -> StructSpecifier

}

Type* StructSpecifier(Node *node)
{
    if(NULL == node)
        return NULL;
    //StructSpecifier -> STRUCT OptTag LC DefList RC

    //StructSpecifier -> STRUCT Tag
}

char *OptTag(Node *node)
{
    if(NULL == node)
        return NULL;
    //OptTag -> ID

    //OptTag -> NULL
}

char *Tag(Node *node)
{
    if(NULL == node)
        return NULL;
    //Tag -> ID;
}

FieldList* VarDec(Node *node, Type* type, int from)
{
    if(NULL == node)
        return NULL;
    //VarDec -> ID

    //VarDec -> VarDec LB INT RB

}

void FunDec(Node *node, Type* type)
{
    if(NULL == node)
        return;
    //FunDec -> ID LP VarList RP

    //FunDec -> ID LP RP
}

FieldList* VarList(Node *node)
{
    if(NULL == node)
        return NULL;
    //VarList -> ParamDec COMMA VarList

    //VarList -> ParamDec
}

FieldList* ParamDec(Node *node)
{
    if(NULL == node)
        return NULL;
    //ParamDec -> Specifier VarDec
}

void CompSt(Node *node, Type* retype)
{
    if(NULL == node)
        return;
    //CompSt -> LC DefList StmtList RC
}

void StmtList(Node *node, Type* retype)
{
    if(NULL == node)
        return;
    //StmtList -> Stmt StmtList

    //StmtList -> NULL
}

void Stmt(Node *node, Type* retype)
{
    if(NULL == node)
        return;
    //Stmt -> Exp SEMI

    //Stmt -> CompSt

    //Stmt -> RETURN Exp SEMI

    //Stmt -> IF LP Exp RP Stmt

    //Stmt -> IF LP Exp RP Stmt ELSE Stmt

    //Stmt -> WHILE LP Exp RP Stmt
}

FieldList* DefList(Node *node, int from)
{
    if(NULL == node)
        return NULL;
    //DefList -> Def DefList

    //DefList -> NULL
}

FieldList* Def(Node *node, int from)
{
    if(NULL == node)
        return NULL;
    //Def -> Specifier DecList SEMI
}

FieldList* DecList(Node *node, Type* type, int from)
{
    if(NULL == node)
        return NULL;
    //DecList -> Dec

    //DecList -> Dec COMMA DecList
}

FieldList* Dec(Node *node, Type* type, int from)
{
    if(NULL == node)
        return NULL;
    //Dec -> VarDec

    //Dec -> VarDec ASSIGNOP Exp
}

Type* Exp(Node *node)
{
    if(NULL == node)
        return NULL;

    //Exp -> Exp ASSIGNOP Exp

    //Exp -> Exp AND Exp

    //Exp -> Exp OR Exp

    //Exp -> Exp RELOP Exp

    //Exp -> Exp PLUS Exp

    //Exp -> Exp MINUS Exp

    //Exp -> Exp STAR Exp

    //Exp -> Exp DIV Exp

    //Exp -> LP Exp RP

    //Exp -> MINUS Exp

    //Exp -> NOT Exp

    //Exp -> ID LP Args RP

    //Exp -> ID LP RP

    //Exp -> Exp LB Exp RB

    //Exp -> Exp DOT ID

    //Exp -> ID

    //Exp -> INT

    //Exp -> FLOAT
}

int Args(Node *node, FieldList* param)
{
    if(NULL == node)
        return 1;

    //Args -> Exp COMMA Args

    //Args -> Exp

}
