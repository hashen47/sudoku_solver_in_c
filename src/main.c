#include <stdio.h>
#include <stdlib.h>
#include "../headers/board.h"
#include "../headers/util.h"


int main(int argc, char **argv)
{
	if (argc == 1)
	{
		printf("Puzzle input is required...\n");
		exit(EXIT_FAILURE);
	}

	const char* puzzle = *(argv + 1);
	if (strlen(puzzle) != GRID_SQR_COUNT)
	{
		printf("Invalid puzzle input...\n");
		exit(EXIT_FAILURE);
	}


	Board *b = BoardInit(puzzle);
	Solution_Array* solutions = BoardSolve(b);
	if (solutions == NULL)
	{
		printf("Internal Error Occur...");
		exit(EXIT_FAILURE);
	}

	printf("\nPuzzle:\n");
	PrintGrid(puzzle);

	if (Solution_Array_Size(solutions) == 0)
	{
		printf("This puzzle hasn't any solution...");
		exit(EXIT_FAILURE);
	}

	printf("Solutions:\n\n");
	for (size_t i = 0; i < Solution_Array_Size(solutions); i++)
	{
		const char* solution = Solution_Array_Get(solutions, i);
		printf("Solution: %zu\n", i+1);
		PrintGrid(solution);
	}

	Solution_Array_Free(solutions);

	exit(EXIT_SUCCESS);
}
