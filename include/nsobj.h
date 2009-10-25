/*
 * =====================================================================================
 *
 *       Filename:  nsobj.h
 *
 *    Description:  Generic nscript 'object'.
 *
 *        Created:  10/25/2009 09:09:10 AM
 *       Compiler:  gcc
 *
 *         Author:  Nikhilesh S (nikki)
 *
 * =====================================================================================
 */

#ifndef __NSOBJ_H__
#define __NSOBJ_H__

#include <dynarray.h>
#include <string.h>

typedef void (*ns_cFunc) ();

/*
 * Variable/constant types.
 */
enum
{
    TY_EMPTY,
    TY_BOOL,
    TY_INT,
    TY_STR,
    TY_FUNC,
    TY_BLOCK
};

/*
 * Represents a script object of any type.
 */
struct ns_obj
{
    short type;
    union
    {
        int bo;                                 //TY_BOOL
        int i;                                  //TY_INT
        struct dynarr *s;                       //TY_STR
        ns_cFunc f;                             //TY_FUNC
        struct dynarr *b;                       //TY_BLOCK
    } u;
};

/*
 * Whether an object is executable (function, block etc.).
 */
int ns_isExecutable(struct ns_obj obj);

/*
 * 'default' object, returned if object not found etc.
 */
extern struct ns_obj ns_defaultObj;

/*
 * Make objects.
 */
struct ns_obj ns_makeBoolObj(int b);
struct ns_obj ns_makeIntObj(int i);
struct ns_obj ns_makeStrObjLen(char *c, unsigned int len);
struct ns_obj ns_makeStrObj(char *c);
struct ns_obj ns_makeFuncObj(ns_cFunc f);

/*
 * Get C types from objects.
 */
#define NS_INTFROMOBJ(obj) (obj).u.i
#define NS_STRFROMOBJ(obj) (obj).u.s

#endif
