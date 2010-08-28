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
 */
void ns_interpretInNamespace(const char *code, const char *filename, struct ns_namespace *ns);

/*
 * Interprets code in a child of the given namespace.
 *
 * The 'filename' parameter is what's displayed in error messages - even if the string
 * wasn't read from a real file, a descriptive name can help with error handling.
 */
void ns_interpretInChild(const char *code, const char *filename, struct ns_namespace *parent);

/*
 * Executes an object.
 */
void ns_execute(struct ns_obj obj);

#endif
