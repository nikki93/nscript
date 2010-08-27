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
 * Can be used to read name, object data into a namespace.
 */
struct ns_namemap
{
    char *key;
    struct ns_obj obj;
}; 

/*
 * A holder of variables. Can take the form of a tree, children 'inherit' the variables
 * from the parents (when children are searched, parents are searched too).
 */
struct ns_namespace
{
    struct trie *vars;
    struct ns_namespace *parent;
}; 

struct ns_namespace *ns_currNamespace;

/*
 * Create a new namespace with the given parent. Use '0' is parent for no parent.
 */
struct ns_namespace *ns_newNamespace(struct ns_namespace *parent);

/*
 * Add a variable with the given name and given object to the given namespace.
 */
void ns_addToNamespace(struct ns_namespace *ns, const char *name, struct ns_obj obj);

/*
 * Add variables from a namemap. The last entry in the namemap must be { 0 }.
 */
void ns_namemapToNamespace(struct ns_namespace *ns, struct ns_namemap *nm);

/*
 * Find the object in the namespace corresponding to the given name. If not found,
 * returns ns_defaultObj.
 *
 * Does not search parent namespaces.
 */
struct ns_obj *ns_searchNamespace(struct ns_namespace *ns, const char *name);

/*
 * Find the object in the namespace corresponding to the given name. If not found,
 * returns ns_defaultObj.
 *
 * Also searches parent namespaces.
 */
struct ns_obj *ns_searchNamespaceInherit(struct ns_namespace *ns, const char *name);

#endif
