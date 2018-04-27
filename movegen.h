#ifndef MOVEGEN_H_
#define MOVEGEN_H_

typedef struct {
	short int boardVal;
	MoveList pruneMove;
} Prunes;


void AddToMoveList  (MoveGen *movegen, char Start_Location, char End_Location, char Piece, char Captured_Piece);
void Addr_Conversion(char boardposition, int Board_Coordinates[2]);

int checkKingSafety		(Board *board, int i,   int j);
int checkKingDiagonal	(Board *board, int row, int col);
int checkKingKnight		(Board *board, int row, int col);
int checkKingHorizontal	(Board *board, int row, int col);
int checkKingVertical	(Board *board, int row, int col);
int checkKingPawn		(Board *board, int row, int col);
int checkCastle			(Board * board, char castle);

Prunes makeMoveTree(Board *board, Move *move, MoveTree *movetree, MoveGen *movegen, MoveGen *movehistory, int depth, short int alphaVal, short int betaVal, MoveList pruneChoice);//Makes tree for moves
void initializeMoveGen(MoveGen *movegen);

void MoveGenFunction(Board *board, Move *move, MoveGen *movegen);
void MoveGenPawn	(Board *board, Move *move, MoveGen *movegen, int count);
void MoveGenKnight	(Board *board, Move *move, MoveGen *movegen, int count);
void MoveGenRook	(Board *board, Move *move, MoveGen *movegen, int count);
void MoveGenBishop	(Board *board, Move *move, MoveGen *movegen, int count);
void MoveGenQueen(Board *board, Move *move, MoveGen *movegen, int count);
void MoveGenKing	(Board *board, Move *move, MoveGen *movegen, int count);
void checkWhiteCastle(Board *board, MoveGen *movegen);
void checkBlackCastle(Board *board, MoveGen *movegen);

//Move Ordering
void quickSortMoveGen(MoveGen *movegen, int frontPivot, int backPivot);
void swapMoves(MoveGen *movegen, int leftPivot, int rightPivot);
int getSortValue(MoveList *move);
int partition(MoveGen *movegen, int frontPivot, int backPivot);
int isIndexHigher(MoveList *move1, MoveList *move2);
int isIndexLower(MoveList *move1, MoveList *move2);

//RESET PRUNES:
void resetPruneChoice(MoveList *prunechoice);
void resetPrunes(Prunes *prunes);
#endif // !MOVEGEN_H_
#pragma once
