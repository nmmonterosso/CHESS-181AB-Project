#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "xboard.h"
#include "move.h"

//GLOBAL Variable for flag
int xboard_flag = 0;
int xMove_flag = 0;
// engine side stored in xSide
int xSide = BLACK_TURN;
int xGo = 0;

extern volatile unsigned long long *zobrist;

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

void xboard(Board *board, Move *movespace, MoveList *move)
{
	/*xboard() -- this will read info given by xboard and will determine functions being sent to chess engine*/

	// store starting location and end location of xboard moves
	int startPosition[2], endPosition[2]; 
	// stores extra content from xboard move string... used to indicate pawn promotion
	char extra[5]; 
	// used for value of how many bytes read by getline()
	int read; 
	// stores line given by xboard/winBoard
	char xboard_string[32];
	char buff[32];
	read = getline(xboard_string, sizeof(xboard_string));

	if (read == OK) 
	{
		// read messages from xboard
		if (strcmp(xboard_string, "xboard") == 0) //first message sent from xboard
		{
			fprintf(stdout, "\n"); //start game
			fflush(stdout);
		}

		// send features to xboard
		if (strcmp(xboard_string, "protover 2") == 0) // when protover 2 is received, present features
		{
			fprintf(stdout,
				"feature myname=\"Super Chess\""	//set chess engine name
				" debug=1"
				" name=0"
				" san=0"					// xboard sends move in coordinate notation g1g3
				" usermove=1"				// display for xboard sending moves is "usermove MOVE"
				" colors=1"					// xboard will send white/black signals
				" sigint=0"					// turn off interruptinf signals sent by xboard
				" sigterm=0"				// turn off termination signal
				" pause=0"					// not using pause command
				" nps=0"					// not using nps command (nodes per second)
				" variant=normal"			// no variant used, playing normal chess
				" ping=0"					// turn off ping protocol
				" setboard=0"				// turn off setboard protocol (***Look Up***)
				" playother=0"				// turn off playother protocol
				" time=0"					// stop xbord from sending time to engine MIGHT NEED IN FUTUR
				" draw=0"					// turn off draw protocol -- user cannot send dra
				" reuse=0"					// xboard will kill engine process after every game and start fresh process
				" analyze=0"				// turn off analysis mode protocol
				" debug=0"					// turn off debug protocol
				" highlight=0"				// turn off highlight protocol
				" ics=0"					// turn off ics command
				" done=1\n"					// done with setting features... no more feature commands
			); 
			fflush(stdout);
		}

		// xboard sends quit signal
		if (strcmp(xboard_string, "quit") == 0) // xboard is exiting
		{
			exit(0); //exit program
		}
		
		// set engine side
		if (strcmp(xboard_string, "white") == 0) {
			// if signal is thrown, engine plays white
			// go will be sent immediately after this signal
			xSide = WHITE_TURN;
		}

		if (strcmp(xboard_string, "go") == 0){
			xGo = 1;
		}

		// read xboard moves
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
				//MoveList temp;
				move->startLocation = 8 * startPosition[1] + startPosition[0];
				move->endLocation   = 8 * endPosition[1]   + endPosition[0];
				move->piece = board->boardSpaces[startPosition[1]][startPosition[0]].pieceType;
				if (board->boardSpaces[endPosition[1]][endPosition[0]].isOccupied)
					move->capturedPiece = board->boardSpaces[endPosition[1]][endPosition[0]].pieceType; // check this
				else
					move->capturedPiece = NO_CAPTURE;
				//CASTLING START:
				if (board->castlingRights > 0x0) {
					checkCastling(xboard_string, board, move);					
				}// end castling
			} // normal move

			else //possibly castling or promotion...etc.
			{
				//pawn promotion e7e8q
				if (strlen(xboard_string) == 5)
				{
					/*Check if valid move*/
					//updating new space
					
					switch (xboard_string[4]) {
					case 'q': if (startPosition[0] == endPosition[0]) {
								move->startLocation = board->boardSpaces[startPosition[1]][startPosition[0]].boardposition;
								move->endLocation = board->boardSpaces[endPosition[1]][endPosition[0]].boardposition;
								move->piece = ((board->turn == WHITE_TURN) ? WHITE_PAWN : BLACK_PAWN);
								move->capturedPiece = ((board->turn == WHITE_TURN) ? WHITE_PROMOTE_QUEEN_NO_CAPTURE : BLACK_PROMOTE_QUEEN_NO_CAPTURE);
							  }
							  else {} 
							  break;								
						
					case 'r': if (startPosition[0] == endPosition[0]) {
								move->startLocation = board->boardSpaces[startPosition[1]][startPosition[0]].boardposition;
								move->endLocation = board->boardSpaces[endPosition[1]][endPosition[0]].boardposition;
								move->piece = ((board->turn == WHITE_TURN) ? WHITE_PAWN : BLACK_PAWN);
								move->capturedPiece = ((board->turn == WHITE_TURN) ? WHITE_PROMOTE_ROOK_NO_CAPTURE : BLACK_PROMOTE_ROOK_NO_CAPTURE);
							  }
							  else {} 
							  break;

					case 'n': if (startPosition[0] == endPosition[0]) {
								move->startLocation = board->boardSpaces[startPosition[1]][startPosition[0]].boardposition;
								move->endLocation = board->boardSpaces[endPosition[1]][endPosition[0]].boardposition;
								move->piece = ((board->turn == WHITE_TURN) ? WHITE_PAWN : BLACK_PAWN);
								move->capturedPiece = ((board->turn == WHITE_TURN) ? WHITE_PROMOTE_KNIGHT_NO_CAPTURE : BLACK_PROMOTE_KNIGHT_NO_CAPTURE);
							  } // end if
							  else {} 
							  break;

					case 'b': if (startPosition[0] == endPosition[0]) {
								move->startLocation = board->boardSpaces[startPosition[1]][startPosition[0]].boardposition;
								move->endLocation = board->boardSpaces[endPosition[1]][endPosition[0]].boardposition;
								move->piece = ((board->turn == WHITE_TURN) ? WHITE_PAWN : BLACK_PAWN);
								move->capturedPiece = ((board->turn == WHITE_TURN) ? WHITE_PROMOTE_BISHOP_NO_CAPTURE : BLACK_PROMOTE_BISHOP_NO_CAPTURE);
							}//end if
							  else {} 
							  break;
					}//end switch
				} // pawn promotion

				  // *** need to check en passant moves ***

				  //castling e1g1, e1c1, e8g8, e8c8
				  // *** need to check castling rights ***
				
			} // usermove != normal move

			xMove_flag = 1; // done receiving move

		} // usermove check
		  
	} // if (read == OK)

	// xboard is done talking
	xboard_flag = 1; 

} //xboard

