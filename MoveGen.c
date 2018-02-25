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

//Summary: Appends the movelist with the current legal move:
void AddToMoveList(MoveGen *movegen, char Start_Location, char End_Location, char Piece, char Captured_Piece){
	movegen->Moves[movegen->count].startLocation = Start_Location;
	movegen->Moves[movegen->count].endLocation   = End_Location;
	movegen->Moves[movegen->count].piece		  = Piece;
	movegen->Moves[movegen->count].capturedPiece = Captured_Piece;
	movegen->count++;
}//addToMoveList

void initializeMoveGen(MoveGen *movegen) {
	for (int i = 0; i <= 100; i++) {
		//Initialize MoveGen List to -1:
		movegen->Moves[i].piece = -1;
		movegen->Moves[i].startLocation = -1;
		movegen->Moves[i].endLocation = -1;
		movegen->Moves[i].capturedPiece = -1;		
	}//endfor	
	movegen->count = 0;
}

void makeMoveTree(Board * board, Move * move, MoveTree *movetree, MoveGen * movegen, MoveGen * movehistory, int depth)
{
	//TODO: Create move tree to desired depth and 	
	if (depth == MAXDEPTH) {
		//BOARD EVALUATE RETURN BOARD EVALUATION:
		return;
	}//end if 
	
	else{
		for (int i = 0; i < movegen->count; i++) {
			//Make Move, Evaluate possible moves, repeat until at max depth.
			makeMove(board, movegen->Moves[i], movehistory, move);				
			printBoard(board);			
			movetree->MoveTreeNode[depth + 1].count = 0;
			MoveGenFunction(board, move, &movetree->MoveTreeNode[depth + 1]);						//Call new movement generation for new boardstate:
			makeMoveTree(board, move, movetree, &movetree->MoveTreeNode[depth + 1], movehistory, depth + 1); //Go one more depth lower:
			printBoard(board);
			unMakeMove(board, movehistory, move);
			printBoard(board);
		}//end for 
	}
}//makeMoveTree





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
			case (WHITE_KNIGHT):MoveGenKnight(board, move, movegen, count); break;
			case (WHITE_BISHOP):MoveGenBishop(board, move, movegen, count); break;
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
	char Start_Location;
	int i, j, a, b;

	if (board->turn == WHITE_TURN) {
		Start_Location = move->whiteSpaces[count][BOARD_POSITION];
		a = Start_Location / 8;
		b = Start_Location % 8;

		if (board->boardSpaces[a + 1][b].isOccupied == NOT_OCCUPIED) {
			AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b].boardposition, WHITE_PAWN, NO_CAPTURE);			
			if (board->boardSpaces[a + 2][b].isOccupied == NOT_OCCUPIED && (a == 1)) 
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 2][b].boardposition, WHITE_PAWN, NO_CAPTURE);							
		}//end if // move forward 1

		//Check Right Diagonal:
		if (b < 7) {
			if ((board->boardSpaces[a + 1][b + 1].isOccupied == IS_OCCUPIED) && ((board->boardSpaces[a + 1][b + 1].pieceType & GET_PIECE_TYPE) <= BLACK)) 
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b + 1].boardposition, WHITE_PAWN, board->boardSpaces[a + 1][b + 1].pieceType);
			//TODO: EN PASSANT HERE:
		}//endif b < 7

		//Check Left Diagonal:
		if (b > 0) {
			if ((board->boardSpaces[a + 1][b - 1].isOccupied == IS_OCCUPIED) && ((board->boardSpaces[a + 1][b - 1].pieceType & GET_PIECE_TYPE) <= BLACK)) 
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b - 1].boardposition, WHITE_PAWN, board->boardSpaces[a + 1][b - 1].pieceType);				
			 
			// TO DO: EN PASSANT HERE
			// TO DO: PROMOTE PAWN:!
		}//end if b<0
		
	}//end if 
	else { //BLACK TURN
		Start_Location = move->blackSpaces[count][BOARD_POSITION];
		a = Start_Location / 8;
		b = Start_Location % 8;

		if (board->boardSpaces[a - 1][b].isOccupied == NOT_OCCUPIED) {
			AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b].boardposition, BLACK_PAWN, NO_CAPTURE);		
			if (board->boardSpaces[a - 2][b].isOccupied == NOT_OCCUPIED && (a == 6)) 
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 2][b].boardposition, BLACK_PAWN, NO_CAPTURE);			
		}//end if // move forward 1

		 //Check Right Diagonal:
		if (b < 7) {
			if ((board->boardSpaces[a - 1][b + 1].isOccupied == IS_OCCUPIED) && ((board->boardSpaces[a - 1][b + 1].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE))
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b + 1].boardposition, BLACK_PAWN, board->boardSpaces[a - 1][b + 1].pieceType);							 
			 //TODO: EN PASSANT HERE:
		}//endif b < 7

		 //Check Left Diagonal:
		if (b > 0) {
			if ((board->boardSpaces[a + 1][b - 1].isOccupied == IS_OCCUPIED) && ((board->boardSpaces[a + 1][b - 1].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE))
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b - 1].boardposition, BLACK_PAWN, board->boardSpaces[a - 1][b - 1].pieceType);				
			 // TO DO: EN PASSANT HERE
			 // TO DO: PROMOTE!:
		}//end if b<0
		//BLACK TURN

	}// end else

}//MoveGen Pawn

