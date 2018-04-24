#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "xboard.h"


static int getline(char *buff, size_t size)
{
	int ch, extra;

	if (fgets(buff, size, stdin) == NULL)
		return NO_INPUT;

	// fgets includes newline so if string is too long there'll be no newline.
	// flush to end of line so that excess does not effect next call
	if (buff[strlen(buff) - 1] != '\n')
	{
		extra = 0;
		while (((ch = getchar()) != '\n') && (ch != EOF))
			extra = 1;
		return (extra == 1) ? TOO_LONG : OK;
	}

	//remove newline and return string 
	buff[strlen(buff) - 1] = '\0';
	return OK;
}

void rmSubstr(char *str, const char *toRemove)
{
	while (str = strstr(str, toRemove))
		memmove(str, str + strlen(toRemove), 1 + strlen(str + strlen(toRemove)));
}

void xboard(Board *board)
{
	/*xboard() -- this will read info given by xboard and will determine functions being sent to chess engine*/
	int startPosition[2], endPosition[2]; // store starting location and end location of xboard moves
	char extra[5]; // stores extra content from xboard move string... used to indicate pawn promotion

	int read; // used for value of how many bytes read by getline()
			  //size_t nbytes = 32; // initial allocation for xboard string
	char xboard_string[32];// stores line given by xboard/winBoard

	read = getline(xboard_string, sizeof(xboard_string));
	if (read == NO_INPUT)
	{
		printf("No Input\n");
	}

	if (read == TOO_LONG) // input is too long for allocation of xboard_string
	{
		// if string is TOO_LONG then we need to reallocate memory

		//	xboard_string = (char*)realloc(xboard_string, 64 * sizeof(char)); //reallocate memory
		printf("Input too long [%s]\n", xboard_string);
	}

	if (read == OK) //xboard_string fits input
	{
		//read messages from xboard
		if (strcmp(xboard_string, "xboard") == 0) //first message sent from xboard
		{
			fprintf(stdout, "\n"); //start game
			fflush(stdout);
		}

		if (strcmp(xboard_string, "protover 2") == 0) // when protover 2 is received, present features
		{
			fprintf(stdout, "feature myname=\"JaeDong\"\n"); //set chess engine name
			fflush(stdout);

			fprintf(stdout, "feature san=0\n");	// moves in e2e4 notation... if san=1 then it would be Nf3 notation
			fflush(stdout);

			fprintf(stdout, "usermove=1\n");	// display for xboard sending moves is "usermove MOVE"
			fflush(stdout);

			fprintf(stdout, "sigint=0\n");		// turn off interruptinf signals sent by xboard
			fflush(stdout);

			fprintf(stdout, "sigterm=0\n");		// turn off termination signal

			fprintf(stdout, "pause=0\n");		// not using pause command
			fflush(stdout);

			fprintf(stdout, "nps=0\n");			// not using nps command (nodes per second)
			fflush(stdout);

			fprintf(stdout, "variant=normal\n"); // no variant used, playing normal chess
			fflush(stdout);

			fprintf(stdout, "done=1\n");		 // done with setting features... no more feature commands
			fflush(stdout);
		}

		if (strcmp(xboard_string, "new") == 0) // resetting board
		{
			//reset board
			//set white on move
			//leave force mode
			//set engine to play black

		}

		//variant VARNAME --> most likely won't need

		if (strcmp(xboard_string, "quit") == 0) // xboard is exiting
		{
			exit(); //exit program
		}

		//random --> can be ignored (for random mode)

		//if (strcmp(xboard_string, "force") == 0) // set engine to force mode
		//{
		//	//stop clocks
		//	//play neither color
		//	//engine should not think, ponder, or make moves of its own
		//}
		//
		//if (strcmp(xboard_string, "go") == 0) // will be sent to exit force mode
		//{
		//	//leave force mode
		//	//start clock
		//}
		//
		//if (strcmp(xboard_string, "draw") == 0) // opponent offers draw
		//{
		//	fprintf(stdout, "offer draw\n");
		//	fflush(stdout);
		//}
		//
		//if (strcmp(xboard_string, "hint") == 0) // user asks for hint
		//{
		//	//should have access from MoveList???
		//	fprintf(stdout, "Hint: xxx\n"); // xxx is the suggested move
		//	fflush(stdout);
		//}
		//
		//if (strcmp(xboard_string, "bk") == 0) // user selects "Book" from xboard menu
		//{
		//	fprintf(stdout, "\tBooking\n"); //we can send any text as long as each line begins with \t
		//	fflush(stdout);
		//}
		//
		//if (strcmp(xboard_string, "undo") == 0) // user wants to undo move
		//{
		//	//set current board state to previous board state
		//
		//}
		//
		// we can use result RESULT {COMMENT} to store result of match
		//
		//need to check time controls
		// st TIME and sd DEPTH
		// nps NODE_RATE <-- engine limitted to certain num of nodes
		//
		// setting time for engine


		/*----- Checking for User Moves -----*/
		if (strncmp(xboard_string, "usermove", 8) == 0)
		{
			/* removing 'usermove ' from xboard_string */
			rmSubstr(xboard_string, "usermove ");

			// when storing into startPosition/endPosition, conver xboard_string chars into ints by using (xboard_string[0] - '0')
			startPosition[1] = (xboard_string[1] - '0') - 1; // storing startPosition
			endPosition[1] = (xboard_string[3] - '0') - 1;	  // storing endPosition	

			switch (xboard_string[0])
			{
			case 'a': startPosition[0] = 0; break;
			case 'b': startPosition[0] = 1; break;
			case 'c': startPosition[0] = 2; break;
			case 'd': startPosition[0] = 3; break;
			case 'e': startPosition[0] = 4; break;
			case 'f': startPosition[0] = 5; break;
			case 'g': startPosition[0] = 6; break;
			case 'h': startPosition[0] = 7; break;
			default: break;
			}

			switch (xboard_string[2])
			{
			case 'a': endPosition[0] = 0; break;
			case 'b': endPosition[0] = 1; break;
			case 'c': endPosition[0] = 2; break;
			case 'd': endPosition[0] = 3; break;
			case 'e': endPosition[0] = 4; break;
			case 'f': endPosition[0] = 5; break;
			case 'g': endPosition[0] = 6; break;
			case 'h': endPosition[0] = 7; break;
			default: break;
			}

			if (strlen(xboard_string) == 4) // normal moves
			{

				/* Might Need to check if Move is VALID */

				// updating new space (endPosition)
				board->boardSpaces[endPosition[1]][endPosition[0]].isOccupied = IS_OCCUPIED; // space is now Occupied
				board->boardSpaces[endPosition[1]][endPosition[0]].pieceType = board->boardSpaces[startPosition[1]][startPosition[0]].pieceType; // new space has startPosition pieceType

																																				 // updating old space (startPosition)
				board->boardSpaces[startPosition[1]][startPosition[0]].isOccupied = NOT_OCCUPIED; // space now unOccupied 
				board->boardSpaces[startPosition[1]][startPosition[0]].pieceType = EMPTY;		// space EMPTY

				board->turn = !(board->turn); //switch turn
											  //printBoard(board);

			} // normal move

			else //possibly castling or promotion...etc.
			{
				//pawn promotion e7e8q
				if (strlen(xboard_string) == 5)
				{
					/*Check if valid move*/
					//updating new space
					board->boardSpaces[endPosition[1]][endPosition[0]].isOccupied = IS_OCCUPIED; // space is now Occupied
																								 // set pawn pieceType to new promotion pieceType
					switch (xboard_string[4]) {
					case 'q': board->boardSpaces[endPosition[1]][endPosition[0]].pieceType = QUEEN;
					case 'r': board->boardSpaces[endPosition[1]][endPosition[0]].pieceType = ROOK;
					case 'n': board->boardSpaces[endPosition[1]][endPosition[0]].pieceType = KNIGHT;
					case 'b': board->boardSpaces[endPosition[1]][endPosition[0]].pieceType = BISHOP;
					}

					// updating old space (startPosition)
					board->boardSpaces[startPosition[1]][startPosition[0]].isOccupied = NOT_OCCUPIED; // space now unOccupied 
					board->boardSpaces[startPosition[1]][startPosition[0]].pieceType = EMPTY;		// space EMPTY

					board->turn = !(board->turn); //switch turn

				} // pawn promotion

				  // *** need to check en passant moves ***

				  //castling e1g1, e1c1, e8g8, e8c8
				  // *** need to check castling rights ***
				if ((strlen(xboard_string) == 4) &&
					(((strncmp(xboard_string, 'e1', 2) == 0) && (board->boardSpaces[xboard_string[1]][xboard_string[0]].pieceType == KING))
						|| (((strncmp(xboard_string, 'e8', 2) == 0) && (board->boardSpaces[xboard_string[1]][xboard_string[0]].pieceType == KING)))))
				{
					if ((strncmp(xboard_string, 'e1c1', 4) == 0) || (strncmp(xboard_string, 'e8c8', 4) == 0))
					{
						board->boardSpaces[startPosition[1]][startPosition[0] - 2].pieceType = KING;
						board->boardSpaces[endPosition[1]][endPosition[0] + 1].pieceType = ROOK;
					}

					else if ((strncmp(xboard_string, 'e1g1', 4) == 0) || (strncmp(xboard_string, 'e8g8', 4) == 0))
					{
						board->boardSpaces[startPosition[1]][startPosition[0] + 2].pieceType = KING;
						board->boardSpaces[endPosition[1]][endPosition[0] - 1].pieceType = ROOK;
					}

					board->turn = !(board->turn); // switch turn
				} // castling


			} // usermove != normal move
		} // usermove check
	} // if (read == OK)
} //xboard