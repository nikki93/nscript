/*
 * nsstack.c
 */

#include <nserror.h>

#include <nsstack.h>

void ns_initStack()
{
    ns_stack = (struct ns_stack *) malloc(sizeof(struct ns_stack));
    ns_stack->next = 0;
    ns_stack->obj = ns_defaultObj;
    ns_stackSize = 0;
}
/* ------------------ */
struct ns_obj ns_pop()
{
    if (ns_stackSize <= 0)
        ns_error("pop: Nothing on stack!");
    struct ns_obj top = ns_stack->obj;
    struct ns_stack *next = ns_stack->next;
    free(ns_stack);
    ns_stack = next;
    --ns_stackSize;
    return top;
}
/* ------------------ */
void ns_push(struct ns_obj obj)
{
    struct ns_stack *new = (struct ns_stack *) malloc(sizeof(struct ns_stack));
    new->obj = obj;
    new->next = ns_stack;
    ns_stack = new;
    ++ns_stackSize;
}