void MoveGenKnight(Board *board, Move *move, MoveGen *movegen, int count)
{
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
					if ((board->boardSpaces[a][b].pieceType & GET_PIECE_TYPE) <= BLACK_PIECE) 
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, WHITE_KNIGHT, board->boardSpaces[a][b].pieceType);
						//CAPTURE!
				}//end if
				else 
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, WHITE_KNIGHT, NO_CAPTURE);
					//NO CAPTURE, MOVE!				
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
					if ((board->boardSpaces[a][b].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE) 
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, BLACK_KNIGHT, board->boardSpaces[a][b].pieceType);
						//CAPTURE!											
				}//end if
				else 
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, BLACK_KNIGHT, NO_CAPTURE);					
			}//end ELSE BREAK
		}//endfor
	}//end else
}//MoveGenKnight

//TODO TEST FOR DEBUGGING PURPOSES:
void MoveGenRook(Board *board, Move *move, MoveGen *movegen, int count)
{
	int Board_Coordinates[2];
	char Start_Location;
	int i, j, a, b;
	if (board->turn == WHITE_TURN) {
		Start_Location = move->whiteSpaces[count][BOARD_POSITION];
		i = Start_Location / 8;
		j = Start_Location % 8;
		for (int x = 0; x < 14; x++) {
			Addr_Conversion(move->rookMoves[i][j][x], Board_Coordinates);
			a = Board_Coordinates[0];
			b = Board_Coordinates[1];
			if (board->boardSpaces[a][b].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a][b].pieceType & GET_PIECE_TYPE <= BLACK))
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, WHITE_ROOK, board->boardSpaces[a][b].pieceType);
				if (a > i)			 //When moving UP
					x = x + (7 - a); //Skip to next index of precompiled move
				else if (a < i)		 //When moving DOWN	
					x = 14;			 //Skip to Vertical precompiled moveset	
				else if (b > j)		 //When moving RIGHT
					x = x + (7 - b); //Skip to next index of vertical precomipled moveset
				else if (b < j)		 //When moving LEFT
					x = 6;			 //Terminating condition	
			}//end if occupied
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, WHITE_ROOK, NO_CAPTURE);
		}//endfor
	}// end if

	else { //BLACK TURN
		Start_Location = move->blackSpaces[count][BOARD_POSITION];
		i = Start_Location / 8;
		j = Start_Location % 8;
		for (int x = 0; x < 14; x++) {
			Addr_Conversion(move->rookMoves[i][j][x], Board_Coordinates);
			a = Board_Coordinates[0];
			b = Board_Coordinates[1];
			if (board->boardSpaces[a][b].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a][b].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE) //ERROR HERE?
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, BLACK_ROOK, board->boardSpaces[a][b].pieceType);
				if (a > i)			 //When moving right
					x = x + (7 - a); //Skip to next index of precompiled move
				else if (a < i)		 //When moving right	
					x = 14;			 //Skip to Vertical precompiled moveset	
				else if (b > j)		 //When moving UP
					x = x + (7 - b); //Skip to next index of vertical precomipled moveset
				else if (b < j)		 //When moving DOWN
					x = 6;			 //Terminating condition
			}//end if occupied
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, BLACK_ROOK, NO_CAPTURE);
		}//end for
	}//end else
}//MoveGenRook



