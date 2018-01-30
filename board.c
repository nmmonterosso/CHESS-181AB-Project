#include "stdafx.h"
#include "board.h"
#include "space.h"

//SEE MACROS IN SPACE.H

//Summary: Calculates each possible movement a rook can move given a starting space.
//Stores the 
//Assumes the Board is empty so this result must be compared to 
/*void setRookMoves(Board *board, int i, int j, space *startSpace)
{
	
	//Calculating horizontal +X direction RIGHT
	int x, y;
	for (x = j; x < 8; x++) {


	}//end FOR +X

	 //Calculating horizontal -X direction LEFT

	for (x = j; x > 0; x--) {


	}//end FOR -X


	//Calculating vertical +y direction: UP
	int x, y;
	for (int y = i; y < 8; y++) {


	}//end FOR +Y

	//Calculating vertical -y direction: Down
	for (y = i; i > 0 y--) {


	}//end FOR -Y


}//verticalHorizontal_move*/

void makeBoard(Board *board)
{
	// Initializes boardstate for a new game:
	// i represents vertical spaces  [0-7] -> (1-8)
	// j represents horizontal spaces[0,7] -> (A-H)
	// example: A-5 = [j, i] == [1, 4]	
	// int boardInt;	
	//board = (board) malloc(sizeof(board));

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			//printf("boardInt = [%d]", boardInt);
			//->boardSpaces = malloc(sizeof(space));
			setSpace(board, i, j);
		}//end for j
	}//end for i
}// makeBoard

void setSpace(Board *board, unsigned int i, unsigned int j)
{	
	//Set Index Value for space's position relative to the board:
	board->boardSpaces[i][j].boardposition = i * 8 + j;

	//Check if vertical index is on the first 2 and last 2 rows
	//and set occupy variable to 1 if true and 0 if false
	if ((i == 0) || (i == 1) || (i == 6) || (i == 7)) {
		board->boardSpaces[i][j].isOccupied = 0x01;
	}//end if 
	else {
		board->boardSpaces[i][j].isOccupied = 0x00;
	}//end Else

	 //Set Piece types on occupied board depending on which i and j value the board is on.

	if ((i == 1) || (i == 6)) {
		(i == 1) ? board->boardSpaces[i][j].pieceType = 0x0D : 0x05;			//Set White/Black Pawns: (0x1101/0x101)
	}//if backline	
	else {
		switch (j) {
		case(0):board->boardSpaces[i][j].pieceType = (ROOK	|0x8) & ((i == 0) ? WHITE : BLACK);	break;	//White/Black Rook	 (0b1010 : 0b0010)
		case(1):board->boardSpaces[i][j].pieceType = (KNIGHT|0x8) & ((i == 0) ? WHITE : BLACK);	break;	//White/Black kNight (0b1100 : 0b0100)
		case(2):board->boardSpaces[i][j].pieceType = (BISHOP|0x8) & ((i == 0) ? WHITE : BLACK);	break;	//White/Black Bishop (0b1011 : 0b0011)
		case(3):board->boardSpaces[i][j].pieceType = (QUEEN	|0x8) & ((i == 0) ? WHITE : BLACK);	break;	//White/Black Queen	 (0b1001 : 0b0001)	
		case(4):board->boardSpaces[i][j].pieceType = (KING	|0x8) & ((i == 0) ? WHITE : BLACK);	break;	//White/Black King	 (0b1000 : 0b0000)
		case(5):board->boardSpaces[i][j].pieceType = (BISHOP|0x8) & ((i == 0) ? WHITE : BLACK);	break;	//White/Black Bishop (0b1011 : 0b0011)	
		case(6):board->boardSpaces[i][j].pieceType = (KNIGHT|0x8) & ((i == 0) ? WHITE : BLACK);	break;	//White/Black kNight (0b1100 : 0b0100)	
		case(7):board->boardSpaces[i][j].pieceType = (ROOK	|0x8) & ((i == 0) ? WHITE : BLACK);	break;	//White/Black Rook	 (0b1010 : 0b0010)
		default:board->boardSpaces[i][j].pieceType = EMPTY;  break; //empty piece default
		}//switch
	}//endElse
}//setPiece



void printBoard(Board *board) {
	// Prints Current Board State to Console:
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			if (board->boardSpaces[i][j].isOccupied == 0x01) {
				switch (board->boardSpaces[i][j].pieceType & 0x07) {
				case(KING):printf("|K "); break;
				case(QUEEN):printf("|Q "); break;
				case(ROOK):printf("|R "); break;
				case(BISHOP):printf("|B "); break;
				case(KNIGHT):printf("|N "); break;
				case(PAWN):printf("|P "); break;
				default:printf("|E "); break; //E = Error, you should not reach this case
				}// endSwitch				
			}//if space is occupied
			else {
				printf("|  ");
			}//endElse
		}//endForJ
		printf("|\n");
	}//endFor I
}//printBoard();