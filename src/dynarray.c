/*
 * dynarr.c
 *
 * Dynarray implementation.
 */

#include <stdlib.h>
#include <string.h>

#include <dynarray.h>

struct dynarr *dynarr_new()
{
    struct dynarr *array = (struct dynarr *) malloc(sizeof(struct dynarr));
    array->size = 0;
    array->alloc_size = 0;
    array->arr = 0;

    return array;
}
/* ------------------ */
struct dynarr *dynarr_new_alloc(unsigned int n)
{
    struct dynarr *array = (struct dynarr *) malloc(sizeof(struct dynarr));
    array->size = 0;
    array->alloc_size = n;
    array->arr = (dynarr_info_t *) malloc(n * sizeof(dynarr_info_t));

    return array;
}
/* ------------------ */
void dynarr_free(struct dynarr *array)
{
    free(array->arr);
    free(array);
}
/* ------------------ */
int dynarr_append(struct dynarr *array, dynarr_info_t info)
{
    int alloc = dynarr_resize_up(array, ++array->size);
    array->arr[array->size - 1] = info;

    return alloc;
}
/* ------------------ */
int dynarr_insert(struct dynarr *array, unsigned int pos, dynarr_info_t info)
{
    if (pos > array->size)
        return 0;
    else if (pos == array->size)
        return dynarr_append(array, info);

    int alloc = dynarr_resize_up(array, ++array->size);

    int move_size = array->size - pos - 1;    /* Old size, so -1 */
    memmove(array->arr + pos + 1, array->arr + pos, move_size * sizeof(dynarr_info_t));
    array->arr[pos] = info;

    return alloc;
}
/* ------------------ */
int dynarr_remove(struct dynarr *array, unsigned int pos)
{
    if (pos >= array->size)
        return 0;

    int rest = array->size - pos - 1;
    if (rest)
        memmove(array->arr + pos, array->arr + pos + 1, rest * sizeof(dynarr_info_t));

    return dynarr_resize_down(array, --array->size);
}
/* ------------------ */
int dynarr_resize_down(struct dynarr *array, unsigned int size)
{
    int old_size = array->alloc_size;

    if (size && !array->alloc_size)
        array->alloc_size = 1;

    while (size < array->alloc_size / DYNARRAY_RESIZE_FACTOR)
        array->alloc_size = array->alloc_size / DYNARRAY_RESIZE_FACTOR;

    if (array->size > size)
        array->size = size;

    int diff = array->alloc_size - old_size;
    if (diff)
        array->arr = (dynarr_info_t *) realloc(array->arr, array->alloc_size * sizeof(dynarr_info_t));

    return diff;
}
/* ------------------ */
int dynarr_resize_up(struct dynarr *array, unsigned int size)
{
    int old_size = array->alloc_size;

    if (!array->alloc_size)
        array->alloc_size = 1;

    while (size > array->alloc_size)
        array->alloc_size = array->alloc_size * DYNARRAY_RESIZE_FACTOR;

    int diff = array->alloc_size - old_size;
    if (diff)
        array->arr = (dynarr_info_t *) realloc(array->arr, array->alloc_size * sizeof(dynarr_info_t));

    return diff;
}
/* ------------------ */
int dynarr_resize_auto(struct dynarr *array, unsigned int size)
{
    if (array->alloc_size > size)
        return dynarr_resize_down(array, size);
    return dynarr_resize_up(array, size);
}
/* ------------------ */
void dynarr_clear(struct dynarr *array)
{
    dynarr_resize_down(array, array->size = 0);
}

