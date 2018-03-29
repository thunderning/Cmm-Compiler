%option yylineno
%{
  #include <stdlib.h>
%}
ws [ \t\n\r]+
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
{letter}+[_a-zA-Z0-9]* { printf("ID is %s\n",yytext);}
{ws} {  }
. {printf("Error type A at Line %d: Mysterious characters %d\n", yylineno, yytext[0]);}
%%
int main(int argc, char** argv){
  if(argc > 1){
    if(!(yyin = fopen(argv[1],"r"))){
      perror(argv[1]);
      return 1;
    }
    while(yylex() != 0);
    return 0;
  }
}
