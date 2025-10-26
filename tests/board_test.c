#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../headers/assert.h"
#include "../headers/util.h"
#include "../headers/board.h"


typedef struct {
	char *puzzle;
	char *solution;
	size_t grid_size;
} Testcase;


Testcase *TestcaseInit(size_t grid_size);
void TestcaseSetPuzzle(Testcase *tc, const char* puzzle);
void TestcaseSetSolution(Testcase *tc, const char* solution);
void TestcaseFree(Testcase *tc);

void load_testcases(const char* filepath, Testcase ***testcases, size_t grid_size, int *rounds);
void testing(const char* filepath, size_t grid_size);


int main(void)
{
	testing("data/puzzles_and_solutions.txt", 81);
	exit(EXIT_SUCCESS);
}


Testcase *TestcaseInit(size_t grid_size)
{
	Testcase *tc = (Testcase*)malloc(sizeof(Testcase));
	if (tc == NULL)
	{
		perror("(TestcaseInit) ERROR");
		exit(EXIT_FAILURE);
	}

	tc->puzzle    = (char*)calloc(grid_size+1, sizeof(char)); 
	tc->solution  = (char*)calloc(grid_size+1, sizeof(char)); 
	tc->grid_size = grid_size;

	if (tc->puzzle == NULL || tc->solution == NULL)
	{
		perror("(TestcaseInit) Puzzle And Solution ERROR");
		exit(EXIT_FAILURE);
	}

	return tc;
}


void TestcaseSetPuzzle(Testcase *tc, const char* puzzle)
{
	strncpy(tc->puzzle, puzzle, tc->grid_size);
}


void TestcaseSetSolution(Testcase *tc, const char* solution)
{
	strncpy(tc->solution, solution, tc->grid_size);
}


void TestcaseFree(Testcase *tc)
{
	if (tc == NULL)
		return;

	free(tc->puzzle);
	free(tc->solution);
	free(tc);
}


void load_testcases(const char* filepath, Testcase ***testcases, size_t grid_size, int *rounds)
{
	FILE *fp = fopen(filepath, "r");
	if (fp == NULL)
	{
		perror("(load_puzzles_inputs) ERROR");
		exit(EXIT_FAILURE);
	}

	fscanf(fp, "%d", rounds);

	*testcases = (Testcase**)malloc(*rounds * sizeof(Testcase*));

	char *line = (char*)calloc(grid_size*2 + 1, sizeof(char));

	for (int r = 0; r < *rounds; r++)
	{
		fscanf(fp, "%s", line);
		(*testcases)[r] = TestcaseInit(grid_size+1);
		TestcaseSetPuzzle((*testcases)[r], line);
		TestcaseSetSolution((*testcases)[r], line+grid_size+1);
	}

	free(line);
	fclose(fp);
}


void testing(const char* filepath, size_t grid_size)
{
	int rounds = 0;
	Testcase **testcases;
	load_testcases(filepath, &testcases, grid_size, &rounds);

	clock_t duration;
	for (int r = 0; r < rounds; r++)
	{
		clock_t start_time = clock();

		Testcase *tc = testcases[r];
		Board *b = BoardInit(tc->puzzle);
		Solution_Array *solutions = BoardSolve(b);

		bool is_correct = false;
		for (size_t i = 0; i < Solution_Array_Size(solutions); i++)
		{
			if (strncmp((const char*)tc->solution, (const char*)Solution_Array_Get(solutions, i), b->grid_size) == 0)
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

			printf("\nPuzzle: %s\n", tc->puzzle);
			GridPrint(tc->puzzle, b->grid_width, b->square_width);

			printf("Expect: %s\n", tc->solution);
			GridPrint(tc->solution, b->grid_width, b->square_width);

			if (Solution_Array_Size(solutions) > 0)
			{
				printf("Real  : %.81s\n", Solution_Array_Get(solutions, 0));
				GridPrint(Solution_Array_Get(solutions, 0), b->grid_width, b->square_width);
			}
			else
			{
				printf("Real  : No Solutions\n");
			}
			break;
		}

		Solution_Array_Free(solutions);
		BoardFree(b);
		duration += clock() - start_time;
	}

	for (int r = 0; r < rounds; r++)
		TestcaseFree(testcases[r]);

	free(testcases);

	printf("\nDuration: %ld seconds\n", duration/CLOCKS_PER_SEC);
}
