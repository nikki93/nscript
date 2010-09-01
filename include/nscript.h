/*
 * =====================================================================================
 *
 *       Filename:  nscript.h
 *
 *    Description:  Simple stack-based scripting language.
 *
 *        Created:  09/01/2010 11:49:53 AM
 *       Compiler:  gcc
 *
 *         Author:  Nikhilesh S (nikki)
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * dynarray.c
 */

#define DYNARRAY_RESIZE_FACTOR 2
typedef char dynarr_info_t;
struct dynarr
{
    unsigned int size;
    unsigned int alloc_size;
    dynarr_info_t *arr;
};

struct dynarr *dynarr_new();
struct dynarr *dynarr_new_alloc(unsigned int n);
void dynarr_free(struct dynarr *array);
int dynarr_append(struct dynarr *array, dynarr_info_t info);
int dynarr_insert(struct dynarr *array, unsigned int pos, dynarr_info_t info);
int dynarr_remove(struct dynarr *array, unsigned int pos);
int dynarr_resize_down(struct dynarr *array, unsigned int size);
int dynarr_resize_up(struct dynarr *array, unsigned int size);
int dynarr_resize_auto(struct dynarr *array, unsigned int size);
void dynarr_clear(struct dynarr *array);

/*
 * nsobj.c
 */

typedef void (*ns_cFunc) (void);

enum
{
    TY_EMPTY,
    TY_BOOL,
    TY_INT,
    TY_FLOAT,
    TY_STR,
    TY_FUNC,
    TY_BLOCK
};

struct ns_obj
{
    short type;
    union
    {
        int bo;                                //TY_BOOL
        int i;                                 //TY_INT
        double fl;                             //TY_FLOAT
        struct dynarr *s;                      //TY_STR
        ns_cFunc f;                            //TY_FUNC
        struct ns_block
        {
            struct dynarr *file;
            int lineNo;
            struct dynarr *str;                  
        } b;                                   //TY_BLOCK
    } u;
};

extern struct ns_obj ns_defaultObj;

struct ns_obj ns_makeBoolObj(int b);
struct ns_obj ns_makeIntObj(int i);
struct ns_obj ns_makeFloatObj(double fl);
struct ns_obj ns_makeStrObjLen(char *c, unsigned int len);
struct ns_obj ns_makeStrObj(char *c);
struct ns_obj ns_makeFuncObj(ns_cFunc f);
void ns_execute(struct ns_obj obj);

#define NS_BOOLFROMOBJ(obj) ((obj).u.bo)
#define NS_INTFROMOBJ(obj) ((obj).u.i)
#define NS_FLOATFROMOBJ(obj) ((obj).u.fl)
#define NS_STRFROMOBJ(obj) ((obj).u.s)
#define NS_FUNCFROMOBJ(obj) ((obj).u.f)

#define NS_ISBOOL(obj) ((obj).type == TY_BOOL)
#define NS_ISINT(obj) ((obj).type == TY_INT)
#define NS_ISFLOAT(obj) ((obj).type == TY_FLOAT)
#define NS_ISSTR(obj) ((obj).type == TY_STR)
#define NS_ISFUNC(obj) ((obj).type == TY_FUNC)
#define NS_ISBLOCK(obj) ((obj).type == TY_BLOCK)
#define NS_ISNUM(obj) ((obj).type == TY_INT || (obj).type == TY_FLOAT)
#define NS_ISEXECUTABLE(obj) ((obj).type == TY_FUNC || (obj).type == TY_BLOCK)

#define NS_INTTOFLOAT(obj) \
    do \
    { \
        (obj).type = TY_FLOAT; \
        NS_FLOATFROMOBJ(obj) = NS_INTFROMOBJ(obj); \
    } while(0)

/*
 * trie.c
 */

typedef struct ns_obj trie_info_t;

struct trie
{
    char initialised;
    trie_info_t info;
    struct trie *arr[256];
};

struct trie *trie_new();
void trie_free(struct trie *trie);
void trie_add(struct trie *trie, const char *c, trie_info_t info);
trie_info_t *trie_get(struct trie *trie, const char *c, trie_info_t *def);

/*
 * error
 */

#define ns_error(fmt, ...)               \
do                                       \
{                                        \
    fprintf(stderr, "\nError: ");        \
    fprintf(stderr, fmt, ##__VA_ARGS__); \
    putc('\n', stderr);                  \
    putc('\n', stderr);                  \
    ns_printContext();                   \
    exit(1);                             \
} while (0)

/*
 * nsstack.c
 */

struct ns_stack
{
    struct ns_obj obj;
    struct ns_stack *next;
} *ns_stack;
int ns_stackSize;

void ns_initStack();
struct ns_obj ns_pop();
void ns_push(struct ns_obj obj);

/*
 * nsnamespace.c
 */

struct ns_namespace
{
    struct trie *vars;
    struct ns_namespace *parent;
}; 
struct ns_namespace *ns_currNamespace;

struct ns_namespace *ns_newNamespace(struct ns_namespace *parent);
void ns_addToNamespace(struct ns_namespace *ns, const char *name, struct ns_obj obj);
struct ns_obj *ns_searchNamespace(struct ns_namespace *ns, const char *name);
struct ns_obj *ns_searchNamespaceInherit(struct ns_namespace *ns, const char *name);

struct ns_namemap
{
    char *key;
    struct ns_obj obj;
}; 

void ns_namemapToNamespace(struct ns_namespace *ns, struct ns_namemap *nm);

/*
 * nsinterpret.c
 */

struct ns_interpContext
{
    struct ns_interpContext *parent;
    const char *line;
    const char *filename;
    int lineNo;
} *ns_currContext;

struct ns_interpContext *ns_pushContext();
void ns_popContext();
void ns_printContext();

void ns_init();
void ns_interpret(const char *code, const char *filename, int lineNo, struct ns_namespace *ns);

#define ns_interpretInNamespace(code, filename, ns) \
    ns_interpret(code, filename, 1, ns)

#define ns_interpretInChild(code, filename, ns) \
    do \
    { \
        ns_interpret(code, filename, 1, ns_newNamespace(ns)); \
    } while (0)

#define ns_interpretInChildLine(code, filename, line, ns) \
    do \
    { \
        ns_interpret(code, filename, line, ns_newNamespace(ns)); \
    } while (0)

#define ns_interpretString(code, ns) \
    ns_interpret(code, "<string>", 1, ns)

#define ns_interpretStringInChild(code, ns) \
    do \
    { \
        ns_interpret(code, "<string>", 1, ns_newNamespace(ns)); \
    } while (0)

/*
 * nsbuiltins.c
 */

extern struct ns_namemap ns_builtinsMap[];
extern struct ns_namespace *ns_builtinsSpace;

void ns_initBuiltinsSpace();

void ns_print();
void ns_add();
void ns_subtract();
void ns_multiply();
void ns_divide();
void ns_exit();
void ns_repeat();
void ns_dup();
void ns_rot();
void ns_if();
void ns_ifelse();
void ns_equals();
void ns_printStack();
void ns_at();
void ns_getline();
void ns_getchar();
void ns_type();

