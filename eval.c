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

// Placement Table Declarations and Initialization
// These tables are from white's perspective, flip the indexing (8-i, 8-j) and the values (-value) for black's perspecitve
// Direct comments on the table should be read bottom to top
// Pawns are encourgaed to press forward and control the center. Pawn structure will be implemented later
char pawnPlaceTable[8][8] = {
{  0,  0,  0,  0,  0,  0,  0,  0 },
{ 50, 50, 50, 50, 50, 50, 50, 50 }, // Promoting pawns is good and you should feel good
{ 10, 10, 20, 30, 30, 20, 10, 10 },
{  5,  5, 10, 25, 25, 10,  5,  5 },
{  0,  0,  0, 20, 20,  0,  0,  0 },
{  5, -5,-10,  0,  0,-10, -5,  5 },
{  5, 10, 10,-20,-20, 10, 10,  5 },
{  0,  0,  0,  0,  0,  0,  0,  0 }  // Panws don't exist down here
};

// Knights are encouraged to control the center and stay away from corners and edges, but are less harshly penalized for advancing to the other side
char knightPlaceTable[8][8] = {
{ -50,-40,-30,-30,-30,-30,-40,-50 },
{ -40,-20,  0,  0,  0,  0,-20,-40 },
{ -30,  0, 10, 15, 15, 10,  0,-30 },
{ -30,  5, 15, 20, 20, 15,  5,-30 },
{ -30,  0, 15, 20, 20, 15,  0,-30 },
{ -30,  5, 10, 15, 15, 10,  5,-30 },
{ -40,-20,  0,  5,  5,  0,-20,-40 },
{ -50,-40,-30,-30,-30,-30,-40,-50 }  // Corner knights are bad and you should feel bad
};

// Bishops are also encoruaged to control the center and stay away from corners and edges
char bishopPlaceTable[8][8] = {
{ -20,-10,-10,-10,-10,-10,-10,-20 },
{ -10,  0,  0,  0,  0,  0,  0,-10 },
{ -10,  0,  5, 10, 10,  5,  0,-10 },
{ -10,  5,  5, 10, 10,  5,  5,-10 },
{ -10,  0, 10, 10, 10, 10,  0,-10 },
{ -10, 10, 10, 10, 10, 10, 10,-10 },
{ -10,  5,  0,  0,  0,  0,  5, 10 }, // Defend your king!
{ -20,-10,-10,-10,-10,-10,-10,-20 }
};

// Rooks should stay put if it is still possible to castlize, and should remain on the center files
char rookPlaceTable[8][8] = {
{  0,  0,  0,  0,  0,  0,  0,  0 },
{  5, 10, 10, 10, 10, 10, 10,  5 }, // Congrats on advancing, +5 (except on the edges. Edges are bad)
{ -5,  0,  0,  0,  0,  0,  0, -5 },
{ -5,  0,  0,  0,  0,  0,  0, -5 },
{ -5,  0,  0,  0,  0,  0,  0, -5 },
{ -5,  0,  0,  0,  0,  0,  0, -5 },
{ -5,  0,  0,  0,  0,  0,  0, -5 },
{  0,  0,  0,  5,  5,  0,  0,  0 }  // Starting corners are good. Castling moves the rook to an equally favorable spot
};

// Queens act similarly to rooks, with a litle bonus here and there thanks to their extra mobility
char queenPlaceTable[8][8] = {
{ -20, -10, -10, -5, -5, -10, -10, -20 }, // Corners aren't as bad for Queens, but are still bad
{ -10,   0,   0,  0,  0,   0,   0, -10 }, // Congrats on advancing, you get nothing
{ -10,   0,   5,  5,  5,   5,   0, -10 }, // Edges aren't as bad either. In fact they may be good in some cases
{  -5,   0,   5,  5,  5,   5,   0,  -5 },
{   0,   0,   5,  5,  5,   5,   0,  -5 }, // That sweet sweet center control
{ -10,   5,   5,  5,  5,   5,   0, -10 },
{ -10,   0,   5,  0,  0,   0,   0, -10 },
{ -20, -10, -10, -5, -5, -10, -10, -20 }
};

