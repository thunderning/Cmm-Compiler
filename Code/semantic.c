#include "semantic.h"
#include "symboltable.h"

void Program(Node* start){
    stackAdd();
    ExtDefList(start->childNodes[0]);
    stackDelete();
}

void ExtDefList(Node* node){
    if(NULL == node)
        return;

    // ExtDefList -> NULL
    if(0 == node->length)
        return;

    // ExtDefList -> ExtDef ExtDefList
    ExtDef(node->childNodes[0]);
    ExtDefList(node->childNodes[1]);
}

void ExtDef(Node *node)
{
    if(NULL == node)
        return;
    switch (node->childNodes[1]->type) {
    case TR_ExtDecList:{
        //ExtDef -> Specifier ExtDecList SEMI
        Type *type = Specifier(node->childNodes[0],ROOT_USE);
        ExtDecList(node->childNodes[1],type);
        break;
    }
    case TR_SEMI:{
        //ExtDef -> Specifier SEMI
        Type *type = Specifier(node->childNodes[0],ROOT_DEFINE);
        break;
    }
    case TR_FunDec:{
        //ExtDef -> Specifier FunDec CompSt
        Type *type = Specifier(node->childNodes[0],ROOT_USE);
        FunDec(node->childNodes[1],type);
        CompSt(node->childNodes[2],type);
        break;
    }
    default:
        assert(0);
        break;
    }
    return;
}

void ExtDecList(Node *node, Type *type)
{
    if(NULL == node)
        return;
    FieldList *fl = VarDec(node->childNodes[0],type);
    Symbol *sb = stSearch(fl->name);
    if(sb != NULL && sb->depth == currentDepth){
        //符号表中存在并且与当前深度相同
        ErrorHandle(3,node->linenum,fl->name);
    }
    else{
        //加入新符号
        Symbol *newSb = malloc(sizeof(Symbol));
        newSb->depth = currentDepth;
        newSb->kind = Variable;
        newSb->lineNum = node->linenum;
        newSb->variableMessage = fl->type;
        strcpy(newSb->name,fl->name);
        stAdd(newSb);
    }
    switch (node->length) {
    case 1:
        //ExtDecList -> VarDec
        break;
    case 3:
        //ExtDecList -> VarDec COMMA ExtDecList
        ExtDecList(node->childNodes[2],type);
        break;
    default:
        assert(0);
        break;
    }
    return;
}

Type* Specifier(Node *node, int root)
{
    if(NULL == node)
        return NULL;
    Node *child = node->childNodes[0];
    switch (child->type) {
    case TR_TYPE:{
        //Specifier -> TYPE
        Type *ret = malloc(sizeof(Type));
        ret->kind = BASIC;
        ret->u.basic = child->valInt;
        return ret;
        break;
    }
    case TR_StructSpecifier:
        //Specifier -> StructSpecifier
        return StructSpecifier(node->childNodes[0],root);
        break;
    default:
        assert(0);
        break;
    }

}

Type *StructSpecifier(Node *node,int root)
{
    if(NULL == node)
        return NULL;
    switch (node->length) {
    case 2:{
        //StructSpecifier -> STRUCT Tag

        char *n = Tag(node->childNodes[1]);
        if(ROOT_DEFINE == root){
            if(n != NULL){
                Symbol *sb = stSearch(n);
                if(sb != NULL && currentDepth == sb->depth){
                    ErrorHandle(16,node->linenum,n);
                }
                else{
                    Symbol *newSb = malloc(sizeof(Symbol));
                    newSb->depth = currentDepth;
                    newSb->kind = Struct;
                    newSb->lineNum = node->linenum;
                    newSb->variableMessage = NULL;
                    strcpy(newSb->name,n);
                    stAdd(newSb);
                }
            }
        }
        else if(ROOT_USE == root){
            if(n != NULL){
                Symbol *sb = stSearch(n);
                if(sb == NULL){
                    ErrorHandle(17,node->linenum,n);
                }
                else{
                    return sb->variableMessage;
                }
            }
        }
        break;
    }
    case 5:{
        //StructSpecifier -> STRUCT OptTag LC DefList RC
        char *n = OptTag(node->childNodes[1]);
        if(n != NULL){
            Symbol *sb = stSearch(n);
            if(sb != NULL && currentDepth == sb->depth){
                ErrorHandle(16,node->linenum,n);
                return NULL;
            }
            else{
                Type *t = malloc(sizeof(Type));
                t->kind = STRUCTURE;
                stackAdd();
                t->u.structure.fl = DefList(node->childNodes[3],ROOT_STRUCT);
                t->u.structure.name = n;
                stackDelete();
                Symbol *newSb = malloc(sizeof(Symbol));
                newSb->depth = currentDepth;
                newSb->kind = Struct;
                newSb->lineNum = node->linenum;
                newSb->variableMessage = t;
                strcpy(newSb->name,n);
                stAdd(newSb);
                return t;
            }
        }
        break;
    }
    default:
        assert(0);
        break;
    }
}

