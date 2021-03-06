%{
  #include <stdio.h>
  #include "myTree.h"
  #include "lex.yy.c"
  #define YYSTYPE Node*
  Node* startNode; //语法树根节点
  int iserror = 0; //判断是否有错误，来确定是否输出语法树
  int yyerror(char *s) {}
  void printfError(char c,int line){
    printf("Error type B at Line %d: Missing \"%c\"\n",line,c);
    iserror = 1;
  }
%}

%token SEMI COMMA
%token ASSIGNOP
%token RELOP
%token PLUS MINUS STAR DIV
%token AND OR NOT DOT
%token LP RP LB RB LC RC
%token TYPE STRUCT IF ELSE RETURN WHILE
%token INT
%token FLOAT
%token ID

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

%%
Program : ExtDefList {startNode = malloc(sizeof(Node));insertNode(1,startNode,TR_Program,$1);}
  ;
ExtDefList : ExtDef ExtDefList {$$ = malloc(sizeof(Node));insertNode(2,$$,TR_ExtDefList,$1,$2);}
  | {$$ = malloc(sizeof(Node));insertNode(0,$$,TR_ExtDefList);}
  ;
ExtDef : Specifier ExtDecList SEMI {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_ExtDef,$1,$2,$3);}
  | Specifier SEMI {$$ = malloc(sizeof(Node));insertNode(2,$$,TR_ExtDef,$1,$2);}
  | Specifier FunDec CompSt {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_ExtDef,$1,$2,$3);}
  ;
ExtDecList : VarDec {$$ = malloc(sizeof(Node));insertNode(1,$$,TR_ExtDecList,$1);}
  | VarDec COMMA ExtDecList {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_ExtDecList,$1,$2,$3);}
  ;

Specifier : TYPE {$$ = malloc(sizeof(Node));insertNode(1,$$,TR_Specifier,$1);}
  | StructSpecifier {$$ = malloc(sizeof(Node));insertNode(1,$$,TR_Specifier,$1);}
  ;
StructSpecifier : STRUCT OptTag LC DefList RC {$$ = malloc(sizeof(Node));insertNode(5,$$,TR_StructSpecifier,$1,$2,$3,$4,$5);}
  | STRUCT Tag {$$ = malloc(sizeof(Node));insertNode(2,$$,TR_StructSpecifier,$1,$2);}
  ;
OptTag : ID {$$ = malloc(sizeof(Node));insertNode(1,$$,TR_OptTag,$1);}
  | {$$ = malloc(sizeof(Node));insertNode(0,$$,TR_ID);}
  ;
Tag : ID {$$ = malloc(sizeof(Node));insertNode(1,$$,TR_Tag,$1);}
  ;

VarDec : ID {$$ = malloc(sizeof(Node));insertNode(1,$$,TR_VarDec,$1);}
  | VarDec LB INT RB {$$ = malloc(sizeof(Node));insertNode(4,$$,TR_VarDec,$1,$2,$3,$4);}
  | VarDec LB error RB {printfError(']',yylineno);}
  ;
FunDec : ID LP VarList RP {$$ = malloc(sizeof(Node));insertNode(4,$$,TR_FunDec,$1,$2,$3,$4);}
  | ID LP RP {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_FunDec,$1,$2,$3);}
  | ID LP error RP {printfError(')',yylineno);}
  ;
VarList : ParamDec COMMA VarList {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_VarList,$1,$2,$3);}
  | ParamDec {$$ = malloc(sizeof(Node));insertNode(1,$$,TR_VarList,$1);}
  ;
ParamDec : Specifier VarDec {$$ = malloc(sizeof(Node));insertNode(2,$$,TR_ParamDec,$1,$2);}
  ;

CompSt : LC DefList StmtList RC {$$ = malloc(sizeof(Node));insertNode(4,$$,TR_CompSt,$1,$2,$3,$4);}
  ;
