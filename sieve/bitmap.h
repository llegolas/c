#ifndef __BITMAP_H__
#define __BITMAP_H__

uint16_t *bit_map_create(size_t bits, unsigned int patern);
void bit_map_destroy(uint16_t *b);
void set_bit(uint16_t *bitmap, size_t bit, bool up);
bool get_bit(uint16_t *bitmap, size_t bit);

#endif
