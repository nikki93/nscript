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

typedef void (*ns_cFunc) (void);

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
        struct ns_block
        {
            struct dynarr *file;
            int lineNo;
            struct dynarr *str;                  
        } b;                                   //TY_BLOCK
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
#define NS_BOOLFROMOBJ(obj) ((obj).u.bo)
#define NS_INTFROMOBJ(obj) ((obj).u.i)
#define NS_FLOATFROMOBJ(obj) ((obj).u.fl)
#define NS_STRFROMOBJ(obj) ((obj).u.s)
#define NS_FUNCFROMOBJ(obj) ((obj).u.f)

/*
 * Type checks.
 *
 * TODO: Make a function due to double evaluation of 'obj'?
 */
#define NS_ISBOOL(obj) ((obj).type == TY_BOOL)
#define NS_ISINT(obj) ((obj).type == TY_INT)
#define NS_ISFLOAT(obj) ((obj).type == TY_FLOAT)
#define NS_ISSTR(obj) ((obj).type == TY_STR)
#define NS_ISFUNC(obj) ((obj).type == TY_FUNC)
#define NS_ISBLOCK(obj) ((obj).type == TY_BLOCK)
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
