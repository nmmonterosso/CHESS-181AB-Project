// Chess2.cpp : Defines the entry point for the console application.
//Nicholas Monterosso
//Main for Chess project

#include "stdafx.h"
#include <stdio.h>
#include "piece.h"
#include "space.h"

void makeBoard(space *board[8][8]);
void Dec2Bin(int num, char *binNum[6]);

int main()
{
	//Initialize board
	char *castlingRights_B, *castlingRights_W = 0x3;  //castling rights: = ({kingside, queenside}) 
	space *board[8][8];
	makeBoard(board); // Initializes board state and pieces:
	printf("Hello World!\n");
	while (1) {		
	}
	return 0;
}

void makeBoard(space *board[8][8])
{	
	
	// i represents vertical spaces  [0-7] -> (1-8)
	// j represents horizontal spaces[0,7] -> (A-H)
	// example: A-5 = [j, i] == [1, 4]	
	int boardInt;
	char *binBoardNum[6];
	for (int i = 0; i < 8; i++) {		
		for (int j = 0; j < 8; j++) {
			boardInt = (i*8 + j);
			//printf("boardInt = [%d]", boardInt);			
			Dec2Bin(boardInt, binBoardNum);

			for (int k = 0; k < 6; k++) {
				board[i][j]->boardposition[k] = binBoardNum[k];
			}//end for k
			
			switch (i) {
			case 1: board[i][j]->isOccupied = 0x1;	
					setPiece(board[i][j], i, j); //create white backline (RNBQKBNR)
					break;
			case 2: board[i][j]->isOccupied = 0x1;
					setPiece(board[i][j], i, i); //create white Pawns    (PPPPPPPP)
					break;	
			case 7: board[i][j]->isOccupied = 0x1;
					setPiece(board[i][j], i, i);//create black Pawns	(PPPPPPPP)
					break;	
			
			case 8:	board[i][j]->isOccupied = 0x1;
					setPiece(board[i][j], i, j);//create black backline (RNBQKBNR)
					break;	
			
			default:board[i][j]->isOccupied = 0;
					setPiece(board[i][j], i, j);//set Space to empty
					break; 
			} // end switch				
		}//end for j
	}//end for i
}// makeBoard



//dec2bin, for converting integer numbers to char
void Dec2Bin(int num, char *binNum[6])
{
	for (int i = 0; i < 6; i++) {
		binNum[i] = num % 2;
		num = num / 2;
	}//for
}//dec2bin 