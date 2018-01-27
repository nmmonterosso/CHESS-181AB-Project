#ifndef PIECE_H_
#define PIECE_H_

//type includes: color, and the type of piece: 
//{000}	== KING
//{001} == Queen
//{010} == ROOK
//{011} == BISHOP
//{100} == KNIGHT
//{101} == PAWN
//{111} == EMPTY (Might be unnecessary)

typedef struct{

	char type;				//color = type[3] LEAST 4 SIGNIFICANT BITS{sign extend}{XXXXVVVV};
	char isCaptured;		//1 if captured, else 0;							   {XXXXXXXV}

} piece;

void makePiece(); //makes new Piece

#endif //PIECE_H
