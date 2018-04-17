#include "stdafx.h"
#include "space.h"
#include "board.h"
#include "move.h"
#include "movegen.h"
#include "evaluation.h"
#include <limits.h>

void Addr_Conversion(char boardposition, int Board_Coordinates[2])
{
	Board_Coordinates[0] = (boardposition) / 8;
	Board_Coordinates[1] = boardposition % 8;
} //Addr_Conversion

  //Summary: Appends the movelist with the current legal move:
void AddToMoveList(MoveGen *movegen, char Start_Location, char End_Location, char Piece, char Captured_Piece) {
	movegen->Moves[movegen->count].startLocation = Start_Location;
	movegen->Moves[movegen->count].endLocation = End_Location;
	movegen->Moves[movegen->count].piece = Piece;
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


//SUMMARY returns 0 if king square is under attack: (unsafe), returns 1 if safe:
int checkKingSafety(Board * board, int i, int j)
{
	if ((checkKingHorizontal(board, i, j) == 0) || (checkKingVertical == 0))
		return 0;

	if (checkKingDiagonal(board, i, j) == 0)
		return 0;

	if (checkKingPawn(board, i, j) == 0)
		return 0;

	if (checkKingKnight(board, i, j) == 0)
		return 0;

	return 1;
}//checkKingSafety

 //SUMMARY: returns 0 if unsafe from a diagonal:
 //Returns 1 if safe from diagonals:
int checkKingDiagonal(Board * board, int row, int col)
{
	int i, j;
		
	if (board->turn == BLACK_TURN) {
		//check top right
		i = row + 1;
		j = col + 1;
		while ((i <= 7) && (j <= 7)) {
			if (board->boardSpaces[i][j].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[i][j].pieceType == BLACK_QUEEN) ||
					(board->boardSpaces[i][j].pieceType == BLACK_BISHOP))
					return 0;
				else
					break;
			}//end if
			i++;
			j++;
		}//end check top right

		 //check bottom right
		i = row - 1;
		j = col + 1;
		while ((i >= 0) && (j <= 7)) {
			if (board->boardSpaces[i][j].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[i][j].pieceType == BLACK_QUEEN) ||
					(board->boardSpaces[i][j].pieceType == BLACK_BISHOP))
					return 0;
				else
					break;
			}//end if
			i--;
			j++;
		}//end check bottom right

		 //check bottom left
		i = row - 1;
		j = col - 1;
		while ((i >= 0) && (j >= 0)) {
			if (board->boardSpaces[i][j].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[i][j].pieceType == BLACK_QUEEN) ||
					(board->boardSpaces[i][j].pieceType == BLACK_BISHOP))
					return 0;
				else
					break;
			}//end if
			i--;
			j--;
		}//end check bottom left
		 //check top left
		i = row + 1;
		j = col - 1;
		while ((i <= 7) && (j >= 0)) {
			if (board->boardSpaces[i][j].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[i][j].pieceType == BLACK_QUEEN) ||
					(board->boardSpaces[i][j].pieceType == BLACK_BISHOP))
					return 0;
				else
					break;
			}//end if
			i++;
			j--;
		}//end check top left
	}//end if BLACK_TURN

	else if (board->turn == WHITE_TURN) {
		//check top right
		i = row + 1;
		j = col + 1;
		while ((i <= 7) && (j <= 7)) {
			if (board->boardSpaces[i][j].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[i][j].pieceType == WHITE_QUEEN) ||
					(board->boardSpaces[i][j].pieceType == WHITE_BISHOP))
					return 0;
				else
					break;
			}//end if
			i++;
			j++;
		}//end check top right

		 //check bottom right
		i = row - 1;
		j = col + 1;
		while ((i >= 0) && (j <= 7)) {
			if (board->boardSpaces[i][j].isOccupied == IS_OCCUPIED) {
				if (board->boardSpaces[i][j].pieceType == WHITE_QUEEN ||
					board->boardSpaces[i][j].pieceType == WHITE_BISHOP)
					return 0;
				else
					break;
			}//end if
			i--;
			j++;
		}//end check bottom right

		 //check bottom left
		i = row - 1;
		j = col - 1;
		while ((i >= 0) && (j >= 0)) {
			if (board->boardSpaces[i][j].isOccupied == IS_OCCUPIED) {
				if (board->boardSpaces[i][j].pieceType == WHITE_QUEEN ||
					board->boardSpaces[i][j].pieceType == WHITE_BISHOP)
					return 0;
				else
					break;
			}//end if
			i--;
			j--;
		}//end check bottom left
		 //check top left
		i = row + 1;
		j = col - 1;
		while ((i <= 7) && (j >= 0)) {
			if (board->boardSpaces[i][j].isOccupied == IS_OCCUPIED) {
				if (board->boardSpaces[i][j].pieceType == WHITE_QUEEN ||
					board->boardSpaces[i][j].pieceType == WHITE_BISHOP)
					return 0;
				else
					break;
			}//end if
			i++;
			j--;
		}//end check top left
	}//end if WHITE_TURN

	return 1;
}//checkKingDiagonal



