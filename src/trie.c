/*
 * trie.c
 */

#include <stdlib.h>

#include <nscript.h>

struct trie *trie_new()
{
    struct trie *trie = malloc(sizeof(struct trie));

    int i;
    for (i = 0; i < 256; ++i)
        trie->arr[i] = 0;
    trie->initialised = 0;

    return trie;
}
/* ------------------ */
void trie_free(struct trie *trie)
{
    int i;
    for (i = 0; i < 256; ++i)
        if (trie->arr[i])
            trie_free(trie->arr[i]);
    free(trie);
}
/* ------------------ */
void trie_add(struct trie *trie, const char *c, trie_info_t info)
{
    for (; *c; ++c)
        trie = (trie->arr[(int)*c] ? trie->arr[(int)*c] : (trie->arr[(int)*c] = trie_new()));

    trie->info = info;
    trie->initialised = 1;
}
/* ------------------ */
trie_info_t *trie_get(struct trie *trie, const char *c, trie_info_t *def)
{
    /*
     * O(strlen(c)) :D
     * Hopefully pointer-following is fast enough.
     */
    while(*c)
        if (!(trie = trie->arr[(int)(*c++)]))
            return def;

    return trie->initialised ? &(trie->info) : def;
}
