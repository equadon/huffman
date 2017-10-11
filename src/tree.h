/**
 * Generic binary search tree.
 *
 * The tree is currently not auto-balanced.
 * @file
 * @author Niklas Persson
 */
#ifndef __TREE_H__
#define __TREE_H__

#include <stdbool.h>
#include "iter.h"

/**
 * Binary search tree type definition.
 */
typedef struct tree tree_t;

/**
 * Function pointer to compare two node keys.
 */
typedef int(*tree_compare_func)(void *, void*);

/**
 * Function pointer to free node keys.
 */
typedef void(*tree_free_func)(void *);

/**
 * Function pointer to print something.
 */
typedef void(*tree_print_func)(void *);

/**
 * Create a new binary search tree.
 * @param cmp fpointer to compare tree keys
 * @param free_key fpointer to free tree keys
 * @return the new binary tree
 */
tree_t *tree_new(tree_compare_func cmp, tree_free_func free_key);

/**
 * Get the size of the tree.
 * @param tree tree to count
 * @return number of non-empty nodes
 */
int tree_size(tree_t *tree);

/**
 * Get the depth of the tree (numbers of edges passed).
 * @param tree tree to check
 * @return depth of the tree
 */
int tree_depth(tree_t *tree);

/**
 * Insert key into tree.
 * @param tree tree to insert to
 * @param key key to insert
 */
void tree_insert(tree_t *tree, void *key);

/**
 * Remove key from tree.
 * Note that tree_remove does not free the memory of the removed key.
 * @param tree tree to remove from
 * @param key key to remove
 */
bool tree_remove(tree_t *tree, void *key);

/**
 * Find node key that matches with the given key.
 * @param tree tree to check
 * @param key key to compare with
 * @return node key that matches with key
 */
void *tree_find(tree_t *tree, void *key);

/**
 * Check if the tree has the given key.
 * @param tree tree to check
 * @param key key to find
 * @return true if key exists in tree
 */
bool tree_has_key(tree_t *tree, void *key);

/**
 * Print tree structure.
 * @param tree tree to print
 * @param print function pointer to print individual key
 */
void tree_print(tree_t *tree, tree_print_func print);

/**
 * Sort tree by dumping it in an array and re-insert.
 * @param tree tree to insert
 * @param size size of each key
 */
void tree_sort(tree_t *tree, int size);

/**
 * Create a new in-order tree iterator.
 * @param tree tree to iterate over
 * @see iter_new()
 * @return Iterator
 */
iter_t *iter_tree(tree_t *tree);

/**
 * Create a new pre-order tree iterator.
 * Note: restart iterator does not work with this iterator.
 * @param tree tree to iterate over
 * @see iter_new()
 * @return Iterator
 */
iter_t *iter_tree_pre(tree_t *tree);

/**
 * Free memory allocated by the tree.
 * Note that tree_free also frees memory of all keys.
 * @param tree tree to free
 */
void tree_free(tree_t *tree);

#endif
