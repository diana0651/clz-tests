#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "clz-tests.h"
#include <time.h>

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0
#define u 50

int main(int argc,char const *argv[]){

	struct timespec start = {0, 0};
	struct timespec end = {0, 0};

	if(argc < 2) return -1;
	

	int N = atoi(argv[1]);	
	int ans;
	int i;
	

	clock_gettime(CLOCK_ID, &start);
	for( i = 0; i < 25 ; i++)
		ans = clz_recursive(N,16);
	clock_gettime(CLOCK_ID, &end);
	printf("%.6lf " , (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);
	
	clock_gettime(CLOCK_ID, &start);
	for( i = 0; i < 25 ; i++)
		ans = clz_iteration(N);
	clock_gettime(CLOCK_ID, &end);
	printf("%.6lf " , (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);
	
	clock_gettime(CLOCK_ID, &start);
	for( i = 0; i < 25 ; i++)
		ans = clz_binary_search(N);
	clock_gettime(CLOCK_ID, &end);
	printf("%.6lf " , (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);

	clock_gettime(CLOCK_ID, &start);
	for( i = 0; i < 25 ; i++)
		ans = clz_byte_shift(N);
	clock_gettime(CLOCK_ID, &end);
	printf("%.6lf " , (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);

	clock_gettime(CLOCK_ID, &start);
	for( i = 0; i < 25 ; i++)
		ans = clz_harley(N);
	clock_gettime(CLOCK_ID, &end);
	printf("%.6lf " , (double) (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec)/ONE_SEC);
	
	printf("%d\n",ans);


	return 0;
}


uint8_t clz_recursive(uint32_t x, int shift_len)
{
    if(shift_len == 0)
        return 0;
    /* shift upper half down, rest is filled up with 0s */
    uint16_t upper = (x >> shift_len);
    // mask upper half away
    uint16_t lower = ( x & (0xFFFF >> (16 - shift_len)) );
    return upper ? clz_recursive(upper, shift_len >> 1) : 
                    shift_len + clz_recursive(lower, shift_len >> 1);
}

int clz_iteration(uint32_t x) {
    int n = 32, c = 16;
    do {
        uint32_t y = x >> c;
        if (y) { n -= c; x = y; }
        c >>= 1;
    } while (c);
    return (n - x);
}

int clz_binary_search(uint32_t x){

    if (x == 0) return 32;
    int n = 0;
    if (x <= 0x0000FFFF) { n += 16; x <<= 16; }
    if (x <= 0x00FFFFFF) { n += 8; x <<= 8; }
    if (x <= 0x0FFFFFFF) { n += 4; x <<= 4; }
    if (x <= 0x3FFFFFFF) { n += 2; x <<= 2; }
    if (x <= 0x7FFFFFFF) { n += 1; x <<= 1; }
    return n;

}

int clz_byte_shift(uint32_t x){

    if (x == 0) return 32;
    int n = 1;
    if ((x >> 16) == 0) { n += 16; x <<= 16; }
    if ((x >> 24) == 0) { n += 8; x <<= 8; }
    if ((x >> 28) == 0) { n += 4; x <<= 4; }
    if ((x >> 30) == 0) { n += 2; x <<= 2; }
    n = n - (x >> 31);
    return n;

}

const char Table[] = {
	32,31, u,16, u,30, 3, u, 15, u, u, u,29,
	10, 2, u, u, u,12,14,21, u,19, u, u,28,
	u,25, u, 9, 1, u, 17, u, 4, u, u, 
	u,11, u, 13,22,20, u,26, u, u,18, 5, u,	
	u,23, u,27, u, 6, u,24, 7, u, 8, u, 0, u
};

int clz_harley(uint32_t x){

    // Propagate leftmost 1-bit to the right 
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
 
    // x = x * 0x6EB14F9 
    x = (x << 3) - x;   // Multiply by 7. 
    x = (x << 8) - x;   // Multiply by 255. 
    x = (x << 8) - x;   // Again. 
    x = (x << 8) - x;   // Again.

    return Table[x >> 26];
}