void MoveGenBishop(Board *board, Move *move, MoveGen *movegen, int count)
{
	int Board_Coordinates[2];
	char Start_Location;
	int i, j, a, b;

	if (board->turn == WHITE_TURN) {
		Start_Location = move->whiteSpaces[count][BOARD_POSITION];
		i = Start_Location / 8;
		j = Start_Location % 8;
		for (int x = 0; x < 13; x++) {
			if (move->bishopMoves[i][j][x] == NO_MOVE)
				break;
			else {
				Addr_Conversion(move->bishopMoves[i][j][x], Board_Coordinates);
				a = Board_Coordinates[0];
				b = Board_Coordinates[1];
				if (board->boardSpaces[a][b].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a][b].pieceType & GET_PIECE_TYPE) <= BLACK)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, WHITE_BISHOP, board->boardSpaces[a][b].pieceType);
					if ((a > i) && (b > j))				  //TOP RIGHT DIRECTION
						x = x + (7 - ((a > b) ? a : b));  //if a > b, then x = x + (7 - a), else x = x + (7 - b)
					else if ((a < i) && (b > j))		  //BOTTOM RIGHT DIRECTION			
						x = x + (7 - ((a > b) ? b : a));  //if a > b, x = x + (7-b), else x = x + (7-a) 	
					else if ((a < i) && (b < j))		  //BOTTOM LEFT DIRECTION
						x = x + ((a > b) ? b : a);		  //if a > b, x = x + b, else x = x + a: 	
					else if ((a > i) && (b < j))		  //TOP LEFT DIRECTION
						x = 13;							  //terminating condition
				}// end if occupied
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, WHITE_BISHOP, NO_CAPTURE);
			}//end else		
		}//endFor

	}// end if
	else { //BLACK TURN
		Start_Location = move->blackSpaces[count][BOARD_POSITION];
		i = Start_Location / 8;
		j = Start_Location % 8;
		for (int x = 0; x < 13; x++) {
			if (move->bishopMoves[i][j][x] == NO_MOVE)
				break;
			else {
				Addr_Conversion(move->bishopMoves[i][j][x], Board_Coordinates);
				a = Board_Coordinates[0];
				b = Board_Coordinates[1];
				if (board->boardSpaces[a][b].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a][b].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE)//ERROR HERE?
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, BLACK_BISHOP, board->boardSpaces[a][b].pieceType);
					if ((a > i) && (b > j))				  //TOP RIGHT DIRECTION
						x = x + (7 - ((a > b) ? a : b));  //if a > b, then x = x + (7 - a), else x = x + (7 - b)
					else if ((a < i) && (b > j))		  //BOTTOM RIGHT DIRECTION			
						x = x + (7 - ((a > b) ? b : a));  //if a > b, x = x + (7-b), else x = x + (7-a) 	
					else if ((a < i) && (b < j))		  //BOTTOM LEFT DIRECTION
						x = x + ((a > b) ? b : a);		  //if a > b, x = x + b, else x = x + a: 	
					else if ((a > i) && (b < j))		  //TOP LEFT DIRECTION
						x = 13;							  //terminating condition
				}// end if occupied
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, BLACK_BISHOP, NO_CAPTURE);
			}//end else		
		}//endFor
	}// end else
}//MoveGenBishop







