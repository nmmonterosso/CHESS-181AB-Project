﻿// Chess2.cpp : Defines the entry point for the console application.
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
#include "eval.h"
#include <limits.h>
//void makeBoard(space *board[8][8]);
//MACROS IN SPACE.H

//GLOBALS: HASH TABLE;
unsigned long long randTable[64][13];
unsigned long long randTurn;
volatile unsigned long long *zobrist;
volatile ht_hash_table *ht;

int main()
{
	//Initialize board		
	Board *board = (Board*)malloc(sizeof(Board));
	Move  *move = (Move*)malloc(sizeof(Move));
	MoveGen *movegen = (MoveGen*)malloc(sizeof(MoveGen));
	MoveGen *movehistory = (MoveGen*)malloc(sizeof(MoveGen));
	MoveTree *movetree = (MoveTree*)malloc(sizeof(MoveTree));
	MoveList blankMove; // Blank movelist that gets passed up and down during pruning
	MoveList tempMove;
	Prunes prunes;
	FILE *log = fopen("C:\\Users\\Nico\\source\\repos\\Chess2\\logs\\log.txt", "w");
	fprintf(log, "START\n");
	fclose(log);

	ht = (ht_hash_table*)malloc(sizeof(ht_hash_table));
	ht = ht_new();
	
	zobrist = (unsigned long long *)malloc(sizeof(unsigned long long));
	*zobrist = 0;
	blankMove.capturedPiece = -1;
	blankMove.endLocation = -1;
	blankMove.piece = -1;
	blankMove.startLocation = -1;

	//xboard variables
	char start; // convert board position to 'a,b,c,...'
	char end;	// convert board position to 'a,b,c,...'
	int startLocation[2];	// get board position of piece being moved
	int endLocation[2];		// get board position of where xboard wants to move piece

	//************************************************************
	//******************* DEBUGGING POSITIONS ********************


	char position2[] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0"; //KiwiPete
	char position3[] = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - 0 0";
	char position4[] = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
	char position5[] = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
	char position6[] = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
	char position7[] = "2Q5/8/8/K6k/8/8/3p4/2N5 w - - 0 10";
	char position8[] = "k7/8/8/8/8/1n6/P7/7K w - - 0 10";
	char debugPosition1[] = "rnb1k1nr/pp1p1ppp/2pppq2/8/1b1PP2P/2N5/PPP2PP1/R1BQKBNR/ b KQkq - 0 0"; //MOVE C1 to g5
	
	//******************** END DEBUGGING POSITIONS ***************
	//************************************************************
	char startingPosition[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";

	makeBoard(board, move, movegen, movehistory);	// Initializes board state and pieces. Precompiles all moves:
	MoveGenFunction(board, move, movegen);			//Initial Movegen:		
	// Set bpard to desired board state:
	setBoard(board, move, startingPosition);
	init_zobrist();
	set_zobrist_value(board, zobrist);
	int evaluation = eval(board, board->turnCount, move);
	printf("starting Eval = [%d]\n", evaluation);
	movegen->count = 0;
	printBoard(board);
	/*******LOG*********/
	printBoardToFile(board);	
	/*******LOG*********/
	MoveGenFunction(board, move, movegen);
	movetree->MoveTreeNode[0] = *movegen;
	
	
	while (1) {
		// if xboard is done continue
		if (xboard_flag == 1) {
			// if xboard sets engine as white, engine returns move
			if (xSide == WHITE_TURN) { // <-- this if statement will only be executed once
				// possibly just return Queen's Pawn or reference Opening book
				while (xGo == 0) {
					xboard(board, move, &tempMove);
				}
				printBoard(board);
				clearMoveGen(movegen);
				clearMoveGen(movehistory);
				MoveGenFunction(board, move, movegen);
				prunes = makeMoveTree(board, move, movetree, movegen, movehistory, 0, SHRT_MIN, SHRT_MAX);				
				printBoardToFile(board);
				printPrunesToFile(prunes);
				makeMove(board, prunes.move, movehistory, move);
				printBoardToFile(board);
				board->turnCount++;

				Addr_Conversion(prunes.move.startLocation, startLocation);
				Addr_Conversion(prunes.move.endLocation, endLocation);
				if (startLocation[0] >= 0 && startLocation[1] >= 0 && endLocation >= 0 && endLocation[1] >= 0) {
					switch (startLocation[1]) {
					case 0: start = 'a'; break;
					case 1: start = 'b'; break;
					case 2: start = 'c'; break;
					case 3: start = 'd'; break;
					case 4: start = 'e'; break;
					case 5: start = 'f'; break;
					case 6: start = 'g'; break;
					case 7: start = 'h'; break;
					default: break;
					}
					switch (endLocation[1]) {
					case 0: end = 'a'; break;
					case 1: end = 'b'; break;
					case 2: end = 'c'; break;
					case 3: end = 'd'; break;
					case 4: end = 'e'; break;
					case 5: end = 'f'; break;
					case 6: end = 'g'; break;
					case 7: end = 'h'; break;
					default: break;
					}

					fprintf(stdout, "move %c%d%c%d\n", start, startLocation[0] + 1, end, endLocation[0] + 1);
					log = fopen("C:\\Users\\Nico\\source\\repos\\Chess2\\logs\\log.txt", "a+");
					fprintf(log, "move %c%d%c%d\n", start, startLocation[0] + 1, end, endLocation[0] + 1);
					fclose(log);
					fflush(stdout);

					printBoard(board);
					printBoardToFile(board);
					evaluation = eval(board, board->turnCount, move);
					resetDebugCounters(board);
					clearMoveGen(movegen);
					clearMoveGen(movehistory);
					
					MoveGenFunction(board, move, movegen);
					movetree->MoveTreeNode[0] = *movegen;
				}
				xboard_flag = 0;	// reset flag
				xSide = BLACK_TURN; // reset flag
				xGo = 0;	// reset flag
			}
			/*
			else if (xSide == BLACK_TURN) {
				// if xboard sets engine as white, engine returns move
				while (xGo == 0) {
					xboard(board, move, tempMove);
				}
				
				printBoard(board);
				clearMoveGen(movegen);
				clearMoveGen(movehistory);
				MoveGenFunction(board, move, movegen);
				prunes = makeMoveTree(board, move, movetree, movegen, movehistory, 0, SHRT_MIN, SHRT_MAX);
				makeMove(board, prunes.move, movehistory, move);
				board->turnCount++;

				Addr_Conversion(prunes.move.startLocation, startLocation);
				Addr_Conversion(prunes.move.endLocation, endLocation);
				if (startLocation[0] >= 0 && startLocation[1] >= 0 && endLocation >= 0 && endLocation[1] >= 0) {
					switch (startLocation[1]) {
					case 0: start = 'a'; break;
					case 1: start = 'b'; break;
					case 2: start = 'c'; break;
					case 3: start = 'd'; break;
					case 4: start = 'e'; break;
					case 5: start = 'f'; break;
					case 6: start = 'g'; break;
					case 7: start = 'h'; break;
					default: break;
					}
					switch (endLocation[1]) {
					case 0: end = 'a'; break;
					case 1: end = 'b'; break;
					case 2: end = 'c'; break;
					case 3: end = 'd'; break;
					case 4: end = 'e'; break;
					case 5: end = 'f'; break;
					case 6: end = 'g'; break;
					case 7: end = 'h'; break;
					default: break;
					}

					fprintf(stdout, "move %c%d%c%d\n", start, startLocation[0] + 1, end, endLocation[0] + 1);
					fflush(stdout);

					printBoard(board);
					evaluation = eval(board, board->turnCount, move);
					resetDebugCounters(board);
					clearMoveGen(movegen);
					clearMoveGen(movehistory);
					MoveGenFunction(board, move, movegen);
					movetree->MoveTreeNode[0] = *movegen;
				}
				xboard_flag = 0;	// reset flag
				xSide = BLACK_TURN; // reset flag	
				xGo = 0;	// reset flag
			}
			*/
			//if board's turn: make the move, else wait for xboard
			else if (xMove_flag == 1) { // xboard done sending a move, now engine needs to send move
				printf("MOVE SENT BY XBOARD->ENGINE\n");
				printBoard(board);				
				//evaluation = eval(board, board->turnCount, tempMove);
				//printf("Eval from XBOARD->Engine = : [%d]\n", evaluation);
				clearMoveGen(movegen);
				clearMoveGen(movehistory);
				MoveGenFunction(board, move, movegen);
				// negamax
				prunes = makeMoveTree(board, move, movetree, movegen, movehistory, 0, SHRT_MIN, SHRT_MAX); //creates move tree based on all possible moves, calls board evaluation function, and makes move
				
				printf("Number of Hash Table hits = [%d]\n", board->hashtablehitcounter);
				printf("Number of Hash Table misses = [%d]\n", board->hashtablemisscounter);
				//printBoard(board);
				printBoardToFile(board); //log
				printPrunesToFile(prunes);
				makeMove(board, prunes.move, movehistory, move);
				printBoardToFile(board); //log
				//printBoard(board);
				//prunes->pruneMove = prunes->prunePath.Moves[2];//TODO: check if this stores the right move:
				board->turnCount++;
				Addr_Conversion(prunes.move.startLocation, startLocation);
				Addr_Conversion(prunes.move.endLocation, endLocation);
				// cannot send move if pruneMove has not been iterated at least once
				if (startLocation[0] >= 0 && startLocation[1] >= 0 && endLocation >= 0 && endLocation[1] >= 0) {
					switch (startLocation[1]){
					case 0: start = 'a'; break;
					case 1: start = 'b'; break;
					case 2: start = 'c'; break;
					case 3: start = 'd'; break;
					case 4: start = 'e'; break;
					case 5: start = 'f'; break;
					case 6: start = 'g'; break;
					case 7: start = 'h'; break;
					default: break;
					}
					switch (endLocation[1]){
					case 0: end = 'a'; break;
					case 1: end = 'b'; break;
					case 2: end = 'c'; break;
					case 3: end = 'd'; break;
					case 4: end = 'e'; break;
					case 5: end = 'f'; break;
					case 6: end = 'g'; break;
					case 7: end = 'h'; break;
					default: break;
					}

					fprintf(stdout, "move %c%d%c%d\n", start, startLocation[0] + 1, end, endLocation[0] + 1);
					fflush(stdout);

					printf("MOVE SENT BY ENGINE->XBOARD\n");
					printBoard(board);
					log = fopen("C:\\Users\\Nico\\source\\repos\\Chess2\\logs\\log.txt", "a+");
					fprintf(log, "MOVE SENT BY ENGINE->XBOARD\n");
					fclose(log);
					printBoardToFile(board); //log
					evaluation = eval(board, board->turnCount, move);
					printf("Eval from engine->XBOARD = [%d]\n", prunes.value);
					log = fopen("C:\\Users\\Nico\\source\\repos\\Chess2\\logs\\log.txt", "a+");
					fprintf(log, "Eval from engine->XBOARD = [%d]\n", prunes.value);
					fclose(log);
					// reset everything for prunes
					resetDebugCounters(board);					
					clearMoveGen(movegen);
					clearMoveGen(movehistory);
					MoveGenFunction(board, move, movegen);
					movetree->MoveTreeNode[0] = *movegen;
					xMove_flag = 0;
				} 
				xboard_flag = 0; // reset flag
			}
			else { // if xMove_flag has not been triggered, ask xboard for its move
				xboard(board, move, &tempMove);
				if (xMove_flag) {
					// makeMove() --> let engine know of xBoard moves
					makeMove(board, tempMove, movehistory, move);
					board->turnCount++;
					//clearMoveList(tempMove);
				}
			}
		}
		else {
			xboard(board, move, &tempMove);
			if (xMove_flag) {
				// makeMove() --> let engine know of xBoard moves
				makeMove(board, tempMove, movehistory, move);
				board->turnCount++;
				//clearMoveList(tempMove);
			}
		}
	}//end while
	return 0;
}