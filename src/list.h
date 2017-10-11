/**
 * Linked list that can store values of any type.
 * @file
 * @author Niklas Persson
 */
#ifndef __LIST_H__
#define __LIST_H__

#include <stdbool.h>
#include "iter.h"

/**
 * List type.
 */
typedef struct list list_t;

/**
 * Function pointer to compare list elements.
 */
typedef int(*list_compare_func)(void *, void *);

/**
 * Function pointer to free element values.
 */
typedef void(*list_free_func)(void *);

/**
 * Create a new linked list.
 * @param cmp fpointer to compare list values
 * @param free_value fpointer to free list values
 * @return the new list.
 */
list_t *list_new(list_compare_func cmp, list_free_func free_value);

/**
 * Add value to the end of the list.
 * @param list list to append to
 * @param value value to append
 */
void list_append(list_t *list, void *value);

/**
 * Insert value at the beginning of the list.
 * @param list list to prepend to
 * @param value value to prepend
 */
void list_prepend(list_t *list, void *value);

/**
 * Insert value at the specified index.
 * @param list list to prepend to
 * @param index index to insert at
 * @param value value to prepend
 */
bool list_insert(list_t *list, int index, void *value);

/**
 * Delete list element with the given value.
 * Note that the value's memory is not freed.
 * @param list list to delete from
 * @param value value of element to delete
 * @return true if list element was successfully deleted
 */
bool list_delete(list_t *list, void *value);

/**
 * Remove list element by index.
 * Note that the value's memory is not freed.
 * @param list list to remove from
 * @param index index of element, -1 for end of list
 * @param value stores the value of the removed element
 * @return true if element was removed successfully
 */
bool list_remove(list_t *list, int index, void **value);

/**
 * Check if list has a specific value.
 * @param list list to check
 * @param value value to search for
 * @return true if the value exists in the list
 */
bool list_has_value(list_t *list, void *value);

/**
 * Clear the list of all elements.
 * Note that all values inside the list will be freed as well as the elements.
 * @param list list to clear
 */
void list_clear(list_t *list);

/**
 * Get value at the given index.
 * @param list list to search
 * @param index element index
 * @return value of the element, NULL if index doesn't exist
 */
void *list_get(list_t *list, int index);

/**
 * Get the first value of the list.
 * @param list list to get value from
 * @return value of the first element, NULL if list is empty
 */
void *list_first(list_t *list);

/**
 * Get the last value of the list.
 * @param list list to get value from
 * @return value of the last element, NULL if list is empty
 */
void *list_last(list_t *list);

/**
 * Get the length of the list.
 * @param list list to get length of
 * @return number of elements in list
 */
int list_length(list_t *list);

/**
 * Free memory allocated by the list.
 * Note that list_free also frees memory of all values.
 * @param list list to free
 */
void list_free(list_t *list);

/**
 * Helper function to create a new list iterator.
 * @param list list to iterate over
 * @see iter_new()
 * @return Iterator
 */
iter_t *iter_list(list_t *list);

/**
 * Helper function to create a new reverse list iterator.
 * @param list list to iterate over
 * @see iter_new()
 * @return Iterator
 */
iter_t *iter_list_reverse(list_t *list);

#endif
