#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "heap.h"

struct heap {
    int capacity;
    int size;
    heap_compare_func cmp;
    heap_heapify_func heapify;
    void **array;
};

// forward declarations
bool max_heapify(heap_t *heap, int index);
bool min_heapify(heap_t *heap, int index);
int heap_find(heap_t *heap, void *value);

heap_t *heap_new(int capacity, heap_compare_func cmp, heap_heapify_func heapify)
{
    heap_t *heap = calloc(1, sizeof(heap_t));

    if (heap != NULL) {
        heap->capacity = capacity;
        heap->size = 0;
        heap->cmp = cmp;
        heap->heapify = heapify;
        heap->array = calloc(capacity, sizeof(void*));
    }

    return heap;
}

heap_t *heap_new_max(int capacity, heap_compare_func cmp)
{
    return heap_new(capacity, cmp, max_heapify);
}

heap_t *heap_new_min(int capacity, heap_compare_func cmp)
{
    return heap_new(capacity, cmp, min_heapify);
}

int heap_size(heap_t *heap)
{
    return heap->size;
}

int heap_parent(int index)
{
    return (index - 1) / 2;
}

int heap_left(int index)
{
    return 2*index + 1;
}

int heap_right(int index)
{
    return 2*index + 2;
}

bool max_heapify(heap_t *heap, int index)
{
    int largest;
    int left = heap_left(index);
    int right = heap_right(index);

    if (left < heap->size && heap->cmp(heap_get(heap, left), heap_get(heap, index)) > 0) {
        largest = left;
    } else {
        largest = index;
    }

    if (right < heap->size && heap->cmp(heap_get(heap, right), heap_get(heap, largest)) > 0) {
        largest = right;
    }

    if (largest != index) {
        void **temp = heap->array[index];
        heap->array[index] = heap->array[largest];
        heap->array[largest] = temp;
        heap->heapify(heap, largest);
        return true;
    }

    return false;
}

bool min_heapify(heap_t *heap, int index)
{
    int smallest;
    int left = heap_left(index);
    int right = heap_right(index);

    if (left < heap->size && heap->cmp(heap_get(heap, left), heap_get(heap, index)) < 0) {
        smallest = left;
    } else {
        smallest = index;
    }

    if (right < heap->size && heap->cmp(heap_get(heap, right), heap_get(heap, smallest)) < 0) {
        smallest = right;
    }

    if (smallest != index) {
        void **temp = heap->array[index];
        heap->array[index] = heap->array[smallest];
        heap->array[smallest] = temp;
        heap->heapify(heap, smallest);
        return true;
    }

    return false;
}

void **heap_get(heap_t *heap, int index)
{
    if (index < 0 || index >= heap->size) return NULL;

    void *value = heap->array[index];

    return value;
}

bool heap_insert(heap_t *heap, void *value)
{
    if (heap->size >= heap->capacity) return false;

    int id = heap->size++;
    int parentId = heap_parent(id);
    heap->array[id] = value;

    // float the value up
    if (parentId != id) {
        while (heap->heapify(heap, parentId)) {
            parentId = heap_parent(parentId);
        }
    }

    return true;
}

void **heap_extract(heap_t *heap)
{
    if (heap->size < 1) return NULL;

    void *value = heap_get(heap, 0);

    heap->array[0] = heap->array[--heap->size];

    heap->heapify(heap, 0);

    return value;
}

void heap_free(heap_t *heap)
{
    free(heap->array);
    free(heap);
}
