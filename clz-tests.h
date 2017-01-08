#include <stdio.h>
#include <stdint.h>

uint8_t clz_recursive(uint32_t x, int shift_len);
int clz_iteration(uint32_t x);
int clz_byte_shift(uint32_t x);
int clz_binary_search(uint32_t x);
int clz_harley(uint32_t x);