void MoveGenQueen(Board * board, Move * move, MoveGen * movegen, int count)
{
	int Board_Coordinates[2];
	char Start_Location;
	int i, j, a, b;
	if (board->turn == WHITE_TURN) {
		Start_Location = move->whiteSpaces[count][BOARD_POSITION];
		i = Start_Location / 8;
		j = Start_Location % 8;
		//ROOK PORTION HERE
		for (int x = 0; x < 14; x++) {
			Addr_Conversion(move->rookMoves[i][j][x], Board_Coordinates);
			a = Board_Coordinates[0];
			b = Board_Coordinates[1];
			if (board->boardSpaces[a][b].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a][b].pieceType & GET_PIECE_TYPE) <= BLACK)
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, WHITE_ROOK, board->boardSpaces[a][b].pieceType);
				if (a > i)			 //When moving UP
					x = x + (7 - a); //Skip to next index of precompiled move
				else if (a < i)		 //When moving DOWN	
					x = 14;			 //Skip to Vertical precompiled moveset	
				else if (b > j)		 //When moving RIGHT
					x = x + (7 - b); //Skip to next index of vertical precomipled moveset
				else if (b < j)		 //When moving LEFT
					x = 6;			 //Terminating condition	
			}//end if occupied
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, WHITE_ROOK, NO_CAPTURE);
		}//endfor WHITE ROOK PORTION

		//WHITE BISHOP PORTION HERE:
		for (int x = 0; x < 13; x++) {
			if (move->bishopMoves[i][j][x] == NO_MOVE)
				break;
			else {
				Addr_Conversion(move->bishopMoves[i][j][x], Board_Coordinates);
				a = Board_Coordinates[0];
				b = Board_Coordinates[1];
				if (board->boardSpaces[a][b].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a][b].pieceType & GET_PIECE_TYPE) <= BLACK)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, WHITE_BISHOP, board->boardSpaces[a][b].pieceType);
					if ((a > i) && (b > j))				  //TOP RIGHT DIRECTION
						x = x + (7 - ((a > b) ? a : b));  //if a > b, then x = x + (7 - a), else x = x + (7 - b)
					else if ((a < i) && (b > j))		  //BOTTOM RIGHT DIRECTION			
						x = x + (7 - ((a > b) ? b : a));  //if a > b, x = x + (7-b), else x = x + (7-a) 	
					else if ((a < i) && (b < j))		  //BOTTOM LEFT DIRECTION
						x = x + ((a > b) ? b : a);		  //if a > b, x = x + b, else x = x + a: 	
					else if ((a > i) && (b < j))		  //TOP LEFT DIRECTION
						x = 13;							  //terminating condition
				}// end if occupied
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, WHITE_BISHOP, NO_CAPTURE);
			}//end else		
		}//endFor WHITE BISHOP
	}// end if

	else { //BLACK TURN
		Start_Location = move->blackSpaces[count][BOARD_POSITION];
		i = Start_Location / 8;
		j = Start_Location % 8;
		//BLACK_ROOK PORTION HERE
		for (int x = 0; x < 14; x++) {
			Addr_Conversion(move->rookMoves[i][j][x], Board_Coordinates);
			a = Board_Coordinates[0];
			b = Board_Coordinates[1];
			if (board->boardSpaces[a][b].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a][b].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE)
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, BLACK_ROOK, board->boardSpaces[a][b].pieceType);
				if (a > i)			 //When moving right
					x = x + (7 - a); //Skip to next index of precompiled move
				else if (a < i)		 //When moving right	
					x = 14;			 //Skip to Vertical precompiled moveset	
				else if (b > j)		 //When moving UP
					x = x + (7 - b); //Skip to next index of vertical precomipled moveset
				else if (b < j)		 //When moving DOWN
					x = 6;			 //Terminating condition
			}//end if occupied
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, BLACK_ROOK, NO_CAPTURE);
		}//end forBLACK ROOK

		//BLACK_BISHOP PORTION HERE
		for (int x = 0; x < 13; x++) {
			if (move->bishopMoves[i][j][x] == NO_MOVE)
				break;
			else {
				Addr_Conversion(move->bishopMoves[i][j][x], Board_Coordinates);
				a = Board_Coordinates[0];
				b = Board_Coordinates[1];
				if (board->boardSpaces[a][b].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a][b].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, BLACK_BISHOP, board->boardSpaces[a][b].pieceType);
					if ((a > i) && (b > j))				  //TOP RIGHT DIRECTION
						x = x + (7 - ((a > b) ? a : b));  //if a > b, then x = x + (7 - a), else x = x + (7 - b)
					else if ((a < i) && (b > j))		  //BOTTOM RIGHT DIRECTION			
						x = x + (7 - ((a > b) ? b : a));  //if a > b, x = x + (7-b), else x = x + (7-a) 	
					else if ((a < i) && (b < j))		  //BOTTOM LEFT DIRECTION
						x = x + ((a > b) ? b : a);		  //if a > b, x = x + b, else x = x + a: 	
					else if ((a > i) && (b < j))		  //TOP LEFT DIRECTION
						x = 13;							  //terminating condition
				}// end if occupied
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, BLACK_BISHOP, NO_CAPTURE);
			}//end else		
		}//endForBLACK BISHOP
	}//end else
}//MoveGenQueen





