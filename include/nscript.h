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
 * Interprets some code.
 */
void ns_interpret(const char *code, struct ns_namespace *parent);

/*
 * Executes an object.
 */
void ns_execute(struct ns_obj obj);

#endif