StmtList : Stmt StmtList {$$ = malloc(sizeof(Node));insertNode(2,$$,TR_StmtList,$1,$2);}
  | {$$ = malloc(sizeof(Node));insertNode(0,$$,TR_StmtList);}
  ;
Stmt : Exp SEMI {$$ = malloc(sizeof(Node));insertNode(2,$$,TR_Stmt,$1,$2);}
  | error SEMI {printfError(';',yylineno);}
  | CompSt {$$ = malloc(sizeof(Node));insertNode(1,$$,TR_Stmt,$1);}
  | RETURN Exp SEMI {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_Stmt,$1,$2,$3);}
  | RETURN error SEMI {printfError(';',yylineno);}
  | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$ = malloc(sizeof(Node));insertNode(5,$$,TR_Stmt,$1,$2,$3,$4,$5);}
  | IF LP Exp RP Stmt ELSE Stmt {$$ = malloc(sizeof(Node));insertNode(7,$$,TR_Stmt,$1,$2,$3,$4,$5,$6,$7);}
  | WHILE LP Exp RP Stmt {$$ = malloc(sizeof(Node));insertNode(5,$$,TR_Stmt,$1,$2,$3,$4,$5);}
  ;

DefList : Def DefList {$$ = malloc(sizeof(Node));insertNode(2,$$,TR_DefList,$1,$2);}
  | {$$ = malloc(sizeof(Node));insertNode(0,$$,TR_DefList);}
  ;
Def : Specifier DecList SEMI {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_Def,$1,$2,$3);}
  | Specifier error SEMI {printfError(';',yylineno);}
  ;
DecList : Dec {$$ = malloc(sizeof(Node));insertNode(1,$$,TR_DecList,$1);}
  | Dec COMMA DecList {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_DecList,$1,$2,$3);}
  ;
Dec : VarDec {$$ = malloc(sizeof(Node));insertNode(1,$$,TR_Dec,$1);}
  | VarDec ASSIGNOP Exp {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_Dec,$1,$2,$3);}
  ;

Exp : Exp ASSIGNOP Exp {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_Exp,$1,$2,$3);}
  | Exp OR Exp {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_Exp,$1,$2,$3);}
  | Exp AND Exp {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_Exp,$1,$2,$3);}
  | Exp RELOP Exp {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_Exp,$1,$2,$3);}
  | Exp MINUS Exp {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_Exp,$1,$2,$3);}
  | Exp PLUS Exp {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_Exp,$1,$2,$3);}
  | Exp DIV Exp {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_Exp,$1,$2,$3);}
  | Exp STAR Exp {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_Exp,$1,$2,$3);}
  | MINUS Exp {$$ = malloc(sizeof(Node));insertNode(2,$$,TR_Exp,$1,$2);}
  | NOT Exp {$$ = malloc(sizeof(Node));insertNode(2,$$,TR_Exp,$1,$2);}
  | Exp LB error RB {printfError(']',yylineno);}
  | Exp LB Exp RB {$$ = malloc(sizeof(Node));insertNode(4,$$,TR_Exp,$1,$2,$3,$4);}
  | ID LP RP {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_Exp,$1,$2,$3);}
  | ID LP error RP {printfError(')',yylineno);}
  | ID LP Args RP {$$ = malloc(sizeof(Node));insertNode(4,$$,TR_Exp,$1,$2,$3,$4);}
  | LP Exp RP {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_Exp,$1,$2,$3);}
  | Exp DOT ID {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_Exp,$1,$2,$3);}
  | ID {$$ = malloc(sizeof(Node));insertNode(1,$$,TR_Exp,$1);}
  | INT {$$ = malloc(sizeof(Node));insertNode(1,$$,TR_Exp,$1);}
  | FLOAT {$$ = malloc(sizeof(Node));insertNode(1,$$,TR_Exp,$1);}
  ;
Args : Exp COMMA Args {$$ = malloc(sizeof(Node));insertNode(3,$$,TR_Args,$1,$2,$3);}
  | Exp {$$ = malloc(sizeof(Node));insertNode(1,$$,TR_Args,$1);}
  ;
