#ifndef __TESTS_H__
#define __TESTS_H__

#include <stdlib.h>
#include <CUnit/Basic.h>

typedef struct test {
    const char *name;
    void (*func)(void);
} test_t;

/*
 * Heap test functions.
 */
int init_suite_heap();
int clean_suite_heap();

extern test_t HEAP_TESTS[];

/*
 * Huffman test functions.
 */
int init_suite_huffman();
int clean_suite_huffman();

extern test_t HUFFMAN_TESTS[];

#endif //__TESTS_H__