void MoveGenKing(Board *board, Move *move, MoveGen *movegen, int count)
{
	int Board_Coordinates[2];
	char Start_Location;
	int i, j, a, b, x;
		

	if (board->turn == WHITE_TURN) {
		Start_Location = move->whiteSpaces[count][BOARD_POSITION];
		a = Start_Location / 8;
		b = Start_Location % 8;
		//CHECKCASTLING WHITE

		//UP MOVEMENT and LEFT RIGHT DIAGONALS:		
		if (a < 7) {
			//MOVE UP:
			if (board->boardSpaces[a + 1][b].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a + 1][b].pieceType & GET_PIECE_TYPE) <= BLACK)
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b].boardposition, WHITE_KING, board->boardSpaces[a + 1][b].pieceType);
			}//end if 
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b + 1].boardposition, WHITE_KING, NO_CAPTURE);
			if (b < 7) {
				//MOVE UP RIGHT DIAGONAL
				if (board->boardSpaces[a + 1][b + 1].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a + 1][b + 1].pieceType & GET_PIECE_TYPE) <= BLACK)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b + 1].boardposition, WHITE_KING, board->boardSpaces[a + 1][b + 1].pieceType);
				}//end if 
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b + 1].boardposition, WHITE_KING, NO_CAPTURE);

			}//end if b < 7
			if (b > 0) {
				//MOVE UP LEFT DIAGONAL
				if (board->boardSpaces[a + 1][b - 1].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a + 1][b - 1].pieceType & GET_PIECE_TYPE) <= BLACK)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b - 1].boardposition, WHITE_KING, board->boardSpaces[a + 1][b - 1].pieceType);
				}//end if 
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b - 1].boardposition, WHITE_KING, NO_CAPTURE);
			}//end if B > 0
		}// end if a < 7
		
		//CHECK RIGHT MOVEMENT
		if (b < 7) {
			if (board->boardSpaces[a][b + 1].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a][b + 1].pieceType & GET_PIECE_TYPE) <= BLACK)
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b + 1].boardposition, WHITE_KING, board->boardSpaces[a][b + 1].pieceType);
			}//end if 
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b + 1].boardposition, WHITE_KING, NO_CAPTURE);

		}// end if b < 7
		

		//CHECK LEFT MOVEMENT
		if (b > 0) {
			if (board->boardSpaces[a][b - 1].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a][b - 1].pieceType & GET_PIECE_TYPE) <= BLACK)
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b - 1].boardposition, WHITE_KING, board->boardSpaces[a][b - 1].pieceType);
			}//end if 
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b - 1].boardposition, WHITE_KING, NO_CAPTURE);

		}// end if b > 0

		//CHECK DOWN AND DOWN DIAGONALS:
		if (a > 0) {
			//MOVE DOWN:
			if (board->boardSpaces[a - 1][b].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a - 1][b].pieceType & GET_PIECE_TYPE) <= BLACK)
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b].boardposition, WHITE_KING, board->boardSpaces[a - 1][b].pieceType);
			}//end if 
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b].boardposition, WHITE_KING, NO_CAPTURE);

			//MOVE DOWN RIGHT
			if (b < 7) {
				if (board->boardSpaces[a - 1][b + 1].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a - 1][b + 1].pieceType & GET_PIECE_TYPE) <= BLACK)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b + 1].boardposition, WHITE_KING, board->boardSpaces[a - 1][b + 1].pieceType);
				}//end if 
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b + 1].boardposition, WHITE_KING, NO_CAPTURE);				
			}// end if b < 7

			//MOVE DOWN LEFT
			if (b > 0) {
				if (board->boardSpaces[a - 1][b - 1].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a - 1][b - 1].pieceType & GET_PIECE_TYPE) <= BLACK)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b - 1].boardposition, WHITE_KING, board->boardSpaces[a - 1][b - 1].pieceType);
				}//end if 
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b - 1].boardposition, WHITE_KING, NO_CAPTURE);
			}//end if b >0
		}//end if a > 0
	}//end if white turn


	else {
		Start_Location = move->blackSpaces[count][BOARD_POSITION];
		a = Start_Location / 8;
		b = Start_Location % 8;
		//CHECK CASTLING BLACK:
		//UP MOVEMENT and LEFT RIGHT DIAGONALS:		
		if (a < 7) {
			//MOVE UP:
			if (board->boardSpaces[a + 1][b].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a + 1][b].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE)
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b].boardposition, BLACK_KING, board->boardSpaces[a + 1][b].pieceType);
			}//end if 
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b + 1].boardposition, BLACK_KING, NO_CAPTURE);
			if (b < 7) {
				//MOVE UP RIGHT DIAGONAL
				if (board->boardSpaces[a + 1][b + 1].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a + 1][b + 1].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b + 1].boardposition, BLACK_KING, board->boardSpaces[a + 1][b + 1].pieceType);
				}//end if 
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b + 1].boardposition, BLACK_KING, NO_CAPTURE);

			}//end if b < 7
			if (b > 0) {
				//MOVE UP LEFT DIAGONAL
				if (board->boardSpaces[a + 1][b - 1].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a + 1][b - 1].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b - 1].boardposition, BLACK_KING, board->boardSpaces[a + 1][b - 1].pieceType);
				}//end if 
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b - 1].boardposition, BLACK_KING, NO_CAPTURE);
			}//end if B > 0
		}// end if a < 7

		 //CHECK RIGHT MOVEMENT
		if (b < 7) {
			if (board->boardSpaces[a][b + 1].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a][b + 1].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE)
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b + 1].boardposition, BLACK_KING, board->boardSpaces[a][b + 1].pieceType);
			}//end if 
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b + 1].boardposition, BLACK_KING, NO_CAPTURE);
		}// end if b < 7
		
		 //CHECK LEFT MOVEMENT
		if (b > 0) {
			if (board->boardSpaces[a][b - 1].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a][b - 1].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE)
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b - 1].boardposition, BLACK_KING, board->boardSpaces[a][b - 1].pieceType);
			}//end if 
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b - 1].boardposition, BLACK_KING, NO_CAPTURE);

		}// end if b > 0

		 //CHECK DOWN AND DOWN DIAGONALS:
		if (a > 0) {
			//MOVE DOWN:
			if (board->boardSpaces[a - 1][b].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a - 1][b].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE)
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b].boardposition, BLACK_KING, board->boardSpaces[a - 1][b].pieceType);
			}//end if 
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b].boardposition, BLACK_KING, NO_CAPTURE);

			//MOVE DOWN RIGHT
			if (b < 7) {
				if (board->boardSpaces[a - 1][b + 1].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a - 1][b + 1].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b + 1].boardposition, BLACK_KING, board->boardSpaces[a - 1][b + 1].pieceType);
				}//end if 
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b + 1].boardposition, BLACK_KING, NO_CAPTURE);
			}// end if b < 7

			 //MOVE DOWN LEFT
			if (b > 0) {
				if (board->boardSpaces[a - 1][b - 1].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a - 1][b - 1].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b - 1].boardposition, BLACK_KING, board->boardSpaces[a - 1][b - 1].pieceType);
				}//end if 
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b - 1].boardposition, BLACK_KING, NO_CAPTURE);
			}//end if b >0
		}//end if a > 0
	}//end else black turn
}// MoveGenKing




