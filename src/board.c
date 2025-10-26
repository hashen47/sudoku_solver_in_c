#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../headers/assert.h"
#include "../headers/board.h"
#include "../headers/util.h"


static void solve(Board *b, int row, int col, Solution_Array *solutions);
void reset_map(bool *map, size_t len);


Board *BoardInit(const char *data)
{
	Board *b = malloc(sizeof(Board));
	if (b == NULL) return NULL; 

	size_t grid_size    = strlen(data);
	size_t grid_width   = (size_t)sqrt(grid_size);
	size_t square_width = (size_t)sqrt(grid_width);
	grid_size = grid_width * grid_width;

	if (grid_size / grid_width != grid_width || grid_width / square_width != square_width)
		return NULL;

	b->grid = (char*)calloc(grid_size, sizeof(char));
	if (b->grid == NULL)
		return NULL;

	b->grid_size    = grid_size;
	b->grid_width   = grid_width;
	b->square_width = square_width;

	b->map = (bool*)calloc(b->grid_width, sizeof(bool)); 
	if (b->map == NULL)
		return NULL;

	strncpy(b->grid, data, grid_size);

	return b;
}


void reset_map(bool *map, size_t len)
{
	for (size_t i = 0; i < len; i++)
		map[i] = false;
}


void BoardSetVal(Board *b, int row, int col, char val)
{
	ASSERT(b, "(BoardSetVal) Board cannot be NULL");
	ASSERT(b->grid, "(BoardSetVal) Board grid cannot be NULL");
	b->grid[row * b->grid_width + col] = val;
}


char BoardGetVal(Board *b, int row, int col)
{
	ASSERT(b, "(BoardGetVal) Board cannot be NULL");
	ASSERT(b->grid, "(BoardGetVal) Board grid cannot be NULL");
	return b->grid[row * b->grid_width + col];
}


void BoardPrint(Board *b)
{
	ASSERT(b, "(BoardPrint) b cannot be NULL");
	ASSERT(b->grid, "(BoardPrint) b->grid cannot be NULL");

	printf("\n");
	for (int i = 0; i < b->grid_width; i++)
	{
		for (int j = 0; j < b->grid_width; j++)
		{
			if (j == 0)
				printf(" ");

			printf("%c ", b->grid[b->grid_width*i + j]);

			if ((j+1) % b->square_width == 0 && j+1 != b->grid_width)
				printf("| ");

			if (j+1 == b->grid_width && true)
				printf("\n");
		}

		if ((i+1) % b->square_width == 0 && i+1 != b->grid_width)
		{
			for (int k = 0; k < b->square_width; k++)
			{
				for (int l = 0; l < b->square_width+1; l++)
				{
					printf("-");
					if (l+1 != b->square_width+1)
						printf("-");
					else if (k+1 != b->square_width)
						printf("+");
				}
			}
			printf("\n");
		}
		// printf("-------+-------+-------\n");
	}
	printf("\n");
}


void GridPrint(const char* grid, size_t grid_width, size_t square_width)
{
	ASSERT(grid != NULL, "(GridPrint) grid cannot be NULL");

	printf("\n");
	for (int i = 0; i < grid_width; i++)
	{
		for (int j = 0; j < grid_width; j++)
		{
			if (j == 0)
				printf(" ");

			printf("%c ", grid[grid_width*i + j]);

			if ((j+1) % square_width == 0 && j+1 != grid_width)
				printf("| ");

			if (j+1 == grid_width && true)
				printf("\n");
		}

		if ((i+1) % square_width == 0 && i+1 != grid_width)
		{
			for (int k = 0; k < square_width; k++)
			{
				for (int l = 0; l < square_width+1; l++)
				{
					printf("-");
					if (l+1 != square_width+1)
						printf("-");
					else if (k+1 != square_width)
						printf("+");
				}
			}
			printf("\n");
		}
		// printf("-------+-------+-------\n");
	}
	printf("\n");
}


