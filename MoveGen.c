#include "stdafx.h"
#include "space.h"
#include "board.h"
#include "move.h"
#include "movegen.h"




void Addr_Conversion(char boardposition, int Board_Coordinates[2])
{
	Board_Coordinates[0] = (boardposition) / 8;
	Board_Coordinates[1] = boardposition % 8;
} //Addr_Conversion



void MoveGenFunction(Board *board, Move *move, MoveGen *movegen) {
	//startingSpace gives all information of current space that is being checked for possible moves
	//Generate all valid move that a given piece can make

	int Board_Coordinates[2]; //stores i and j vals for board
	int i, j;		//storing Board_Coordinates Values
	int iPos, jPos; //store Board_Coordinates of moves (i.e. move->rookMoves[i][j][k])
	int k = 0;		//temporary variable for while loops
	
	if (board->turn == WHITE_TURN) {
		for (int count = 0; count < 16; count++) {
			switch ((int)move->whiteSpaces[count][PIECE_TYPE]) {
			case (WHITE_PAWN):	MoveGenPawn(board, move, movegen, count);	break;
			case (WHITE_ROOK):	MoveGenRook(board, move, movegen, count);	break;
			case (WHITE_KNIGHT):MoveGenKnight(board, move, movegen, count);break;
			case (WHITE_BISHOP):MoveGenBishop(board, move, movegen, count);break;
			case (WHITE_QUEEN):	MoveGenRook(board, move, movegen, count); MoveGenBishop(board, move, movegen, count);	break;
			case (WHITE_KING):	MoveGenKing(board, move, movegen, count);	break;
			default: break;
			}//endswitch
		}//end for
	}//end if turn == white turn


	else {//BLACK TURN
		for (int count = 0; count < 16; count++) {
			switch ((int)move->blackSpaces[count][PIECE_TYPE]) {
			case (BLACK_PAWN):	MoveGenPawn(board, move, movegen, count);	break;
			case (BLACK_ROOK):	MoveGenRook(board, move, movegen, count);	break;
			case (BLACK_KNIGHT):MoveGenKnight(board, move, movegen, count); break;
			case (BLACK_BISHOP):MoveGenBishop(board, move, movegen, count); break;
			case (BLACK_QUEEN):	MoveGenRook(board, move, movegen, count); MoveGenBishop(board, move, movegen, count);	break;
			case (BLACK_KING):	MoveGenKing(board, move, movegen, count);	break;
			default: break;
			}
		}//end for
	}//end else (BLACK TURN)
}//MoveGenFunction

void MoveGenPawn(Board *board, Move *move, MoveGen *movegen, int count)
{	
	if (board->turn == WHITE_TURN) {
			
		
	}//end if 
	else { //BLACK TURN

	}// end else

}//MoveGen Pawn

void MoveGenKnight(Board *board, Move *move, MoveGen *movegen, int count)
{//TODO FIX ERROR WITH COUNT, FIND A WAY TO MAKE THE COUNT A GLOBAL VARIABLE. POSSIBLY
//RENAME int COUNT IN THIS FUNCTION CALL
	int Board_Coordinates[2];
	char Start_Location;
	int i, j, a, b;
	
	if (board->turn == WHITE_TURN) {
		Start_Location = move->whiteSpaces[count][BOARD_POSITION];	
		i = Start_Location / 8;
		j = Start_Location % 8;
		
		for (int x = 0; x < 8; x++) {			
			if (move->knightMoves[i][j][x] == -1)
				break;
			else {
				//CHECK TO SEE IF SPACE IS OCCUPIED OR UNOCCUPIED
				Addr_Conversion(move->knightMoves[i][j][x], Board_Coordinates);
				a = Board_Coordinates[0];
				b = Board_Coordinates[1];
				if (board->boardSpaces[a][b].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a][b].pieceType & GET_PIECE_TYPE) <= BLACK_PIECE) {
						movegen->Moves[movegen->count].capturedPiece = board->boardSpaces[a][b].pieceType;
						movegen->Moves[movegen->count].startLocation = Start_Location;
						movegen->Moves[movegen->count].endLocation = board->boardSpaces[a][b].boardposition;
						movegen->Moves[movegen->count].piece = WHITE_KNIGHT;
						movegen->count++;
					}//end if (CAPTURE)
				}//end if
				else {
					movegen->Moves[movegen->count].capturedPiece = NO_CAPTURE;
					movegen->Moves[movegen->count].startLocation = Start_Location;
					movegen->Moves[movegen->count].endLocation = board->boardSpaces[a][b].boardposition;
					movegen->Moves[movegen->count].piece = WHITE_KNIGHT;
					movegen->count++;
				}//end else
			}//end ELSE BREAK
		}//endfor
	}//end if 

	else { //BLACK TURN
		Start_Location = move->blackSpaces[count][BOARD_POSITION];
		i = Start_Location / 8;
		j = Start_Location % 8;

		for (int x = 0; x < 8; x++) {
			if (move->knightMoves[i][j][x] == -1)
				break;
			else {
				//CHECK TO SEE IF SPACE IS OCCUPIED OR UNOCCUPIED
				Addr_Conversion(move->knightMoves[i][j][x], Board_Coordinates);
				a = Board_Coordinates[0];
				b = Board_Coordinates[1];
				if (board->boardSpaces[a][b].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a][b].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE) {
						movegen->Moves[movegen->count].capturedPiece = board->boardSpaces[a][b].pieceType;
						movegen->Moves[movegen->count].startLocation = Start_Location;
						movegen->Moves[movegen->count].endLocation = board->boardSpaces[a][b].boardposition;
						movegen->Moves[movegen->count].piece = BLACK_KNIGHT;
						movegen->count++;
					}//end if (CAPTURE)
				}//end if
				else {
					movegen->Moves[movegen->count].capturedPiece = NO_CAPTURE;
					movegen->Moves[movegen->count].startLocation = Start_Location;
					movegen->Moves[movegen->count].endLocation = board->boardSpaces[a][b].boardposition;
					movegen->Moves[movegen->count].piece = BLACK_KNIGHT;
					movegen->count++;
				}//end else
			}//end ELSE BREAK
		}//endfor
	}//end else
}//MoveGenKnight

void MoveGenRook(Board *board, Move *move, MoveGen *movegen, int count)
{
	if (board->turn == WHITE_TURN) {


	}// end if
	else { //BLACK TURN

	}//end else


}//MoveGenRook

void MoveGenBishop(Board *board, Move *move, MoveGen *movegen, int count)
{
	if (board->turn == WHITE_TURN) {


	}// end if
	else { //BLACK TURN

	}// end else

}//MoveGenBishop

void MoveGenKing(Board *board, Move *move, MoveGen *movegen, int count)
{
	if (board->turn == WHITE_TURN) {


	}//end if
	else { //BLACK TURN

	}//end else

}//MoveGenKing



