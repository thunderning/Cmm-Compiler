#include <stdlib.h>
#include <stdio.h>
#include "myTree.h"
extern FILE* yyin;
extern Node* startNode;

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
  //if(startNode != NULL) printTree(startNode);
  return 0;
}
