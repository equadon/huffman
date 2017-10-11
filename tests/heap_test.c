#include <string.h>
#include "tests.h"
#include "heap.h"

int init_suite_heap()
{
    return 0;
}

int clean_suite_heap()
{
    return 0;
}

int cmp_ints(void *a, void *b)
{
    return (*(int*)a - *(int*)b);
}

void test_max_heap(heap_t *heap)
{
    CU_ASSERT(*(int*) heap_get(heap, 0) == 10);
    CU_ASSERT(*(int*) heap_get(heap, 1) == 7);
    CU_ASSERT(*(int*) heap_get(heap, 2) == 8);
    CU_ASSERT(*(int*) heap_get(heap, 3) == 4);
    CU_ASSERT(*(int*) heap_get(heap, 4) == 5);
    CU_ASSERT(*(int*) heap_get(heap, 5) == 6);
    CU_ASSERT(*(int*) heap_get(heap, 6) == 1);

    CU_ASSERT(*(int*) heap_extract(heap) == 10);

    CU_ASSERT(heap_size(heap) == 6);

    CU_ASSERT(*(int*) heap_get(heap, 0) == 8);
    CU_ASSERT(*(int*) heap_get(heap, 1) == 7);
    CU_ASSERT(*(int*) heap_get(heap, 2) == 6);
    CU_ASSERT(*(int*) heap_get(heap, 3) == 4);
    CU_ASSERT(*(int*) heap_get(heap, 4) == 5);
    CU_ASSERT(*(int*) heap_get(heap, 5) == 1);
}

void test_max_heap_insert()
{
    int numbers[] = { 5, 10, 6, 4, 7, 8, 1 };
    int length = sizeof(numbers) / sizeof(int);

    heap_t *heap = heap_new_max(length, cmp_ints);

    for (int i = 0; i < length; i++) {
        CU_ASSERT(heap_insert(heap, &numbers[i]) == true);
    }

    int number = 12;
    CU_ASSERT(heap_insert(heap, &number) == false);

    CU_ASSERT(heap_size(heap) == length);

    test_max_heap(heap);

    heap_free(heap);
}

void test_min_heap(heap_t *heap)
{
    CU_ASSERT(*(int*) heap_get(heap, 0) == 1);
    CU_ASSERT(*(int*) heap_get(heap, 1) == 5);
    CU_ASSERT(*(int*) heap_get(heap, 2) == 4);
    CU_ASSERT(*(int*) heap_get(heap, 3) == 10);
    CU_ASSERT(*(int*) heap_get(heap, 4) == 7);
    CU_ASSERT(*(int*) heap_get(heap, 5) == 8);
    CU_ASSERT(*(int*) heap_get(heap, 6) == 6);

    CU_ASSERT(*(int*) heap_extract(heap) == 1);

    CU_ASSERT(heap_size(heap) == 6);

    CU_ASSERT(*(int*) heap_get(heap, 0) == 4);
    CU_ASSERT(*(int*) heap_get(heap, 1) == 5);
    CU_ASSERT(*(int*) heap_get(heap, 2) == 6);
    CU_ASSERT(*(int*) heap_get(heap, 3) == 10);
    CU_ASSERT(*(int*) heap_get(heap, 4) == 7);
    CU_ASSERT(*(int*) heap_get(heap, 5) == 8);
}

void test_min_heap_insert()
{
    int numbers[] = { 5, 10, 6, 4, 7, 8, 1 };
    int length = sizeof(numbers) / sizeof(int);

    heap_t *heap = heap_new_min(length, cmp_ints);

    for (int i = 0; i < length; i++) {
        CU_ASSERT(heap_insert(heap, &numbers[i]) == true);
    }

    int number = 12;
    CU_ASSERT(heap_insert(heap, &number) == false);

    CU_ASSERT(heap_size(heap) == length);

    test_min_heap(heap);

    heap_free(heap);
}

test_t HEAP_TESTS[] = {
    { "max-heap insert", test_max_heap_insert },
    { "min-heap insert", test_min_heap_insert },
    { NULL }
};
