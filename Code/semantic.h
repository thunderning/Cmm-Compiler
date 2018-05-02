#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "mytree.h"
#include "type.h"
//High-level Definitions
void Program(Node* start);
void ExtDefList(Node *node);
void ExtDef(Node *node);
void ExtDecList(Node *node, Type *type);
//Specifiers
Type* Specifier(Node *node);
Type* StructSpecifier(Node *node);
char* OptTag(Node *node);
char* Tag(Node *node);
//Declarators
FieldList* VarDec(Node *node, Type* type, int from);
void FunDec(Node *node, Type * type);
FieldList* VarList(Node *node);
FieldList* ParamDec(Node *node);
//Statements
void CompSt(Node *node, Type * retype);
void StmtList(Node *node, Type * retype);
void Stmt(Node *node, Type * retype);
//Local Definitions
FieldList* DefList(Node *node, int from);
FieldList* Def(Node *node, int from);
FieldList* DecList(Node *node, Type * type, int from);
FieldList* Dec(Node *node, Type * type, int from);
//Expressions
Type * Exp(Node *node);
int Args(Node *node, FieldList* param);

#endif // SEMANTIC_H
