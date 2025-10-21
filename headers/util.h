#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#ifndef UTIL_H
#define UTIL_H


typedef struct {
	int offset;
	size_t el_size;
	size_t buf_size;
	char** buf;
} Solution_Array;


Solution_Array *Solution_Array_Init(size_t el_size);
bool Solution_Array_Append(Solution_Array *arr, const char* data);
size_t Solution_Array_Size(Solution_Array *arr);
const char* Solution_Array_Get(Solution_Array *arr, int index);
void Solution_Array_Free(Solution_Array *arr);


#endif
