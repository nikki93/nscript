/*
 * nscript.c
 */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <nsobj.h>
#include <nserror.h>
#include <nsnamespace.h>
#include <nsbuiltins.h>
#include <nsstack.h>

#include <nscript.h>

void ns_init()
{
    ns_initStack();
    ns_initBuiltinsSpace();
    ns_currNamespace = ns_builtinsSpace;
}
/* ------------------ */
struct ns_interpContext *ns_pushContext()
{
    struct ns_interpContext *newCon = malloc(sizeof(struct ns_interpContext));
    newCon->line = 0;
    newCon->lineNo = 1;
    newCon->parent = ns_currContext;
    return ns_currContext = newCon;
}
/* ------------------ */
void ns_popContext()
{
    struct ns_interpContext *parent = ns_currContext->parent;
    free(ns_currContext);
    ns_currContext = parent;
}
/* ------------------ */
void ns_printContext()
{
    struct ns_interpContext *con;
    for (con = ns_currContext; con; con = con->parent)
    {
        fprintf(stderr, "%s:%d:\t", con->filename, con->lineNo);

        const char *c = con->line;
        while (isspace(*c))
            ++c;
        while (*c && *c != '\n')
            putc(*c++, stderr);
        putc('\n', stderr);
        putc('\n', stderr);
    }
}
/* ------------------ */
void ns_interpretInNamespace(const char *code, const char *filename, struct ns_namespace *ns)
{
    enum
    {
        MD_NONE,

        MD_READINT,
        MD_READFLOATPART,
        MD_READSTR,
        MD_READBLOCK,

        MD_GETVAR,
        MD_SETVAR,
    };

    int mode = MD_NONE;
    int callFunc = 1;
    int blockDepth = 0;
    int negative = 0;
    int blockLine;
    char stringChar = 0;
    double floatMult = 0.1;

    const char *curr = code, p;
    struct dynarr *buf = dynarr_new();

    struct ns_namespace *oldns = ns_currNamespace;
    ns_currNamespace = ns;

    struct ns_interpContext *con = ns_pushContext();
    con->line = curr;
    con->filename = filename;

    do
    {
        switch (mode)
        {
            case MD_NONE:
                //Integer constant. Will convert to float if there's a '.'.
                if (isdigit(*curr) || (*(curr + 1) && (*curr == '-') && isdigit(*(curr + 1)) && (negative = 1)))
                {
                    mode = MD_READINT;
                    ns_push(ns_makeIntObj(0));

                    if (negative)
                        break;

                    goto read_int;
                }
                //String constant.
                else if (*curr == '\'' || *curr == '"')
                {
                    stringChar = *curr;
                    mode = MD_READSTR;

                    dynarr_clear(buf);

                    break;
                }
                //Function reference.
                else if (*curr == '&')
                {
                    callFunc = 0;
                    mode = MD_GETVAR;

                    dynarr_clear(buf);

                    break;
                }
                //Block of code.
                else if (*curr == '{')
                {
                    mode = MD_READBLOCK;
                    blockDepth = 1;
                    blockLine = con->lineNo;

                    dynarr_clear(buf);

                    break;
                }
                //Comment.
                else if (*curr == '#')
                {
                    while (*++curr && (*curr != '\n'));
                    --curr; /* To go over the newline again. */
                }
                //Variable definition.
                else if (*curr == '$')
                {
                    mode = MD_SETVAR;

                    dynarr_clear(buf);

                    break;
                }
                //Variable read.
                else if (!isspace(*curr))
                {
                    mode = MD_GETVAR;

                    dynarr_clear(buf);

                    goto get_var;
                }
                break;

            case MD_READINT:
read_int:
                if (isdigit(*curr))
                    ns_stack->obj.u.i = ns_stack->obj.u.i * 10 + *curr - '0';
                else if (*curr == '.')
                {
                    NS_INTTOFLOAT(ns_stack->obj);
                    mode = MD_READFLOATPART;
                }
                else
                {
                    if (negative)
                        ns_stack->obj.u.i *= -1;
                    negative = 0;
                    mode = MD_NONE;
                }
                break;

            case MD_READFLOATPART:
                if (isdigit(*curr))
                {
                    ns_stack->obj.u.fl += floatMult * (*curr - '0');
                    floatMult *= 0.1;
                }
                else
                {
                    if (negative)
                        ns_stack->obj.u.fl *= -1;
                    floatMult = 0.1;
                    negative = 0;
                    mode = MD_NONE;
                }
                break;

            case MD_READSTR:
                if (*curr != stringChar || (stringChar == '\'' && (*(curr + 1) == '\'')))
                {
                    char c = *curr;

                    //Escape sequences. If the string is delimited by ', the only escape sequence is '',
                    //if it's delimited by ", do the usual C escape sequences.
                    if (stringChar == '\'')
                    {
                        if (*curr == '\'' && *(curr + 1) == '\'')
                        {
                            c = '\'';
                            ++curr;
                        }
                    }
                    else if (*curr == '\\')
                        switch (*(curr + 1))
                        {
                            case 'n':
                                c = '\n';
                                ++curr;
                                break;

                            case 't':
                                c = '\t';
                                ++curr;
                                break;

                            case 'v':
                                c = '\v';
                                ++curr;
                                break;

                            case 'b':
                                c = '\b';
                                ++curr;
                                break;

                            case 'r':
                                c = '\r';
                                ++curr;
                                break;

                            case 'f':
                                c = '\f';
                                ++curr;
                                break;

                            case 'a':
                                c = '\a';
                                ++curr;
                                break;

                            default:
                                c = *(curr + 1);
                                ++curr;
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

                    obj.u.b.str = dynarr_new_alloc(buf->size);
                    strcpy(obj.u.b.str->arr, buf->arr);

                    obj.u.b.file = dynarr_new_alloc(strlen(filename) + 7);
                    sprintf(obj.u.b.file->arr, "%s-%d", filename, blockLine);

                    ns_push(obj);
                    mode = MD_NONE;
                }
                break;

            case MD_GETVAR:
get_var:
                if (!isspace(*curr))
                    dynarr_append(buf, *curr);
                else
                {
                    dynarr_append(buf, '\0');
                    mode = MD_NONE;

                    struct ns_obj obj = *ns_searchNamespaceInherit(ns_currNamespace, buf->arr);

                    if (obj.type != TY_EMPTY)
                    {
                        if (callFunc && NS_ISEXECUTABLE(obj))
                            ns_execute(obj);
                        else
                            ns_push(obj);
                        goto fin_get_var;
                    }

                    ns_error("Variable '%s' not found!", buf->arr);

fin_get_var:
                    //Reset for next time.
                    callFunc = 1;
                }
                break;

            case MD_SETVAR:
                if (!isspace(*curr))
                    dynarr_append(buf, *curr);
                else
                {
                    dynarr_append(buf, '\0');
                    struct ns_obj *obj = ns_searchNamespace(ns_currNamespace, buf->arr);

                    if (obj->type != TY_EMPTY)
                        *obj = ns_pop();
                    else
                        ns_addToNamespace(ns_currNamespace, buf->arr, ns_pop());
                    mode = MD_NONE;
                }
                break;
        }

        if (*curr == '\n')
        {
            con->line = curr + 1;
            ++con->lineNo;
        }
    } while (*curr++);

    dynarr_free(buf);
    ns_popContext();

    ns_currNamespace = oldns;
}
/* ------------------ */
void ns_interpretInChild(const char *code, const char *filename, struct ns_namespace *parent)
{
    ns_interpretInNamespace(code, filename, ns_newNamespace(parent));
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
            ns_interpretInChild(obj.u.b.str->arr, obj.u.b.file->arr, ns_currNamespace);
            break;
    }
}