char *OptTag(Node *node)
{
    if(NULL == node)
        return NULL;
    switch (node->length) {
    case 0:
        //OptTag -> NULL
        return NULL;
        break;
    case 1:
        //OptTag -> ID
        return node->childNodes[0]->valString;
    default:
        break;
    }
}

char *Tag(Node *node)
{
    if(NULL == node)
        return NULL;
    //Tag -> ID;
    return node->childNodes[0]->valString;
}

FieldList* VarDec(Node *node, Type* type)
{
    if(NULL == node)
        return NULL;
    FieldList *fl = malloc(sizeof(FieldList));
    switch (node->length) {
    case 1:
        //VarDec -> ID
        fl->name = node->childNodes[0]->valString;
        fl->tail = NULL;
        fl->type = type;
        return fl;
        break;
    case 4:{
        //VarDec -> VarDec LB INT RB
        FieldList *fltemp = VarDec(node->childNodes[0],type);
        if(fltemp != NULL){
            fl->name = fltemp->name;
            fl->tail = NULL;
            Type *t = malloc(sizeof(Type));
            t->kind = ARRAY;
            t->u.array.elem = fltemp->type;
            t->u.array.size = node->childNodes[2]->valInt;
            fl->type = t;
            return fl;
        }
        break;
    }
    default:
        break;
    }
    return NULL;
}

void FunDec(Node *node, Type* type)
{

    if(NULL == node)
        return;
    Symbol *sb = stSearch(node->childNodes[0]->valString);
    if(sb != NULL && sb->depth == currentDepth){
        ErrorHandle(4,node->linenum,sb->name);
        stackAdd();
        switch (node->length) {
        case 3:
            //FunDec -> ID LP RP
            break;
        case 4:
            //FunDec -> ID LP VarList RP
            VarList(node->childNodes[2]);
        default:
            break;
        }
        return;
    }
    Symbol *newSb = malloc(sizeof(Symbol));
    newSb->depth = currentDepth;
    newSb->kind = Func;
    newSb->lineNum = node->linenum;
    strcpy(newSb->name,node->childNodes[0]->valString);
    FuncMessage *fm = malloc(sizeof(FuncMessage));
    fm->returnType = type;
    stAdd(newSb);
    //在新的深度记录参数，在大括号结束时退栈
    stackAdd();
    switch (node->length) {
    case 3:
        fm->input = NULL;
        //FunDec -> ID LP RP
        break;
    case 4:
        //FunDec -> ID LP VarList RP
        fm->input = VarList(node->childNodes[2]);
    default:
        break;
    }
    newSb->funcMessage = fm;
    return;
}

FieldList* VarList(Node *node)
{
    if(NULL == node)
        return NULL;
    switch (node->length) {
    case 1:
        //VarList -> ParamDec
        return ParamDec(node->childNodes[0]);
        break;
    case 3:{
        //VarList -> ParamDec COMMA VarList
        FieldList *fl = ParamDec(node->childNodes[0]);
        fl->tail = VarList(node->childNodes[2]);
        return fl;
    }
    default:
        break;
    }
}

FieldList* ParamDec(Node *node)
{
    if(NULL == node)
        return NULL;
    //ParamDec -> Specifier VarDec
    Type *t = Specifier(node->childNodes[0],ROOT_USE);
    FieldList *fl = VarDec(node->childNodes[1],t);
    Symbol *sb = stSearch(fl->name);
    if(sb != NULL && sb->depth == currentDepth){
        //符号表中存在并且与当前深度相同
        ErrorHandle(3,node->linenum,fl->name);
    }
    else{
        //加入新符号
        Symbol *newSb = malloc(sizeof(Symbol));
        newSb->depth = currentDepth;
        newSb->kind = Variable;
        newSb->lineNum = node->linenum;
        newSb->variableMessage = fl->type;
        strcpy(newSb->name,fl->name);
        stAdd(newSb);
    }
    return fl;
}

void CompSt(Node *node, Type* retype)
{
    if(NULL == node)
        return;
    //CompSt -> LC DefList StmtList RC
    DefList(node->childNodes[1],ROOT_COMPST);
    StmtList(node->childNodes[2],retype);
    //退栈
    stackDelete();
}

