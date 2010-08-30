/*
 * =====================================================================================
 *
 *       Filename:  nscript.h
 *
 *    Description:  A simple stack-based scriping language.
 *
 *        Created:  10/23/2009 10:47:52 PM
 *       Compiler:  gcc
 *
 *         Author:  Nikhilesh S (nikki)
 *
 * =====================================================================================
 */

#ifndef __NSCRIPT_H__
#define __NSCRIPT_H__

#include <nsobj.h>
#include <nsnamespace.h>

/*
 * Holds interpretter context (current line, line number). This is used to display
 * error messages. Each time a block is executed, one element is added. When execution
 * is done, it's removed.
 */
struct ns_interpContext
{
    struct ns_interpContext *parent;
    const char *line; /* Char at beginning of line - print until '\n' */
    const char *filename;
    int lineNo;
} *ns_currContext;

struct ns_interpContext *ns_pushContext();
void ns_popContext();
void ns_printContext();

/*
 * Initialises the interpretter.
 */
void ns_init();

/*
 * Interprets code in the given namespace. Blocks are run in new children.
 *
 * The 'filename' parameter is what's displayed in error messages - even if the string
 * wasn't read from a real file, a descriptive name can help with error handling.
 *
 * The 'lineNo' parameter should indicate the line number of the first line in the
 * string. This may be useful if the code is embedded in a file and doesn't start at 
 * the beginning. This is again used in error messages.
 */
void ns_interpret(const char *code, const char *filename, int lineNo, struct ns_namespace *ns);

/*
 * Some functions/macros that wrap ns_interpret().
 */

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
 * Executes an object.
 */
void ns_execute(struct ns_obj obj);

#endif
