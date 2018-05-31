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
#include "movegen.h"

// Placement Table Declarations and Initialization
// These tables are from white's perspective, flip the indexing (8-i, 8-j) and the values (-value) for black's perspecitve
// Direct comments on the table should be read bottom to top
// Pawns are encourgaed to press forward and control the center. Pawn structure will be implemented later
char pawnPlaceTable[8][8] = {
{ 0,  0,  0,  0,  0,  0,  0,  0 },
{ 50, 50, 50, 50, 50, 50, 50, 50 }, // Promoting pawns is good and you should feel good
{ 10, 10, 20, 30, 30, 20, 10, 10 },
{ 5,  5, 10, 25, 25, 10,  5,  5 },
{ 0,  0,  0, 20, 20,  0,  0,  0 },
{ 5, -5,-10,  0,  0,-10, -5,  5 },
{ 5, 10, 10,-20,-20, 10, 10,  5 },
{ 0,  0,  0,  0,  0,  0,  0,  0 }  // Panws don't exist down here
};

char pawnPlaceTableMid[8][8] = {
{ 0,  0,  0,  0,  0,  0,  0,  0 },
{ 50, 50, 50, 50, 50, 50, 50, 50 }, // Promoting pawns is good and you should feel good
{ 15, 20, 30, 30, 30, 30, 20, 15 },
{ 10, 15, 20, 25, 25, 20, 15, 10 },
{ 5, 10, 15, 20, 20, 15,  5,  5 },
{ 5,  5,  0,  0,  0,  0,  5,  5 },
{ 0,  0,  0,-20,-20,  0,  0,  0 },
{ 0,  0,  0,  0,  0,  0,  0,  0 }  // Panws don't exist down here
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
{ 0,  0,  0,  0,  0,  0,  0,  0 },
{ 5, 10, 10, 10, 10, 10, 10,  5 }, // Congrats on advancing, +5 (except on the edges. Edges are bad)
{ -5,  0,  0,  0,  0,  0,  0, -5 },
{ -5,  0,  0,  0,  0,  0,  0, -5 },
{ -5,  0,  0,  0,  0,  0,  0, -5 },
{ -5,  0,  0,  0,  0,  0,  0, -5 },
{ -5,  0,  0,  0,  0,  0,  0, -5 },
{  0,  0,  0,  5,  5,  0,  0,  0 }  // Starting corners are good. Castling moves the rook to an equally favorable spot
};


// Queens act similarly to rooks, with a litle bonus here and there thanks to their extra mobility
/*char queenPlaceTable[8][8] = {
{ -20, -10, -10, -5, -5, -10, -10, -20 }, // Corners aren't as bad for Queens, but are still bad
{ -10,   0,   0,  0,  0,   0,   0, -10 }, // Congrats on advancing, you get nothing
{ -10,   0,   5,  5,  5,   5,   0, -10 }, // Edges aren't as bad either. In fact they may be good in some cases
{  -5,   0,   5,  5,  5,   5,   0,  -5 },
{   0,   0,   5,  5,  5,   5,   0,  -5 }, // That sweet sweet center control
{ -10,   5,   5,  5,  5,   5,   0, -10 },
{ -10,   0,   5,  0,  0,   0,   0, -10 },
{ -20, -10, -10, -5, -5, -10, -10, -20 }
};*/

// Kings should hide in their corners until the endgame, then they should head toward the center
char kingPlaceTable[8][8] = {
{ -30,-40,-40,-50,-50,-40,-40,-30  }, // How would you even get this far?
{ -30,-40,-40,-50,-50,-40,-40,-30  }, // Please stop
{ -30,-40,-40,-50,-50,-40,-40,-30, }, // WHY?
{ -30,-40,-40,-50,-50,-40,-40,-30, }, // No
{ -20,-30,-30,-40,-40,-30,-30,-20, }, // No
{ -10,-20,-20,-20,-20,-20,-20,-10, }, // Don't you dare
{ 20, 20,  0,  0,  0,  0, 20, 20, }, // Could be better
{ 20, 40, 10,  0,  0, 40, 30, 20  }  // Safe
};