void StmtList(Node *node, Type* retype)
{
    if(NULL == node)
        return;
    switch (node->length) {
    case 0:
        //StmtList -> NULL
        return;
        break;
    case 2:
        //StmtList -> Stmt StmtList
        Stmt(node->childNodes[0],retype);
        StmtList(node->childNodes[1],retype);
        return;
    default:
        break;
    }
}

void Stmt(Node *node, Type* retype)
{
    if(NULL == node)
        return;
    switch (node->length) {
    case 1:
        //Stmt -> CompSt
        stackAdd();
        CompSt(node->childNodes[0],retype);
        break;
    case 2:
        //Stmt -> Exp SEMI
        Exp(node->childNodes[0],0,NULL);
        break;
    case 3:{
        //Stmt -> RETURN Exp SEMI
        Type *t = Exp(node->childNodes[1],0,NULL);
        if(!TypeEqual(t,retype))
            ErrorHandle(8,node->linenum,"");
        break;
    }
    case 5:{
        //Stmt -> IF LP Exp RP Stmt
        //Stmt -> WHILE LP Exp RP Stmt
        Type *t = Exp(node->childNodes[2],0,NULL);
        if(t == NULL)
            return;
        if(t->kind != BASIC || t->u.basic == 0)
            ErrorHandle(7,node->linenum,"");
        Stmt(node->childNodes[4],retype);
        break;
    }
    case 7:{
        //Stmt -> IF LP Exp RP Stmt ELSE Stmt
        Type *t = Exp(node->childNodes[2],0,NULL);
        if(t == NULL)
            return;
        if(t->kind != BASIC || t->u.basic == 0)
            ErrorHandle(7,node->linenum,"");
        Stmt(node->childNodes[4],retype);
        Stmt(node->childNodes[6],retype);
        break;
    }
    default:
        break;
    }
}

FieldList *DefList(Node *node, int root)
{
    if(NULL == node)
        return NULL;
    switch (node->length) {
    case 0:
        //DefList -> NULL
        return NULL;
        break;
    case 2:{
        //DefList -> Def DefList
        FieldList *fl1 = Def(node->childNodes[0],root);
        FieldList *fl2 = DefList(node->childNodes[1],root);
        FieldList *p = fl1;
        if(p == NULL)
            return fl2;
        for(;p->tail != NULL;p = p->tail);
        p->tail = fl2;
        return fl1;
        break;
    }
    default:
        break;
    }
}

FieldList *Def(Node *node, int root)
{
    if(NULL == node)
        return NULL;
    Type *t = Specifier(node->childNodes[0],ROOT_USE);
    FieldList *fl = DecList(node->childNodes[1],t,root);
    return fl;
    //Def -> Specifier DecList SEMI
}

FieldList *DecList(Node *node, Type* type, int root)
{
    if(NULL == node)
        return NULL;
    FieldList *fl1 = Dec(node->childNodes[0],type,root);
    switch (node->length) {
    case 1:
        //DecList -> Dec
        return fl1;
        break;
    case 3:{
        //DecList -> Dec COMMA DecList
        FieldList *fl2 = DecList(node->childNodes[2],type,root);
        if(fl1 == NULL)
            return fl2;
        fl1->tail = fl2;
        return fl1;
        break;
    }
    default:
        break;
    }
}

FieldList *Dec(Node *node, Type* type, int root)
{
    if(NULL == node)
        return NULL;
    FieldList *fl = VarDec(node->childNodes[0],type);
    Symbol *sb = stSearch(fl->name);
    if(sb != NULL && sb->depth == currentDepth){
        if(ROOT_STRUCT == root){
            ErrorHandle(15,node->linenum,fl->name);
        }
        else if(ROOT_COMPST == root){
            ErrorHandle(3,node->linenum,fl->name);
        }
        return NULL;
    }
    Symbol *newSb = malloc(sizeof(Symbol));
    newSb->depth = currentDepth;
    newSb->kind = Variable;
    newSb->lineNum = node->linenum;
    newSb->variableMessage = fl->type;
    strcpy(newSb->name,fl->name);
    stAdd(newSb);
    switch (node->length) {
    case 1:
        //Dec -> VarDec
        return fl;
        break;
    case 3:{
        //Dec -> VarDec ASSIGNOP Exp
        Type *t = Exp(node->childNodes[2],0,NULL);
        if(!TypeEqual(fl->type,t))
            ErrorHandle(5,node->linenum,"");
        if(ROOT_STRUCT == root)
            ErrorHandle(15,node->linenum,fl->name);
        return fl;
        break;
    }
    default:
        break;
    }
}

