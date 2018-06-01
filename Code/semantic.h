#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "myTree.h"
#include "type.h"
#include "symbolTable.h"
#include "intercode.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
//root type
enum RootType{
    ROOT_STRUCT,ROOT_COMPST,ROOT_DEFINE,ROOT_USE
};
//High-level Definitions
void Program(Node* start);
void ExtDefList(Node *node);
void ExtDef(Node *node);
void ExtDecList(Node *node, Type *type);
//Specifiers
Type* Specifier(Node *node,int root);
Type* StructSpecifier(Node *node,int root);
char* OptTag(Node *node);
char* Tag(Node *node);
//Declarators
FieldList* VarDec(Node *node, Type* type);
void FunDec(Node *node, Type * type);
FieldList* VarList(Node *node);
FieldList* ParamDec(Node *node);
//Statements
void CompSt(Node *node, Type * retype);
void StmtList(Node *node, Type * retype);
void Stmt(Node *node, Type * retype);
//Local Definitions
FieldList* DefList(Node *node,int root);
FieldList* Def(Node *node,int root);
FieldList* DecList(Node *node, Type * type,int root);
FieldList* Dec(Node *node, Type * type,int root);
//Expressions
Type * Exp(Node *node,int ifLeft,int *retLeft,Operand *place);
Type * Cond(Node *node,Operand *label1,Operand *label2);
FieldList *Args(Node *node,Operand *head);
//Error handle
void ErrorHandle(int type,int line,char* info);
//Type equal
int TypeEqual(Type *t1,Type *t2);
#endif // SEMANTIC_H
