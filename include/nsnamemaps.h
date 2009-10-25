/*
 * =====================================================================================
 *
 *       Filename:  nsnamemaps.h
 *
 *    Description:  The name->object mappings.
 *
 *        Created:  10/25/2009 09:10:31 AM
 *       Compiler:  gcc
 *
 *         Author:  Nikhilesh S (nikki)
 *
 * =====================================================================================
 */

#ifndef __NSNAMEMAPS_H__
#define __NSNAMEMAPS_H__

#include <trie.h>

/*
 * A temporary construct read from when the tries are created.
 */
struct ns_namemap
{
    char *key;
    struct ns_obj obj;
}; 
extern struct ns_namemap ns_funcmap[], ns_operatormap[], ns_constantmap[];

/*
 * The tries holding the pre-defined function, operator and constant objects.
 */
extern struct trie *ns_functrie;
extern struct trie *ns_operatortrie;
extern struct trie *ns_constanttrie;

/*
 * The trie holding user-made variables.
 */
extern struct trie *ns_variabletrie;

/*
 * Fill in the tries with info from the temporary namemaps.
 */
void ns_initNameMaps();

/*
 * Whether a character is a prefix for an operator.
 */
int ns_isOperatorPrefix(char c);

/*
 * Find the object in the namemap corresponding to the given key. If not found,
 * returns ns_defaultObj.
 */
struct ns_obj ns_findFunc(const char *key);
struct ns_obj ns_findOperator(const char *key);
struct ns_obj ns_findConstant(const char *key);

/*
 * Initialise the variable container (in the beginning, empty).
 */
void ns_initVariableMap();

/*
 * Find a variable made by the user.
 */
struct ns_obj *ns_findVariable(const char *name);

/*
 * When a user makes a new variable.
 */
void ns_addVariable(const char *name, struct ns_obj obj);
#endif
