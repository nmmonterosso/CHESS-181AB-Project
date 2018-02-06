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

	if ((startingSpace->pieceType & 0x7) == PAWN)
	{
		//Moves[0] = Move->rookMoves[i][j+1][1];
	}

	else if ((startingSpace->pieceType & 0x7) == ROOK)
	{
		for (k = 0; move->rookMoves[i][j][k] != 14; k++)
		{
			Addr_Conversion(move->rookMoves[i][j][k], Board_Coordinates);
			iPos = Board_Coordinates[0]; //i coordinates in reference to board
			jPos = Board_Coordinates[1]; //j coordinates in reference to board

			if (board->boardSpaces[iPos][jPos].isOccupied == 0) //check if possible move is Occupied
			{
				Moves[k] = move->rookMoves[i][j][k];
			}
			else
			{
				if ((board->boardSpaces[iPos][jPos].pieceType & WHITE) != (startingSpace->pieceType & WHITE)) //if not allies
				{
					Moves[k] = move->rookMoves[i][j][k]; //store move
				}
				else
				{

				}
			}

		}


	}

	else if ((startingSpace->pieceType & 0x7) == QUEEN)
	{

	}

	else if ((startingSpace->pieceType & 0x7) == KNIGHT)
	{

	}

	else if ((startingSpace->pieceType & 0x7) == KING)
	{

	}

}
















////bitwise AND for getting rid of MSB and focus on 3 LSB
//if ((startingSpace->pieceType & 0x7) == KING) //{000} == KING
//{
//	//move right
//	if (board->boardSpaces[i][j + 1]->isOccupied == 0 && j != 7) //checking if potential move is occupied
//	{
//		Moves[0] = startingSpace->boardposition | 0x1; //if not occupied, store move in Moves
//	}
//	else { Moves[0] = startingSpace->boardposition; }

//	//move left
//	if (board->boardSpaces[i][j - 1]->isOccupied == 0 && j != 0)
//	{
//		Moves[1] = startingSpace->boardposition ^ 0x1;
//	}
//	else { Moves[1] = startingSpace->boardposition; }

//	//move up
//	if (board->boardSpaces[i + 1][j]->isOccupied == 0 && i != 7)
//	{
//		Moves[2] = startingSpace->boardposition | 0x8;
//	}
//	else { Moves[2] = startingSpace->boardposition; }

//	//move down
//	if (board->boardSpaces[i - 1][j]->isOccupied == 0 && i != 0)
//	{
//		Moves[3] = startingSpace->boardposition ^ 0x8;
//	}
//	else { Moves[3] = startingSpace->boardposition; }

//	//upper right diagonal
//	if (board->boardSpaces[i + 1][j + 1]->isOccupied == 0 && (j != 7 && i != 7))
//	{
//		Moves[4] = (startingSpace->boardposition | 0x1) | 0x8;
//	}
//	else { Moves[4] = startingSpace->boardposition; }

//	//upper left diagonal
//	if (board->boardSpaces[i + 1][j - 1]->isOccupied == 0 && (j != 0 && i != 7))
//	{
//		Moves[5] = (startingSpace->boardposition | 0x1) ^ 0x8;
//	}
//	else { Moves[5] = startingSpace->boardposition; }

//	//lower left diagonal
//	if (board->boardSpaces[i - 1][j - 1]->isOccupied == 0 && (j != 0 && i != 0))
//	{
//		Moves[6] = (startingSpace->boardposition ^ 0x1) ^ 0x8;
//	}
//	else { Moves[6] = startingSpace->boardposition; }

//	//lower right diagonal
//	if (board->boardSpaces[i - 1][j + 1]->isOccupied == 0 && (j != 7 && i != 0))
//	{
//		Moves[7] = (startingSpace->boardposition ^ 0x1) | 0x8;
//	}
//	else { Moves[7] = startingSpace->boardposition; }
//} //if KING

//else if ((startingSpace->pieceType & 0x7) == QUEEN) //{001} = QUEEN
//{
//	//Move Straight up
//	while (board->boardSpaces[i + 1][j]->isOccupied == 0 && i != 7)
//	{
//		Moves[k] = startingSpace->boardposition | 0x8;
//		i++;
//		k++;
//	}
//	
//	//Move Straight Down
//} //else if QUEEN