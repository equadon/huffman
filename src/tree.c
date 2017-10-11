/**
 * @author Niklas Persson
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tree.h"
#include "stack.h"

typedef struct node node_t;

/**
 * Binary search tree.
 */
struct tree {
    node_t *root;       /**< root of the tree */
    tree_compare_func cmp;   /**< compare function to use when comparing nodes */
    tree_free_func free_key; /**< free function to use when freeing nodes */
    stack2_t *pre_stack;
};

/**
 * Binary search tree node.
 */
struct node {
    void *key;      /**< key that holds the data for each node */
    node_t *parent; /**< parent of the node */
    node_t *left;   /**< left node */
    node_t *right;  /**< right node */
};

node_t *node_new(node_t *parent)
{
    node_t *node = calloc(1, sizeof(node_t));

    if (node != NULL) {
        node->parent = parent;
    }

    return node;
}

tree_t *tree_new(tree_compare_func cmp, tree_free_func free_key)
{
    tree_t *tree = calloc(1, sizeof(tree_t));

    if (tree != NULL) {
        tree->root = node_new(NULL);
        tree->cmp = cmp;
        tree->free_key = free_key;
        tree->pre_stack = stack_new(NULL, NULL);
    }

    return tree;
}

node_t *node_minimum(node_t *node)
{
    if (node == NULL) {
        return NULL;
    }

    while (node->left != NULL) {
        node = node->left;
    }

    return node;
}

/**
 * Get the start node of the tree.
 * @see iter_start_fun
 */
void *tree_start(void *tree)
{
    return node_minimum(((tree_t *) tree)->root);
}

int node_size(node_t *node)
{
    if (node == NULL || node->key == NULL) {
        return 0;
    } else {
        return 1 + node_size(node->left) + node_size(node->right);
    }
}

int tree_size(tree_t *tree)
{
    return node_size(tree->root);
}

int node_depth(node_t *node)
{
    if (node == NULL) {
        return -1;
    } else {
        int left = node_depth(node->left);
        int right = node_depth(node->right);
        return 1 + ((left >= right) ? left : right);
    }
}

int tree_depth(tree_t *tree)
{
    if (tree == NULL || tree->root == NULL) {
        return 0;
    }

    return node_depth(tree->root);
}

void tree_insert(tree_t *tree, void *key)
{
    node_t *current = tree->root;

    if (current == NULL) {
        current = node_new(NULL);
        current->key = key;
        tree->root = current;
        return;
    }

    while (current->key != NULL) {
        int cmp = tree->cmp(key, current->key);

        if (cmp < 0) {
            if (current->left == NULL) {
                current->left = node_new(current);
            }

            current = current->left;
        } else if (cmp > 0) {
            if (current->right == NULL) {
                current->right = node_new(current);
            }

            current = current->right;
        } else {
            break;
        }
    }

    current->key = key;
}

/**
 * Get the next node.
 * This function uses a successive in-order depth first search to find the next node.
 * @see iter_next_fun
 */
void *node_next(void *n)
{
    node_t *node = (node_t *) n;

    if (node->right != NULL) {
        node = node_minimum(node->right);
    } else {
        while (node->parent != NULL && node == node->parent->right) {
            node = node->parent;
        }

        node = node->parent;
    }

    return node;
}

node_t *node_find(tree_t *tree, node_t *node, void *key)
{
    while (node != NULL && node->key != NULL) {
        if (tree->cmp(node->key, key) == 0) {
            return node;
        }

        node = node_next(node);
    }

    return NULL;
}

void node_replace(tree_t *tree, node_t *a, node_t *b)
{
    if (a->parent == NULL) {
        tree->root = b;
    } else if (a == a->parent->left) {
        a->parent->left = b;
    } else {
        a->parent->right = b;
    }

    if (b != NULL) {
        b->parent = a->parent;
    }
}

void node_free(node_t *node, tree_free_func free_key)
{
    if (node == NULL) {
        return;
    }

    void *key = node->key;
    node_free(node->left, free_key);
    node_free(node->right, free_key);

    if (free_key != NULL) {
        free_key(key);
    }

    free(node);
}

