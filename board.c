#include "stdafx.h"
#include "board.h"
#include "space.h"
#include "move.h"
#include "movegen.h"
//SEE MACROS IN SPACE.H




//Summary: Initializes the list of the pieces and the positions they occupy for each piece. 
//MoveGen combines this list with our precompiled moves to determine our moveset:
void initializePositions(Move *move) {
	//Initialize Black positions:
	int k = 0;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 8; j++) {
			switch (j) {
			case(0): move->blackSpaces[k][BOARD_POSITION] = ((i == 0) ? 48 : 56);
				move->blackSpaces[k][PIECE_TYPE] = ((i == 0) ? BLACK_PAWN : BLACK_ROOK);	break;
			case(1): move->blackSpaces[k][BOARD_POSITION] = ((i == 0) ? 49 : 57);
				move->blackSpaces[k][PIECE_TYPE] = ((i == 0) ? BLACK_PAWN : BLACK_KNIGHT);	break;
			case(2): move->blackSpaces[k][BOARD_POSITION] = ((i == 0) ? 50 : 58);
				move->blackSpaces[k][PIECE_TYPE] = ((i == 0) ? BLACK_PAWN : BLACK_BISHOP);	break;
			case(3): move->blackSpaces[k][BOARD_POSITION] = ((i == 0) ? 51 : 59);
				move->blackSpaces[k][PIECE_TYPE] = ((i == 0) ? BLACK_PAWN : BLACK_QUEEN);	break;
			case(4): move->blackSpaces[k][BOARD_POSITION] = ((i == 0) ? 52 : 60);
				move->blackSpaces[k][PIECE_TYPE] = ((i == 0) ? BLACK_PAWN : BLACK_KING);	break;
			case(5): move->blackSpaces[k][BOARD_POSITION] = ((i == 0) ? 53 : 61);
				move->blackSpaces[k][PIECE_TYPE] = ((i == 0) ? BLACK_PAWN : BLACK_BISHOP);	break;
			case(6): move->blackSpaces[k][BOARD_POSITION] = ((i == 0) ? 54 : 62);
				move->blackSpaces[k][PIECE_TYPE] = ((i == 0) ? BLACK_PAWN : BLACK_KNIGHT);	break;
			case(7): move->blackSpaces[k][BOARD_POSITION] = ((i == 0) ? 55 : 63);
				move->blackSpaces[k][PIECE_TYPE] = ((i == 0) ? BLACK_PAWN : BLACK_ROOK);	break;
			}//end switch
			k++;
		}//end for
	}//endfor

	k = 0;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 8; j++) {
			switch (j) {
			case(0): move->whiteSpaces[k][BOARD_POSITION] = ((i == 0) ? 8 : 0);
				move->whiteSpaces[k][PIECE_TYPE] = ((i == 0) ? WHITE_PAWN : WHITE_ROOK);	break;
			case(1): move->whiteSpaces[k][BOARD_POSITION] = ((i == 0) ? 9 : 1);
				move->whiteSpaces[k][PIECE_TYPE] = ((i == 0) ? WHITE_PAWN : WHITE_KNIGHT);	break;
			case(2): move->whiteSpaces[k][BOARD_POSITION] = ((i == 0) ? 10 : 2);
				move->whiteSpaces[k][PIECE_TYPE] = ((i == 0) ? WHITE_PAWN : WHITE_BISHOP);	break;
			case(3): move->whiteSpaces[k][BOARD_POSITION] = ((i == 0) ? 11 : 3);
				move->whiteSpaces[k][PIECE_TYPE] = ((i == 0) ? WHITE_PAWN : WHITE_QUEEN);	break;
			case(4): move->whiteSpaces[k][BOARD_POSITION] = ((i == 0) ? 12 : 4);
				move->whiteSpaces[k][PIECE_TYPE] = ((i == 0) ? WHITE_PAWN : WHITE_KING);	break;
			case(5): move->whiteSpaces[k][BOARD_POSITION] = ((i == 0) ? 13 : 5);
				move->whiteSpaces[k][PIECE_TYPE] = ((i == 0) ? WHITE_PAWN : WHITE_BISHOP);	break;
			case(6): move->whiteSpaces[k][BOARD_POSITION] = ((i == 0) ? 14 : 6);
				move->whiteSpaces[k][PIECE_TYPE] = ((i == 0) ? WHITE_PAWN : WHITE_KNIGHT);	break;
			case(7): move->whiteSpaces[k][BOARD_POSITION] = ((i == 0) ? 15 : 7);
				move->whiteSpaces[k][PIECE_TYPE] = ((i == 0) ? WHITE_PAWN : WHITE_ROOK);	break;
			}//end switch
			k++;
		}//end for
	}//endfor
}//initalize Positions



