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
 * Initialises the interpretter.
 */
void ns_init();

/*
 * Interprets code in the given namespace. Blocks are run in new children.
 */
void ns_interpretInNamespace(const char *code, struct ns_namespace *ns);

/*
 * Interprets code in a child of the given namespace.
 */
void ns_interpretInChild(const char *code, struct ns_namespace *parent);

/*
 * Executes an object.
 */
void ns_execute(struct ns_obj obj);

#endif
