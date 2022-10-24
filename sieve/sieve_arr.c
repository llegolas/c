// This version will work up to arround the first 67000000 primes.
// we are limited by the size of the stack vor the sieve array. It cannot be static as we are using dynamic size

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "bitmap.h"

int main(int argc, char **argv)
{
    size_t i,k;
    size_t num_primes = 67000000;
    size_t num_ints = num_primes / (sizeof(uint16_t)*8) + (num_primes % (sizeof(uint16_t)*8) && 1);

    // create
    uint16_t sieve[num_ints];
    uint16_t *sieve_p=sieve;

    // and initalize the sieve with all even indices as not primes and all odd indices as potential primes ( 0x55 -> 0101 0101 big-endian )
    // uint16_t *sieve = bit_map_create(num_primes, 0x55);
    memset(sieve_p, 0x55, sizeof(sieve));
    // 1 is not and 2 is prime
    set_bit(sieve_p, 1, false);
    set_bit(sieve_p, 2, true);
     
    // starting from 3 as 2 is covered upon initalization
    for (i=3;i*i < num_primes; i+=2){
        if (get_bit(sieve_p,i*i)) {
            for (k=i*i; k<=num_primes; k+=i*2){
                set_bit(sieve_p,k,false);
            };
        };
    };

    // ottput stuff
    k=0;
    for (i=0; i <= num_primes; i++){
        if (get_bit(sieve_p,i)){
            // printf ("%u \n",i);
            k++;
        };
    };
    printf("%u \n",k);
    exit (0);
}
