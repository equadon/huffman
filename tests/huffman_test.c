#include <string.h>
#include "tests.h"
#include "heap.h"
#include "huffman.h"

int init_suite_huffman()
{
    return 0;
}

int clean_suite_huffman()
{
    return 0;
}

typedef struct letters {
    int freq;
    char *str;
} letters_t;

int huff_compare_chars(void *a, void *b)
{
    letters_t *l1 = a;
    letters_t *l2 = b;

    return l1->freq - l2->freq;
}

void test_min_priority_queue()
{
    letters_t t = { .freq = 2, .str = "t" };
    letters_t e = { .freq = 1, .str = "e" };
    letters_t s = { .freq = 1, .str = "s" };

    heap_t *heap = heap_new_min(3, huff_compare_chars);

    CU_ASSERT(heap_insert(heap, &t) == true);
    CU_ASSERT(heap_insert(heap, &e) == true);
    CU_ASSERT(heap_insert(heap, &s) == true);

    CU_ASSERT(heap_size(heap) == 3);

    letters_t *x = (letters_t *) heap_extract(heap);
    CU_ASSERT(heap_size(heap) == 2);
    CU_ASSERT_STRING_EQUAL(x->str, "e");

    x = (letters_t *) heap_extract(heap);
    CU_ASSERT(heap_size(heap) == 1);
    CU_ASSERT_STRING_EQUAL(x->str, "s");

    x = (letters_t *) heap_extract(heap);
    CU_ASSERT(heap_size(heap) == 0);
    CU_ASSERT_STRING_EQUAL(x->str, "t");

    x = (letters_t *) heap_extract(heap);
    CU_ASSERT(heap_size(heap) == 0);
    CU_ASSERT(x == NULL);

    heap_free(heap);
}

test_t HUFFMAN_TESTS[] = {
    { "min-priority queue", test_min_priority_queue },
    { NULL }
};