int checkKingKnight(Board * board, int row, int col)
{
	if (board->turn == BLACK_TURN) {
		for (int count = 0; count < 8; count++) {
			switch (count) {
				//UP RIGHT:
			case 0: if ((row + 2 < 8) && (col + 1 < 8)) {
				if (board->boardSpaces[row + 2][col + 1].pieceType == BLACK_KNIGHT)
					return 0;
			}break;

				//RIGHT-> UP:
			case 1: if ((row + 1 < 8) && (col + 2 < 8)) {
				if (board->boardSpaces[row + 1][col + 2].pieceType == BLACK_KNIGHT)
					return 0;
			}break;

				//RIGHT-> DOWN:
			case 2: if ((row - 1 >= 0) && (col + 2 < 8)) {
				if (board->boardSpaces[row - 1][col + 2].pieceType == BLACK_KNIGHT)
					return 0;
			}break;
				//DOWN -> RIGHT:
			case 3: if ((row - 2 >= 0) && (col + 1 < 8)) {
				if (board->boardSpaces[row - 2][col + 1].pieceType == BLACK_KNIGHT)
					return 0;
			}break;
				//DOWN -> LEFT:
			case 4: if ((row - 2 >= 0) && (col - 1 >= 0)) {
				if (board->boardSpaces[row - 2][col - 1].pieceType == BLACK_KNIGHT)
					return 0;
			}break;
				//LEFT -> DOWN:
			case 5: if ((row - 1 >= 0) && (col - 2 >= 0)) {
				if (board->boardSpaces[row - 1][col - 2].pieceType == BLACK_KNIGHT)
					return 0;
			}break;
				//LEFT -> UP:
			case 6: if ((row + 1 < 8) && (col - 2 >= 0)) {
				if (board->boardSpaces[row + 1][col - 2].pieceType == BLACK_KNIGHT)
					return 0;
			}break;
				//UP -> LEFT
			case 7: if ((row + 2 < 8) && (col - 1 >= 0)) {
				if (board->boardSpaces[row + 2][col - 1].pieceType == BLACK_KNIGHT)
					return 0;
			}break;
			default: break;
			}//end switch
		}//end for loop count:
	}//end if BLACK_TURN:

	else if (board->turn == WHITE_TURN) {
		for (int count = 0; count < 8; count++) {
			switch (count) {
				//UP RIGHT:
			case 0: if ((row + 2 < 8) && (col + 1 < 8)) {
				if (board->boardSpaces[row + 2][col + 1].pieceType == WHITE_KNIGHT)
					return 0;
			}break;

				//RIGHT-> UP:
			case 1: if ((row + 1 < 8) && (col + 2 < 8)) {
				if (board->boardSpaces[row + 1][col + 2].pieceType == WHITE_KNIGHT)
					return 0;
			}break;

				//RIGHT-> DOWN:
			case 2: if ((row - 1 >= 0) && (col + 2 < 8)) {
				if (board->boardSpaces[row - 1][col + 2].pieceType == WHITE_KNIGHT)
					return 0;
			}break;
				//DOWN -> RIGHT:
			case 3: if ((row - 2 >= 0) && (col + 1 < 8)) {
				if (board->boardSpaces[row - 2][col + 1].pieceType == WHITE_KNIGHT)
					return 0;
			}break;

				//DOWN -> LEFT:
			case 4: if ((row - 2 >= 0) && (col - 1 >= 0)) {
				if (board->boardSpaces[row - 2][col - 1].pieceType == WHITE_KNIGHT)
					return 0;
			}break;
				//LEFT -> DOWN:
			case 5: if ((row - 1 >= 0) && (col - 2 >= 0)) {
				if (board->boardSpaces[row - 1][col - 2].pieceType == WHITE_KNIGHT)
					return 0;
			}break;
				//LEFT -> UP:
			case 6: if ((row + 1 < 8) && (col - 2 >= 0)) {
				if (board->boardSpaces[row + 1][col - 2].pieceType == WHITE_KNIGHT)
					return 0;
			}break;
				//UP -> LEFT
			case 7: if ((row + 2 < 8) && (col - 1 >= 0)) {
				if (board->boardSpaces[row + 2][col - 1].pieceType == WHITE_KNIGHT)
					return 0;
			}break;
			default: break;
			}//end switch
		}//end ifFOR LOOP

		return 1;
	}
}

//Summary: Returns 0 if enemy queen/rook occupies in the same
//column and has a clear path to capture KING.
//Returns 1 if King is safe in this direction:
int checkKingHorizontal(Board * board, int row, int col)
{
	if (board->turn == BLACK_TURN) {
		for (int j = board->whiteKingCoordinates[1] + 1; j < 8; j++) {
			if (board->boardSpaces[row][j].isOccupied) {
				if ((board->boardSpaces[row][j].pieceType == BLACK_ROOK) ||
					(board->boardSpaces[row][j].pieceType == BLACK_QUEEN))
					return 0;
				else
					break;
			}//end if occupied		
		}//end for right check

		for (int j = board->whiteKingCoordinates[1] - 1; j >= 0; j--) {
			if (board->boardSpaces[row][j].isOccupied) {
				if ((board->boardSpaces[row][j].pieceType == BLACK_ROOK) ||
					(board->boardSpaces[row][j].pieceType == BLACK_QUEEN))
					return 0;
				else
					break;
			}//end if occupied	
		}//end for leftcheck
	}//end if BLACK_TURN

	else if (board->turn == WHITE_TURN) {
		for (int j = board->blackKingCoordinates[1] + 1; j < 8; j++) {
			if (board->boardSpaces[row][j].isOccupied) {
				if ((board->boardSpaces[row][j].pieceType == WHITE_ROOK) ||
					(board->boardSpaces[row][j].pieceType == WHITE_QUEEN))
					return 0;
				else
					break;
			}//end if occupied		
		}//end for right check

		for (int j = board->blackKingCoordinates[1] - 1; j >= 0; j--) {
			if (board->boardSpaces[row][j].isOccupied) {
				if ((board->boardSpaces[row][j].pieceType == WHITE_ROOK) ||
					(board->boardSpaces[row][j].pieceType == WHITE_QUEEN))
					return 0;
				else
					break;
			}//end if occupied	
		}//end for leftcheck
	}//end if WHITE_TURN
	return 1;
}//checkKingHorizontal

 //Summary: Returns 0 if enemy queen/rook occupies in the same
 //column and has a clear path to capture KING.
 //Returns 1 if King is safe in this direction:
