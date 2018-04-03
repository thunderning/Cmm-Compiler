%option yylineno
%{
  #include <stdlib.h>
  #include "df.h"
  #include "syntax.tab.h"
  int yycolumn = 1;
  #define YY_USER_ACTION \
  yylloc.first_line = yylloc.last_line = yylineno; \
   yylloc.first_column = yycolumn; \
   yylloc.last_column = yycolumn + yyleng - 1; \
   yycolumn += yyleng;
%}

ws [ \t\r]+
letter [_a-zA-Z]
digit [0-9]
oct [0-7]
hex [a-fA-F0-9]
%%

";" {printf("SEMI\n");}
"," {printf("COMMA\n");}
"=" {printf("ASSIGNOP\n");}
">" {printf("RELOP\n");}
"<" {printf("RELOP\n");}
">=" {printf("RELOP\n");}
"<=" {printf("RELOP\n");}
"==" {printf("RELOP\n");}
"!=" {printf("RELOP\n");}
"+" {printf("PLUS\n");}
"-" {printf("MINUS\n");}
"*" {printf("STAR\n");}
"/" {printf("DIV\n");}
"&&" {printf("AND\n");}
"||" {printf("OR\n");}
"." {printf("DOT\n");}
"!" {printf("NOT\n");}
int {printf("TYPE\n");}
float {printf("TYPE\n");}
"(" {printf("LP\n");}
")" {printf("RP\n");}
"[" {printf("LB\n");}
"]" {printf("RB\n");}
"{" {printf("LC\n");}
"}" {printf("RC\n");}
struct {printf("STRUCT\n");}
return {printf("RETURN\n");}
if {printf("IF\n");}
else {printf("ELSE\n");}
while {printf("WHILE\n");}
0|[1-9]{digit}* { printf("DEC Integer value %d\n", (int)(strtol(yytext,NULL,10))); }
0{oct}+ { printf("OCT Integer value %d\n", (int)(strtol(yytext,NULL,8))); }
0[xX]{hex}+ { printf("HEX Integer value %d\n", (int)(strtol(yytext,NULL,16))); }
{digit}+"."{digit}+ {printf("Float value %f\n", strtof(yytext,NULL));}
{letter}+[_a-zA-Z0-9]* { printf("ID is %s\n",yytext);}
{ws} {  }
[\n] {yycolumn = 1;}
. {printf("Error type A at Line %d: Mysterious characters %d\n", yylineno, yytext[0]);}
%%
