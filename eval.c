/* EVALUATION FUNCTION
This function is stil a work in progress.
It will go through a number of code blocks that will ultimately assign a value to a current board state in chess.
This function will be multiple times in a serach tree funciton 
*/

// Include relevant headers
#include "stdafx.h"
#include "board.h"
#include "space.h"
#include "eval.h"

// General Variable Declarations
short int boardVal = 0; // Value of current board state
char i, j, x, y; // Indexers
// char cal, car; // Values in the corners of the rook placement table. Depends on castling rights. We'll use these later

// Function Declaration
short int eval(Board *board, unsigned char turnCount, Move *move)
{/*
 This function starts by looping through the baord, finding occupied spaces, and switch-casing the piece type on them

INPUTS:
char* baord
	A pointer to the current chess board in memory
unsigned char* turnCount
	Represents the number of turns
Move *move
	A struct holding the possible moves that can be made
	move->picetypeMoves (8x8xmax#ofmoves array) precompiled lists of possible end locations (3rd-dim) for a given piece type and postion (1st-dim, 2nd-dim)
	move->blackSpaces (16x2 arrary) or move->whiteSpaces (16x2 array) stores the type (2nd-dim) and loactions (1st-dim) for each move black and white can make

OUTPUTS:
short int boardVal
Signed short integer that is the value of the given board in centi-pawns. Negative in favor of Black, Positive in favor of white.
*/
	
	// Material Consideration. Loop through the pieces still on the board via whiteSpaces/blackSpaces, and assign values based on each piece and its position.
	for (j = 0; j < 16; j++)
		{
		// White pieces
		y = move->whiteSpaces[j][0] / 8; // Get the piece's file index
		x = move->whiteSpaces[j][0] % 8; // Get the piece's rank index
		
		// Condition on piece's type, and add its material value and placeTable value to boardVal
		switch (move->whiteSpaces[j][1]) 
		{
		case(WHITE_KING):	boardVal = boardVal + kingPlaceTable[y][x];			break;
		case(WHITE_QUEEN):	boardVal = boardVal + 900 + queenPlaceTable[y][x];	break; 
		case(WHITE_ROOK):	boardVal = boardVal + 500 + rookPlaceTable[y][x];	break; 
		case(WHITE_BISHOP):	boardVal = boardVal + 350 + bishopPlaceTable[y][x];	break; 
		case(WHITE_KNIGHT):	boardVal = boardVal + 325 + knightPlaceTable[y][x];	break;
		case(WHITE_PAWN):	boardVal = boardVal + 100 + pawnPlaceTable[y][x];	break;
		default:			printf("Wut\n"); break;
		}
		// Black pieces
		y = move->blackSpaces[j][0] / 8; // Get the piece's file index
		x = move->blackSpaces[j][0] % 8; // Get the piece's rank index

		// Condition on piece's type, and add its material value and placeTable value to boardVal
		switch (move->blackSpaces[j][1])
		{
		case(BLACK_PAWN):	boardVal = boardVal - 100 - pawnPlaceTable[8 - y][8 - x];	break;
		case(BLACK_KNIGHT):	boardVal = boardVal - 325 - knightPlaceTable[8 - y][8 - x];	break;
		case(BLACK_BISHOP):	boardVal = boardVal - 350 - bishopPlaceTable[8 - y][8 - x];	break;
		case(BLACK_ROOK):	boardVal = boardVal - 500 - rookPlaceTable[8 - y][8 - x];	break;
		case(BLACK_QUEEN):	boardVal = boardVal - 900 - queenPlaceTable[8 - y][8 - x];	break;
		case(BLACK_KING):	boardVal = boardVal - kingPlaceTable[8 - y][8 - x];			break;
		default:			printf("Wut\n"); break;
		}
	}

	// Pawn Structure


	//All done! Return the board value
	return(boardVal);
}