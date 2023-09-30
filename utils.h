#ifndef _UTILS_H__
#define _UTILS_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <sys/time.h>


// https://nullprogram.com/blog/2022/06/26/#addendum-dont-exit-the-debugger
#define assert(c) if (!(c)) __builtin_trap();

//improve with a logger
#define check_malloc(c) if(!(c)){ fprintf(stderr, "Malloc failed !\n"); exit(EXIT_FAILURE); } 

long get_elapsed_time(struct timeval start, struct timeval end);

#define TIME_BLOCK_EXEC(msg, ...) do {                                           \
    struct timeval __start, __end;                                               \
    gettimeofday(&__start, NULL);                                                \
    __VA_ARGS__                                                                  \
    gettimeofday(&__end, NULL);                                                  \
    printf("%s execution time: %ldms\n", msg, get_elapsed_time(__start, __end)); \
} while(0);


#endif // _UTILS_H__
