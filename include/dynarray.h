/*
 * =====================================================================================
 *
 *       Filename:  dynarray.h
 *
 *    Description:  Dynamic array.
 *
 *        Created:  10/25/2009 07:35:24 AM
 *       Compiler:  gcc
 *
 *         Author:  Nikhilesh S (nikki)
 *
 * =====================================================================================
 */

#ifndef __DYNARRAY_H__
#define __DYNARRAY_H__

#define DYNARRAY_RESIZE_FACTOR 2                     /* Amount to scale array allocation size by */

typedef char dynarr_info_t;                          /* Element type */

struct dynarr
{
    unsigned int size;                               /* Number of elements initialised */
    unsigned int alloc_size;                         /* Number of elements allocated for */
    dynarr_info_t *arr;                              /* First element */
};

/* Make a new empty array */
struct dynarr *dynarr_new();

/* Make a new empty array with an initial allocation */
struct dynarr *dynarr_new_alloc(unsigned int n);

/* Free memory used by the array */
void dynarr_free(struct dynarr *array);

/* Add an element, resizing if necessary */
int dynarr_append(struct dynarr *array, dynarr_info_t info);

/* Insert 'info' before 'pos'th element, returning change in allocation size */
int dynarr_insert(struct dynarr *array, unsigned int pos, dynarr_info_t info);

/* Remove an element, returning change in allocation size */
int dynarr_remove(struct dynarr *array, unsigned int pos);

/* Increase allocation size to 'best fit', returning change in allocation size */
int dynarr_resize_down(struct dynarr *array, unsigned int size);

/* Decrease allocation size to 'best fit', returning change in allocation size */
int dynarr_resize_up(struct dynarr *array, unsigned int size);

/* Automatically increase/decrease allocation size to 'best fit', returning change 
   in allocation size */
int dynarr_resize_auto(struct dynarr *array, unsigned int size);

/* Remove all elements */
void dynarr_clear(struct dynarr *array);

#endif
