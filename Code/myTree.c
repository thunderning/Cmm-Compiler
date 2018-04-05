#include "myTree.h"
void insertNode(int num,...){
  va_list parg;
  va_start(parg, num);
  Node* pNode = NULL;
  Node* cNode = NULL;
  enum NodeType nt;
  pNode = va_arg(parg,Node*);
  nt = va_arg(parg,enum NodeType);    //前两个参数为根节点
  pNode->length = 0;
  pNode->type = nt;
  for(int i = 0;i < num;i++){
    cNode = va_arg(parg,Node*);
    //cNode->linenum = line;
    pNode->childNodes[pNode->length] = cNode;
    pNode->length ++;
  }
  if(num > 0) pNode->linenum = pNode->childNodes[0]->linenum;
  va_end(parg);
}

void printTree(Node* start,int depth){
  if (start->length == 0){
    if (start->type == TR_Program||start->type == TR_ExtDefList||start->type == TR_ExtDef||start->type == TR_ExtDecList||start->type == TR_Specifier||start->type == TR_StructSpecifier||
    start->type ==TR_OptTag||start->type == TR_Tag||start->type == TR_VarDec||start->type == TR_FunDec||start->type == TR_VarList||start->type == TR_ParamDec||start->type == TR_CompSt||
    start->type ==TR_StmtList||start->type == TR_Stmt||start->type == TR_DefList||start->type == TR_Def||start->type == TR_DecList||start->type == TR_Dec||start->type == TR_Exp||start->type == TR_Args) {
      return;
    }
  }
  for (size_t j = 0; j < depth; j++) {
    printf("  ");
  }
  printf("%s", Nodename[start->type]);
  switch (start->type) {
    case TR_INT:
      printf(" : %d", start->valInt);break;
    case TR_FLOAT:
      printf(" : %f", start->valFloat);break;
    case TR_ID:
      printf(" : %s", start->valString);break;
    case TR_TYPE:
      printf(" : %s", start->valInt == 1? "int" : "float");break;
  }
  if (start->type == TR_Program||start->type == TR_ExtDefList||start->type == TR_ExtDef||start->type == TR_ExtDecList||start->type == TR_Specifier||start->type == TR_StructSpecifier||
  start->type ==TR_OptTag||start->type == TR_Tag||start->type == TR_VarDec||start->type == TR_FunDec||start->type == TR_VarList||start->type == TR_ParamDec||start->type == TR_CompSt||
  start->type ==TR_StmtList||start->type == TR_Stmt||start->type == TR_DefList||start->type == TR_Def||start->type == TR_DecList||start->type == TR_Dec||start->type == TR_Exp||start->type == TR_Args) {
    printf(" (%d)",start->linenum );
  }
  printf("\n");
  if (start->length == 0) return;
  for (int i = 0; i < start->length; i++) {
    printTree(start->childNodes[i],depth+1);
  }
}
