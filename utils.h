#ifndef _UTILS_H__
#define _UTILS_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>


// https://nullprogram.com/blog/2022/06/26/#addendum-dont-exit-the-debugger
#define assert(c) if (!(c)) __builtin_trap();

//improve with a logger
#define check_malloc(c) if(!(c)) fprintf(stderr, "Malloc failed !\n"); exit(EXIT_FAILURE); 


#endif // _UTILS_H__
