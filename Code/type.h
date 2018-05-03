#ifndef TYPE_H
#define TYPE_H
#define ST_MAX 256
typedef struct Type_ Type;
typedef struct FieldList_ FieldList;
struct Type_
{
  enum { BASIC, ARRAY, STRUCTURE } kind;
  union
  {
    // 基本类型 1为int 0为float
    int basic;
    // 数组类型信息包括元素类型与数组大小构成
    struct { Type* elem; int size; } array;
    // 结构体类型信息是一个链表
    struct {char *name; FieldList *fl;} structure;
  } u;
};

struct FieldList_
{
  char* name; // 域的名字
  Type* type; // 域的类型
  FieldList* tail; // 下一个域
};


#endif // TYPE_H
