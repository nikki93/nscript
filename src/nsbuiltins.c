/*
 * nsbuiltins.c
 */

#include <string.h>
#include <stdio.h>

#include <nscript.h>

void ns_add()
{
    struct ns_obj a = ns_pop();

    if (!(NS_ISNUM(a) && NS_ISNUM(ns_stack->obj)))
        ns_error("add: Attempted to add non-numbers!");

    if (a.type == TY_FLOAT && ns_stack->obj.type == TY_FLOAT)
        ns_stack->obj.u.fl += a.u.fl;
    else if (a.type == TY_INT && ns_stack->obj.type == TY_INT)
        ns_stack->obj.u.i += a.u.i;
    else if (a.type == TY_INT)
    {
        NS_INTTOFLOAT(a);
        ns_stack->obj.u.fl += a.u.fl;
    }
    else if (ns_stack->obj.type == TY_INT)
    {
        NS_INTTOFLOAT(ns_stack->obj);
        ns_stack->obj.u.fl += a.u.fl;
    }
}
/* ------------------ */
void ns_assign()
{
    struct ns_obj s = ns_pop();
    struct ns_obj o = ns_pop();

    if (!NS_ISSYM(s))
        ns_error("assign: Need a symbol to identify variable to assign to!");

    struct ns_obj *p = ns_searchNamespace(ns_currNamespace, s.u.sym->arr);

    /*
     * Function symbols are a special case.
     * Override symbol assignment and assign
     * the function object.
     */
    if(NS_ISSYM(o))
    {
        struct ns_obj *q = ns_searchNamespace(ns_builtinsSpace, o.u.sym->arr);
    
        if(q->type == TY_FUNC)
        {
                if(p->type != TY_EMPTY)
                {
                        p->type = TY_FUNC;
                        p->u.f = q->u.f;  
                }
                else
                        ns_addToNamespace(ns_currNamespace, s.u.sym->arr, ns_makeFuncObj(q->u.f));
                return;
        }
    }

    /*
     * Symbol assignment
     */
    if (p->type != TY_EMPTY)
        *p = o;
    else
        ns_addToNamespace(ns_currNamespace, s.u.sym->arr, o);
}
/* ------------------ */
void ns_at()
{
    struct ns_obj pos = ns_pop();

    if (pos.type != TY_INT)
        ns_error("at: Need integer position!");

    if (pos.u.i >= ns_stackSize)
        ns_error("at: Position out of bounds!");

    struct ns_stack *curr = ns_stack;
    while (pos.u.i--)
        curr = curr->next;

    ns_push(curr->obj);
}
/* ------------------ */
void ns_divide()
{
    struct ns_obj a = ns_pop();

    if (!(NS_ISNUM(a) && NS_ISNUM(ns_stack->obj)))
        ns_error("divide: Attempted to find the quotient of non-numbers!");

    if (a.type == TY_FLOAT && ns_stack->obj.type == TY_FLOAT)
    {
        if (a.u.fl == 0)
            ns_error("divide: Attempted divide by zero!");
        ns_stack->obj.u.fl /= a.u.fl;
    }
    else if (a.type == TY_INT && ns_stack->obj.type == TY_INT)
    {
        if (a.u.i == 0)
            ns_error("divide: Attempted divide by zero!");
        ns_stack->obj.u.i /= a.u.i;
    }
    else if (a.type == TY_INT)
    {
        if (a.u.fl == 0)
            ns_error("divide: Attempted divide by zero!");
        NS_INTTOFLOAT(a);
        ns_stack->obj.u.fl /= a.u.fl;
    }
    else if (ns_stack->obj.type == TY_INT)
    {
        if (a.u.fl == 0)
            ns_error("divide: Attempted divide by zero!");
        NS_INTTOFLOAT(ns_stack->obj);
        ns_stack->obj.u.fl /= a.u.fl;
    }
}
/* ------------------ */
void ns_dup()
{
    ns_push(ns_stack->obj);
}
/* ------------------ */
void ns_equals()
{
    struct ns_obj obj1 = ns_pop();
    struct ns_obj obj2 = ns_pop();
    struct ns_obj ans;
    ans.type = TY_BOOL;

    if (obj1.type != obj2.type)
        ans.u.bo = 0;
    else
        switch (obj1.type)
        {
            case TY_BOOL:
                ans.u.bo = obj2.u.bo == obj2.u.bo;
                break;

            case TY_FLOAT:
                ans.u.bo = obj1.u.fl == obj2.u.fl;
                break;

            case TY_INT:
                ans.u.bo = obj1.u.i == obj2.u.i;
                break;

            case TY_STR:
                ans.u.bo = !strcmp(obj1.u.s->arr, obj2.u.s->arr);
                break;

            case TY_FUNC:
                ans.u.bo = obj1.u.f == obj2.u.f;
                break;

            case TY_BLOCK:
                ans.u.bo = !strcmp(obj1.u.b.str->arr, obj2.u.b.str->arr);
                break;
        }

    ns_push(ans);
}
/* ------------------ */
void ns_exit()
{
    struct ns_obj code = ns_pop();

    if (code.type != TY_INT)
        ns_error("exit: Need an integer exit code!");

    exit(code.u.i);
}
/* ------------------ */
void ns_getchar()
{
    ns_push(ns_makeChar(getchar()));
}
/* ------------------ */
void ns_getline()
{
    struct ns_obj obj;
    obj.type = TY_STR;
    obj.u.s = dynarr_new();

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        dynarr_append(obj.u.s, (char) c);

    ns_push(obj);
}
/* ------------------ */
void ns_getch()
{
    struct ns_obj pos = ns_pop();
    struct ns_obj str = ns_pop();

    if (!NS_ISINT(pos))
        ns_error("getch: Need integer position!");
    if (!NS_ISSTR(str))
        ns_error("getch: Need string source!");

    ns_push(ns_makeChar(NS_STRFROMOBJ(str)[NS_INTFROMOBJ(pos)]));
}
/* ------------------ */
void ns_if()
{
    struct ns_obj code = ns_pop();
    struct ns_obj cond = ns_pop();

    if (cond.type != TY_BOOL)
        ns_error("if: Need boolean for condition!");
    if (code.type != TY_FUNC && code.type != TY_BLOCK)
        ns_error("if: Need an executable to run!");

    if (cond.u.bo)
        ns_execute(code);
}
/* ------------------ */
void ns_ifelse()
{
    struct ns_obj code2 = ns_pop();
    struct ns_obj code1 = ns_pop();
    struct ns_obj cond = ns_pop();

    if (cond.type != TY_BOOL)
        ns_error("ifelse: Need boolean for condition!");
    if (code1.type != TY_FUNC && code1.type != TY_BLOCK)
        ns_error("ifelse: Need an executable to run!");
    if (code2.type != TY_FUNC && code2.type != TY_BLOCK)
        ns_error("ifelse: Need an executable to run!");

    if (cond.u.bo)
        ns_execute(code1);
    else
        ns_execute(code2);
}
/* ------------------ */
void ns_len()
{
    struct ns_obj str = ns_pop();

    if (!NS_ISSTR(str))
        ns_error("len: Need a string!");

    ns_push(ns_makeIntObj(str.u.s->size - 1));
}
/* ------------------ */
void ns_multiply()
{
    struct ns_obj a = ns_pop();

    if (!(NS_ISNUM(a) && NS_ISNUM(ns_stack->obj)))
        ns_error("multiply: Attempted to multiply non-numbers!");

    if (a.type == TY_FLOAT && ns_stack->obj.type == TY_FLOAT)
        ns_stack->obj.u.fl *= a.u.fl;
    else if (a.type == TY_INT && ns_stack->obj.type == TY_INT)
        ns_stack->obj.u.i *= a.u.i;
    else if (a.type == TY_INT)
    {
        NS_INTTOFLOAT(a);
        ns_stack->obj.u.fl *= a.u.fl;
    }
    else if (ns_stack->obj.type == TY_INT)
    {
        NS_INTTOFLOAT(ns_stack->obj);
        ns_stack->obj.u.fl *= a.u.fl;
    }
}
/* ------------------ */
void ns_parentAssign()
{
    struct ns_obj s = ns_pop();
    struct ns_obj o = ns_pop();

    if (!NS_ISSYM(s))
        ns_error("parentAssign: Need a symbol to identify variable to assign to!");

    struct ns_obj *p = ns_searchNamespaceInherit(ns_currNamespace, s.u.sym->arr);

    /*
     * Function symbols are a special case.
     * Override symbol assignment and assign
     * the function object.
     */
    if(NS_ISSYM(o))
    {
        struct ns_obj *q = ns_searchNamespaceInherit(ns_builtinsSpace, o.u.sym->arr);
    
        if(q->type == TY_FUNC)
        {
                if(p->type != TY_EMPTY)
                {
                        p->type = TY_FUNC;
                        p->u.f = q->u.f;  
                }
                else
                        ns_addToNamespace(ns_currNamespace, s.u.sym->arr, ns_makeFuncObj(q->u.f));
                return;
        }
    }

    /*
     * Symbol assignment
     */
    if (p->type != TY_EMPTY)
        *p = o;
    else
        ns_addToNamespace(ns_currNamespace, s.u.sym->arr, o);
}
/* ------------------ */
void ns_print()
{
    struct ns_obj obj = ns_pop();
    switch (obj.type)
    {
        case TY_BOOL:
            if (obj.u.bo)
                printf("true");
            else
                printf("false");
            break;

        case TY_INT:
            printf("%d", obj.u.i);
            break;

        case TY_FLOAT:
            printf("%f", obj.u.fl);
            break;

        case TY_STR:
            printf("%s", obj.u.s->arr);
            break;

        case TY_FUNC:
            printf("C function object at %p", (void *)(size_t)obj.u.f);
            break;

        case TY_BLOCK:
            printf("{%s}", obj.u.b.str->arr);
            break;

        case TY_SYM:
            printf("&%s", obj.u.sym->arr);
            break;
    }
}
/* ------------------ */
void ns_printStack()
{
    int num = ns_stackSize;
    struct ns_stack *curr = ns_stack;
    while (num--)
    {
        printf("%d -> ", ns_stackSize - num - 1);
        ns_push(curr->obj);
        ns_print();
        printf("\n");
        curr = curr->next;
    }
    puts("--");
}
/* ------------------ */
void ns_repeat()
{
    struct ns_obj num = ns_pop();
    struct ns_obj func = ns_pop();

    if (num.type != TY_INT)
        ns_error("repeat: Need an integer repeat count!");
    if (!NS_ISEXECUTABLE(func))
        ns_error("repeat: Need an executable to run!");

    while (num.u.i--)
        ns_execute(func);
}
/* ------------------ */
void ns_rot()
{
    struct ns_stack *tmp1 = ns_stack->next;
    struct ns_stack *tmp2 = ns_stack;
    tmp2->next = tmp1->next;
    ns_stack = tmp1;
    ns_stack->next = tmp2;
}
/* ------------------ */
void ns_setch()
{
    struct ns_obj pos = ns_pop();
    struct ns_obj ch = ns_pop();
    struct ns_obj *str = &ns_stack->obj;

    if (!NS_ISINT(pos))
        ns_error("setch: Need integer position!");
    if (!NS_ISSTR(ch))
        ns_error("setch: Need string source!");
    if (!NS_ISSTR(*str))
        ns_error("setch: Need string destination!");

    str->u.s->arr[NS_INTFROMOBJ(pos)] = NS_STRFROMOBJ(ch)[0];
}
/* ------------------ */
void ns_subtract()
{
    struct ns_obj a = ns_pop();

    if (!(NS_ISNUM(a) && NS_ISNUM(ns_stack->obj)))
        ns_error("subtract: Attempted to subtract non-numbers!");

    if (a.type == TY_FLOAT && ns_stack->obj.type == TY_FLOAT)
        ns_stack->obj.u.fl -= a.u.fl;
    else if (a.type == TY_INT && ns_stack->obj.type == TY_INT)
        ns_stack->obj.u.i -= a.u.i;
    else if (a.type == TY_INT)
    {
        NS_INTTOFLOAT(a);
        ns_stack->obj.u.fl -= a.u.fl;
    }
    else if (ns_stack->obj.type == TY_INT)
    {
        NS_INTTOFLOAT(ns_stack->obj);
        ns_stack->obj.u.fl -= a.u.fl;
    }
}
/* ------------------ */
void ns_type()
{
    struct ns_obj obj = ns_pop();

    ns_push(ns_makeIntObj(obj.type));
}