// Kings should hide in their corners until the endgame, then they should head toward the center
char kingPlaceTable[8][8] = {
{ -30,-40,-40,-50,-50,-40,-40,-30 }, // How would you even get this far?
{ -30,-40,-40,-50,-50,-40,-40,-30 }, // Please stop
{ -30,-40,-40,-50,-50,-40,-40,-30, }, // WHY?
{ -30,-40,-40,-50,-50,-40,-40,-30, }, // No
{ -20,-30,-30,-40,-40,-30,-30,-20, }, // No
{ -10,-20,-20,-20,-20,-20,-20,-10, }, // Don't you dare
{  20, 20,  0,  0,  0,  0, 20, 20, }, // Could be better
{  20, 30, 10,  0,  0, 10, 30, 20 }  // Safe
};

// For the endgame, kings should move towards the center
char kingPlaceTableEnd[8][8] = {
{ -50,-40,-30,-20,-20,-30,-40,-50, }, // Bad
{ -30,-20,-10,  0,  0,-10,-20,-30, }, // Wait
{ -30,-10, 20, 30, 30, 20,-10,-30, }, // Okay
{ -30,-10, 30, 40, 40, 30,-10,-30, }, // Nice
{ -30,-10, 30, 40, 40, 30,-10,-30, }, // Nice
{ -30,-10, 20, 30, 30, 20,-10,-30, }, // Getting warmer
{ -30,-30,  0,  0,  0,  0,-30,-30, }, // No
{ -50,-30,-30,-30,-30,-30,-30,-50 }  // No
};


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
 // General Variable Declarations
	short int boardVal = 0; // Value of current board state
	char i, j, x, y; // Indexers: x = file, y = rank, j = whiteSPaces/blackSpaces index
	// char cal, car; // Values in the corners of the rook placement table. Depends on castling rights. We'll use these later
	int end = 0; //initialize flag for end of whiteSpaces/blackSpaces
	for (j = 0; j < 16; j++)
	{
		// White pieces
		y = move->whiteSpaces[j][0] / 8; // Get the piece's file index
		x = move->whiteSpaces[j][0] % 8; // Get the piece's rank index

		/*printf("White Space Y == %d\n", y);
		printf("White Space X == %d\n", x);*/

		// Condition on piece's type, and add its material value and placeTable value to boardVal
		switch (move->whiteSpaces[j][1])
		{
		case(WHITE_KING):	boardVal = boardVal + 20000 + kingPlaceTable[7 - y][x];			break;
		case(WHITE_QUEEN):	boardVal = boardVal + 900 + queenPlaceTable[7 - y][x];	break;
		case(WHITE_ROOK):	boardVal = boardVal + 500 + rookPlaceTable[7 - y][x];	break;
		case(WHITE_BISHOP):	boardVal = boardVal + 350 + bishopPlaceTable[7 - y][x];	break;
		case(WHITE_KNIGHT):	boardVal = boardVal + 325 + knightPlaceTable[7 - y][x];	break;
		case(WHITE_PAWN):	boardVal = boardVal + 100 + pawnPlaceTable[7 - y][x];	break;
		default:			end = 1;  break;
		}

		//printf("%d\n", boardVal);
		if (end == 1)
			break;
	}

	end = 0; //reset flag

	for (j = 0; j < 16; j++)
	{
		// Black pieces
		y = move->blackSpaces[j][0] / 8; // Get the piece's file index
		x = move->blackSpaces[j][0] % 8; // Get the piece's rank index

		/*printf("Black Space Y == %d\n", y);
		printf("Black Space X == %d\n", x);*/

		// Condition on piece's type, and add its material value and placeTable value to boardVal
		switch (move->blackSpaces[j][1])
		{
		case(BLACK_PAWN):	boardVal = boardVal - 100 - pawnPlaceTable[y][7 - x];	break;
		case(BLACK_KNIGHT):	boardVal = boardVal - 325 - knightPlaceTable[y][7 - x];	break;
		case(BLACK_BISHOP):	boardVal = boardVal - 350 - bishopPlaceTable[y][7 - x];	break;
		case(BLACK_ROOK):	boardVal = boardVal - 500 - rookPlaceTable[y][7 - x];	break;
		case(BLACK_QUEEN):	boardVal = boardVal - 900 - queenPlaceTable[y][7 - x];	break;
		case(BLACK_KING):	boardVal = boardVal - 20000 - kingPlaceTable[y][7 - x];			break;
		default:			end = 1; break;
		}

		//printf("%d\n", boardVal);
		if (end == 1)
			break;
	}
	//printf("%d\n", boardVal);

	//All done! Return the board value
	return(boardVal);
}