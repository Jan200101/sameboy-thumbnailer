#include <stdlib.h>
#include <stdio.h>

#include "common.h"

void* local_malloc(size_t size)
{
    void* mem = malloc(size);
    if (!mem) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(-1);
    }
    return mem; 
}