#ifndef EVAL_H
#define EVAL_H

// Function Prototype for eval.c
short int eval(Board *board, unsigned char turnCount, Move *move);

// Placement Table Declarations and Initialization
// These tables are from white's perspective, flip the indexing (8-i, 8-j) and the values (-value) for black's perspecitve
// Direct comments on the table should be read bottom to top

// Pawns are encourgaed to press forward and control the center. Pawn structure will be implemented later
// Pruning struct, used in MakeMoveTree
typedef struct {
	short int alphaVal;
	short int betaVal;
	Move * pruneChoice;
}Prunes;

#endif // EVAL_H

// lalala this is a change