int checkKingVertical(Board * board, int row, int col)
{
	if (board->turn == BLACK_TURN) {
		for (int i = board->whiteKingCoordinates[0] + 1; i < 8; i++) {
			if (board->boardSpaces[i][col].isOccupied) {
				if ((board->boardSpaces[i][col].pieceType == BLACK_ROOK) ||
					(board->boardSpaces[i][col].pieceType == BLACK_QUEEN))
					return 0;
				else
					break;
			}//end if occupied		
		}//end for right check

		for (int i = board->whiteKingCoordinates[1] - 1; i >= 0; i--) {
			if (board->boardSpaces[i][col].isOccupied) {
				if ((board->boardSpaces[i][col].pieceType == BLACK_ROOK) ||
					(board->boardSpaces[i][col].pieceType == BLACK_QUEEN))
					return 0;
				else
					break;
			}//end if occupied	
		}//end for leftcheck
	}//end if BLACK_TURN

	else if (board->turn == WHITE_TURN) {
		for (int i = board->blackKingCoordinates[0] + 1; i < 8; i++) {
			if (board->boardSpaces[i][col].isOccupied) {
				if ((board->boardSpaces[i][col].pieceType == WHITE_ROOK) ||
					(board->boardSpaces[i][col].pieceType == WHITE_QUEEN))
					return 0;
				else
					break;
			}//end if occupied		
		}//end for right check

		for (int i = board->blackKingCoordinates[0] - 1; i >= 0; i--) {
			if (board->boardSpaces[i][col].isOccupied) {
				if ((board->boardSpaces[i][col].pieceType == WHITE_ROOK) ||
					(board->boardSpaces[i][col].pieceType == WHITE_QUEEN))
					return 0;
				else
					break;
			}//end if occupied	
		}//end for leftcheck
	}//end if WHITE_TURN
	return 1;
}//checkKingVertical



int checkKingPawn(Board * board, int row, int col)
{

	if (board->turn == BLACK_TURN) {
		//CHECK enemy Pawn Attacks:
		if ((board->boardSpaces[row + 1][col + 1].pieceType == BLACK_PAWN) ||
			(board->boardSpaces[row + 1][col - 1].pieceType == BLACK_PAWN))
			return 0;
	}// end if BLACK_TURN
	else if (board->turn == WHITE_TURN) {
		//CHECK enemy Pawn Attacks:
		if ((board->boardSpaces[row - 1][col + 1].pieceType == WHITE_PAWN) ||
			(board->boardSpaces[row - 1][col - 1].pieceType == WHITE_PAWN))
			return 0;
	}//end if WHITE_TURN

	return 1;
}//checkKingPawn

//Summary: Checks Legality of castle move depending on which move was used:
//RETURNS 1 on LEGAL MOVE, RETURNS 0 ON ILLEGAL MOVE
int checkCastle(Board *board, char castle) {

	if ((castle == WHITE_CASTLE_KINGSIDE) || (castle == WHITE_CASTLE_QUEENSIDE)) {
		if (checkKingSafety(board, 0, 4) == 0)
			return 0;

		if (castle == WHITE_CASTLE_KINGSIDE) {
			if (checkKingSafety(board, 0, 5) == 0)
				return 0;
			if (checkKingSafety(board, 0, 6) == 0)
				return 0;
		}//if white castle Kingside

		else if (castle == WHITE_CASTLE_QUEENSIDE){
			if (checkKingSafety(board, 0, 3) == 0)
				return 0;
			if (checkKingSafety(board, 0, 2) == 0)
				return 0;
		}//check Queenside
	}//if white castle

	else if ((castle == BLACK_CASTLE_KINGSIDE) || (castle == BLACK_CASTLE_QUEENSIDE)) {
		if (checkKingSafety(board, 7, 4) == 0)
			return 0;

		if (castle == BLACK_CASTLE_KINGSIDE) {
			if (checkKingSafety(board, 7, 5) == 0)
				return 0;
			if (checkKingSafety(board, 7, 6) == 0)
				return 0;
		}//if black castle kingside

		else if (castle == BLACK_CASTLE_QUEENSIDE) {
			if (checkKingSafety(board, 7, 3) == 0)
				return 0;
			if (checkKingSafety(board, 7, 2) == 0)
				return 0;
		}//if black castle queenside
	}//if black castle
	//SUCCESS!
	return 1; 
}//checkCastle


