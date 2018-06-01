/* 包含的头文件 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"

extern int currentDepth = 0;

void stackLink(StackList **headStack, Symbol *node)
{
    Symbol **head = &((*headStack)->firstSymbol);
    /* 判断链表是否为空 */
    if(NULL == *head)
    {
        /* 为空 */
        *head = node;
        (*head)->stackNext = NULL;
    }
    else
    {
        /* 不为空 */
        node->stackNext = *head;
        *head = node;
    }
}

void hashLink(Symbol **head, Symbol *node)
{
    /* 判断链表是否为空 */
    if(NULL == *head)
    {
        /* 为空 */
        *head = node;
        (*head)->hashNext = NULL;
        (*head)->hashLast = NULL;
    }
    else
    {
        /* 不为空 */
        node->hashNext = *head;
        (*head)->hashLast = node;
        node->hashLast = NULL;
        *head = node;
    }
}

void stackAdd()
{
  //printf("add!\n");
  if (NULL == stackHead) {
    stackHead = malloc(sizeof(StackList));
    stackHead->firstSymbol = NULL;
    stackHead->next = NULL;
    stackTail = stackHead;
  }
  else{
    stackTail->next =  malloc(sizeof(StackList));
    stackTail = stackTail->next;
    stackTail->firstSymbol = NULL;
    stackTail->next = NULL;
  }
  currentDepth++;
}

void hashDelete(Symbol *node)
{
    if(node->hashLast == NULL){
        symbleTable[countHash(node->name)] = node->hashNext;
    }
    else{
        node->hashLast->hashNext = node->hashNext;
    }
    free(node);
}

void stackDelete()
{
  Symbol *p = stackTail->firstSymbol;
  while (p) {
    Symbol *temp = p;
    p = p->stackNext;
    hashDelete(temp);
  }
  if(stackHead == stackTail){
    free(stackHead);
  }
  else{
    StackList *q;
    for(q = stackHead; q->next != stackTail; q = q -> next);
    free(stackTail);
    stackTail = q;
    q->next = NULL;
  }
  currentDepth--;
  //printf("delete!\n");

}
//计算hash值
int countHash(char *name)
{
  int ret = 0;
  int i = 0;
  while (name[i]!= '\0') {
    ret += (int)name[i];
    i++;
  }
  return ret % ST_MAX;
}

void stAdd(Symbol *node)
{
  int hash_value = countHash(node -> name);
  if (stackTail == NULL) {
    stackAdd();
  }
  hashLink(&symbleTable[hash_value],node);
  stackLink(&stackTail,node);
}

Symbol* stSearch(char *name)
{
  int hash_value = countHash(name);
  Symbol *ret= symbleTable[hash_value];
  while (ret != NULL && strcmp(ret->name,name) != 0) {
    ret = ret->hashNext;
  }
  return ret;
}

void printSymbol(){
    StackList *p = stackHead;
    while (p) {
        Symbol *q = p->firstSymbol;
        while (q) {
            printf("%s -> ",q->name);
            q = q -> stackNext;
        }
        printf("\n");
        p = p->next;
    }
}
