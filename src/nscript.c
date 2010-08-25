/*
 * nscript.c
 */

#include <nsobj.h>
#include <nserror.h>
#include <nsnamemaps.h>
#include <nsstack.h>

#include <nscript.h>

/* ------------------ */
void ns_init()
{
    ns_initStack();
    ns_initNameMaps();
    ns_initVariableMap();
}
/* ------------------ */
void ns_interpret(const char *code)
{
    const char *curr;
    struct dynarr *buf;
    char *bufptr;

    buf = dynarr_new();

    enum
    {
        MD_READINT,
        MD_READSTR,
        MD_READNAME,
        MD_READBLOCK,
        MD_READVARNAME,
        MD_NONE
    };

    int mode = MD_NONE;
    int callFunc = 1;
    int isOperator = 0;
    int blockDepth = 0;
    int negative = 0;
    char stringChar = 0;

    do
    {
        curr = code++;

        switch (mode)
        {
            case MD_NONE:
                //Integer constant.
                if (isdigit(*curr) || ((curr + 1) && (*curr == '-') && isdigit(*(curr + 1)) && (negative = 1)))
                {
                    mode = MD_READINT;
                    ns_push(ns_makeIntObj(0));

                    if (negative)
                        break;

                    goto do_int;
                }
                //String constant.
                else if (*curr == '\'' || *curr == '"')
                {
                    stringChar = *curr;
                    mode = MD_READSTR;

                    dynarr_clear(buf);

                    break;
                }
                //Name, operator.
                else if (isalpha(*curr) || *curr == '_' || (ns_isOperatorPrefix(*curr) && (isOperator = 1)))
                {
                    mode = MD_READNAME;

                    dynarr_clear(buf);

                    goto do_name;
                }
                //Function reference.
                else if (*curr == '&')
                {
                    callFunc = 0;
                    mode = MD_READNAME;

                    dynarr_clear(buf);

                    if (ns_isOperatorPrefix(*(curr + 1)))
                        isOperator = 1;
                    break;
                }
                //Block of code.
                else if (*curr == '{')
                {
                    mode = MD_READBLOCK;
                    blockDepth = 1;

                    dynarr_clear(buf);

                    break;
                }
                //Comment.
                else if (*curr == '#')
                    while (*++code && (*code != '\n'));
                else if (*curr == '$')
                {
                    mode = MD_READVARNAME;

                    dynarr_clear(buf);

                    break;
                }
                break;

            case MD_READINT:
do_int:
                if (isdigit(*curr))
                    ns_stack->obj.u.i = ns_stack->obj.u.i * 10 + *curr - '0';
                else
                {
                    if (negative)
                        ns_stack->obj.u.i *= -1;
                    negative = 0;
                    mode = MD_NONE;
                }
                break;

            case MD_READSTR:
                if (*curr != stringChar)
                {
                    char c = *curr;

                    //Escape sequence.
                    if (*curr == '\\')
                        switch (*(curr + 1))
                        {
                            case 'n':
                                c = '\n';
                                ++code;
                                break;

                            case '\\':
                                c = '\\';
                                ++code;
                                break;

                            case 't':
                                c = '\t';
                                ++code;
                                break;

                            default:
                                c = *(curr + 1);
                                ++code;
                                break;
                        }

                    dynarr_append(buf, c);
                }
                else
                {
                    dynarr_append(buf, '\0');

                    ns_push(ns_makeStrObjLen(buf->arr, buf->size));
                    mode = MD_NONE;
                }
                break;

            case MD_READNAME:
do_name:
                //Operators end on space, names end on non-alphanumeric.
                if ((isOperator && !isspace(*curr)) || isalnum(*curr) || *curr == '_')
                    dynarr_append(buf, *curr);
                else
                {
                    dynarr_append(buf, '\0');
                    mode = MD_NONE;

                    struct ns_obj obj;

                    obj = ns_findFunc(buf->arr);
                    if (obj.type != TY_EMPTY)
                    {
                        if (callFunc)
                            obj.u.f();
                        else
                            ns_push(obj);
                        goto finish;
                    }

                    obj = ns_findOperator(buf->arr);
                    if (obj.type != TY_EMPTY)
                    {
                        if (callFunc)
                            obj.u.f();
                        else
                            ns_push(obj);
                        goto finish;
                    }

                    obj = ns_findConstant(buf->arr);
                    if (obj.type != TY_EMPTY)
                    {
                        ns_push(obj);
                        goto finish;
                    }

                    obj = *ns_findVariable(buf->arr);
                    if (obj.type != TY_EMPTY)
                    {
                        if (callFunc && ns_isExecutable(obj))
                            ns_execute(obj);
                        else
                            ns_push(obj);
                        goto finish;
                    }

                    ns_error("Name '%s' not found!", buf->arr);

finish:
                    //Reset for next time.
                    callFunc = 1;
                    isOperator = 0;
                }
                break;

            case MD_READBLOCK:
                if (*curr == '{')
                    ++blockDepth;
                if (*curr == '}')
                    --blockDepth;
                if (blockDepth > 0)
                    dynarr_append(buf, *curr);
                else
                {
                    dynarr_append(buf, '\0');

                    struct ns_obj obj;
                    obj.type = TY_BLOCK;
                    obj.u.b = dynarr_new();
                    dynarr_resize_up(obj.u.b, buf->size);
                    strcpy(obj.u.b->arr, buf->arr);
                    ns_push(obj);
                    mode = MD_NONE;
                }
                break;

            case MD_READVARNAME:
                if (isalnum(*curr) || *curr == '_')
                    dynarr_append(buf, *curr);
                else
                {
                    dynarr_append(buf, '\0');
                    struct ns_obj *obj = ns_findVariable(buf->arr);

                    if (obj->type != TY_EMPTY)
                        *obj = ns_pop();
                    else
                        ns_addVariable(buf->arr, ns_pop());
                    mode = MD_NONE;
                }
                break;
        }
    } while (*curr);

    dynarr_free(buf);
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
            ns_interpret(obj.u.b->arr);
            break;
    }
}