bool IsBoardColOK(Board *b, int col)
{
	ASSERT(b, "(IsBoardColOK) Board cannot be NULL");
	ASSERT(b->grid, "(IsBoardColOK) Board grid cannot be NULL");
	ASSERT(col >= 0, "(IsBoardColOK) col cannot be negative");
	ASSERT(col < b->grid_width, "(IsBoardColOK) col cannot be exceed 8");

	reset_map(b->map, b->grid_width);
	for (int row = 0; row < b->grid_width; row++)
	{
		int index = (BoardGetVal(b, row, col) - '0') - 1;
		if (index < 0) continue;

		if (b->map[index])
			return false;
		b->map[index] = true;
	}

	return true;
}


bool IsBoardRowOK(Board *b, int row)
{
	ASSERT(b, "(IsBoardRowOK) Board cannot be NULL");
	ASSERT(b->grid, "(IsBoardRowOK) Board grid cannot be NULL");
	ASSERT(row >= 0, "(IsBoardRowOK) row cannot be negative");
	ASSERT(row < b->grid_width, "(IsBoardRowOK) row cannot be exceed 8");

	reset_map(b->map, b->grid_width);
	for (int col = 0; col < b->grid_width; col++)
	{
		int index = (BoardGetVal(b, row, col) - '0') - 1;
		if (index < 0) continue;

		if (b->map[index])
			return false;
		b->map[index] = true;
	}

	return true;
}


bool IsBoardBlockOK(Board *b, int row, int col)
{
	ASSERT(b, "(IsBoardBlockOK) Board cannot be NULL");
	ASSERT(b->grid, "(IsBoardBlockOK) Board grid cannot be NULL");
	ASSERT(row >= 0, "(IsBoardBlockOK) row cannot be negative");
	ASSERT(row < b->grid_width, "(IsBoardBlockOK) row cannot exceed 8");
	ASSERT(col >= 0, "(IsBoardBlockOK) col cannot be negative");
	ASSERT(col < b->grid_width, "(IsBoardBlockOK) col cannot exceed 8");

	int start_col = 0;
	int start_row = 0;

	for (int row_mult = 0; row_mult < b->square_width-1; row_mult++)
	{
		if (b->square_width + b->square_width * row_mult)
		{
			start_row = b->square_width * row_mult;
			break;
		}
	}

	for (int col_mult = 0; col_mult < b->square_width-1; col_mult++)
	{
		if (b->square_width + b->square_width * col_mult)
		{
			start_col = b->square_width * col_mult;
			break;
		}
	}

	reset_map(b->map, b->grid_width); 
	for (int row = start_row; row < start_row+b->square_width; row++)
	{
		for (int col = start_col; col < start_col+b->square_width; col++)
		{
			int index = (BoardGetVal(b, row, col) - '0') - 1;
			if (index < 0) continue;

			if (b->map[index])
				return false;
			b->map[index] = true;
		}
	}

	return true;
}


static void solve(Board *b, int row, int col, Solution_Array *solutions)
{
	if (row >= b->grid_width)
	{
		Solution_Array_Append(solutions, b->grid);
		return;
	}

	int next_row = row;
	if (col >= b->grid_width-1)
		next_row++;

	if (BoardGetVal(b, row, col) != '0')
	{
		solve(b, next_row, (col+1) % b->grid_width, solutions);
		return;
	}

	char prev_val = BoardGetVal(b, row, col);
	for (int val = 1; val < b->grid_width+1; val++)
	{
		BoardSetVal(b, row, col, (char)('0' + val));

		if (IsBoardColOK(b, col) && IsBoardRowOK(b, row) && IsBoardBlockOK(b, row, col))
			solve(b, next_row, (col+1) % b->grid_width, solutions);

		BoardSetVal(b, row, col, prev_val);
	}
}


Solution_Array *BoardSolve(Board *b)
{
	Solution_Array *solutions = Solution_Array_Init(b->grid_size);
	if (solutions == NULL)
		return NULL;

	solve(b, 0, 0, solutions);
	return solutions;
}


void BoardFree(Board *b)
{
	if (b == NULL)
		return;
	free(b->grid);
	free(b->map);
	free(b);
}
