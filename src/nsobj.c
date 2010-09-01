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

    obj.u.s = dynarr_new();
    dynarr_resize_up(obj.u.s, len);
    strcpy(obj.u.s->arr, c);

    return obj;
}
/* ------------------ */
struct ns_obj ns_makeStrObj(char *c)
{
    return ns_makeStrObjLen(c, strlen(c));
}
/* ------------------ */
struct ns_obj ns_makeFuncObj(ns_cFunc f)
{
    struct ns_obj obj = { TY_FUNC, { .f = f } };
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
    }
}