/*
 * Export the C functions to nscript.
 */

struct ns_namemap ns_builtinsMap[] =
{
    /* Functions. */
    { "at", { TY_FUNC, { .f = ns_at } } },
    { "dup", { TY_FUNC, { .f = ns_dup } } },
    { "exit", { TY_FUNC, { .f = ns_exit } } },
    { "getchar", { TY_FUNC, { .f = ns_getchar } } },
    { "getline", { TY_FUNC, { .f = ns_getline } } },
    { "ifelse", { TY_FUNC, { .f = ns_ifelse } } },
    { "if", { TY_FUNC, { .f = ns_if } } },
    { "len", { TY_FUNC, { .f = ns_len } } },
    { "printStack", { TY_FUNC, { .f = ns_printStack } } },
    { "print", { TY_FUNC, { .f = ns_print } } },
    { "repeat", { TY_FUNC, { .f = ns_repeat } } },
    { "rot", { TY_FUNC, { .f = ns_rot } } },
    { "type", { TY_FUNC, { .f = ns_type } } },
    { "getch", { TY_FUNC, { .f = ns_getch } } },
    { "setch", { TY_FUNC, { .f = ns_setch } } },

    /* Operators. */
    { "+", { TY_FUNC, { .f = ns_add } } },
    { "-", { TY_FUNC, { .f = ns_subtract } } },
    { "*", { TY_FUNC, { .f = ns_multiply } } },
    { "/", { TY_FUNC, { .f = ns_divide } } },
    { "==", { TY_FUNC, { .f = ns_equals } } },
    { "=", { TY_FUNC, { .f = ns_assign } } },
    { "p=", { TY_FUNC, { .f = ns_parentAssign } } },

    /* Constants. */
    { "true", { TY_BOOL, { .bo = 1 } } },
    { "false", { TY_BOOL, { .bo = 0 } } },

    { "ty_empty", { TY_INT, { .i = TY_EMPTY } } },
    { "ty_bool", { TY_INT, { .i = TY_BOOL } } },
    { "ty_int", { TY_INT, { .i = TY_INT } } },
    { "ty_float", { TY_INT, { .i = TY_FLOAT } } },
    { "ty_str", { TY_INT, { .i = TY_STR } } },
    { "ty_func", { TY_INT, { .i = TY_FUNC } } },
    { "ty_block", { TY_INT, { .i = TY_BLOCK } } },

    { 0 }
};
/* ------------------ */
struct ns_namespace *ns_builtinsSpace;
/* ------------------ */
void ns_initBuiltinsSpace()
{
    ns_builtinsSpace = ns_newNamespace(0);
    ns_namemapToNamespace(ns_builtinsSpace, ns_builtinsMap);
}
