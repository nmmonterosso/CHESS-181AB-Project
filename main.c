// Chess2.cpp : Defines the entry point for the console application.
//Nicholas Monterosso
//Main for Chess project

#include "stdafx.h" //comment out this line or add your own #include stdio, etc.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "piece.h"
#include "space.h"
#include "board.h"
#include "move.h"
#include "movegen.h"
#include "xboard.h"
#include <limits.h>
//void makeBoard(space *board[8][8]);
//MACROS IN SPACE.H

//GLOBALS: HASH TABLE;
unsigned long long randTable[64][13];
volatile unsigned long long *zobrist; 


int main()
{
	//Initialize board		
	Board *board = (Board*)malloc(sizeof(Board));
	Move  *move = (Move*)malloc(sizeof(Move));
	MoveGen *movegen = (MoveGen*)malloc(sizeof(MoveGen));
	MoveGen *movehistory = (MoveGen*)malloc(sizeof(MoveGen));
	MoveTree *movetree = (MoveTree*)malloc(sizeof(MoveTree));
	MoveList blankMove; // Blank movelist that gets passed up and down during pruning
	Prunes prunes;
	ht_hash_table* ht = ht_new(); //create new hash table:
	zobrist = (unsigned long long *)malloc(sizeof(unsigned long long));
	*zobrist = 0;
	blankMove.capturedPiece = -1;
	blankMove.endLocation = -1;
	blankMove.piece = -1;
	blankMove.startLocation = -1;
	//int *MoveCounter = (int *)malloc(sizeof(int));
	//*MoveCounter = 0;

	//xboard variables
	char piece;
	char startLocation[2];
	char endLocation[2];

	//DEBUGGING POSITIONS//
	char position2[] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0"; //KiwiPete
	char position3[] = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - 0 0";
	char position4[] = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
	char position5[] = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
	char position6[] = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
	char position7[] = "2Q5/8/8/K6k/8/8/3p4/2N5 w - - 0 10";
	char position8[] = "k7/8/8/8/8/1n6/P7/7K w - - 0 10";
	char startingPosition[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";
	//END DEBUGGING POSITIONS//
	makeBoard(board, move, movegen, movehistory);	// Initializes board state and pieces. Precompiles all moves:
	MoveGenFunction(board, move, movegen);			//Initial Movegen:
													//movetree->MoveTreeNode[0] = *movegen;			//Root Movegen:
	printBoard(board);  //Prints Board into Console:
	
	printf("Hello World!\n");

	setBoard(board, move, startingPosition);
	printBoard(board); //Prints Second board:

	init_zobrist();
	set_zobrist_value(board, zobrist);

	movegen->count = 0;
	MoveGenFunction(board, move, movegen);
	movetree->MoveTreeNode[0] = *movegen;
	
					   //static long randTable[64][13];l
					   //MoveGen *movegen = realloc(movegen, sizeof(MoveGen));
					   //movegen->count = 0;// pseudo reset
					   //Calculate Black Moves:
					   //MoveGenFunction(board, move, movegen);	
					   //printBoard(board);

	while (1) {
		//xboard(board); //listen to xboard
		prunes = makeMoveTree(board, move, movetree, movegen, movehistory, 0, SHRT_MIN, SHRT_MAX, blankMove); //creates move tree based on all possible moves, calls board evaluation function, and makes move

		//creates move tree based on all possible moves, calls board evaluation function, and makes move
		/*printf("total # of nodes: = [%d]\n", board->PerftNodeCounter);
		printf("total # of captures: = [%d]\n", board->PerftCaptureCounter);
		printf("total # of EP Captures: = [%d]\n", board->PerftEPCapture);
		printf("total # of castling: = [%d]\n", board->PerftCastleCounter);
		printf("total # of pawn Promotions = [%d]\n", board->PerftPromotionCounter);*/
		//printBoard(board);

		//if board's turn: make the move, else wait for xboard
		makeMove(board, prunes.pruneMove, movehistory, move);
		printBoard(board);
		resetDebugCounters(board);	
		resetPrunes(&prunes);
		resetPruneChoice(&blankMove);		
		clearMoveGen(movegen);
		clearMoveGen(movehistory);		
		MoveGenFunction(board, move, movegen);
		movetree->MoveTreeNode[0] = *movegen;
		
		
		
		// xboard stuff

		//sending move to xboard
		//address conversion to coordinate notation for xboard
		//startLocation[1] = prunes.pruneMove.startLocation / 8; //i values
		//startLocation[0] = prunes.pruneMove.startLocation % 8; //a,b,c,d,... coordinates

		//endLocation[1] = prunes.pruneMove.endLocation / 8;
		//endLocation[0] = prunes.pruneMove.endLocation % 8;

		//switch (startLocation[0])
		//{
		//case 0: startLocation[0] = 'a'; break;
		//case 1: startLocation[0] = 'b'; break;
		//case 2: startLocation[0] = 'c'; break;
		//case 3: startLocation[0] = 'd'; break;
		//case 4: startLocation[0] = 'e'; break;
		//case 5: startLocation[0] = 'f'; break;
		//case 6: startLocation[0] = 'g'; break;
		//case 7: startLocation[0] = 'h'; break;
		//default: break;
		//}
		//
		//switch (endLocation[0])
		//{		
		//case 0: endLocation[0] = 'a'; break;
		//case 1: endLocation[0] = 'b'; break;
		//case 2: endLocation[0] = 'c'; break;
		//case 3: endLocation[0] = 'd'; break;
		//case 4: endLocation[0] = 'e'; break;
		//case 5: endLocation[0] = 'f'; break;
		//case 6: endLocation[0] = 'g'; break;
		//case 7: endLocation[0] = 'h'; break;
		//default: break;
		//}

		////send signal to xboard
		//fprintf(stdout, "move %c%c%c%c", startLocation[0], startLocation[1], endLocation[0], endLocation[1]);
		//end xboard stuff 
		//PRINTF # of captures:
		//PrintF # of checks:

		//MAKE BEST MOVE HERE if it is BOT's TURN:
		//else wait for opponent input:
		//Make Tree:
		//MoveGeneration Function:		
	}//end while
	return 0;
}