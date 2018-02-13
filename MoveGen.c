#include "stdafx.h"
#include "space.h"
#include "board.h"
#include "move.h"




void Addr_Conversion(char boardposition, int Board_Coordinates[2])
{
	Board_Coordinates[0] = (boardposition) / 8;
	Board_Coordinates[1] = boardposition % 8;
} //Addr_Conversion

 



void MoveGen(space *startingSpace, Board *board, char Moves[27], Move *move) {
	//startingSpace gives all information of current space that is being checked for possible moves
	//Generate all valid move that a given piece can make

	int Board_Coordinates[2]; //stores i and j vals for board
	int i, j;		//storing Board_Coordinates Values
	int iPos, jPos; //store Board_Coordinates of moves (i.e. move->rookMoves[i][j][k])
	int k = 0;		//temporary variable for while loops

	Addr_Conversion(startingSpace->boardposition, Board_Coordinates);
	i = Board_Coordinates[0];
	j = Board_Coordinates[1];
	


	//MOVING PAWN
	if ((startingSpace->pieceType & 0x7) == PAWN)
	{
		//Moves[0] = Move->rookMoves[i][j+1][1];
	}


	//MOVING ROOK
	else if ((startingSpace->pieceType & 0x7) == ROOK)
	{
		for (k = 0; move->rookMoves[i][j][k] != 14; k++)
		{
			Addr_Conversion(move->rookMoves[i][j][k], Board_Coordinates);
			iPos = Board_Coordinates[0]; //i coordinates in reference to board
			jPos = Board_Coordinates[1]; //j coordinates in reference to board

			if (board->boardSpaces[iPos][jPos].isOccupied == 0) //check if possible move is not Occupied
			{
				Moves[k] = move->rookMoves[i][j][k];
			}
			else //if not occupied, check if square is occupied by ally or enemy
			{
				if ((board->boardSpaces[iPos][jPos].pieceType & 8) != (startingSpace->pieceType & 8)) //if not allies
				{
					Moves[k] = move->rookMoves[i][j][k]; //store move
				} //end if
				else //else if allies... got to next part in array
				{
					
				} //end else
			} //end else

		} //for loop

	}



	//MOVING KNIGHT
	else if ((startingSpace->pieceType & 0x7) == KNIGHT)
	{
		for (k = 0; move->knightMoves[i][j][k]; k++) //check through preallocated list for possible moves
		{
			Addr_Conversion(move->knightMoves[i][j][k], Board_Coordinates);
			iPos = Board_Coordinates[0]; //i coordinates in reference to board
			jPos = Board_Coordinates[1]; //j coordinates in reference to board

			if (board->boardSpaces[iPos][jPos].isOccupied == 0) //if not occupied
			{
				Moves[k] = move->knightMoves[i][j][k]; //store move
			} //if
			//if pieceType >= --> 8 WHITE else BLACK
			else if ((board->boardSpaces[iPos][jPos].pieceType & 8) != (startingSpace->pieceType & 8)) //if not allies
			{
				Moves[k] = move->knightMoves[i][j][k]; //capture piece and store as valid move
			}
			//if square is occupied by ally just continue to next value of k
		}

	}



	//MOVING BISHOP
	else if ((startingSpace->pieceType & 0x7) == BISHOP)
	{
		for (k = 0; move->bishopMoves[i][j][k]; k++) //check through preallocated list for bishop moves
		{
			Addr_Conversion(move->knightMoves[i][j][k], Board_Coordinates);
			iPos = Board_Coordinates[0]; //i coordinates in reference to board
			jPos = Board_Coordinates[1]; //j coordinates in reference to board
			
			if (board->boardSpaces[iPos][jPos].isOccupied == 0)
			{ 
				Moves[k] = move->bishopMoves[i][j][k];
			}
			else if ((board->boardSpaces[iPos][jPos].pieceType & 8) != (startingSpace->pieceType & 8)) //if not allies
			{
				Moves[k] = move->bishopMoves[i][j][k]; 
				//jump to next diagonal
				if (8 - i > 8 - j)
			}
			else //if ally jump to next diagonal
			{

			}
		}
	}


	//MOVING QUEEN
	else if ((startingSpace->pieceType & 0x7) == QUEEN)
	{

	}


	else if ((startingSpace->pieceType & 0x7) == KING)
	{

	}

}



