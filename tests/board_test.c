#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../headers/assert.h"
#include "../headers/util.h"
#include "../headers/board.h"


int rounds = 0;


typedef struct {
	char puzzle[GRID_SQR_COUNT];
	char solution[GRID_SQR_COUNT];
} Testcase;


void load_puzzle_inputs(Testcase ***testcases)
{
	FILE *fp = fopen("data/puzzles_and_solutions.txt", "r");
	if (fp == NULL)
	{
		perror("(load_puzzles_inputs) ERROR");
		exit(EXIT_FAILURE);
	}

	fscanf(fp, "%d", &rounds);

	*testcases = (Testcase**)malloc(rounds * sizeof(Testcase*));

	char line[GRID_SQR_COUNT*2 + 1] = {0};
	for (int r = 0; r < rounds; r++)
	{
		(*testcases)[r] = malloc(sizeof(Testcase));
		fscanf(fp, "%s", line);
		strncpy((*testcases)[r]->puzzle, line, GRID_SQR_COUNT);
		strncpy(((*testcases)[r])->solution, line+GRID_SQR_COUNT+1, GRID_SQR_COUNT);
	}

	fclose(fp);
}


int main(void)
{
	clock_t duration;
	Testcase **testcases;
	load_puzzle_inputs(&testcases);

	for (int r = 0; r < rounds; r++)
	{
		Testcase *tc = testcases[r];
		Board *b = BoardInit(tc->puzzle);

		clock_t start_time = clock();
		Solution_Array *solutions = BoardSolve(b);
		duration += clock() - start_time;

		bool is_correct = false;
		for (size_t i = 0; i < Solution_Array_Size(solutions); i++)
		{
			if (strncmp((const char*)tc->solution, (const char*)Solution_Array_Get(solutions, i), GRID_SQR_COUNT) == 0)
			{
				is_correct = true;
				break;
			}
		}

		if (is_correct)
		{
			printf("TESTCASE %d: PASSED\n", r+1);
		}
		else
		{
			printf("\nTESTCASE %d: FAILED\n", r+1);

			printf("\nPuzzle: %.81s\n", tc->puzzle);
			PrintGrid(tc->puzzle);

			printf("Expect: %.81s\n", tc->solution);
			PrintGrid(tc->solution);

			if (Solution_Array_Size(solutions) > 0)
			{
				printf("Real  : %.81s\n", Solution_Array_Get(solutions, 0));
				PrintGrid(Solution_Array_Get(solutions, 0));
			}
			else
			{
				printf("Real  : No Solutions\n");
			}
			break;
		}
	}

	printf("\nDuration: %ld seconds\n", duration/CLOCKS_PER_SEC);

	exit(EXIT_SUCCESS);
}
