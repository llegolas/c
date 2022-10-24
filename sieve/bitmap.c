#include <stdint.h>
#include <stdbool.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

uint16_t *bit_map_create(size_t bits, unsigned int patern)
{
    size_t num_ints = bits / (sizeof(uint16_t)*8) + (bits % (sizeof(uint16_t)*8) && 1);
    uint16_t *bitmap = calloc(num_ints, sizeof(uint16_t));
    if (bitmap == NULL)
        err(1, "calloc error in bit_map_create().\n");
    
    if (patern > 0) {
        memset(bitmap, patern, num_ints*sizeof(uint16_t));
    }
    
    return bitmap;
}

// when i is integer (i >> log2(x)) is the quotient when deviding by powers of 2. Here the power is x 
// so it becomes (i >> 4) and is equivalent of (i/16)
// (i&(x-1)) is the reminder when deviding by powers of 2. 
// it becomes (i&15) and is equivalent of (i%16) the (15 -...) part is big-endiannes.
// We want our bits stored from left to right    

void bit_map_destroy(uint16_t *b)
{
    free(b);
    b = NULL;
}

void set_bit(uint16_t *bitmap, size_t bit, bool up)
{
    if (up) {
        bitmap[bit >> 4] |= 1 << (15 - (bit&15)); // big-endian
    } else {
        bitmap[bit >> 4] &= ~(1 << (15 - (bit&15))); // big-endian
    }
}

unsigned int get_bit(uint16_t *bitmap, size_t bit)
{
    return ( (bitmap[bit >> 4] >> (15 - bit&15)) & 1);
}
