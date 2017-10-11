/**
 * The generic iterator is used to iterate over a data structure.
 * @file
 * @author Niklas Persson
 */
#ifndef __ITER_H__
#define __ITER_H__

#include <stdbool.h>

/**
 * Iterator type.
 */
typedef struct iter iter_t;

/**
 * Function pointer to get the value of an element.
 */
typedef void *(* iter_value_func)(void *);

/**
 * Function pointers to get the next element.
 */
typedef void *(* iter_next_func)(void *);

/**
 * Create a new iterator.
 * @param start starting point of iterator
 * @param value fpointer to get the value of an element
 * @param next fpointer to get the next element
 * @return An iterator that points at the first element
 */
iter_t *iter_new(void *start, iter_value_func value, iter_next_func next);

/**
 * Check if the iterator has another element.
 * @param it iterator
 * @return True if the iterator has another element, otherwise false
 */
bool iter_has_next(iter_t *it);

/**
 * Return the current element's value and step to the next element.
 * @param it iterator
 * @return The value of the current element
 */
void *iter_next(iter_t *it);

/**
 * Restart iterator.
 */
void iter_restart(iter_t *it);

/**
 * Get element by id.
 * @param it iterator
 * @param index index of value
 * @return Value at index.
 */
void *iter_get(iter_t *it, int index);

/**
 * Return the length of the iterator and restart it.
 * @param it iterator
 * @return Length of the iterator
 */
int iter_length(iter_t *it);

/**
 * Free the memory allocated by the iterator.
 */
void iter_free(iter_t *it);

#endif // __ITER_H__
