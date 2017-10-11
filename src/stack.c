#include <assert.h>
#include <stdlib.h>
#include "stack.h"
#include "list.h"

struct stack {
    list_t *list;
};

stack2_t *stack_new(stack_compare_func cmp, stack_free_func free_value)
{
    stack2_t *stack = calloc(1, sizeof(stack2_t));

    if (stack != NULL) {
        stack->list = list_new(cmp, free_value);
    }

    return stack;
}

void stack_push(stack2_t *stack, void *value)
{
    assert(stack != NULL);
    list_append(stack->list, value);
}

void *stack_pop(stack2_t *stack)
{
    assert(stack != NULL);

    if (stack_length(stack) == 0) {
        return NULL;
    }

    void *data;
    list_remove(stack->list, -1, &data);
    return data;
}

void *stack_peek(stack2_t *stack)
{
    assert(stack != NULL);

    if (stack_length(stack) == 0) {
        return NULL;
    }

    return list_last(stack->list);
}

int stack_length(stack2_t *stack)
{
    assert(stack != NULL);
    return list_length(stack->list);
}

void stack_clear(stack2_t *stack)
{
    list_clear(stack->list);
}

void stack_free(stack2_t *stack)
{
    assert(stack != NULL);
    list_free(stack->list);
    free(stack);
}

iter_t *iter_stack(stack2_t *stack)
{
    if (stack == NULL) {
        return NULL;
    }

    return iter_list(stack->list);
}

iter_t *iter_stack_reverse(stack2_t *stack)
{
    if (stack == NULL) {
        return NULL;
    }

    return iter_list_reverse(stack->list);
}
