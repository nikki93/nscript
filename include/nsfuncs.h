/*
 * =====================================================================================
 *
 *       Filename:  nsfuncs.h
 *
 *    Description:  'pre-defined' functions that come with nscript.
 *
 *        Created:  10/25/2009 09:10:12 AM
 *       Compiler:  gcc
 *
 *         Author:  Nikhilesh S (nikki)
 *
 * =====================================================================================
 */

#ifndef __NSFUNCS_H__
#define __NSFUNCS_H__

/*
 * Print an object on the stack to standard output.
 */
void ns_print();

/*
 * Add two integers
 */
void ns_add();

/*
 * Subract one integer from another.
 */
void ns_subtract();

/*
 * Multiply an integer by another.
 */
void ns_multiply();

/*
 * Divide an integer by another.
 */
void ns_divide();

/*
 * Exit the program.
 */
void ns_exit();

/*
 * Repeat an executable a given number of times.
 */
void ns_repeat();

/*
 * Push the last object onto the stack again.
 */
void ns_dup();

/*
 * Swap positions of last two objects on stack.
 */
void ns_rot();

/*
 * Execute a block/function if a condition is matched.
 */

void ns_if();

/*
 * Execute a block/function if a condition is matched, else execute an
 * 'else' block.
 */

void ns_ifelse();

/*
 * Checks last two objects on stack for equality.
 */

void ns_equals();

/*
 * Dumps the stack.
 */

void ns_printStack();

/*
 * Pushes object at position on stack.
 */

void ns_at();

#endif
