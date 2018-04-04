%option yylineno
%{
  #include <stdlib.h>
  #include <string.h>
  #define YYSTYPE Node*
  #include "syntax.tab.h"
  #define YY_USER_ACTION \
    yylval = malloc(sizeof(Node)); \
    yylval->linenum = yylineno; \
    yylval->length = 0;
%}

ws [ \t\r\n]+
letter [_a-zA-Z]
digit [0-9]
oct [0-7]
hex [a-fA-F0-9]
%%

";" {yylval->type = TR_SEMI;return SEMI;}
"," {yylval->type = TR_COMMA;return COMMA;}
"=" {yylval->type = TR_ASSIGNOP;return ASSIGNOP;}
">" {yylval->type = TR_RELOP;return RELOP;}
"<" {yylval->type = TR_RELOP;return RELOP;}
">=" {yylval->type = TR_RELOP;return RELOP;}
"<=" {yylval->type = TR_RELOP;return RELOP;}
"==" {yylval->type = TR_RELOP;return RELOP;}
"!=" {yylval->type = TR_RELOP;return RELOP;}
"+" {yylval->type = TR_PLUS;return PLUS;}
"-" {yylval->type = TR_MINUS;return MINUS;}
"*" {yylval->type = TR_STAR;return STAR;}
"/" {yylval->type = TR_DIV;return DIV;}
"&&" {yylval->type = TR_AND;return AND;}
"||" {yylval->type = TR_OR;return OR;}
"." {yylval->type = TR_DOT;return DOT;}
"!" {yylval->type = TR_NOT;return NOT;}
int {yylval->type = TR_TYPE;yylval->valInt = 1;return TYPE;}
float {yylval->type = TR_TYPE;yylval->valInt = 0;return TYPE;}
"(" {yylval->type = TR_LP;return LP;}
")" {yylval->type = TR_RP;return RP;}
"[" {yylval->type = TR_LB;return LB;}
"]" {yylval->type = TR_RB;return RB;}
"{" {yylval->type = TR_LC;return LC;}
"}" {yylval->type = TR_RC;return RC;}
struct {yylval->type = TR_STRUCT;return STRUCT;}
return {yylval->type = TR_RETURN;return RETURN;}
if {yylval->type = TR_IF;return IF;}
else {yylval->type = TR_ELSE;return ELSE;}
while {yylval->type = TR_WHILE;return WHILE;}
0|[1-9]{digit}* { yylval->valInt = (int)(strtol(yytext,NULL,10)); yylval->type = TR_INT;return INT; }
0{oct}+ { yylval->valInt =  (int)(strtol(yytext,NULL,8)); yylval->type = TR_INT;return INT; }
0[xX]{hex}+ {  yylval->valInt = (int)(strtol(yytext,NULL,16)); yylval->type = TR_INT;return INT; }
{digit}+"."{digit}+ {yylval->valFloat = strtof(yytext,NULL); yylval->type = TR_FLOAT;return FLOAT;}
{letter}+[_a-zA-Z0-9]* { strcpy(yylval->valString,yytext); yylval->type = TR_ID;return ID;}
{ws} {  }
. {printf("Error type A at Line %d: Mysterious characters %d\n", yylineno, yytext[0]);}
%%