Prunes makeMoveTree(Board * board, Move * move, MoveTree *movetree, MoveGen * movegen, MoveGen * movehistory, int depth, short int alphaVal, short int betaVal, MoveGen *prevPath)
{	
	Prunes prunes; //keeps track of alpha beta values and the final move path
	//MoveGen * prevPath; //keeps track the previously examined movepath

	// Defining Base Case
	if (depth == MAXDEPTH) {
		//BOARD EVALUATE RETURN BOARD EVALUATION:
		prunes.boardVal = eval(board, 10, move); //TODO: keep track of turn count
		prunes.movePath = movehistory;
		board->PerftNodeCounter = board->PerftNodeCounter + 1; //Increment # of legal moves counter for debugging purposes.
		return prunes;
	}//end if 

	// after iteration 1 --> depth = depth + 1
	else {
		for (int i = 0; i < movegen->count; i++) {
			//Make Move, Evaluate possible moves, repeat until at max depth.
			makeMove(board, movegen->Moves[i], movehistory, move);
			//printBoard(board);

			//TODO CHECK LEGALITY OF CASTLING:
			if ((movegen->Moves[i].capturedPiece >= 82) && (movegen->Moves[i].capturedPiece <= 85)) {
				if (checkCastle(board, movegen->Moves[i].capturedPiece) == 0) {
					unMakeMove(board, movehistory, move);
					board->PerftCastleCounter--;
					continue;
				}//end if checkCastle
			}//end 

			if (checkKingSafety(board, ((board->turn == WHITE_TURN) ? board->blackKingCoordinates[0] : board->whiteKingCoordinates[0]),
				((board->turn == WHITE_TURN) ? board->blackKingCoordinates[1] : board->whiteKingCoordinates[1]))) {

				movetree->MoveTreeNode[depth + 1].count = 0;
				MoveGenFunction(board, move, &movetree->MoveTreeNode[depth + 1]);						//Call new movement generation for new boardstate:
				
				//Alpha-beta pruning algorithm
				prunes = makeMoveTree(board, move, movetree, &movetree->MoveTreeNode[depth + 1], movehistory, depth + 1, alphaVal, betaVal, prevPath); //Go one more depth lower:
				unMakeMove(board, movehistory, move); // We examined one depth lower, unmake the move we did
				if (board->turn == BLACK_TURN) { //Minimizing player's turn to evaluate
					if (prunes.boardVal <= alphaVal) { // If we fail the hard-alpha cutoff, we prune
						prunes.boardVal = betaVal; //Pruning: take better alternative beta
						prunes.movePath = prevPath; //Pruning: take move path associated with better alternative
						return prunes;
					}// end if hard-alpha cutoff
					if (prunes.boardVal < betaVal) { //Found a better alternative, update beta
						betaVal = prunes.boardVal; //Remember better alternate value
						prevPath = prunes.movePath; //Remember the associated move path
					}// end if beta update
					prunes.boardVal = betaVal;
					prunes.movePath = prevPath;

					printf("ALPHA_VAL = %d\n", alphaVal);
					printf("BETA_VAL = %d\n", betaVal);
				}//end if minimizer pruning
				if (board->turn == WHITE_TURN) { //Maximizing player's turn to evaluate
					if (prunes.boardVal >= betaVal) { // If we fail the hard-alpha cutoff, we prune
						prunes.boardVal = alphaVal; //Pruning: take better alternative beta
						prunes.movePath = prevPath; //Pruning: take move path associated with better alternative
						unMakeMove(board, movehistory, move); //Pruning: unmake the move and return
						return prunes;
					}// end if hard-alpha cutoff
					if (prunes.boardVal > alphaVal) { //Found a better alternative, update beta
						alphaVal = prunes.boardVal; //Remember better alternate value
						prevPath = prunes.movePath; //Remember the associated move path
					}// end if beta update
					prunes.boardVal = alphaVal;
					prunes.movePath = prevPath;

					printf("ALPHA_VAL = %d\n", alphaVal);
					printf("BETA_VAL = %d\n", betaVal);
				} // end if maximizer pruning
				// Pruning is done, return statement outside of the for loop
							
				
			//	printBoard(board);
			}//end if 
			else {
			//	printf("BAD:\n");
			//	printBoard(board);
				if (movehistory->Moves[movehistory->count - 1].capturedPiece != NO_CAPTURE)
					board->PerftCaptureCounter--;
				if (movehistory->Moves[movehistory->count - 1].capturedPiece == EN_PASSANT) {
					board->PerftEPCapture--;
					board->PerftCaptureCounter--;
				}
				if ((movehistory->Moves[movehistory->count - 1].capturedPiece >= 32) && (movehistory->Moves[movehistory->count - 1].capturedPiece <= 79)) //if promotion
					board->PerftPromotionCounter--;
				unMakeMove(board, movehistory, move);
			//	printf("ENDBAD\n");
			//	printBoard(board);				
			}
		}//end for 
		// Pruning is done, kick back up one level
		return prunes;
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
			case (WHITE_QUEEN):	MoveGenQueen(board, move, movegen, count);  break;
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
			case (BLACK_QUEEN):	MoveGenQueen(board, move, movegen, count);  break;
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
	//TODO: CHECK IF MOVING FOWARD WILL PUT KING IN CHECK. CHECK WHICH DIRECTION KING IS IN:
	if (board->turn == WHITE_TURN) {
		Start_Location = move->whiteSpaces[count][BOARD_POSITION];
		a = Start_Location / 8;
		b = Start_Location % 8;
		if (a < 6) {
			if (board->boardSpaces[a + 1][b].isOccupied == NOT_OCCUPIED) {
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b].boardposition, WHITE_PAWN, NO_CAPTURE);
				if (board->boardSpaces[a + 2][b].isOccupied == NOT_OCCUPIED && (a == 1)) {
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 2][b].boardposition, WHITE_PAWN, NO_CAPTURE);
				}//move forward 2
			}//end if // move forward 1

			 //Check Right Diagonal:
			if (b < 7) {
				if ((board->boardSpaces[a + 1][b + 1].isOccupied == IS_OCCUPIED) && ((board->boardSpaces[a + 1][b + 1].pieceType & GET_PIECE_TYPE) <= BLACK))
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b + 1].boardposition, WHITE_PAWN, board->boardSpaces[a + 1][b + 1].pieceType);
				if ((board->boardSpaces[a + 1][b + 1].isOccupied == NOT_OCCUPIED) && (board->boardSpaces[a + 1][b + 1].boardposition == board->epSquare))
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b + 1].boardposition, WHITE_PAWN, EN_PASSANT);
			}//endif b < 7

			 //Check Left Diagonal:
			if (b > 0) {
				if ((board->boardSpaces[a + 1][b - 1].isOccupied == IS_OCCUPIED) && ((board->boardSpaces[a + 1][b - 1].pieceType & GET_PIECE_TYPE) <= BLACK))
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b - 1].boardposition, WHITE_PAWN, board->boardSpaces[a + 1][b - 1].pieceType);
				if ((board->boardSpaces[a + 1][b - 1].isOccupied == NOT_OCCUPIED) && (board->boardSpaces[a + 1][b - 1].boardposition == board->epSquare))
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b - 1].boardposition, WHITE_PAWN, EN_PASSANT);				
			}//end if b<0
		}//end if a < 6
		 //TODO PROMOTION
		else if (a == 6) {
			if (board->boardSpaces[7][b].isOccupied == NOT_OCCUPIED) {
				AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b].boardposition, WHITE_PAWN, WHITE_PROMOTE_QUEEN_NO_CAPTURE);
				AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b].boardposition, WHITE_PAWN, WHITE_PROMOTE_BISHOP_NO_CAPTURE);
				AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b].boardposition, WHITE_PAWN, WHITE_PROMOTE_KNIGHT_NO_CAPTURE);
				AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b].boardposition, WHITE_PAWN, WHITE_PROMOTE_ROOK_NO_CAPTURE);
			}//end if space in front unoccuppied:

			 //CAPTURE LEFT/RIGHT AND PROMOTE:
			if (b < 7) {
				if ((board->boardSpaces[7][b + 1].isOccupied == IS_OCCUPIED) && ((board->boardSpaces[7][b + 1].pieceType & GET_PIECE_TYPE) <= BLACK))
					if (board->boardSpaces[7][b + 1].pieceType == BLACK_QUEEN) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_QUEEN_CAPTURE_QUEEN);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_ROOK_CAPTURE_QUEEN);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_BISHOP_CAPTURE_QUEEN);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_KNIGHT_CAPTURE_QUEEN);
					}//if capturing black queen and promotion:
					else if (board->boardSpaces[7][b + 1].pieceType == BLACK_ROOK) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_QUEEN_CAPTURE_ROOK);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_ROOK_CAPTURE_ROOK);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_BISHOP_CAPTURE_ROOK);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_KNIGHT_CAPTURE_ROOK);
					}// else if capturing lack rook

					else if (board->boardSpaces[7][b + 1].pieceType == BLACK_BISHOP) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_QUEEN_CAPTURE_BISHOP);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_ROOK_CAPTURE_BISHOP);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_BISHOP_CAPTURE_BISHOP);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_KNIGHT_CAPTURE_BISHOP);
					}
					else if (board->boardSpaces[7][b + 1].pieceType == BLACK_KNIGHT) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_QUEEN_CAPTURE_KNIGHT);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_ROOK_CAPTURE_KNIGHT);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_BISHOP_CAPTURE_KNIGHT);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b + 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_KNIGHT_CAPTURE_KNIGHT);
					}
			}//end check right side:

			 //check left side:
			if (b > 0) {
				if ((board->boardSpaces[7][b - 1].isOccupied == IS_OCCUPIED) && ((board->boardSpaces[7][b - 1].pieceType & GET_PIECE_TYPE) <= BLACK))
					if (board->boardSpaces[7][b - 1].pieceType == BLACK_QUEEN) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_QUEEN_CAPTURE_QUEEN);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_ROOK_CAPTURE_QUEEN);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_BISHOP_CAPTURE_QUEEN);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_KNIGHT_CAPTURE_QUEEN);
					}//if capturing black queen and promotion:
					else if (board->boardSpaces[7][b - 1].pieceType == BLACK_ROOK) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_QUEEN_CAPTURE_ROOK);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_ROOK_CAPTURE_ROOK);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_BISHOP_CAPTURE_ROOK);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_KNIGHT_CAPTURE_ROOK);
					}// else if capturing lack rook

					else if (board->boardSpaces[7][b - 1].pieceType == BLACK_BISHOP) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_QUEEN_CAPTURE_BISHOP);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_ROOK_CAPTURE_BISHOP);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_BISHOP_CAPTURE_BISHOP);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_KNIGHT_CAPTURE_BISHOP);
					}//end if capturing bishop + promote
					else if (board->boardSpaces[7][b - 1].pieceType == BLACK_KNIGHT) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_QUEEN_CAPTURE_KNIGHT);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_ROOK_CAPTURE_KNIGHT);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_BISHOP_CAPTURE_KNIGHT);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[7][b - 1].boardposition, WHITE_PAWN, WHITE_PROMOTE_KNIGHT_CAPTURE_KNIGHT);
					}//end if capturing knight
			}//end check left side
		}// if next pawn move results in promotion: 

	}//end if 
	else { //BLACK TURN
		Start_Location = move->blackSpaces[count][BOARD_POSITION];
		a = Start_Location / 8;
		b = Start_Location % 8;
		if (a > 1) {
			if (board->boardSpaces[a - 1][b].isOccupied == NOT_OCCUPIED) {
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b].boardposition, BLACK_PAWN, NO_CAPTURE);
				if (board->boardSpaces[a - 2][b].isOccupied == NOT_OCCUPIED && (a == 6)) {
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 2][b].boardposition, BLACK_PAWN, NO_CAPTURE);
				}// move forward 2:
			}//end if // move forward 1

			 //Check Right Diagonal:
			if (b < 7) {
				if ((board->boardSpaces[a - 1][b + 1].isOccupied == IS_OCCUPIED) && ((board->boardSpaces[a - 1][b + 1].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE))
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b + 1].boardposition, BLACK_PAWN, board->boardSpaces[a - 1][b + 1].pieceType);
				if ((board->boardSpaces[a - 1][b + 1].isOccupied == NOT_OCCUPIED) && (board->boardSpaces[a - 1][b + 1].boardposition == board->epSquare))
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b + 1].boardposition, BLACK_PAWN, EN_PASSANT);
			}//endif b < 7

			 //Check Left Diagonal:
			if (b > 0) {
				if ((board->boardSpaces[a - 1][b - 1].isOccupied == IS_OCCUPIED) && ((board->boardSpaces[a + 1][b - 1].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE))
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b - 1].boardposition, BLACK_PAWN, board->boardSpaces[a - 1][b - 1].pieceType);
				if ((board->boardSpaces[a - 1][b - 1].isOccupied == NOT_OCCUPIED) && (board->boardSpaces[a - 1][b - 1].boardposition == board->epSquare))
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b - 1].boardposition, BLACK_PAWN, EN_PASSANT);
				// TO DO: PROMOTE!:
			}//end if b<0
		}// if a > 1 (no promotion possible)

		else if (a == 1) {
			if (board->boardSpaces[0][b].isOccupied == NOT_OCCUPIED) {
				AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b].boardposition, BLACK_PAWN, BLACK_PROMOTE_QUEEN_NO_CAPTURE);
				AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b].boardposition, BLACK_PAWN, BLACK_PROMOTE_BISHOP_NO_CAPTURE);
				AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b].boardposition, BLACK_PAWN, BLACK_PROMOTE_KNIGHT_NO_CAPTURE);
				AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b].boardposition, BLACK_PAWN, BLACK_PROMOTE_ROOK_NO_CAPTURE);
			}//end if space in front unoccuppied:
			 //check right side
			if (b < 7) {
				if ((board->boardSpaces[0][b + 1].isOccupied == IS_OCCUPIED) && (board->boardSpaces[0][b + 1].pieceType & getpieceType >= WHITE_PIECE)) {
					if (board->boardSpaces[0][b + 1].pieceType == WHITE_QUEEN) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_QUEEN_CAPTURE_QUEEN);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_ROOK_CAPTURE_QUEEN);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_BISHOP_CAPTURE_QUEEN);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_KNIGHT_CAPTURE_QUEEN);
					}//if capturing white queen and promotion:
					else if (board->boardSpaces[0][b + 1].pieceType == BLACK_ROOK) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_QUEEN_CAPTURE_ROOK);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_ROOK_CAPTURE_ROOK);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_BISHOP_CAPTURE_ROOK);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_KNIGHT_CAPTURE_ROOK);
					}// else if capturing white rook

					else if (board->boardSpaces[0][b + 1].pieceType == BLACK_BISHOP) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_QUEEN_CAPTURE_BISHOP);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_ROOK_CAPTURE_BISHOP);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_BISHOP_CAPTURE_BISHOP);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_KNIGHT_CAPTURE_BISHOP);
					}// if capturing white bishop
					else if (board->boardSpaces[0][b + 1].pieceType == BLACK_KNIGHT) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_QUEEN_CAPTURE_KNIGHT);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_ROOK_CAPTURE_KNIGHT);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_BISHOP_CAPTURE_KNIGHT);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b + 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_KNIGHT_CAPTURE_KNIGHT);
					}// if capturing white knight
				}//end if capturing piece
			}//end check right side
			 //check left side:
			if (b > 0) {
				if ((board->boardSpaces[0][b - 1].isOccupied == IS_OCCUPIED) && (board->boardSpaces[0][b - 1].pieceType & getpieceType >= WHITE_PIECE)) {
					if (board->boardSpaces[0][b - 1].pieceType == WHITE_QUEEN) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_QUEEN_CAPTURE_QUEEN);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_ROOK_CAPTURE_QUEEN);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_BISHOP_CAPTURE_QUEEN);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_KNIGHT_CAPTURE_QUEEN);
					}//if capturing white queen and promotion:
					else if (board->boardSpaces[0][b - 1].pieceType == BLACK_ROOK) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_QUEEN_CAPTURE_ROOK);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_ROOK_CAPTURE_ROOK);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_BISHOP_CAPTURE_ROOK);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_KNIGHT_CAPTURE_ROOK);
					}// else if capturing white rook

					else if (board->boardSpaces[0][b - 1].pieceType == BLACK_BISHOP) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_QUEEN_CAPTURE_BISHOP);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_ROOK_CAPTURE_BISHOP);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_BISHOP_CAPTURE_BISHOP);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_KNIGHT_CAPTURE_BISHOP);
					}// if capturing white bishop
					else if (board->boardSpaces[0][b - 1].pieceType == BLACK_KNIGHT) {
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_QUEEN_CAPTURE_KNIGHT);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_ROOK_CAPTURE_KNIGHT);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_BISHOP_CAPTURE_KNIGHT);
						AddToMoveList(movegen, Start_Location, board->boardSpaces[0][b - 1].boardposition, BLACK_PAWN, BLACK_PROMOTE_KNIGHT_CAPTURE_KNIGHT);
					}// if capturing white knight
				}//end if capturing piece
			}//end check left side:
		}//end if a == 1
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
						x = x + (7 - ((a > b) ? a : b));  //if a > b, x = x + (7-b), else x = x + (7-a) 	
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
						x = x + (7 - ((a > b) ? a : b));  //if a > b, x = x + (7-b), else x = x + (7-a) 	
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
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, WHITE_QUEEN, board->boardSpaces[a][b].pieceType);
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
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, WHITE_QUEEN, NO_CAPTURE);
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
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, WHITE_QUEEN, board->boardSpaces[a][b].pieceType);
					if ((a > i) && (b > j))				  //TOP RIGHT DIRECTION
						x = x + (7 - ((a > b) ? a : b));  //if a > b, then x = x + (7 - a), else x = x + (7 - b)
					else if ((a < i) && (b > j))		  //BOTTOM RIGHT DIRECTION			
						x = x + (7 - ((a > b) ? a : b));  //if a > b, x = x + (7-b), else x = x + (7-a) 	
					else if ((a < i) && (b < j))		  //BOTTOM LEFT DIRECTION
						x = x + ((a > b) ? b : a);		  //if a > b, x = x + b, else x = x + a: 	
					else if ((a > i) && (b < j))		  //TOP LEFT DIRECTION
						x = 13;							  //terminating condition
				}// end if occupied
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, WHITE_QUEEN, NO_CAPTURE);
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
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, BLACK_QUEEN, board->boardSpaces[a][b].pieceType);
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
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, BLACK_QUEEN, NO_CAPTURE);
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
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, BLACK_QUEEN, board->boardSpaces[a][b].pieceType);
					if ((a > i) && (b > j))				  //TOP RIGHT DIRECTION
						x = x + (7 - ((a > b) ? a : b));  //if a > b, then x = x + (7 - a), else x = x + (7 - b)
					else if ((a < i) && (b > j))		  //BOTTOM RIGHT DIRECTION			
						x = x + (7 - ((a > b) ? a : b));  //if a > b, x = x + (7-b), else x = x + (7-a) 	
					else if ((a < i) && (b < j))		  //BOTTOM LEFT DIRECTION
						x = x + ((a > b) ? b : a);		  //if a > b, x = x + b, else x = x + a: 	
					else if ((a > i) && (b < j))		  //TOP LEFT DIRECTION
						x = 13;							  //terminating condition
				}// end if occupied
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b].boardposition, BLACK_QUEEN, NO_CAPTURE);
			}//end else		
		}//endForBLACK BISHOP
	}//end else
}//MoveGenQueen





