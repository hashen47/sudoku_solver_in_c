#include "util.h"


#ifndef BOARD_H
#define BOARD_H


typedef struct {
	size_t square_width;
	size_t grid_width;
	size_t grid_size;
	char *grid;
	bool *map;
} Board;


Board *BoardInit(const char *data);
void BoardPrint(Board *b);
void GridPrint(const char* grid, size_t grid_width, size_t square_width);
void BoardSetVal(Board *b, int row, int col, char val);
char BoardGetVal(Board *b, int row, int col);
bool IsBoardColOK(Board *b, int col);
bool IsBoardRowOK(Board *b, int row);
bool IsBoardBlockOK(Board *b, int row, int col);
Solution_Array *BoardSolve(Board *b);
void BoardFree(Board *b);


#endif
