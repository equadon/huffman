#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"
#include "heap.h"
#include "list.h"

struct huffman_node {
    char *str;
    int freq;
    huffman_node_t *parent;
    huffman_node_t *left;
    huffman_node_t *right;
};

int sort_letters(const char input[], huffman_node_t *nodes[]);
int huffman_compare(void *a, void *b);
huffman_node_t *build_huffman_tree(heap_t *heap, const char *input, huffman_node_t *nodes[], int *tree_size);
void huffman_index_chars(huffman_node_t *node, char *index[], char *prefix);

huffman_node_t *huffman_build_tree(bit_array_t *bits)
{
    int i = 0;
    int bit_length = bit_array_length(bits);
    huffman_node_t *root = NULL;
    huffman_node_t *node = NULL;
    while (i < bit_length) {
        while (node != NULL && node->left != NULL && node->right != NULL) {
            node = node->parent;
        }

        if (!bit_array_test(bits, i)) {
            i++;
            huffman_node_t *new_node = calloc(1, sizeof(huffman_node_t));
            new_node->parent = node;
            if (node == NULL) {
                root = new_node;
                node = new_node;
            } else {
                if (node->left == NULL) {
                    node->left = new_node;
                    node = node->left;
                } else if (node->right == NULL) {
                    node->right = new_node;
                    node = node->right;
                } else {
                    assert(node->left == NULL && node->right == NULL);
                }
            }
        } else {
            char letter = 0;
            i++;
            for (int j = 7; j >= 0; j--) {
                if (bit_array_test(bits, i++))
                    letter |= (1 << j);
            }
            if (node->left == NULL) {
                node->left = calloc(1, sizeof(huffman_node_t));
                node->left->str = calloc(2, sizeof(char));
                node->left->str[0] = letter;
            } else if (node->right == NULL) {
                node->right = calloc(1, sizeof(huffman_node_t));
                node->right->str = calloc(2, sizeof(char));
                node->right->str[0] = letter;
            } else {
                assert(node->left == NULL && node->right == NULL);
            }
        }
    }

    return root;
}

huffman_node_t *huffman_new_tree(const char *input, int *tree_size, int *unique_letters)
{
    huffman_node_t **nodes = calloc(256, sizeof(huffman_node_t*));
    *unique_letters = sort_letters(input, nodes);

    // build priority queue and get root node
    heap_t *heap = heap_new_min(strlen(input), huffman_compare);

    return build_huffman_tree(heap, input, nodes, tree_size);
}

char **huffman_build_index(huffman_node_t *root)
{
    char **index = calloc(256, sizeof(char*));

    huffman_index_chars(root, index, "");

    return index;
}

void huffman_encode_bits(bit_array_t *array, char *index[], char *input)
{
    int pos = 0;
    for (int i = 0; i < strlen(input); i++) {
        char *bits = index[(int) input[i]];
        for (int j = 0; j < strlen(bits); j++) {
            if (bits[j] == '1')
                bit_array_set(array, pos);
            pos++;
        }
    }
}

void encode_tree(bit_array_t *bits, huffman_node_t *node, int *index)
{
    if (node == NULL) return;

    if (node->left == NULL && node->right == NULL) {
        uint8_t letter = node->str[0];

        bit_array_set(bits, (*index)++);
        for (int i = 7; i >= 0; i--) {
            if ((letter & (1 << i)) != 0) {
                bit_array_set(bits, *index);
            }
            (*index)++;
        }
    } else {
        (*index)++;
        encode_tree(bits, node->left, index);
        encode_tree(bits, node->right, index);
    }
}

bit_array_t *huffman_encode_tree(huffman_node_t *root, int tree_size, int unique_letters)
{
    bit_array_t *bits = bit_array_new(tree_size + 8 * unique_letters);

    int index = 0;
    encode_tree(bits, root, &index);

    return bits;
}