void MoveGenKing(Board *board, Move *move, MoveGen *movegen, int count)
{
	int Board_Coordinates[2];
	char Start_Location;
	int i, j, a, b, x;
	//TODO NEED TO FIX SO KING CANNOT MOVE ITSELF INTO CHECK.	

	if (board->turn == WHITE_TURN) {
		Start_Location = move->whiteSpaces[count][BOARD_POSITION];
		a = Start_Location / 8;
		b = Start_Location % 8;
		//CHECKCASTLING WHITE
		checkWhiteCastle(board, movegen);
		//UP MOVEMENT and LEFT RIGHT DIAGONALS:		
		if (a < 7) {
			//MOVE UP:			
			if (board->boardSpaces[a + 1][b].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a + 1][b].pieceType & GET_PIECE_TYPE) <= BLACK)
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b].boardposition, WHITE_KING, board->boardSpaces[a + 1][b].pieceType);
			}//end if 
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b].boardposition, WHITE_KING, NO_CAPTURE);
			//end Move UP:

			//MOVE UP RIGHT DIAGONAL:

			if (b < 7) {				
				if (board->boardSpaces[a + 1][b + 1].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a + 1][b + 1].pieceType & GET_PIECE_TYPE) <= BLACK)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b + 1].boardposition, WHITE_KING, board->boardSpaces[a + 1][b + 1].pieceType);
				}//end if 
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b + 1].boardposition, WHITE_KING, NO_CAPTURE);
				//end UPRIGHT clear:
			}//end if B < 7:

			 //MOVE UP LEFT DIAGONAL
			if (b > 0) {
				if (board->boardSpaces[a + 1][b - 1].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a + 1][b - 1].pieceType & GET_PIECE_TYPE) <= BLACK)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b - 1].boardposition, WHITE_KING, board->boardSpaces[a + 1][b - 1].pieceType);
				}//end if 
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b - 1].boardposition, WHITE_KING, NO_CAPTURE);
				//end check up Left
			}//end b > 0
		}// end if a < 7

		 //CHECK RIGHT MOVEMENT		
		if (b < 7) {			
			if (board->boardSpaces[a][b + 1].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a][b + 1].pieceType & GET_PIECE_TYPE) <= BLACK)
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b + 1].boardposition, WHITE_KING, board->boardSpaces[a][b + 1].pieceType);
			}//end if 
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b + 1].boardposition, WHITE_KING, NO_CAPTURE);
			//end CHECK RIGHT SAFETY
		}// end if b < 7

		 //CHECK LEFT MOVEMENT
		if (b > 0) {
			if (board->boardSpaces[a][b - 1].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a][b - 1].pieceType & GET_PIECE_TYPE) <= BLACK)
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b - 1].boardposition, WHITE_KING, board->boardSpaces[a][b - 1].pieceType);
			}//end if 
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b - 1].boardposition, WHITE_KING, NO_CAPTURE);
			//end CHECK LEFT MOVEMENT
		}// end if b > 0

		 //CHECK DOWN AND DOWN DIAGONALS:
		if (a > 0) {
			//CHECK MOVE DOWN:			
			if (board->boardSpaces[a - 1][b].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a - 1][b].pieceType & GET_PIECE_TYPE) <= BLACK)
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b].boardposition, WHITE_KING, board->boardSpaces[a - 1][b].pieceType);
			}//end if 
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b].boardposition, WHITE_KING, NO_CAPTURE);
			//end CHECK DOWN SAFETY

			//MOVE DOWN RIGHT			
			if (b < 7) {				
				if (board->boardSpaces[a - 1][b + 1].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a - 1][b + 1].pieceType & GET_PIECE_TYPE) <= BLACK)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b + 1].boardposition, WHITE_KING, board->boardSpaces[a - 1][b + 1].pieceType);
				}//end if 
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b + 1].boardposition, WHITE_KING, NO_CAPTURE);
				//end CHECK DOWN RIGHT SAFETY
			}// end if b < 7

			 //MOVE DOWN LEFT
			if (b > 0) {				
				if (board->boardSpaces[a - 1][b - 1].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a - 1][b - 1].pieceType & GET_PIECE_TYPE) <= BLACK)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b - 1].boardposition, WHITE_KING, board->boardSpaces[a - 1][b - 1].pieceType);
				}//end if 
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b - 1].boardposition, WHITE_KING, NO_CAPTURE);
				//end CHECK MOVE DOWN LEFT SAFETY
			}//end if b >0
		}//end if a > 0
	}//end if white turn


	else if(board->turn == BLACK_TURN){
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
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a + 1][b].boardposition, BLACK_KING, NO_CAPTURE);			
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
			//end if check right safety
		}// end if b < 7

		 //CHECK LEFT MOVEMENT
		if (b > 0) {			
			if (board->boardSpaces[a][b - 1].isOccupied == IS_OCCUPIED) {
				if ((board->boardSpaces[a][b - 1].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE)
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b - 1].boardposition, BLACK_KING, board->boardSpaces[a][b - 1].pieceType);
			}//end if 
			else
				AddToMoveList(movegen, Start_Location, board->boardSpaces[a][b - 1].boardposition, BLACK_KING, NO_CAPTURE);
			//end check left safety
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
			//end check DOWN safety
			//MOVE DOWN RIGHT
			if (b < 7) {				
				if (board->boardSpaces[a - 1][b + 1].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a - 1][b + 1].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b + 1].boardposition, BLACK_KING, board->boardSpaces[a - 1][b + 1].pieceType);
				}//end if 
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b + 1].boardposition, BLACK_KING, NO_CAPTURE);
				//end check down right safety
			}// end if b < 7

			 //MOVE DOWN LEFT
			if (b > 0) {				
				if (board->boardSpaces[a - 1][b - 1].isOccupied == IS_OCCUPIED) {
					if ((board->boardSpaces[a - 1][b - 1].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE)
						AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b - 1].boardposition, BLACK_KING, board->boardSpaces[a - 1][b - 1].pieceType);
				}//end if 
				else
					AddToMoveList(movegen, Start_Location, board->boardSpaces[a - 1][b - 1].boardposition, BLACK_KING, NO_CAPTURE);
				//end check down left safety
			}//end if b >0
		}//end if a > 0
	}//end else black turn
}// MoveGenKing

