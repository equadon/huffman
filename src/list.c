/**
 * @author Niklas Persson
 */
#include <stdlib.h>
#include "list.h"

typedef struct elem elem_t;

struct list {
    list_compare_func cmp;
    list_free_func free_value;
    elem_t *first;
    elem_t *last;
    int length;
};

struct elem {
    void *value;
    elem_t *prev;
    elem_t *next;
};

list_t *list_new(list_compare_func cmp, list_free_func free_value)
{
    list_t *list = calloc(1, sizeof(list_t));

    if (list != NULL) {
        list->cmp = cmp;
        list->free_value = free_value;
    }

    return list;
}

elem_t *elem_new(void *value)
{
    elem_t *elem = calloc(1, sizeof(elem_t));

    if (elem != NULL) {
        elem->value = value;
    }

    return elem;
}

elem_t *list_get_element(list_t *list, int index)
{
    elem_t *current = list->first;
    int i = 0;

    while (current != NULL && current->next != NULL && i < index) {
        current = current->next;
        i++;
    }

    if (i == index) {
        return current;
    } else {
        return NULL;
    }
}

void list_append(list_t *list, void *value)
{
    elem_t *new = elem_new(value);

    if (list->first == NULL) {
        list->first = new;
    }

    if (list->last != NULL) {
        new->prev = list->last;
        list->last->next = new;
    }

    list->last = new;
    list->length++;
}

void list_prepend(list_t *list, void *value)
{
    if (list->last == NULL) {
        list_append(list, value);
    } else {
        elem_t *new = elem_new(value);
        new->next = list->first;
        list->first->prev = new;
        list->first = new;
        list->length++;
    }
}

bool list_insert(list_t *list, int index, void *value)
{
    if (index == -1 || index == list->length) {
        list_append(list, value);
    } else if (index == 0) {
        list_prepend(list, value);
    } else if (index > 0 && index < list->length) {
        elem_t *current = list_get_element(list, index);

        if (current != NULL) {
            elem_t *new = elem_new(value);
            new->prev = current->prev;
            new->next = current;

            if (current->prev != NULL) {
                current->prev->next = new;
            }

            current->prev = new;
            list->length++;
        }
    } else {
        return false;
    }

    return true;
}

/**
 * Remove an element from the list.
 * @param list list to remove from
 * @param elem the element (elem_t) to remove
 * @return true if successfully removed element
 */
bool _list_remove_element(list_t *list, elem_t *current)
{
    if (current == NULL) {
        return false;
    }

    if (current->next == NULL) {
        list->last = current->prev;
    }

    if (current->prev != NULL) {
        current->prev->next = current->next;
    }

    if (current->next != NULL) {
        current->next->prev = current->prev;
    }

    if (current == list->first) {
        list->first = current->next;
    }

    free(current);
    list->length--;
    return true;
}

elem_t *list_find(list_t *list, void *value)
{
    elem_t *current = list->first;

    while (current != NULL) {
        if (list->cmp(current->value, value) == 0) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

bool list_delete(list_t *list, void *value)
{
    elem_t *current = list_find(list, value);
    return _list_remove_element(list, current);
}

bool list_remove(list_t *list, int index, void **value)
{
    elem_t *current = NULL;

    if (index == 0) {
        current = list->first;
    } else if (index == -1) {
        current = list->last;
    } else {
        current = list_get_element(list, index);
    }

    if (current == NULL) {
        return false;
    }

    if (value == NULL) {
        list->free_value(current->value);
    } else {
        if (current != NULL) {
            *value = current->value;
        }
    }

    return _list_remove_element(list, current);
}

bool list_has_value(list_t *list, void *value)
{
    elem_t *found = list_find(list, value);
    return found != NULL && list->cmp(found->value, value) == 0;
}

void list_clear(list_t *list)
{
    elem_t *current = list->first;

    while (current != NULL) {
        elem_t *temp = current;
        list->free_value(temp->value);
        current = current->next;
        free(temp);
    }

    list->first = NULL;
    list->last = NULL;
    list->length = 0;
}

void *list_get(list_t *list, int index)
{
    if (index == -1) {
        return list_last(list);
    }

    elem_t *elem = list_get_element(list, index);
    return (elem == NULL) ? NULL : elem->value;
}

void *list_first(list_t *list)
{
    if (list->first == NULL) {
        return NULL;
    }

    return list->first->value;
}

void *list_last(list_t *list)
{
    if (list->last == NULL) {
        return NULL;
    }

    return list->last->value;
}

int list_length(list_t *list)
{
    return list->length;
}

/**
 * Get the start elem_t of the list.
 * @see iter_start_fun
 */
void *list_start(void *list)
{
    if (list == NULL) {
        return NULL;
    }

    return ((list_t *) list)->first;
}

/**
 * Get the last elem_t of the list.
 * @see iter_start_fun
 */
void *list_end(void *list)
{
    if (list == NULL) {
        return NULL;
    }

    return ((list_t *) list)->last;
}

/**
 * Get the elem_t value.
 * @see iter_value_fun
 */
void *list_value(void *elem)
{
    if (elem == NULL) {
        return NULL;
    }

    return ((elem_t *) elem)->value;
}

/**
 * Get the next etem_t.
 * @see iter_next_fun
 */
void *list_next(void *elem)
{
    if (elem == NULL) {
        return NULL;
    }

    return ((elem_t *) elem)->next;
}

/**
 * Get the previuos elem_t.
 * @see iter_next_fun
 */
void *list_prev(void *elem)
{
    if (elem == NULL) {
        return NULL;
    }

    return ((elem_t *) elem)->prev;
}

iter_t *iter_list(list_t *list)
{
    if (list == NULL) {
        return NULL;
    }

    return iter_new(list_start(list), list_value, list_next);
}

iter_t *iter_list_reverse(list_t *list)
{
    if (list == NULL) {
        return NULL;
    }

    return iter_new(list_end(list), list_value, list_prev);
}

void list_free(list_t *list)
{
    if (list != NULL) {
        elem_t *temp = NULL;
        elem_t *current = list->first;

        while (current != NULL) {
            temp = current;
            current = current->next;
            list->free_value(temp->value);
            free(temp);
        }

        free(list);
    }
}
