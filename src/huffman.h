#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include "bit_array.h"

typedef struct huffman_node huffman_node_t;

huffman_node_t *huffman_new_tree(const char *input, int *tree_size, int *unique_letters);
huffman_node_t *huffman_build_tree(bit_array_t *bits);

char **huffman_build_index(huffman_node_t *root);

bit_array_t *huffman_encode_tree(huffman_node_t *root, int tree_size, int unique_letters);

bit_array_t *huffman_encode(char *index[], char *message);

char *huffman_decode(huffman_node_t *root, bit_array_t *bits);

void huffman_free(huffman_node_t *root, char *index[]);

#endif //__HUFFMAN_H__