bit_array_t *huffman_encode(char *index[], char *message)
{
    // count compressed bit count
    int compressed_bits = 0;
    for (int i = 0; i < strlen(message); i++) {
        compressed_bits += strlen(index[(int) message[i]]);
    }

    bit_array_t *bits = bit_array_new(compressed_bits);

    huffman_encode_bits(bits, index, message);

    return bits;
}

char *huffman_decode(huffman_node_t *root, bit_array_t *bits)
{
    int index = 0;
    int i = 0;
    int bit_len = bit_array_length(bits);
    char decoded[bit_len];

    huffman_node_t *node = root;
    while (i <= bit_len) {
        if (node->left == NULL && node->right == NULL) {
            decoded[index++] = node->str[0];
            node = root;
        } else {
            bool left = (bit_array_test(bits, i)) ? false : true;
            if (left)
                node = node->left;
            else
                node = node->right;
            i++;
        }
    }

    decoded[index] = '\0';

    return strdup(decoded);
}

int sort_letters(const char input[], huffman_node_t *nodes[])
{
    int unique = 0;

    // Reset letter nodes
    for (int i = 0; i < 256; i++) {
        char str[] = { i, '\0' };

        nodes[i] = calloc(1, sizeof(huffman_node_t));
        nodes[i]->str = strdup(str);
        nodes[i]->freq = 0;
    }

    size_t len = strlen(input);
    for (int i = 0; i < len; i++) {
        if (nodes[(int) input[i]]->freq == 0)
            unique++;

        nodes[(int) input[i]]->freq++;
    }

    return unique;
}

int huffman_compare(void *a, void *b)
{
    huffman_node_t *l1 = a;
    huffman_node_t *l2 = b;

    return l1->freq - l2->freq;
}

huffman_node_t *build_huffman_tree(heap_t *heap, const char *input, huffman_node_t *nodes[], int *tree_size)
{
    // insert letters
    for (int i = 0; i < 256; i++) {
        if (nodes[i]->freq > 0) {
            heap_insert(heap, nodes[i]);
            (*tree_size)++;
        } else {
            free(nodes[i]->str);
            free(nodes[i]);
        }
    }

    // build tree
    while (heap_size(heap) > 1) {
        huffman_node_t *left = (huffman_node_t *) heap_extract(heap);
        huffman_node_t *right = (huffman_node_t *) heap_extract(heap);

        huffman_node_t *node = calloc(1, sizeof(huffman_node_t));
        node->left = left;
        node->right = right;
        node->freq = 0;

        int length = 1;

        if (left != NULL) {
            length += strlen(left->str);
            node->freq += left->freq;
        }

        if (right != NULL) {
            length += strlen(right->str);
            node->freq += right->freq;
        }

        node->str = calloc(strlen(node->left->str) + strlen(node->right->str) + 1, sizeof(char));
        sprintf(node->str, "%s%s", node->left->str, node->right->str);

        heap_insert(heap, node);
        (*tree_size)++;
    }

    huffman_node_t *root = (huffman_node_t *) heap_get(heap, 0);

    heap_free(heap);
    free(nodes);

    return root;
}

void huffman_index_chars(huffman_node_t *node, char *index[], char *prefix)
{
    if (node == NULL) return;

    if (node->left == NULL && node->right == NULL) {
        index[(int) node->str[0]] = strdup(prefix);
    } else {
        char prefixLeft[strlen(prefix) + 1];
        char prefixRight[strlen(prefix) + 1];

        sprintf(prefixLeft, "%s0", prefix);
        sprintf(prefixRight, "%s1", prefix);

        huffman_index_chars(node->left, index, prefixLeft);
        huffman_index_chars(node->right, index, prefixRight);
    }
}

void huffman_node_free(huffman_node_t *node)
{
    if (node == NULL) return;

    huffman_node_free(node->left);
    huffman_node_free(node->right);

    free(node->str);
    free(node);
}

void huffman_free(huffman_node_t *root, char *index[])
{
    if (index != NULL) {
        for (int i = 0; i < 256; i++)
            free(index[i]);
        free(index);
    }

    huffman_node_free(root);
}
