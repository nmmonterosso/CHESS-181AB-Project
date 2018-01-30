#ifndef BOARD_H_
#define BOARD_H_

#include "space.h"


typedef struct {

	space boardSpaces[8][8];

}Board;





#endif // !
#

void makeBoard(Board *board);
void setRookMoves(Board *board, int i, int j);//TODO Precomputes vertical and horizontal movement for each space on the board
void printBoard(Board *board);
void setSpace(Board *board, unsigned int i, unsigned int j);