void makeBoard(Board *board, Move *move, MoveGen *movegen, MoveGen *movehistory)
{
	// Initializes boardstate for a new game:
	// i represents vertical spaces  [0-7] -> (1-8)
	// j represents horizontal spaces[0,7] -> (A-H)
	// example: A-5 = [j, i] == [1, 4]	
	// int boardInt;	
	//board = (board) malloc(sizeof(board));

	board->turn = WHITE_TURN;	 //Initialize WHITE TURN
	board->castlingRights = 0xF; //KQkq
	board->PerftCaptureCounter = 0;
	board->PerftNodeCounter = 0;
	board->PerftCastleCounter = 0;
	board->PerftEPCapture = 0;
	board->PerftPromotionCounter = 0;
	board->epSquare = NO_EN_PASSANT;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			//printf("boardInt = [%d]", boardInt);			
			setSpace(board, i, j);
		}//end for j
	}//end for i
	setMoves(board, move, movegen, movehistory); //sets moves
	initializePositions(move);		//sets spaces for the black and white pieces:
									//checkMoves(board, move); UNRESOLVED SYMBOL ERROR
}// makeBoard


void setColorSpaces(Board *board, Move *move) {
	int x = 0;
	int y = 0;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((board->boardSpaces[i][j].isOccupied == IS_OCCUPIED) &&
				((board->boardSpaces[i][j].pieceType & GET_PIECE_TYPE) <= BLACK_PIECE)) {
				move->blackSpaces[x][BOARD_POSITION] = board->boardSpaces[i][j].boardposition;
				move->blackSpaces[x][PIECE_TYPE] = board->boardSpaces[i][j].pieceType;
				x++;
			}//end if 
			else if ((board->boardSpaces[i][j].isOccupied == IS_OCCUPIED) &&
				((board->boardSpaces[i][j].pieceType & GET_PIECE_TYPE) >= WHITE_PIECE)) {
				move->whiteSpaces[y][BOARD_POSITION] = board->boardSpaces[i][j].boardposition;
				move->whiteSpaces[y][PIECE_TYPE] = board->boardSpaces[i][j].pieceType;
				y++;
			}//end else			
		}//end for j
	}//end for

	if (x < 16) {
		for (x; x < 16; x++) {
			move->blackSpaces[x][BOARD_POSITION] = EMPTY;
			move->blackSpaces[x][PIECE_TYPE] = EMPTY;
		}//endfor
	}//end if x

	if (y < 16) {
		for (y; y < 16; y++) {
			move->whiteSpaces[y][BOARD_POSITION] = EMPTY;
			move->whiteSpaces[y][PIECE_TYPE] = EMPTY;
		}//endfor
	}//end if x

}//setColorSpaces





void setSpace(Board *board, unsigned int i, unsigned int j)
{
	//Set Index Value for space's position relative to the board:
	board->boardSpaces[i][j].boardposition = i * 8 + j;

	//Check if vertical index is on the first 2 and last 2 rows
	//and set occupy variable to 1 if true and 0 if false
	if ((i == 0) || (i == 1) || (i == 6) || (i == 7)) {
		board->boardSpaces[i][j].isOccupied = 0x01;
	}//end if 
	else {
		board->boardSpaces[i][j].isOccupied = 0x00;
	}//end Else

	 //Set Piece types on occupied board depending on which i and j value the board is on.

	if ((i == 1) || (i == 6)) {
		(board->boardSpaces[i][j].pieceType = ((i == 1) ? WHITE_PAWN : BLACK_PAWN));			//Set White/Black Pawns: (0x1101/0x101)
	}//if backline	
	else if ((i == 0) || (i == 7)) {
		switch (j) {
		case(0):board->boardSpaces[i][j].pieceType = ((i == 0) ? WHITE_ROOK : BLACK_ROOK);	break;	//White/Black Rook	 (0b1010 : 0b0010)
		case(1):board->boardSpaces[i][j].pieceType = ((i == 0) ? WHITE_KNIGHT : BLACK_KNIGHT);	break;	//White/Black kNight (0b1100 : 0b0100)
		case(2):board->boardSpaces[i][j].pieceType = ((i == 0) ? WHITE_BISHOP : BLACK_BISHOP);	break;	//White/Black Bishop (0b1011 : 0b0011)
		case(3):board->boardSpaces[i][j].pieceType = ((i == 0) ? WHITE_QUEEN : BLACK_QUEEN);	break;	//White/Black Queen	 (0b1001 : 0b0001)	
		case(4):board->boardSpaces[i][j].pieceType = ((i == 0) ? WHITE_KING : BLACK_KING);	break;	//White/Black King	 (0b1000 : 0b0000)
		case(5):board->boardSpaces[i][j].pieceType = ((i == 0) ? WHITE_BISHOP : BLACK_BISHOP);	break;	//White/Black Bishop (0b1011 : 0b0011)	
		case(6):board->boardSpaces[i][j].pieceType = ((i == 0) ? WHITE_KNIGHT : BLACK_KNIGHT);	break;	//White/Black kNight (0b1100 : 0b0100)	
		case(7):board->boardSpaces[i][j].pieceType = ((i == 0) ? WHITE_ROOK : BLACK_ROOK);	break;	//White/Black Rook	 (0b1010 : 0b0010)
		default:board->boardSpaces[i][j].pieceType = EMPTY;  break; //empty piece default
		}//switch
	}//endElse
}

