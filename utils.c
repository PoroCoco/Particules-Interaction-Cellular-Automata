#include <stdlib.h>
#include <stdio.h>

#include "utils.h"



long get_elapsed_time(struct timeval start, struct timeval end)
{
	return ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec))/1000.0;
}