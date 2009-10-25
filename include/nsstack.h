/*
 * =====================================================================================
 *
 *       Filename:  nsstack.h
 *
 *    Description:  Stack handling.
 *
 *        Created:  10/25/2009 09:09:58 AM
 *       Compiler:  gcc
 *
 *         Author:  Nikhilesh S (nikki)
 *
 * =====================================================================================
 */

#ifndef __NSSTACK_H__
#define __NSSTACK_H__

#include <stdlib.h>

#include <nsobj.h>

/*
 * The stack.
 */
struct ns_stack
{
    struct ns_obj obj;
    struct ns_stack *next;
} *ns_stack;
int ns_stackSize;

/*
 * Initialise the stack (initially empty except for an ns_defaultObj).
 */
void ns_initStack();

/*
 * Return and remove the last added object from the stack.
 */
struct ns_obj ns_pop();

/*
 * Add an object to the stack.
 */
void ns_push(struct ns_obj obj);

#endif
