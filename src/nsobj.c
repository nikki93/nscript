/*
 * nsobj.c
 */

#include <string.h>

#include <nscript.h>

struct ns_obj ns_defaultObj = { TY_EMPTY, { .i = 0 } };
/* ------------------ */
struct ns_obj ns_makeBoolObj(int b)
{
    struct ns_obj obj = { TY_BOOL, { .bo = b } };
    return obj;
}
/* ------------------ */
struct ns_obj ns_makeIntObj(int i)
{
    struct ns_obj obj = { TY_INT, { .i = i } };
    return obj;
}
/* ------------------ */
struct ns_obj ns_makeFloatObj(double fl)
{
    struct ns_obj obj = { TY_FLOAT, { .fl = fl } };
    return obj;
}
/* ------------------ */
struct ns_obj ns_makeStrObjLen(char *c, unsigned int len)
{
    struct ns_obj obj;
    obj.type = TY_STR;

    obj.u.s = dynarr_new_alloc(len + 1);
    dynarr_resize_up(obj.u.s, len);
    obj.u.s->size = len + 1;
    strcpy(obj.u.s->arr, c);

    return obj;
}
/* ------------------ */
struct ns_obj ns_makeStrObj(char *c)
{
    return ns_makeStrObjLen(c, strlen(c));
}
/* ------------------ */
struct ns_obj ns_makeSymObjLen(char *c, unsigned int len)
{
    struct ns_obj obj;
    obj.type = TY_SYM;

    obj.u.sym = dynarr_new_alloc(len + 1);
    dynarr_resize_up(obj.u.sym, len);
    obj.u.sym->size = len + 1;
    strcpy(obj.u.sym->arr, c);

    return obj;
}
/* ------------------ */
struct ns_obj ns_makeSymObj(char *c)
{
    return ns_makeSymObjLen(c, strlen(c));
}
/* ------------------ */
struct ns_obj ns_makeFuncObj(ns_cFunc f)
{
    struct ns_obj obj = { TY_FUNC, { .f = f } };
    return obj;
}
/* ------------------ */
struct ns_obj ns_makeChar(char c)
{
    struct ns_obj obj;
    obj.type = TY_STR;
    obj.u.s = dynarr_new_alloc(2);
    obj.u.s->size = 2;
    obj.u.s->arr[0] = c;
    obj.u.s->arr[1] = '\0';

    return obj;
}
/* ------------------ */
void ns_execute(struct ns_obj obj)
{
    switch (obj.type)
    {
        case TY_FUNC:
            obj.u.f();
            break;

        case TY_BLOCK:
            ns_interpretInChildLine(obj.u.b.str->arr, obj.u.b.file->arr, 
                    obj.u.b.lineNo, ns_currNamespace);
            break;

        case TY_SYM:
            {
                struct ns_obj *e = ns_searchNamespaceInherit(ns_currNamespace, 
                        obj.u.sym->arr);

                if (e == &ns_defaultObj)
                    ns_error("exec: Could not find variable '%s'!", 
                            obj.u.sym->arr);
                if (!NS_ISEXECUTABLE(*e))
                    ns_error("exec: '%s' is not executable!", 
                            obj.u.sym->arr);

                ns_execute(*e);
                break;
            }
    }
}