void checkCastling(char * xboard_string, Board * board, MoveList * move)
{
	if ((strncmp(xboard_string, "e1c1", 4) == 0) && ((board->castlingRights & CHECK_WHITE_CASTLE_QUEENSIDE) == 0x4)) {
		move->piece = WHITE_KING;
		move->startLocation = 4;
		move->endLocation = 2;
		move->capturedPiece = WHITE_CASTLE_QUEENSIDE;
	}
	else if ((strncmp(xboard_string, "e8c8", 4) == 0) && ((board->castlingRights & CHECK_BLACK_CASTLE_QUEENSIDE) == 0x1)) {
		move->piece = BLACK_KING;
		move->startLocation = 60;
		move->endLocation = 58;
		move->capturedPiece = BLACK_CASTLE_QUEENSIDE;
	}

	else if ((strncmp(xboard_string, "e1g1", 4) == 0) && ((board->castlingRights & CHECK_WHITE_CASTLE_KINGSIDE) == 0x8)) {
		move->piece = WHITE_KING;
		move->startLocation = 4;
		move->endLocation = 6;
		move->capturedPiece = WHITE_CASTLE_KINGSIDE;

	}

	else if ((strncmp(xboard_string, "e8g8", 4) == 0) && ((board->castlingRights & CHECK_BLACK_CASTLE_KINGSIDE) == 0x2)) {
		move->piece = BLACK_KING;
		move->startLocation = 60;
		move->endLocation = 62;
		move->capturedPiece = BLACK_CASTLE_KINGSIDE;
	}
} //checkCastling