//Check if white castling is valid and adds to movelist if move is legal
//Possibly need to see when Im using the checkKingSafetfy function becasue 
//This might result in a significant slowdown unintended.
void checkWhiteCastle(Board *board, MoveGen *movegen) {
	if ((board->castlingRights & CHECK_WHITE_CASTLE_KINGSIDE) == 8)
		if ((board->boardSpaces[0][5].isOccupied == NOT_OCCUPIED) && (board->boardSpaces[0][6].isOccupied == NOT_OCCUPIED))
			if ((checkKingSafety(board, 0, 4)) && (checkKingSafety(board, 0, 5)) && (checkKingSafety(board, 0, 6)))
				AddToMoveList(movegen, 4, 6, WHITE_KING, WHITE_CASTLE_KINGSIDE);	
	//checkKingside
	if ((board->castlingRights & CHECK_WHITE_CASTLE_QUEENSIDE) == 4) 
		if ((board->boardSpaces[0][3].isOccupied == NOT_OCCUPIED) && (board->boardSpaces[0][2].isOccupied == NOT_OCCUPIED))
			if ((checkKingSafety(board, 0, 4)) && (checkKingSafety(board, 0, 3)) && (checkKingSafety(board, 0, 2)))
				AddToMoveList(movegen, 4, 2, WHITE_KING, WHITE_CASTLE_QUEENSIDE);
	//checkQueenside
}//checkWhiteCastle


