// Chess2.cpp : Defines the entry point for the console application.
//Nicholas Monterosso
//Main for Chess project

#include "stdafx.h" //comment out this line or add your own #include stdio, etc.
#include <stdio.h>
#include "piece.h"
#include "space.h"
#include "board.h"

//void makeBoard(space *board[8][8]);
//MACROS //



int main()
{
	//Initialize board
	char *castlingRights = 0xF;  //castling rights: = ({Wkingside, Wqueenside}, {Bkingside, BQueenside}, {4-bit unused so far}) 
	Board *board = malloc(sizeof(Board));
	ma3keBoard(board);	// Initializes board state and pieces:
	printBoard(board);  //Prints Board into Console:
	printf("Hello World!\n");
	while (1) {		
		//MoveGeneration Function:
		//BoardEvaluation Function:
	}
	return 0;
}




