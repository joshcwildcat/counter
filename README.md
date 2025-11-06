Implement a thread-safe counter in C without using any standard synchronosation techniques (no Atomics, mutuxes, semaphores etc).

The trick I used was to use a single helper thread to manage the counter. Work is dispatched to the thread by using the POSIX extension for real time signals. We are depending on the fact these signals will be queued and dispatched in a FIFO manner.

This does not work on Mac OS natively since it doesn't support real time signals. I've provided a dockerfile that can be used.

## On Mac OS (and probably Windows)

In the project root directory build the docker image:

% docker build -t linux-dev .

Then start it and open a shell:

% docker run --rm -it -v $(pwd):/workspace linux-dev

Then follow Linux instructions:

## Linux

gcc -c counter.c

g++ main.cc counter.o

./a.out