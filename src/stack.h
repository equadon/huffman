/**
 * Generic stack.
 *
 * Currently just a convenient abstraction layer on top of {@link list.h}.
 * @file
 */
#ifndef __STACK_H__
#define __STACK_H__

#include <stdbool.h>
#include "iter.h"

/**
 * Stack type.
 */
typedef struct stack stack2_t;

/**
 * Function pointer to compare elements.
 */
typedef int(*stack_compare_func)(void *, void *);

/**
 * Function pointer to free element values.
 */
typedef void(*stack_free_func)(void *);

/**
 * Create a new stack.
 * @param cmp compare values function pointer
 * @param free_value free value function pointer
 * @return the new stack
 */
stack2_t *stack_new(stack_compare_func cmp, stack_free_func free_value);

/**
 * Push value on top of the stack.
 * @param stack stack to push to
 * @param value value that will be placed on the top of the stack<Plug>(neosnippet_expand)
 */
void stack_push(stack2_t *stack, void *value);

/**
 * Pop the to value of the stack.
 * @param stack stack to pop from
 * @return value that was put in last in the stack
 */
void *stack_pop(stack2_t *stack);

/**
 * Peek at the value on top of the stack without removing it.
 * @param stack stack to peek
 * @return value of the last pushed element
 */
void *stack_peek(stack2_t *stack);

/**
 * Get the length of the stack.
 * @param stack stack to check
 * @return length of the stack
 */
int stack_length(stack2_t *stack);

void stack_clear(stack2_t *stack);

/**
 * Free memory allocated by the stack.
 * Note that stack_free will also free memory used by the values.
 * @param stack stack to free
 */
void stack_free(stack2_t *stack);

/**
 * Helper function to create a new stack iterator.
 * @param stack stack to iterate over
 * @see iter_new()
 * @return Iterator
 */
iter_t *iter_stack(stack2_t *stack);

/**
 * Helper function to create a new reverse stack iterator.
 * @param stack stack to iterate over
 * @see iter_new()
 * @return Iterator
 */
iter_t *iter_stack_reverse(stack2_t *stack);

#endif //__STACK_H__
