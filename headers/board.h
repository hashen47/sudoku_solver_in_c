#include "util.h"


#ifndef BOARD_H
#define BOARD_H


#define GRID_SQR_COUNT 81


typedef struct {
	char *grid;
} Board;


Board *BoardInit(const char *data);
void PrintGrid(const char* grid);
void BoardSetVal(Board *b, int row, int col, char val);
char BoardGetVal(Board *b, int row, int col);
bool IsBoardColOK(Board *b, int col);
bool IsBoardRowOK(Board *b, int row);
bool IsBoardBlockOK(Board *b, int row, int col);
Solution_Array *BoardSolve(Board *b);
void BoarFree(Board *b);


#endif