void resetDebugCounters(Board *board)
{
	board->PerftCaptureCounter = 0;
	board->PerftCastleCounter = 0;
	board->PerftEPCapture = 0;
	board->PerftNodeCounter = 0;
	board->PerftPromotionCounter = 0;
}
//setPiece


void setPiece(Board *board, char piece, int row, int col) {
	if ((piece == 'P') || (piece == 'p') ||
		(piece == 'R') || (piece == 'r') ||
		(piece == 'N') || (piece == 'n') ||
		(piece == 'B') || (piece == 'b') ||
		(piece == 'Q') || (piece == 'q') ||
		(piece == 'K') || (piece == 'k')) {
		board->boardSpaces[row][col].isOccupied = IS_OCCUPIED;

		switch (piece) {
		case 'P': board->boardSpaces[row][col].pieceType = WHITE_PAWN; break;
		case 'p': board->boardSpaces[row][col].pieceType = BLACK_PAWN; break;
		case 'R': board->boardSpaces[row][col].pieceType = WHITE_ROOK; break;
		case 'r': board->boardSpaces[row][col].pieceType = BLACK_ROOK; break;
		case 'N': board->boardSpaces[row][col].pieceType = WHITE_KNIGHT; break;
		case 'n': board->boardSpaces[row][col].pieceType = BLACK_KNIGHT; break;
		case 'B': board->boardSpaces[row][col].pieceType = WHITE_BISHOP; break;
		case 'b': board->boardSpaces[row][col].pieceType = BLACK_BISHOP; break;
		case 'Q': board->boardSpaces[row][col].pieceType = WHITE_QUEEN; break;
		case 'q': board->boardSpaces[row][col].pieceType = BLACK_QUEEN; break;
		case 'K': board->boardSpaces[row][col].pieceType = WHITE_KING;
			updateKingCoordinates(board, WHITE_KING, row, col);
			break;
		case 'k': board->boardSpaces[row][col].pieceType = BLACK_KING;
			updateKingCoordinates(board, BLACK_KING, row, col);
			break;
		default:  break;
		}//endswitch
	}//end if 
}//setPiece

void setWhiteSpaces(Board *board, int number, int row, int col) {
	for (int i = 0; i < number; i++) {
		board->boardSpaces[row][col].isOccupied = NOT_OCCUPIED;
		board->boardSpaces[row][col].pieceType = EMPTY;
		col++;
	}//end for
}//setWhiteSpaces

 //Sets board based on FEN notation string input:
void setBoard(Board * board, Move *move, char command[])
{
	char  * token = (char *)malloc(sizeof(char *));
	int i = 0;
	printf("Splitting %s into tokens\n", command);
	token = strtok(command, "/");
	while (token != NULL) {
		printf("Token [%d] = [%s]\n", i, token);
		if (i > 7) //BOARD RIGHTS
			token = strtok(NULL, " ");
		else {
			int j = 0;
			for (int k = 0; j < 8; k++) {
				if ((token[k] >= '1') && (token[k] <= '8')) {
					int whiteSpace = token[k] - '0';
					setWhiteSpaces(board, whiteSpace, (7 - i), j);
					j += whiteSpace;
				}//end if, checking for number of whiteSpaces
				else {
					setPiece(board, token[k], (7 - i), j);
					j++;
				}//end else, setting piece since no whitespace				
			}//end for
			if (i >= 6)
				token = strtok(NULL, " ");
			else
				token = strtok(NULL, "/");
		}//end else

		i++;
	}//endwhile	
	 //TODO SET CASTLING RIGHTS, EP RIGHTS, TURNCOUNTER;
	setColorSpaces(board, move);
}//setBoard


void printBoard(Board *board) {
	// Prints Current Board State to Console:
	printf("**************************\n");
	for (int i = 7; i >= 0; i--) {
		for (int j = 0; j < 8; j++) {
			if (board->boardSpaces[i][j].isOccupied == 0x01) {
				switch (board->boardSpaces[i][j].pieceType & GET_PIECE_TYPE) {
				case(WHITE_KING):	printf("|K "); break;
				case(BLACK_KING):	printf("|K*"); break;
				case(WHITE_QUEEN):	printf("|Q "); break;
				case(BLACK_QUEEN):	printf("|Q*"); break;
				case(WHITE_ROOK):	printf("|R "); break;
				case(BLACK_ROOK):	printf("|R*"); break;
				case(WHITE_BISHOP):	printf("|B "); break;
				case(BLACK_BISHOP):	printf("|B*"); break;
				case(WHITE_KNIGHT):	printf("|N "); break;
				case(BLACK_KNIGHT):	printf("|N*"); break;
				case(WHITE_PAWN):	printf("|P "); break;
				case(BLACK_PAWN):	printf("|P*"); break;
				default:printf("|E "); break; //E = Error, you should not reach this case
				}// endSwitch				
			}//if space is occupied
			else {
				printf("|  ");
			}//endElse
		}//endForJ
		printf("|\n");
	}//endFor I
	printf("**************************\n\n\n");

}//printBoard();