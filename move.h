#ifndef MOVE_H_
#define MOVE_H_
#include "board.h"

typedef struct {

	char rookMoves[8][8][14]; //8x8 array of 14 long array. (3D array). Exactly 14 destinations exist for each rook.
	char knightMoves[8][8][8]; //Knights can move at max 8 different locations
	char bishopMoves[8][8][13]; //Bishops can move at maximum 8 locations
	//TODO Put Pawn Moves as well:

}Move;

void setRookMoves(Board *board, int i, int j, Move *move);//TODO Precomputes vertical and horizontal movement for each space on the board
void setMoves(Board *board, Move *move);
#endif // !MOVE_H_
#pragma once
