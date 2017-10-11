/**
 * @author Niklas Persson
 */
#include <stdlib.h>
#include "iter.h"

struct iter {
    iter_value_func value;
    iter_next_func next;
    void *start;
    void *current;
};

iter_t *iter_new(void *start, iter_value_func value, iter_next_func next)
{
    iter_t *it = calloc(1, sizeof(iter_t));

    if (it != NULL) {
        it->value = value;
        it->next = next;
        it->start = start;
        it->current = start;
    }

    return it;
}

bool iter_has_next(iter_t *it)
{
    return it->current != NULL && it->value(it->current) != NULL;
}

void *iter_next(iter_t *it)
{
    void *data = it->value(it->current);
    it->current = it->next(it->current);
    return data;
}

void iter_restart(iter_t *it)
{
    it->current = it->start;
}

void *iter_get(iter_t *it, int index)
{
    void *data = NULL;
    int i = -1;

    while (i < index && iter_has_next(it)) {
        data = iter_next(it);
        i++;
    }

    if (i == index) {
        return data;
    } else {
        return NULL;
    }
}

int iter_length(iter_t *it)
{
    int count = 0;
    void *start = it->current;

    while (iter_has_next(it)) {
        iter_next(it);
        count++;
    }

    it->current = start;
    return count;
}

void iter_free(iter_t *it)
{
    free(it);
}

