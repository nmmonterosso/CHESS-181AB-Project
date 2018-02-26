#ifndef MOVEGEN_H_
#define MOVEGEN_H_




void AddToMoveList  (MoveGen *movegen, char Start_Location, char End_Location, char Piece, char Captured_Piece);
void Addr_Conversion(char boardposition, int Board_Coordinates[2]);

void makeMoveTree(Board *board, Move *move, MoveTree *movetree, MoveGen *movegen, MoveGen *movehistory, int depth, int *MoveCounter);//Makes tree for moves
void initializeMoveGen(MoveGen *movegen);

void MoveGenFunction(Board *board, Move *move, MoveGen *movegen);
void MoveGenPawn	(Board *board, Move *move, MoveGen *movegen, int count);
void MoveGenKnight	(Board *board, Move *move, MoveGen *movegen, int count);
void MoveGenRook	(Board *board, Move *move, MoveGen *movegen, int count);
void MoveGenBishop	(Board *board, Move *move, MoveGen *movegen, int count);
void MoveGenKing	(Board *board, Move *move, MoveGen *movegen, int count);
void MoveGenQueen	(Board *board, Move *move, MoveGen *movegen, int count);

#endif // !MOVEGEN_H_
#pragma once
