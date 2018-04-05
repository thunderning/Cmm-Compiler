#include <stdlib.h>
#include <stdio.h>
#include "myTree.h"
extern FILE* yyin;
extern Node* startNode;
extern int iserror;

int main(int argc, char** argv){
  if (argc <= 1 ) return 1;
  FILE* f = fopen(argv[1], "r");
  if (!f)
  {
    perror(argv[1]);
    return 1;
  }
  yyrestart(f);
  yyparse();
  if(iserror == 0) printTree(startNode,0);
  return 0;
}
