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
    TY_FLOAT,
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
        int bo;                                //TY_BOOL
        int i;                                 //TY_INT
        double fl;                             //TY_FLOAT
        struct dynarr *s;                      //TY_STR
        ns_cFunc f;                            //TY_FUNC
        struct dynarr *b;                      //TY_BLOCK
    } u;
};

/*
 * 'default' object, returned if object not found etc.
 */
extern struct ns_obj ns_defaultObj;

/*
 * C -> nscript.
 */
struct ns_obj ns_makeBoolObj(int b);
struct ns_obj ns_makeIntObj(int i);
struct ns_obj ns_makeFloatObj(double fl);
struct ns_obj ns_makeStrObjLen(char *c, unsigned int len);
struct ns_obj ns_makeStrObj(char *c);
struct ns_obj ns_makeFuncObj(ns_cFunc f);

/*
 * nscript -> C.
 */
#define NS_INTFROMOBJ(obj) ((obj).u.i)
#define NS_FLOATFROMOBJ(obj) ((obj).u.fl)
#define NS_STRFROMOBJ(obj) ((obj).u.s)

/*
 * Type checks.
 */
#define NS_ISNUM(obj) ((obj).type == TY_INT || (obj).type == TY_FLOAT)
#define NS_ISEXECUTABLE(obj) ((obj).type == TY_FUNC || (obj).type == TY_BLOCK)

/*
 * Convert an integer object to a float object.
 */
#define NS_INTTOFLOAT(obj) \
    do \
    { \
        (obj).type = TY_FLOAT; \
        NS_FLOATFROMOBJ(obj) = NS_INTFROMOBJ(obj); \
    } while(0)

#endif
