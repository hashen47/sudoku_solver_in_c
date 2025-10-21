#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/assert.h"
#include "../headers/board.h"
#include "../headers/util.h"


static void solve(Board *b, int row, int col, Solution_Array *solutions);


Board *BoardInit(const char *data)
{
	Board *b = malloc(sizeof(Board));
	if (b == NULL) return NULL; 

	b->grid = (char*)calloc(GRID_SQR_COUNT, sizeof(char));
	if (b->grid == NULL) return NULL;

	strncpy(b->grid, data, GRID_SQR_COUNT);

	return b;
}


void BoardSetVal(Board *b, int row, int col, char val)
{
	ASSERT(b, "(BoardSetVal) Board cannot be NULL");
	ASSERT(b->grid, "(BoardSetVal) Board grid cannot be NULL");
	b->grid[row * 9 + col] = val;
}


char BoardGetVal(Board *b, int row, int col)
{
	ASSERT(b, "(BoardGetVal) Board cannot be NULL");
	ASSERT(b->grid, "(BoardGetVal) Board grid cannot be NULL");
	return b->grid[9*row + col];
}


void PrintGrid(const char *grid)
{
	ASSERT(grid, "(PrintGrid) grid cannot be NULL");

	printf("\n");
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (j == 0)
				printf(" ");

			printf("%c ", grid[9*i + j]);

			if ((j+1) % 3 == 0 && j+1 != 9)
				printf("| ");

			if (j+1 == 9 && true)
				printf("\n");
		}

		if ((i+1) % 3 == 0 && i+1 != 9)
			printf("-------+-------+-------\n");
	}
	printf("\n");
}


bool IsBoardColOK(Board *b, int col)
{
	ASSERT(b, "(IsBoardColOK) Board cannot be NULL");
	ASSERT(b->grid, "(IsBoardColOK) Board grid cannot be NULL");
	ASSERT(col >= 0, "(IsBoardColOK) col cannot be negative");
	ASSERT(col < 9, "(IsBoardColOK) col cannot be exceed 8");

	bool map[9] = {0};
	for (int row = 0; row < 9; row++)
	{
		int index = (BoardGetVal(b, row, col) - '0') - 1;
		if (index < 0) continue;

		if (map[index])
			return false;
		map[index] = true;
	}

	return true;
}


bool IsBoardRowOK(Board *b, int row)
{
	ASSERT(b, "(IsBoardRowOK) Board cannot be NULL");
	ASSERT(b->grid, "(IsBoardRowOK) Board grid cannot be NULL");
	ASSERT(row >= 0, "(IsBoardRowOK) row cannot be negative");
	ASSERT(row < 9, "(IsBoardRowOK) row cannot be exceed 8");

	bool map[9] = {0};
	for (int col = 0; col < 9; col++)
	{
		int index = (BoardGetVal(b, row, col) - '0') - 1;
		if (index < 0) continue;

		if (map[index])
			return false;
		map[index] = true;
	}

	return true;
}


bool IsBoardBlockOK(Board *b, int row, int col)
{
	ASSERT(b, "(IsBoardBlockOK) Board cannot be NULL");
	ASSERT(b->grid, "(IsBoardBlockOK) Board grid cannot be NULL");
	ASSERT(row >= 0, "(IsBoardBlockOK) row cannot be negative");
	ASSERT(row < 9, "(IsBoardBlockOK) row cannot exceed 8");
	ASSERT(col >= 0, "(IsBoardBlockOK) col cannot be negative");
	ASSERT(col < 9, "(IsBoardBlockOK) col cannot exceed 8");

	int start_col = 6;
	int start_row = 6;

	if (row < 3)
	{
		start_row = 0;
		if (col < 3)
			start_col = 0;
		else if (col < 6)
			start_col = 3;
	}
	else if (row < 6)
	{
		start_row = 3;
		if (col < 3)
			start_col = 0;
		else if (col < 6)
			start_col = 3;
	}
	else
	{
		if (col < 3)
			start_col = 0;
		else if (col < 6)
			start_col = 3;
	}

	bool map[9] = {0};
	for (int row = start_row; row < start_row+3; row++)
	{
		for (int col = start_col; col < start_col+3; col++)
		{
			int index = (BoardGetVal(b, row, col) - '0') - 1;
			if (index < 0) continue;

			if (map[index])
				return false;
			map[index] = true;
		}
	}

	return true;
}


static void solve(Board *b, int row, int col, Solution_Array *solutions)
{
	if (row >= 9)
	{
		Solution_Array_Append(solutions, b->grid);
		return;
	}

	int next_row = row;
	if (col >= 8)
		next_row++;

	if (BoardGetVal(b, row, col) != '0')
	{
		solve(b, next_row, (col+1) % 9, solutions);
		return;
	}

	char prev_val = BoardGetVal(b, row, col);
	for (int val = 1; val < 10; val++)
	{
		BoardSetVal(b, row, col, (char)('0' + val));

		if (IsBoardColOK(b, col) && IsBoardRowOK(b, row) && IsBoardBlockOK(b, row, col))
			solve(b, next_row, (col+1) % 9, solutions);

		BoardSetVal(b, row, col, prev_val);
	}
}


Solution_Array *BoardSolve(Board *b)
{
	Solution_Array *solutions = Solution_Array_Init(GRID_SQR_COUNT);
	if (solutions == NULL)
		return NULL;

	solve(b, 0, 0, solutions);
	return solutions;
}
