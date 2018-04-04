#include "stdarg.h"
#include "stdio.h"
enum NodeType{
  TR_SEMI, TR_COMMA, TR_ASSIGNOP, TR_RELOP,
  TR_PLUS, TR_MINUS, TR_STAR, TR_DIV, TR_AND, TR_OR, TR_NOT, TR_DOT, TR_LP, TR_RP,
  TR_LB, TR_RB, TR_LC, TR_RC, TR_TYPE, TR_STRUCT, TR_IF, TR_ELSE, TR_RETURN,
  TR_WHILE, TR_INT, TR_FLOAT, TR_ID, TR_LOWER_THAN_ELSE, TR_Program,
  TR_ExtDefList, TR_ExtDef, TR_ExtDecList, TR_Specifier, TR_StructSpecifier,
  TR_OptTag, TR_Tag, TR_VarDec, TR_FunDec, TR_VarList, TR_ParamDec, TR_CompSt,
  TR_StmtList, TR_Stmt, TR_DefList, TR_Def, TR_DecList, TR_Dec, TR_Exp, TR_Args
};

static const char *const Nodename[] =
{
  "SEMI", "COMMA", "ASSIGNOP", "RELOP",
  "PLUS", "MINUS", "STAR", "DIV", "AND", "OR", "NOT", "DOT", "LP", "RP",
  "LB", "RB", "LC", "RC", "TYPE", "STRUCT", "IF", "ELSE", "RETURN",
  "WHILE", "INT", "FLOAT", "ID", "LOWER_THAN_ELSE", "Program",
  "ExtDefList", "ExtDef", "ExtDecList", "Specifier", "StructSpecifier",
  "OptTag", "Tag", "VarDec", "FunDec", "VarList", "ParamDec", "CompSt",
  "StmtList", "Stmt", "DefList", "Def", "DecList", "Dec", "Exp", "Args"
};

typedef struct node{
  enum NodeType type;
  int valInt;
  float valFloat;
  char valString[32];
  int length;
  struct node* childNodes[10];
  int linenum;
} Node;

void insertNode(int num,...);
void printTree(Node* start,int depth);
