#include <thread>
#include <vector>
#include <iostream>
#include <assert.h>

#include "counter.h"

#define NUM_THREADS 3
#define ITERATIONS 12

void increment() {
    for(int i=0; i<ITERATIONS; i++) {
        incr();
    }
}

int main() {

    init_counter_lib();

    std::vector<std::thread> threads;
    for(int i=0; i < NUM_THREADS; i++) {
        threads.push_back(std::thread(increment));
    }

    for(int i=0; i < NUM_THREADS; i++) {
        threads[i].join();
    }
    std::cout << "Final count: " << get() << std::endl;
    assert(get() == NUM_THREADS * ITERATIONS);
    return 0;
}