Type* Exp(Node *node, int ifLeft, int *retLeft)
{
    if(NULL == node)
        return NULL;
    Type *ret = NULL;
    switch (node->length) {
    case 1:
        switch (node->childNodes[0]->type) {
        case TR_ID:{
            //Exp -> ID
            Symbol *sb = stSearch(node->childNodes[0]->valString);
            if(sb == NULL)
                ErrorHandle(1,node->linenum,node->childNodes[0]->valString);
            else
                ret = sb->variableMessage;
            if(1 == ifLeft)
                *retLeft = 1;
            break;
        }
        case TR_INT:
            //Exp -> INT
            ret = malloc(sizeof(Type));
            ret->kind = BASIC;
            ret->u.basic = 1;
            break;
        case TR_FLOAT:
            //Exp -> FLOAT
            ret = malloc(sizeof(Type));
            ret->kind = BASIC;
            ret->u.basic = 0;
            break;
        default:
            break;
        }
        break;
    case 2:
        ret = Exp(node->childNodes[1],0,NULL);
        switch (node->childNodes[0]->type) {
        case TR_MINUS:
            //Exp -> MINUS Exp
            if(ret->kind != BASIC)
                ErrorHandle(7,node->linenum,"");
            break;
        case TR_NOT:
            //Exp -> NOT Exp
            if(ret->kind != BASIC || ret->u.basic == 0)
                ErrorHandle(7,node->linenum,"");
            break;
        default:
            break;
        }
        break;
    case 3:
        switch (node->childNodes[1]->type) {
        case TR_ASSIGNOP:{
            //Exp -> Exp ASSIGNOP Exp
            int isleft = 0;
            Type *t1 = Exp(node->childNodes[0],1,&isleft);
            Type *t2 = Exp(node->childNodes[2],0,NULL);
            if(isleft == 0)
                ErrorHandle(6,node->linenum,"");
            if(!TypeEqual(t1,t2))
                ErrorHandle(5,node->linenum,"");
            ret = t1;
            break;
        }
        case TR_AND:
            //Exp -> Exp AND Exp
        case TR_OR:{
            //Exp -> Exp OR Exp
            Type *t1 = Exp(node->childNodes[0],0,NULL);
            Type *t2 = Exp(node->childNodes[2],0,NULL);
            if(t1->u.basic != 1 || t2->u.basic != 1)
                ErrorHandle(7,node->linenum,"");
            ret = t1;
            break;
        }
        case TR_RELOP:
            //Exp -> Exp RELOP Exp
        case TR_PLUS:
            //Exp -> Exp PLUS Exp
        case TR_MINUS:
            //Exp -> Exp MINUS Exp
        case TR_STAR:
            //Exp -> Exp STAR Exp
        case TR_DIV:{
            //Exp -> Exp DIV Exp
            Type *t1 = Exp(node->childNodes[0],0,NULL);
            Type *t2 = Exp(node->childNodes[2],0,NULL);
            if(t1 == NULL || t2 == NULL || t1->kind != BASIC || t2->kind != BASIC || TypeEqual(t1,t2) == 0)
                ErrorHandle(7,node->linenum,"");
            ret = t1;
            break;
        }
        case TR_Exp:{
            //Exp -> LP Exp RP
            Type *t1 = Exp(node->childNodes[0],0,NULL);
            if(t1->kind != BASIC)
                ErrorHandle(7,node->linenum,"");
            ret = t1;
            break;
        }
        case TR_LP:{
            //Exp -> ID LP RP
            Symbol *sb = stSearch(node->childNodes[0]->valString);
            if(sb == NULL)
                ErrorHandle(2,node->linenum,node->childNodes[0]->valString);
            else if(sb->kind != Func)
                ErrorHandle(11,node->linenum,node->childNodes[0]->valString);
            else
                ret = sb->funcMessage->returnType;
            break;
        }
        case TR_DOT:{
            //Exp -> Exp DOT ID
            Type *t = Exp(node->childNodes[0],0,NULL);
            if(t->kind != STRUCTURE)
                ErrorHandle(13,node->linenum,"");
            else{
                int flag = 0;
                FieldList *p;
                for(p = t->u.structure.fl;p != NULL;p = p->tail)
                    if(strcmp(p->name,node->childNodes[2]->valString) == 0){
                        flag = 1;
                        break;
                    }
                if(0 == flag)
                    ErrorHandle(14,node->linenum,node->childNodes[2]->valString);
                else
                    ret = p->type;
            }
            if(1 == ifLeft)
                *retLeft = 1;
            break;
        }
        default:
            break;
        }
        break;
    case 4:
        switch (node->childNodes[0]->type) {
        case TR_ID:{
            //Exp -> ID LP Args RP
            Symbol *sb = stSearch(node->childNodes[0]->valString);
            if(sb == NULL)
                ErrorHandle(2,node->linenum,node->childNodes[0]->valString);
            else if(sb->kind != Func)
                ErrorHandle(11,node->linenum,node->childNodes[0]->valString);
            else{
                FieldList *fl1 = sb->funcMessage->input;
                FieldList *fl2 = Args(node->childNodes[2]);
                int flag = 0;
                while(fl1 != NULL && fl2 != NULL){
                    if(!TypeEqual(fl1->type,fl2->type))
                        flag = 1;
                    fl1 = fl1->tail;
                    fl2 = fl2->tail;
                }
                if(fl1 != fl2)
                    flag = 1;
                if(1 == flag)
                    ErrorHandle(9,node->linenum,node->childNodes[0]->valString);
                else
                    ret = sb->funcMessage->returnType;
            }
            break;
        }
        case TR_Exp:{
            //Exp -> Exp LB Exp RB
            Type *t1 = Exp(node->childNodes[0],0,NULL);
            Type *t2 = Exp(node->childNodes[2],0,NULL);
            if(t1->kind != ARRAY)
                ErrorHandle(10,node->linenum,"");
            else if(t2->u.basic != 1)
                ErrorHandle(12,node->linenum,"");
            else
                ret = t1->u.array.elem;
            if(1 == ifLeft)
                *retLeft = 1;
            break;
        }
        default:
            break;
        }
        break;
    default:
        break;
    }
    return ret;
}

