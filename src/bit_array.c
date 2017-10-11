#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bit_array.h"

#define BYTE_SIZE 8

struct bit_array {
    unsigned int bit_length;
    unsigned int length;
    uint8_t *data;
};

bit_array_t *bit_array_new(unsigned int length)
{
    unsigned int array_length = (length + BYTE_SIZE - 1) / BYTE_SIZE;
    bit_array_t *array = calloc(1, sizeof(bit_array_t));

    if (array != NULL) {
        array->data = calloc(array_length, sizeof(uint8_t));
        array->bit_length = length;
        array->length = array_length;
    }

    return array;
}

unsigned int bit_array_length(bit_array_t *array)
{
    return array->bit_length;
}

unsigned int bit_index(bit_array_t *array, unsigned int n)
{
    return n / BYTE_SIZE;
}

unsigned int bit_pos(bit_array_t *array, unsigned int n)
{
    return n % BYTE_SIZE;
}

void bit_array_set(bit_array_t *array, unsigned int n)
{
    unsigned int i = bit_index(array, n);
    unsigned int pos = bit_pos(array, n);
    array->data[i] |= (1 << pos);
}

bool bit_array_unset(bit_array_t *array, unsigned int n)
{
    unsigned int i = bit_index(array, n);
    unsigned int pos = bit_pos(array, n);
    array->data[i] &= ~(1 << pos);
    return true;
}

void bit_array_toggle(bit_array_t *array, unsigned int n)
{
    unsigned int i = bit_index(array, n);
    unsigned int pos = bit_pos(array, n);
    array->data[i] ^= (1 << pos);
}

bool bit_array_test(bit_array_t *array, unsigned int n)
{
    unsigned int i = bit_index(array, n);
    unsigned int pos = bit_pos(array, n);
    return (array->data[i] & (1 << pos)) != 0;
}

int bit_array_count(bit_array_t *array)
{
    int n;
    int count = 0;

    for (int i = 0; i < array->length; i++) {
        n = array->data[i];

        while (n) {
            count += n & 1;
            n = n >> 1;
        }
    }

    return count;
}

void bit_array_fill(bit_array_t *array)
{
    for (int i = 0; i < array->length; i++) {
        array->data[i] = ~0;
    }
}

void bit_array_clear(bit_array_t *array)
{
    for (int i = 0; i < array->length; i++) {
        array->data[i] &= 0;
    }
}

void bit_array_print(bit_array_t *array)
{
    int pos = 0;
    for (int i = 0; i < array->length; i++) {
        for (int j = 0; j < BYTE_SIZE; j++) {
            printf("%d", (array->data[i] & (1 << j)) != 0);
            if (++pos >= array->bit_length) return;
        }
    }
}

bit_array_t *bit_array_read(FILE *file)
{
    unsigned int length;
    fread(&length, sizeof(length), 1, file);

    bit_array_t *array = bit_array_new(length);

    fread(array->data, sizeof(array->data[0]), array->length, file);

    return array;
}

void bit_array_write(bit_array_t *array, FILE *file)
{
    fwrite(&array->bit_length, sizeof(array->bit_length), 1, file);
    fwrite(array->data, sizeof(array->data[0]), array->length, file);
}

void bit_array_free(bit_array_t *array)
{
    free(array->data);
    free(array);
}
