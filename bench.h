#ifndef BENCH_H
#define BENCH_H

// #include <stdint.h>
// #include <stdio.h>

// #include "random/random.h"

// Benchmarking
#define WARMUP          100000
#define BENCH_LOOPS     200000
#define TEST_LOOPS      10000
#define CLK_PER_SEC     3.5E9
#define CLK_PER_nSEC    3.5

uint64_t cpucycles(void);



#endif