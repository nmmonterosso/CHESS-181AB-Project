#ifndef MOVE_H_
#define MOVE_H_
#include "board.h"

void setBishopMoves(Board *board, int i, int j, Move *move);
void setKnightMoves(Board *board, int i, int j, Move *move);
void setRookMoves(Board *board, int i, int j, Move *move);//TODO Precomputes vertical and horizontal movement for each space on the board
void setMoves(Board *board, Move *move, MoveGen *movegen);

//void checkBishopMoves(Board *board, char *bishopMoves[8][8][13]);
//void checkKnightMoves(Board *board, char *knightMoves[8][8][8]);
//void checkRookMoves  (Board *board, char *rookMoves[8][8][14]);
//void checkMoves	   (Board *board, Move *move);
#endif // !MOVE_H_
#pragma once
