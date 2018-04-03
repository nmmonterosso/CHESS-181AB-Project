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

//GLOBAL VARIABLES HERE:

//END GLOBAL VARIABLES

int main()
{
	//Initialize board		
	Board *board =		(Board*)malloc(sizeof(Board));
	Move  *move =		(Move*)malloc(sizeof(Move));
	MoveGen *movegen =	(MoveGen*)malloc(sizeof(MoveGen));
	MoveGen *movehistory =	(MoveGen*)malloc(sizeof(MoveGen));
	MoveTree *movetree =	(MoveTree*)malloc(sizeof(MoveTree));
	//int *MoveCounter = (int *)malloc(sizeof(int));
	//*MoveCounter = 0;
	
	//DEBUGGING POSITIONS//
	char position2[] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0"; //KiwiPete
	char position3[] = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - 0 0";
	char position4[] = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
	char position5[] = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
	char position6[] = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
	//END DEBUGGING POSITIONS//
	makeBoard(board, move, movegen, movehistory);	// Initializes board state and pieces. Precompiles all moves:
	//MoveGenFunction(board, move, movegen);			//Initial Movegen:
	//movetree->MoveTreeNode[0] = *movegen;			//Root Movegen:
	printBoard(board);  //Prints Board into Console:
	printf("Hello World!\n");
		
	setBoard(board, move, position3);
	movegen->count = 0;
	MoveGenFunction(board, move, movegen);			
	movetree->MoveTreeNode[0] = *movegen;
	printBoard(board); //Prints Second board:
	//REALLOCATE
	//MoveGen *movegen = realloc(movegen, sizeof(MoveGen));
	//movegen->count = 0;// pseudo reset
	//Calculate Black Moves:
	//MoveGenFunction(board, move, movegen);	
	//printBoard(board);
	
	while (1) {	

		makeMoveTree(board, move, movetree, movegen, movehistory, 0); //creates move tree based on all possible moves, calls board evaluation function, and makes move
		printf("total # of nodes: = [%d]\n", board->PerftNodeCounter);
		printf("total # of captures: = [%d]\n", board->PerftCaptureCounter);
<<<<<<< HEAD
		printf("total # of EP captures: = [%d]\n", board->PerftEPCapture);
		printf("total # of castles: = [%d]\n", board->PerftCastleCounter);	
		printBoard(board);


=======
		printf("total # of EP Captures: = [%d]\n", board->PerftEPCapture);
		printf("total # of castling: = [%d]\n", board->PerftCastleCounter);
		printf("total # of pawn Promotions = [%d]\n", board->PerftPromotionCounter);
		printBoard(board);
		resetDebugCounters(board);
		//PRINTF # of captures:
>>>>>>> d70d34c540db5e646d2f50beb52da074d759452f
		//PrintF # of checks:

		//MAKE BEST MOVE HERE if it is BOT's TURN:
		//else wait for opponent input:
		//Make Tree:
		//MoveGeneration Function:		
	}//end while
	return 0;
}




