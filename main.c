// Chess2.cpp : Defines the entry point for the console application.
//Nicholas Monterosso
//Main for Chess project

#include "stdafx.h" //comment out this line or add your own #include stdio, etc.
#include <stdio.h>
#include <string.h>
#include "piece.h"
#include "space.h"
#include "board.h"
#include "move.h"
#include "movegen.h"
//void makeBoard(space *board[8][8]);
//MACROS IN SPACE.H




int main()
{
	//Initialize board		
	Board *board =		(Board*)malloc(sizeof(Board));
	Move  *move =		(Move*)malloc(sizeof(Move));
	MoveGen *movegen =	(MoveGen*)malloc(sizeof(MoveGen));
	MoveGen *movehistory =	(MoveGen*)malloc(sizeof(MoveGen));
	MoveTree *movetree =	(MoveTree*)malloc(sizeof(MoveTree));
	int *MoveCounter = (int *)malloc(sizeof(int));
	*MoveCounter = 0;
	//Debugging Board Postions:
	char position2[100] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -"; //KiwiPete


	makeBoard(board, move, movegen, movehistory);	// Initializes board state and pieces. Precompiles all moves:
	MoveGenFunction(board, move, movegen);			//Initial Movegen:
	movetree->MoveTreeNode[0] = *movegen;			//Root Movegen:
	printBoard(board);  //Prints Board into Console:
	printf("Hello World!\n");
	setBoard(board, position2);
	printBoard(board); //Prints Second board:
	//REALLOCATE
	//MoveGen *movegen = realloc(movegen, sizeof(MoveGen));
	//movegen->count = 0;// pseudo reset
	//Calculate Black Moves:
	//MoveGenFunction(board, move, movegen);	
	//printBoard(board);
	
	while (1) {	

		makeMoveTree(board, move, movetree, movegen, movehistory, 0, MoveCounter); //creates move tree based on all possible moves, calls board evaluation function, and makes move
		printf("total # of nodes: = [%d]", *MoveCounter);
		//PRINTF # of captures:
		//PrintF # of checks:

		//MAKE bEST MOVE HERE if it is BOT's TURN:
		//else wait for opponent input:
		//Make Tree:
		//MoveGeneration Function:		
	}//end while
	return 0;
}