void checkBlackCastle(Board *board, MoveGen *movegen) {
	if ((board->castlingRights & CHECK_BLACK_CASTLE_KINGSIDE) == 2)
		if ((board->boardSpaces[7][5].isOccupied == NOT_OCCUPIED) && (board->boardSpaces[7][6].isOccupied == NOT_OCCUPIED))
			//if ((checkKingSafety(board, 0, 4)) && (checkKingSafety(board, 0, 5)) && (checkKingSafety(board, 0, 6))) //CHECK LEGALITY AFTER THE FACT:
				AddToMoveList(movegen, 60, 62, BLACK_KING, BLACK_CASTLE_KINGSIDE);
	//checkKingside
	if ((board->castlingRights & CHECK_BLACK_CASTLE_QUEENSIDE) == 1)
		if ((board->boardSpaces[7][3].isOccupied == NOT_OCCUPIED) && (board->boardSpaces[7][2].isOccupied == NOT_OCCUPIED))
			//if ((checkKingSafety(board, 0, 4)) && (checkKingSafety(board, 0, 3)) && (checkKingSafety(board, 0, 2))) //CHECK LEGALITY AFTER MAKING THE MOVE:
				AddToMoveList(movegen, 60, 58, BLACK_KING, BLACK_CASTLE_QUEENSIDE);
	//checkQueenside
}//checkBlackCastle