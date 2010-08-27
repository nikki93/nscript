/*
 * =====================================================================================
 *
 *       Filename:  trie.h
 *
 *    Description:  Trie.
 *
 *        Created:  10/25/2009 08:50:19 AM
 *       Compiler:  gcc
 *
 *         Author:  Nikhilesh S (nikki)
 *
 * =====================================================================================
 */

#ifndef __TRIE_H__
#define __TRIE_H__

#include <nsobj.h>

typedef struct ns_obj trie_info_t;                   /* Element type */

/*
 * Actually, hashmaps could be used for 'pre-defined' things, with
 * a perfect hash function (possible keys are statically known).
 *
 * Anyway...
 */

struct trie
{
    char initialised;
    trie_info_t info;
    struct trie *arr[256];                           /* 'Children' */
};

/* Create a new empty trie */
struct trie *trie_new();

/* Free memory used by the trie */
void trie_free(struct trie *trie);

/* Add an element, with given key */
void trie_add(struct trie *trie, const char *c, trie_info_t info);

/* Get the element with given key, default if not found */
trie_info_t *trie_get(struct trie *trie, const char *c, trie_info_t *def);

#endif
