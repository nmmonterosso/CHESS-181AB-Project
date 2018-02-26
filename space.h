#ifndef SPACE_H_
#define SPACE_H_

#define MAXDEPTH 3

#define WHITE 0xF
#define BLACK 0x7

#define KING	0x0
#define QUEEN	0x1
#define ROOK	0x2
#define BISHOP	0x3
#define KNIGHT	0x4
#define PAWN	0x5
#define EMPTY	0xF

#define WHITE_KING   8
#define WHITE_QUEEN  9
#define WHITE_ROOK   10
#define WHITE_BISHOP 11
#define WHITE_KNIGHT 12
#define WHITE_PAWN   13

#define BLACK_KING   KING
#define BLACK_QUEEN  QUEEN
#define BLACK_ROOK   ROOK
#define BLACK_BISHOP BISHOP
#define BLACK_KNIGHT KNIGHT
#define BLACK_PAWN   PAWN

#define WHITE_TURN 1
#define BLACK_TURN 0

#define PIECE_TYPE 1
#define BOARD_POSITION 0
#define IS_OCCUPIED 1
#define NOT_OCCUPIED 0
#define NO_CAPTURE -1
#define NO_MOVE -1

#define WHITE_PIECE 0x8 
#define BLACK_PIECE 0x7
#define GET_PIECE_TYPE 0xF


typedef struct{
	//type includes: color, and the type of piece: 1 = white 0 = black
	//{000}	== KING
	//{001} == Queen
	//{010} == ROOK
	//{011} == BISHOP
	//{100} == KNIGHT
	//{101} == PAWN
	//{111} == EMPTY (Might be unnecessary)
	char boardposition;		// Space number identifier on the board {6 BITS}
	char isOccupied;		// 1 if occupied, 0 if unoccupied		{1 BIT }
	char pieceType;			// type of piece occupying the space	{4 BITS}
	//TODO CHECK ENUM for string types:	
} space;


char getboardposition(space *board);	//returns boardposition
char getisOccupied(space *board);		//returns 1 if occupied, 0 if not
char getpieceType(space *board);		

#endif //SPACE_H