// For the endgame, kings should move towards the center
char kingPlaceTableEnd[8][8] = {
{ -50,-40,-30,-20,-20,-30,-40,-50, }, // 0
{ -30,-20,-10,  0,  0,-10,-20,-30, }, // 1
{ -30,-10, 20, 30, 30, 20,-10,-30, }, // 2
{ -30,-10, 30, 40, 40, 30,-10,-30, }, // 3
{ -30,-10, 30, 40, 40, 30,-10,-30, }, // 4
{ -30,-10, 20, 30, 30, 20,-10,-30, }, // 5
{ -30,-30,  0,  0,  0,  0,-30,-30, }, // 6
{ -50,-30,-30,-30,-30,-30,-30,-50  }  // 7
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
	short int materialVal = 0; // Material value of the current piece being examined
	short int pawnStructVal = 0; // Value of the pawn structure for a player
	char currSpace; // The current numeric space
	char j, x, y; // Indexers: x = file, y = rank, j = whiteSPaces/blackSpaces index, i,p = pawn array index
	short int ldBonus, lBonus, luBonus; // Flags marking if a pawn is left up, left down, or left adjacent
	int bishopCount = 0;
										// char cal, car; // Values in the corners of the rook placement table. Depends on castling rights. We'll use these later
	int end = 0; //initialize flag for end of whiteSpaces/blackSpaces

				 // White pieces consideration
	for (j = 0; j < 16; j++)
	{
		currSpace = move->whiteSpaces[j][0];
		y = currSpace / 8; // Get the piece's file index
		x = currSpace % 8; // Get the piece's rank index

		//printf("White Space Y == %d\n", y);
		//printf("White Space X == %d\n", x);

		// Condition on piece's type, and add its material value and placeTable value to boardVal
		switch (move->whiteSpaces[j][1])
		{
		case(WHITE_KING):	
							materialVal = ((turnCount <= 50) ? kingPlaceTable[7 - y][x] : kingPlaceTableEnd[7 - y][x]);
							boardVal = boardVal + materialVal;
							break;
		case(WHITE_QUEEN):	materialVal = 900;
							boardVal = boardVal + materialVal;							
							break;
		case(WHITE_ROOK):	materialVal = 500;//  +rookPlaceTable[7 - y][x];
							boardVal = boardVal + materialVal;
							break;
		case(WHITE_BISHOP):	materialVal = 350 + bishopPlaceTable[7 - y][x];
							boardVal = boardVal + materialVal;
							bishopCount++;
							break;
		case(WHITE_KNIGHT):	materialVal = 325 + knightPlaceTable[7 - y][x];
							boardVal = boardVal + materialVal;
							break;
		case(WHITE_PAWN):	materialVal = 100 + ((turnCount <= 30) ? pawnPlaceTable[7 - y][x] : pawnPlaceTableMid[7 - y][x]);
			// Check spaces to the left for pawns
							ldBonus = ((x > 0) & (board->boardSpaces[y - 1][x - 1].isOccupied) & (board->boardSpaces[y - 1][x - 1].pieceType == WHITE_PAWN)) ? 10 : 0;							
							luBonus = ((x > 0) & (board->boardSpaces[y + 1][x - 1].isOccupied) & (board->boardSpaces[y + 1][x - 1].pieceType == WHITE_PAWN)) ? 10 : 0;
							// Determine pawn strcuture bonus
							pawnStructVal = ldBonus + luBonus;
						//	printf("Pawn bonus %d\n", pawnStructVal);
							boardVal = boardVal + materialVal + pawnStructVal;
							break;
		default:	 break;
		}

		//printf("%d\n", boardVal);
		//if (end == 1)
		//	break;
	}
	//end = 0; //reset flag
	if (bishopCount >= 2)
		boardVal = boardVal + 50;
	bishopCount = 0;
			 // Black piece consideration
	for (j = 0; j < 16; j++)
	{
		currSpace = move->blackSpaces[j][0];
		y = currSpace / 8; // Get the piece's file index
		x = currSpace % 8; // Get the piece's rank index

		//printf("Black Space Y == %d\n", y);
		//printf("Black Space X == %d\n", x);

		// Condition on piece's type, and add its material value and placeTable value to boardVal
		switch (move->blackSpaces[j][1])
		{
		case(BLACK_PAWN):
			materialVal = 100 + ((turnCount <= 30) ? pawnPlaceTable[y][7 - x]: pawnPlaceTableMid[y][7 - y]);
			// Check spaces to the left for pawns
			ldBonus = ((x > 0) & (board->boardSpaces[y - 1][x - 1].isOccupied) & (board->boardSpaces[y - 1][x - 1].pieceType == BLACK_PAWN)) ? 10 : 0;			
			luBonus = ((x > 0) & (board->boardSpaces[y + 1][x - 1].isOccupied) & (board->boardSpaces[y + 1][x - 1].pieceType == BLACK_PAWN)) ? 10 : 0;
			// Determine pawn strcuture bonus
			pawnStructVal = ldBonus + luBonus;
			//printf("Pawn bonus %d\n", pawnStructVal);
			boardVal = boardVal - materialVal - pawnStructVal;
			break;
		case(BLACK_KNIGHT):	materialVal = 325 + knightPlaceTable[y][7 - x];
							boardVal = boardVal - materialVal;
							break;
		case(BLACK_BISHOP):	materialVal = 350 + bishopPlaceTable[y][7 - x];
							boardVal = boardVal - materialVal;
							bishopCount++;
							break;
		case(BLACK_ROOK):	materialVal = 500;// + rookPlaceTable[y][7 - x];
							boardVal = boardVal - materialVal;
							break;
		case(BLACK_QUEEN):	materialVal = 900;
							boardVal = boardVal - materialVal;							
							break;
		case(BLACK_KING):	materialVal = ((turnCount <= 50) ? kingPlaceTable[y][7 - x] : kingPlaceTableEnd[y][7 - x]);
							boardVal = boardVal - materialVal;
							break;
		default:	break;// end = 1; break;
		}		
	}
	//printf("%d\n", boardVal);
	if (bishopCount >= 2)
		boardVal = boardVal - 50;
	//All done! Return the board value
	return(boardVal);
}