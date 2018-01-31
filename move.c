#include "stdafx.h"
#include "board.h"
#include "move.h"


//Summary: Calculates each possible movement a rook can move given a starting space.
//Stores the destination board positions as a result. 
//Assumes the Board is empty so this result must be compared to the current boardstate
//before making decisions.
void setRookMoves(Board *board, int i, int j, Move *move)
{
	int x, y, k;			
	for (k = 0; k < 14; k++) {
		move->rookMoves[i][j][k] = 0; //Initialize array to 0 
	}//End FOR initialization
	k = 0;
	//Calculating horizontal +X direction (RIGHT)
	for(x = j + 1; x < 8; x++) {
		move->rookMoves[i][j][k] = board->boardSpaces[i][x].boardposition; //Sets the boardPosition for the valid move:
		k++;
	}//end FOR +X RIGHT	
	 //Calculating horizontal -X direction LEFT	
	for (x = j - 1; x >= 0; x--) {
		move->rookMoves[i][j][k] = board->boardSpaces[i][x].boardposition; //Sets the boardPosition for the valid move:
		k++;
	}//end FOR -X LEFT
	 //Calculating vertical +y direction: UP	
	for (y = i + 1; y < 8; y++) {
		move->rookMoves[i][j][k] = board->boardSpaces[y][j].boardposition;
		k++;
	}//end FOR +Y

	 //Calculating vertical -y direction: Down
	for (y = i - 1; y >= 0; y--) {
		move->rookMoves[i][j][k] = board->boardSpaces[y][j].boardposition;
		k++;
	}//end FOR -Y
}//verticalHorizontal_move


//Summary: Passes the board and arguments to preconfigure the board movement.
//All movement in this board is assumed to be on an empty board and must be compared
//to the present board state to determine obstacle collisions.
void setMoves(Board *board, Move *move) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			//printf("boardInt = [%d]", boardInt);			
			setRookMoves(board, i, j, move);
			//TODO SET PAWN MOVES 
			//SET BISHOP MOVES 
			//SET QUEEN MOVES
			//SET KNIGHT MOVES
		}//end for j
	}//end for i
}//setMoves