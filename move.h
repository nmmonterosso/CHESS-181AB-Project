#ifndef MOVE_H_
#define MOVE_H_
#include "board.h"



void makeMove(Board *board, MoveList move, MoveGen *moveHistory, Move *moveSpace);
void unMakeMove(Board *board, MoveGen *moveHistory, Move *moveSpace);
void updateColorSpaces(Board *board, MoveList move, Move *movespace, int undo);
void updateCastleColor(Board *board, MoveList move, Move *moveSpace, int undo);
void updateEPSquare(Board *board, MoveList move);
void updatePrevEPSquare(Board *board, MoveList prevmove);
void updateKingCoordinates(Board *board, char piece, int i, int j);
void clearMoveList(MoveList *movelist);

void demoteWhitePiece(Board *board, MoveList move);
void demoteBlackPiece(Board *board, MoveList move);
void demoteWhiteSpace(Move *moveSpace, MoveList move, int i);
void demoteBlackSpace(Move *moveSpace, MoveList move, int i);
void promotePawn(Board *board, MoveList move);
void promoteWhiteSpace(Move *moveSpace, char promotedPiece, int i);
void promoteBlackSpace(Move *moveSpace, char promotedPiece, int i);

void whiteCastleKingSide(Board *board);
void whiteCastleQueenSide(Board *board);
void blackCastleKingSide(Board *board);
void blackCastleQueenSide(Board *board);

void unmakeWhiteCastleKingSide(Board *board, MoveGen *movehistory);
void unmakeWhiteCastleQueenSide(Board *board, MoveGen *movehistory);
void unmakeBlackCastleKingSide(Board *board, MoveGen *movehistory);
void unmakeBlackCastleQueenSide(Board *board, MoveGen *movehistory);

void changeWhiteCastlingRights(Board *board, MoveList move);
void changeBlackCastlingRights(Board *board, MoveList move);
void restoreWhiteCastlingRights(Board *board, MoveGen *moveHistory);
void restoreBlackCastlingRights(Board *board, MoveGen *moveHistory);

void updateWhiteCastleSpace(Board *board, Move *moveSpace, char side);
void updateBlackCastleSpace(Board *board, Move *moveSpace, char side);
void undoWhiteCastleSpace(Board *board, Move *moveSpace, char side);
void undoBlackCastleSpace(Board *board, Move *moveSpace, char side);

void setBishopMoves(Board *board, int i, int j, Move *move);
void setKnightMoves(Board *board, int i, int j, Move *move);
void setRookMoves(Board *board, int i, int j, Move *move);//TODO Precomputes vertical and horizontal movement for each space on the board
void setMoves(Board *board, Move *move, MoveGen *movegen, MoveGen *movehistory);



//void checkBishopMoves(Board *board, char *bishopMoves[8][8][13]);
//void checkKnightMoves(Board *board, char *knightMoves[8][8][8]);
//void checkRookMoves  (Board *board, char *rookMoves[8][8][14]);
//void checkMoves	   (Board *board, Move *move);
#endif // !MOVE_H_
#pragma once