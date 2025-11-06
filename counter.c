
#include <stddef.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

enum OP {
   INCR,
   GET
};

typedef struct counter_data {
    enum OP op;
    int getResult; 
} CounterData;

void* counter_thread(void* argos) {
    static int count = 0; 
    sigset_t set;
    siginfo_t info;
    sigemptyset(&set);
    sigaddset(&set, SIGRTMIN);
    while(1) {
        int ret = sigwaitinfo(&set, &info);
        if (ret == SIGRTMIN) {
            // Check if sender provided data
            if (info.si_code == SI_QUEUE) {
                CounterData *data = (CounterData *)info.si_value.sival_ptr;
                if(data) {
                    switch(data->op) {
                        case INCR:
                            count++;
                            break;
                        case GET:
                            data->getResult = count; 
                            break;
                        default:
                             fprintf(stderr, "Unknown OP: %d\n", data->op);
                    }
                }
            }
        }
    }
}

static pthread_t counterTid;

// Must be called from the main thread before any threads are created!
void init_counter_lib() {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGRTMIN);
    pthread_sigmask(SIG_BLOCK, &set, NULL);
    pthread_create(&counterTid, NULL, counter_thread, NULL);
    sleep(1); // Make 
}

static CounterData incrData = { INCR, 0};

void incr() {
    union sigval val;
    val.sival_ptr = &incrData;
    if(sigqueue(getpid(), SIGRTMIN, val)) {
        fprintf( stderr, "Error on incr: %d: %s\n", errno, strerror(errno));
    }
}

int get() {
    CounterData *data   = malloc(sizeof(CounterData));
    data->op = GET;
    data->getResult = -1;
    union sigval val;
    val.sival_ptr = data;
    if(sigqueue(getpid(), SIGRTMIN, val)) {
       fprintf( stderr, "Error on get: %d: %s\n", errno, strerror(errno));
    }
    while(data->getResult == -1) sched_yield();
    int result = data->getResult;
    free(data);
    return result;
}