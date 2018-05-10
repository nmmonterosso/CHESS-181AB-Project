#ifndef EVAL_H
#define EVAL_H

// Function Prototype for eval.c
short int eval(Board *board, unsigned char turnCount, Move *move);
int checkConnectedPawns(Board *board, int y, int x);
// Pruning struct, used in MakeMoveTree

#endif // EVAL_H