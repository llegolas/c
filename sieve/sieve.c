#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <sys/time.h>

#include "bitmap.h"

size_t num_primes = 100000000;

int main(int argc, char **argv)
{
    size_t i,k=0;
    struct timeval time_start, time_end;

    // create and initalize the sieve with all even indices as not primes and all odd indices as potential primes ( 0x55 -> 0101 0101 big-endian )
    uint16_t *sieve = bit_map_create(num_primes, 0x55);
    
    // 1 is not and 2 is prime
    set_bit(sieve, 1, false);
    set_bit(sieve, 2, true);
    
    gettimeofday(&time_start,0);

    for (i=3;i*i <= num_primes; i+=2) {
        if (get_bit(sieve,i*i) == 1) {
            for (k=i*i; k <= num_primes; k+=i*2) {
                set_bit(sieve,k,false);
            };
        };
    };

    gettimeofday (&time_end,0);
    long seconds = time_end.tv_sec - time_start.tv_sec;
    long microseconds = time_end.tv_usec - time_start.tv_usec;
    double elapsed = seconds + microseconds*1e-6;
    printf("The sieve took %f seconds\n",elapsed);

    // output stuff
    k=0;
    for (i=0; i <= num_primes; i++){
        if (get_bit(sieve,i)){
            // printf ("%u \n",i);
            k++;
        };
    };
    printf("there are %u primes between 0 and %u \n",k,num_primes);
    
    bit_map_destroy(sieve);

    exit (0);
}
