#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory_lab.h"
/*
 *
 */
void* malloc(size_t size)
{
	return memory_alloc(size);
}

void free(void* ptr)
{
	memory_free(ptr);
}


