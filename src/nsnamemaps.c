/*
 * nsnamemaps.c
 */

#include <nsobj.h>
#include <nsfuncs.h>

#include <nsnamemaps.h>

/*
 * Actually, hashmaps could be used for 'pre-defined' things, with
 * a perfect hash function (possible keys are statically known).
 *
 * Anyway...
 */

struct trie *ns_functrie = 0;
struct trie *ns_operatortrie = 0;
struct trie *ns_constanttrie = 0;
/* ------------------ */
struct trie *ns_variabletrie = 0;
/* ------------------ */
struct ns_namemap ns_funcmap[] =                //Functions.
{
    { "print", { TY_FUNC, { .f = ns_print } } },
    { "add", { TY_FUNC, { .f = ns_add } } },
    { "subtract", { TY_FUNC, { .f = ns_subtract } } },
    { "multiply", { TY_FUNC, { .f = ns_multiply } } },
    { "divide", { TY_FUNC, { .f = ns_divide } } },
    { "exit", { TY_FUNC, { .f = ns_exit } } },
    { "repeat", { TY_FUNC, { .f = ns_repeat } } },
    { "dup", { TY_FUNC, { .f = ns_dup } } },
    { "rot", { TY_FUNC, { .f = ns_rot } } },
    { "if", { TY_FUNC, { .f = ns_if } } },
    { "ifelse", { TY_FUNC, { .f = ns_ifelse } } },
    { "equals", { TY_FUNC, { .f = ns_equals } } },
    { "printStack", { TY_FUNC, { .f = ns_printStack } } },
    { "at", { TY_FUNC, { .f = ns_at } } },
    { "getline", { TY_FUNC, { .f = ns_getline } } },
    { "getchar", { TY_FUNC, { .f = ns_getchar } } },
    { "type", { TY_FUNC, { .f = ns_type } } },

    { 0, { TY_EMPTY, { .i = 0 } } }
}, ns_operatormap[] =                           //Operators.
{
    { "+", { TY_FUNC, { .f = ns_add } } },
    { "-", { TY_FUNC, { .f = ns_subtract } } },
    { "*", { TY_FUNC, { .f = ns_multiply } } },
    { "/", { TY_FUNC, { .f = ns_divide } } },

    { "==", { TY_FUNC, { .f = ns_equals } } },

    { 0, { TY_EMPTY, { .i = 0 } } }
}, ns_constantmap[] =                           //Constants.
{
    { "true", { TY_BOOL, { .bo = 1 } } },
    { "false", { TY_BOOL, { .bo = 0 } } },

    { "ty_empty", { TY_INT, { .i = TY_EMPTY } } },
    { "ty_bool", { TY_INT, { .i = TY_BOOL } } },
    { "ty_int", { TY_INT, { .i = TY_INT } } },
    { "ty_float", { TY_INT, { .i = TY_FLOAT } } },
    { "ty_str", { TY_INT, { .i = TY_STR } } },
    { "ty_func", { TY_INT, { .i = TY_FUNC } } },
    { "ty_block", { TY_INT, { .i = TY_BLOCK } } },

    { 0, { TY_EMPTY, { .i = 0 } } }
};
/* ------------------ */
void ns_initNameMaps()
{
    struct ns_namemap *curr = ns_funcmap;
    ns_functrie = trie_new();
    for (; curr->key; ++curr)
        trie_add(ns_functrie, curr->key, curr->obj);

    curr = ns_operatormap;
    ns_operatortrie = trie_new();
    for (; curr->key; ++curr)
        trie_add(ns_operatortrie, curr->key, curr->obj);

    curr = ns_constantmap;
    ns_constanttrie = trie_new();
    for (; curr->key; ++curr)
        trie_add(ns_constanttrie, curr->key, curr->obj);
}
/* ------------------ */
int ns_isOperatorPrefix(char c)
{
    return ns_operatortrie->arr[c] ? 1 : 0;
}
/* ------------------ */
struct ns_obj ns_findFunc(const char *key)
{
    return trie_get(ns_functrie, key, ns_defaultObj);
}
/* ------------------ */
struct ns_obj ns_findOperator(const char *key)
{
    return trie_get(ns_operatortrie, key, ns_defaultObj);
}
/* ------------------ */
struct ns_obj ns_findConstant(const char *key)
{
    return trie_get(ns_constanttrie, key, ns_defaultObj);
}
/* ------------------ */
void ns_initVariableMap()
{
    ns_variabletrie = trie_new();
}
/* ------------------ */
struct ns_obj *ns_findVariable(const char *name)
{
    struct trie *trie = ns_variabletrie;
    for (; *name; ++name)
        if (!(trie = trie->arr[*name]))
            return &ns_defaultObj;

    return &(trie->info);
}
/* ------------------ */
void ns_addVariable(const char *name, struct ns_obj obj)
{
    trie_add(ns_variabletrie, name, obj);
}