FieldList *Args(Node *node)
{
    if(NULL == node)
        return NULL;
    FieldList *fl = malloc(sizeof(FieldList));
    fl->tail = NULL;
    fl->type = Exp(node->childNodes[0],0,NULL);
    switch (node->length) {
    case 1:
        //Args -> Exp
        break;
    case 3:
        //Args -> Exp COMMA Args
        fl->tail = Args(node->childNodes[2]);
        break;
    default:
        break;
    }
    return fl;

}

void ErrorHandle(int type, int line, char *info)
{
    switch (type) {
    case 1:
        printf("Error type 1 at Line %d: Undefined variable \"%s\"\n",line,info);
        break;
    case 2:
        printf("Error type 2 at Line %d: Undefined function \"%s\"\n",line,info);
        break;
    case 3:
        printf("Error type 3 at Line %d: Redefined variable \"%s\"\n",line,info);
        break;
    case 4:
        printf("Error type 4 at Line %d: Redefined function \"%s\"\n",line,info);
        break;
    case 5:
        printf("Error type 5 at Line %d: Type mismatched for assignment\n",line);
        break;
    case 6:
        printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable\n",line);
        break;
    case 7:
        printf("Error type 7 at Line %d: Type mismatched for operands\n",line);
        break;
    case 8:
        printf("Error type 8 at Line %d: Type mismatched for return\n",line);
        break;
    case 9:
        printf("Error type 9 at Line %d: Function \"%s\" is not applicable for arguments\n",line,info);
        break;
    case 10:
        printf("Error type 10 at Line %d: Not an array\n",line);
        break;
    case 11:
        printf("Error type 11 at Line %d: \"%s\" is not a function\n",line,info);
        break;
    case 12:
        printf("Error type 12 at Line %d: Not an integer\n",line);
        break;
    case 13:
        printf("Error type 13 at Line %d: Illegal use of \".\"\n",line);
        break;
    case 14:
        printf("Error type 14 at Line %d: Non-existent field \"%s\"\n",line,info);
        break;
    case 15:
        printf("Error type 15 at Line %d: Redefined or initial field \"%s\"\n",line,info);
        break;
    case 16:
        printf("Error type 16 at Line %d: Duplicated name \"%s\"\n",line,info);
        break;
    case 17:
        printf("Error type 17 at Line %d: Undefined structure \"%s\"\n",line,info);
        break;
    default:
        assert(0);
        break;
    }
}

int TypeEqual(Type *t1, Type *t2)
{
    if(t1 == NULL || t2 == NULL)
        return 0;
    if(t1->kind == t2->kind){
        switch (t1->kind) {
        case BASIC:
            if(t1->u.basic == t2->u.basic)
                return 1;
            break;
        case ARRAY:
            if(TypeEqual(t1->u.array.elem,t2->u.array.elem))
                return 1;
            break;
        case STRUCTURE:
            if(strcmp(t1->u.structure.name,t2->u.structure.name) == 0)
                return 1;
            break;
        default:
            break;
        }
    }
    return 0;
}
