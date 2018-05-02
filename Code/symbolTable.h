#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "type.h"
typedef struct symbol_ Symbol;
typedef struct FuncMessage_ FuncMessage;
typedef struct stacklist StackList;
#define ST_MAX 256

/* 定义一个表示链表的结构体指针 */
struct symbol_ {
    int depth;
    char* name;
    enum{Func,Variable,Struct} kind;
    int defTag;//用于判断函数是否被定义
    int lineNum;
    FuncMessage* funcMessage;
    Type* variableMessage;
    //哈希表中解决哈希冲突的链表指针
    Symbol* hashLast;
    Symbol* hashNext;
    //维护变量每一层作用域的链表指针
    Symbol* stackNext;
};

struct FuncMessage_
{
    Type* returnType;//返回值类型
    FieldList* input;//参数
};

struct stacklist {
  Symbol* firstSymbol;
  StackList *next;
};
//open hash
Symbol* symbleTable[ST_MAX];
//stack
StackList *stackHead,*stackTail;
//当前深度
int currentDepth;
//压栈
void stackAdd();
//弹栈
void stackDelete();
//向符号表中添加符号
void stAdd(Symbol *node);
//符号表中搜索符号
Symbol* stSearch(char *name);
//打印符号表，调试需要
void printSymbol();

#endif // SYMBOLTABLE_H
