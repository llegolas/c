#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/time.h>

#include "bitmap.h"

struct arg_struct {
    size_t id;
    size_t start_index;
    size_t end_index;
    uint16_t *bitmap;
};

size_t compute_start_index (size_t start_index, size_t i){
    size_t start;
    if (i*i >= start_index){
            start=i*i;
        } else {
            if ( start_index % i == 0 ){
                start = start_index;
            }else{
                start = start_index + (i - (start_index % i));
            };
        };
    if (start % 2 == 0 ) {start+=i;};
    return start;
};

void *sieve_f(void *arguments)
{
    size_t i,k,start=0;
    struct arg_struct *args=(struct arg_struct*) arguments;
    for (i=3;i*i <= args->end_index && start < args->end_index; i+=2){
        start = compute_start_index (args->start_index,i);
        for (k=start; k <= args->end_index; k+=i*2){
            set_bit(args->bitmap,k,false);
        };
    };
};

int main(int argc, char **argv)
{   
    size_t num_primes = 100000000;
    size_t i,k=0;
    // size_t t = get_nprocs_conf();
    size_t num_threads = 8;
    struct timeval time_start, time_end;

    pthread_t threads[num_threads];
    struct arg_struct arguments[num_threads];

    // create and initalize the sieve with all even indices as not primes and all odd indices as potential primes ( 0x55 -> 0101 0101 big-endian )
    uint16_t *sieve = bit_map_create(num_primes, 0x55);
    
    // 1 is not and 2 is prime
    set_bit(sieve, 1, false);
    set_bit(sieve, 2, true);

    for (i=0; i <= num_threads-1; i++){
        // printf("%u) %u - %u \n",i, i*d+1, (i+1)*d );
        arguments[i].id = i;
        arguments[i].start_index=i*(num_primes / num_threads)+1;
        arguments[i].end_index= ((i+1)*(num_primes / num_threads) <= num_primes) ? (i+1)*(num_primes / num_threads)  : num_primes;
        arguments[i].bitmap=sieve;
    };

    gettimeofday(&time_start,0);
     
    // Schedule threads
    for (i=0; i < num_threads; i++){
        // printf("scheduling thread %u \n", i);
        pthread_create(&(threads[i]),NULL,sieve_f,&arguments[i]);
    };

    for (i=0; i < num_threads; i++){
        pthread_join (threads[i],NULL);
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
            //printf ("%u \n",i);
            k++;
        };
    };
    printf("there are %u primes between 0 and %u \n",k,num_primes);
    
    bit_map_destroy(sieve);

    exit (0);
}