bool tree_remove(tree_t *tree, void *key)
{
    if (tree == NULL || key == NULL) {
        return false;
    }

    node_t *min = node_minimum(tree->root);
    node_t *node = node_find(tree, min, key);

    if (node == NULL) {
        return false;
    }

    if (node->left == NULL) {
        node_replace(tree, node, node->right);
    } else if (node->right == NULL) {
        node_replace(tree, node, node->left);
    }  else {
        node_t *min = node_minimum(node->right);

        if (min->parent != node) {
            node_replace(tree, min, min->right);
            min->right = node->right;
            min->right->parent = min;
        }

        node_replace(tree, node , min);
        min->left = node->left;
        min->left->parent = min;
    }

    free(node);
    return true;
}

void *tree_find(tree_t *tree, void *key)
{
    node_t *node = node_find(tree, tree_start(tree), key);

    if (node == NULL) {
        return NULL;
    } else {
        return node->key;
    }
}

bool tree_has_key(tree_t *tree, void *key)
{
    return node_find(tree, tree_start(tree), key) != NULL;
}

void node_print(node_t *node, tree_print_func print, char *prefix, bool is_tail)
{
    if (node == NULL || node->key == NULL) {
        return;
    }

    printf("%s%s", prefix, (is_tail ? "└── " : "├── "));
    print(node->key);
    printf("\n");
    char prefix_next[255];
    sprintf(prefix_next, "%s%s", prefix, (is_tail ? "    " : "│   "));
    node_print(node->left, print, prefix_next, node->right == NULL);
    node_print(node->right, print, prefix_next, true);
}

void tree_print(tree_t *tree, tree_print_func print)
{
    node_print(tree->root, print, "", true);
}

// Dump node keys in array
void _tree_dump(tree_t *tree, void **keys, int length)
{
    node_t *node = node_minimum(tree->root);

    for (int i = 0; i < length; i++) {
        *(keys + i) = node->key;
        node->key = NULL;
        node = node_next(node);
    }

    node_free(tree->root, NULL);
    tree->root = node_new(NULL);
}

// Load tree with keys in array
void _tree_load(tree_t *tree, void **keys, int start, int end)
{
    int middle = start + (end - start) / 2;
    tree_insert(tree, *(keys + middle));

    if (start == end) {
        return;
    }

    if (middle - 1 >= start) {
        _tree_load(tree, keys, start, middle - 1);
    }

    if (middle + 1 <= end) {
        _tree_load(tree, keys, middle + 1, end);
    }
}

void tree_sort(tree_t *tree, int size)
{
    int length = tree_size(tree);

    if (length == 0) {
        return;
    }

    void **keys = calloc(length, size);

    if (keys == NULL) {
        return;
    }

    _tree_dump(tree, keys, length);
    _tree_load(tree, keys, 0, length - 1);
    free(keys);
}

/**
 * Get the node key.
 * @see iter_value_fun
 */
void *node_key(void *node)
{
    if (node == NULL) {
        return NULL;
    }

    return ((node_t *) node)->key;
}

iter_t *iter_tree(tree_t *tree)
{
    if (tree == NULL) {
        return NULL;
    }

    return iter_new(tree_start(tree), node_key, node_next);
}

void *node_key_pre(void *t)
{
    tree_t *tree = t;

    node_t *current = stack_peek(tree->pre_stack);

    if (current == NULL) {
        return NULL;
    } else {
        return current->key;
    }
}

void *node_next_pre(void *t)
{
    tree_t *tree = t;

    node_t *current = stack_pop(tree->pre_stack);

    if (current->right)
        stack_push(tree->pre_stack, current->right);
    if (current->left)
        stack_push(tree->pre_stack, current->left);

    return t;
}

iter_t *iter_tree_pre(tree_t *tree)
{
    if (tree == NULL) {
        return NULL;
    }

    stack_clear(tree->pre_stack);

    stack_push(tree->pre_stack, tree->root);

    return iter_new(tree, node_key_pre, node_next_pre);
}

void tree_free(tree_t *tree)
{
    node_free(tree->root, tree->free_key);
    stack_free(tree->pre_stack);
    free(tree);
}
