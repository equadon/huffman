#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__

#include <stdbool.h>

typedef struct bit_array bit_array_t;

/**
 * Create a new bit array.
 * @param length length in bits
 * @return new bit array
 */
bit_array_t *bit_array_new(unsigned int length);

/**
 * Get the length (in bits) of the bit array.
 * @param array bit array to check
 * @return length in bits
 */
unsigned int bit_array_length(bit_array_t *array);

/**
 * Set bit to 1 at index.
 * @param array bit array
 * @param n bit index
 */
void bit_array_set(bit_array_t *array, unsigned int n);

/**
 * Set bit to 0 at index.
 * @param array bit array
 * @param n bit index
 * @return true if bit was changed
 */
bool bit_array_unset(bit_array_t *array, unsigned int n);

/**
 * Toggle bit at index.
 * @param array bit array
 * @param n bit index
 */
void bit_array_toggle(bit_array_t *array, unsigned int n);

/**
 * Test if bit at position n is 1.
 * @param array bit array to check
 * @param n bit index
 * @return true if bit at position n is 1
 */
bool bit_array_test(bit_array_t *array, unsigned int n);

/**
 * Count number of set bits in array.
 * @param array bit array to count
 * @return number of bits set to 1 in array
 */
int bit_array_count(bit_array_t *array);

/**
 * Fill bit array with 1's.
 * @param array bit array to fill
 */
void bit_array_fill(bit_array_t *array);

/**
 * Reset the entire array to zero.
 * @param array array to reset
 */
void bit_array_clear(bit_array_t *array);

void bit_array_print(bit_array_t *array);

bit_array_t *bit_array_read(FILE *file);
void bit_array_write(bit_array_t *array, FILE *file);

/**
 * Free memory allocated by the bit array.
 * @param array bit array to deallocate
 */
void bit_array_free(bit_array_t *array);

#endif //__BIT_ARRAY_H__
