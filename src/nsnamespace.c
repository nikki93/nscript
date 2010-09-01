/*
 * nsnamespace.c
 */

#include <stdlib.h>

#include <nscript.h>

struct ns_namespace *ns_newNamespace(struct ns_namespace *parent)
{
    struct ns_namespace *ns = (struct ns_namespace *) malloc(sizeof(struct ns_namespace));
    ns->vars = trie_new();
    ns->parent = parent;

    return ns;
}
/* ------------------ */
void ns_addToNamespace(struct ns_namespace *ns, const char *name, struct ns_obj obj)
{
    trie_add(ns->vars, name, obj);
}
/* ------------------ */
void ns_namemapToNamespace(struct ns_namespace *ns, struct ns_namemap *nm)
{
    for (; nm->key; ++nm)
        ns_addToNamespace(ns, nm->key, nm->obj);
}
/* ------------------ */
struct ns_obj *ns_searchNamespace(struct ns_namespace *ns, const char *name)
{
    return trie_get(ns->vars, name, &ns_defaultObj);
}
/* ------------------ */
struct ns_obj *ns_searchNamespaceInherit(struct ns_namespace *ns, const char *name)
{
    struct ns_obj *obj;
    for (; ns; ns = ns->parent)
        if ((obj = ns_searchNamespace(ns, name)) != &ns_defaultObj)
            return obj;

    return &ns_defaultObj;
}

