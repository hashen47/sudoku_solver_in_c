#include "../headers/assert.h"
#include "../headers/util.h"


Solution_Array *Solution_Array_Init(size_t el_size)
{
	Solution_Array *arr = (Solution_Array*) malloc(sizeof(Solution_Array));
	if (arr == NULL)
		return NULL;

	arr->offset = 0;
	arr->el_size = el_size;
	arr->buf_size = 2;

	arr->buf = malloc(arr->buf_size * sizeof(char*));
	if (arr->buf == NULL)
		return NULL;

	return arr;
}


bool Solution_Array_Append(Solution_Array *arr, const char* data)
{
	ASSERT(arr != NULL, "arr cannot be NULL");
	ASSERT(arr->buf != NULL, "arr->buf cannot be NULL");

	if (arr->offset + 1 >= arr->buf_size)
	{
		arr->buf_size *= 2;

		char** new_buf = (char**)realloc(arr->buf, arr->buf_size * sizeof(char*));
		if (new_buf == NULL)
			return false;

		arr->buf = new_buf;
	}

	*(arr->buf + arr->offset) = malloc(arr->el_size * sizeof(char));
	if (*(arr->buf + arr->offset) == NULL)
		return false;

	strncpy(*(arr->buf + arr->offset), data, arr->el_size);
	arr->offset++;

	return true;
}


size_t Solution_Array_Size(Solution_Array *arr)
{
	ASSERT(arr != NULL, "arr cannot be NULL");
	ASSERT(arr->buf != NULL, "arr->buf cannot be NULL");
	return (size_t)arr->offset;
}


const char* Solution_Array_Get(Solution_Array *arr, int index)
{
	ASSERT(arr != NULL, "arr cannot be NULL");
	ASSERT(arr->buf != NULL, "arr->buf cannot be NULL");

	size_t size = Solution_Array_Size(arr);
	if (size == 0 || size <= index)
		return NULL;

	return (const char*)(*(arr->buf + index));
}


void Solution_Array_Free(Solution_Array *arr)
{
	if (arr == NULL)
		return;

	if (arr->buf != NULL)
	{
		for (size_t i = 0; i < Solution_Array_Size(arr); i++)
			free(*(arr->buf + i));

		free(arr->buf);
	}

	free(arr);
}
