#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdbool.h>

typedef struct heap heap_t;

typedef int(*heap_compare_func)(void *a, void *b);
typedef bool(*heap_heapify_func)(heap_t *heap, int index);

heap_t *heap_new(int capacity, heap_compare_func cmp, heap_heapify_func heapify);
heap_t *heap_new_max(int capacity, heap_compare_func cmp);
heap_t *heap_new_min(int capacity, heap_compare_func cmp);

int heap_size(heap_t *heap);

void **heap_get(heap_t *heap, int index);

bool heap_insert(heap_t *heap, void *value);
void **heap_extract(heap_t *heap);

void heap_free(heap_t *heap);

#endif //__HEAP_H__
