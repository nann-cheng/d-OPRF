// BENCHMARKING

#include <time.h>
#include "arith.h"
#include "bench.h"


// Returns nanoseconds on the clock
uint64_t cpucycles(void){
    struct timespec time;

    clock_gettime(CLOCK_REALTIME, &time);
    return (int64_t)(time.tv_sec*1e9 + time.tv_nsec);
}



// void benchmark(void function_to_bench()){
//     uint64_t cycles_pre = 0, cycles_post = 0, cycles = 0;
//     get_pk(Q);
    
//     // printf("\nWARMING UP\n");
//     // printf("----------------------------------------------------------------------------------\n");
//     for(int i = 0; i < WARMUP; i++){
//         function_to_bench();
//     }

//     // printf("\n\nBENCHMARKING\n");
//     // printf("----------------------------------------------------------------------------------\n\n");
//     for(int i = 0; i < BENCH_LOOPS; i++){
//         cycles_pre = cpucycles();
//         function_to_bench();
//         cycles_post = cpucycles();
//         cycles += (cycles_post-cycles_pre);
//     }
//     printf("Function runs in ................................... %8" PRIu64 " cycles\n", cycles/BENCH_LOOPS);
//     printf("Function runs in ................................... %5.3g ms\n", ((double)(cycles/BENCH_LOOPS)/CLK_PER_SEC)*1E3);

// }
