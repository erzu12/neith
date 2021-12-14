#include "dataStructures.h"

#include <stdio.h>
#include <stdlib.h>

void** Alloc2DArr(int x, int y, int typeSize) {
    void **arr = (void**)malloc(x * sizeof(void*));
    int allocSize = y * x * typeSize;
    char* arr2 = (char*)malloc(allocSize);
    if (arr == NULL || arr2 == NULL) {
        printf("2Darray alloc error");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < x; i++) {
        arr[i] = arr2 + (i * y) * typeSize;
    }
    return arr;
}

void Del2DArr(void **arr) {
    free(arr[0]);
    free(arr);
}

