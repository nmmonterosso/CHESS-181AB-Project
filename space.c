#include "stdafx.h"
//#include "board.h"
#include "space.h"





char getboardposition(space * board) //range: 0x00 - 0x3F (0 to 63)
{
	return board->boardposition;
}//getboardposition()

char getisOccupied(space * board) // returns 0x01 if occupied, 0x00 if occupied
{
	return board->isOccupied; 
}//getisOccupied

char getpieceType(space * board)
{
	return board->pieceType;
}//getpieceType


