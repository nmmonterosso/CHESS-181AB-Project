#include "stdafx.h"
#include "board.h"
#include "space.h"
#include "move.h"
#include "movegen.h"
//SEE MACROS IN SPACE.H

extern unsigned long long randTable[64][13];
extern unsigned long long randTurn;
extern int hitflag;


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
	board->hashtablehitcounter = 0;
	board->hashtablemisscounter = 0;
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
	board->hashtablehitcounter = 0;
	board->hashtablemisscounter = 0;
}//resetDebug Counters


void clearMoveTree(MoveTree * movetree)
{	
	for (int i = 0; i < MAXDEPTH; i++) 
		clearMoveGen(&movetree->MoveTreeNode[i]);	
}

void clearMoveGen(MoveGen * movegen)
{
	for (int i = 0; i < 80; i++) {

		movegen->Moves[i].startLocation = -1;
		movegen->Moves[i].endLocation	= -1;
		movegen->Moves[i].piece			= -1;
		movegen->Moves[i].capturedPiece = -1;
	}//end for
	movegen->count = 0;
}//clearMoveGen



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
	char  *token = (char *)malloc(sizeof(char *));
	int i = 0;
	printf("Splitting %s into tokens\n", command);
	token = (char *)strtok(command, "/");
	while (token != NULL) {
		printf("Token [%d] = [%s]\n", i, token);
		if (i > 7) { //BOARD RIGHTS
			if (i == 9) {
				if (token[0] == '-')
					board->castlingRights = 0x0;
				else
					board->castlingRights = 0xF;
			}//end if 
			token = strtok(NULL, " ");
		}
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


//HASH TABLE FUNCTIONS:

 //Summary: Creates new hash table item index:
static ht_item* ht_new_item(const unsigned long long zobrist, int depth, int flag, int eval, MoveList move) {
	ht_item* i = malloc(sizeof(ht_item));
	i->zobrist	= zobrist;
	i->depth	= depth;
	i->flag		= flag;
	i->eval		= eval;	
	setMove(&i->move, move);
	return i;
} //ht_new_item

  //Summary: Creates new hash table
ht_hash_table* ht_new() {
	ht_hash_table* ht = (ht_hash_table *)malloc(sizeof(ht_hash_table));

	ht->size = 2097152; //2^21 ~2 million 
	ht->count = 0;
	ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
	for (int i = 0; i < ht->size; i++) 		
		ht->items[i] = (ht_item*)malloc(sizeof(ht_item));
	
	return ht;
}//ht_new

 //Summary: Deletes item from hash table:
static void ht_del_item(ht_item* i) {
	free(i->zobrist);
	free(i->depth);
	free(i->flag);
	free(i->eval);	
	free(i->move);
	free(i);
}//ht_del_item

 //Summary: Deletes hash table structure:
void ht_del_hash_table(ht_hash_table* ht) {
	for (int i = 0; i < ht->size; i++) {
		ht_item* item = ht->items[i];
		if (item != NULL)
			ht_del_item(item);
	}//end for
	free(ht->items);
	free(ht);
}//ht_del_hash_table


void setMove(MoveList *dest, MoveList source) {
	dest->startLocation = source.startLocation;
	dest->endLocation = source.endLocation;
	dest->piece = source.piece;
	dest->capturedPiece = source.capturedPiece;
}//setMove


//Summary: Checks if the item is inside the hash table and returns the prune value
//associated with it
Prunes ht_read(ht_hash_table * ht, volatile unsigned long long * zobrist, int depth)
{
	Prunes prunes;
	ht_item *item = get_ht_item(ht, zobrist);
	if (item) {
		if ((item->zobrist == *zobrist) && (item->depth >= depth)) {
			prunes.boardVal = item->eval;
			prunes.pruneMove = item->move;
			hitflag = 1;
		} // if hit
		else {
			prunes.boardVal = 0;
			prunes.pruneMove.piece = -1;
			prunes.pruneMove.startLocation = -1;
			prunes.pruneMove.endLocation = -1;
			prunes.pruneMove.capturedPiece = -1;
			hitflag = 0;
		}
	}//end if:
	return prunes;
} // ht_read()

  //Summary: Replace when Hash Table Collisions or Misses:
void ht_write(ht_hash_table * ht, volatile unsigned long long * zobrist, int depth, int flag, int eval, MoveList move)
{
	ht_item * item = get_ht_item(ht, zobrist);
	if (item) {
		if ((item->zobrist != *zobrist) || (item->depth < depth)) {
			item->zobrist = *zobrist;
			item->depth = depth;
			item->flag = flag;
			item->eval = eval;
			item->move = move;
		}//end if 
	}
	else {
		item = ht_new_item(*zobrist, depth, flag, eval, move); //should not hit this line
	}
}//ht_replace_item



ht_item * get_ht_item(ht_hash_table * ht, volatile unsigned long long * zobrist){

	return ht->items[*zobrist % ht->size];
}// get_item




//Returns 0 on false, 1 on true:
int isInTable(ht_item * item, volatile unsigned long long * zobrist, int depth)
{
	if ((item->zobrist == *zobrist) && (item->depth >= depth))
		return 1;
	else	
		return 0;
}
//ht_replace_item

//Sets random number table for Hash Function:
void init_zobrist() {
	srand(999631412);
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 13; j++) {
			randTable[i][j] = ((unsigned long long)RAND() << 48) ^
							  ((unsigned long long)RAND() << 35) ^
							  ((unsigned long long)RAND() << 22) ^
							  ((unsigned long long)RAND() << 9)  ^
							  ((unsigned long long)RAND() >> 4);
		}//end for j
	}//end for i
	randTurn =  ((unsigned long long)RAND() << 48) ^
				((unsigned long long)RAND() << 35) ^
				((unsigned long long)RAND() << 22) ^
				((unsigned long long)RAND() << 9) ^
				((unsigned long long)RAND() >> 4);
	
}// init_zobrist()

void set_zobrist_value(Board *board, volatile unsigned long long *zobrist) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			switch (board->boardSpaces[i][j].pieceType) {
			case(WHITE_PAWN):	*zobrist = (*zobrist ^ randTable[8 * i + j][HASH_WHITE_PAWN]);	break;
			case(WHITE_KNIGHT):	*zobrist = (*zobrist ^ randTable[8 * i + j][HASH_WHITE_KNIGHT]); break;
			case(WHITE_BISHOP):	*zobrist = (*zobrist ^ randTable[8 * i + j][HASH_WHITE_BISHOP]); break;
			case(WHITE_ROOK):	*zobrist = (*zobrist ^ randTable[8 * i + j][HASH_WHITE_ROOK]);	break;
			case(WHITE_QUEEN):	*zobrist = (*zobrist ^ randTable[8 * i + j][HASH_WHITE_QUEEN]);	break;
			case(WHITE_KING):	*zobrist = (*zobrist ^ randTable[8 * i + j][HASH_WHITE_KING]);	break;
			case(BLACK_PAWN):	*zobrist = (*zobrist ^ randTable[8 * i + j][HASH_BLACK_PAWN]);	break;
			case(BLACK_KNIGHT):	*zobrist = (*zobrist ^ randTable[8 * i + j][HASH_BLACK_KNIGHT]); break;
			case(BLACK_BISHOP):	*zobrist = (*zobrist ^ randTable[8 * i + j][HASH_BLACK_BISHOP]); break;
			case(BLACK_ROOK):	*zobrist = (*zobrist ^ randTable[8 * i + j][HASH_BLACK_ROOK]);	break;
			case(BLACK_QUEEN):	*zobrist = (*zobrist ^ randTable[8 * i + j][HASH_BLACK_QUEEN]);	break;
			case(BLACK_KING):	*zobrist = (*zobrist ^ randTable[8 * i + j][HASH_BLACK_KING]);	break;
			default:			*zobrist = (*zobrist ^ randTable[8 * i + j][EMPTY_HASH]);		break;
			}//end switch				
		} // for j
	} // for i	
	*zobrist = *zobrist ^ randTurn; //Last Step for zobrist number for current turn:
} //set_zobrist_value


//Summary: Updates on makemove and unmake move regardless of 
void update_zobrist(MoveList move, volatile unsigned long long *zobrist) {
	
	switch (move.piece) {
	case(WHITE_PAWN):	*zobrist = (*zobrist ^ randTable[move.startLocation][HASH_WHITE_PAWN]);
						*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_WHITE_PAWN]); 
						break;
	case(WHITE_KNIGHT):	*zobrist = (*zobrist ^ randTable[move.startLocation][HASH_WHITE_KNIGHT]); 
						*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_WHITE_KNIGHT]); 
						break;
	case(WHITE_BISHOP):	*zobrist = (*zobrist ^ randTable[move.startLocation][HASH_WHITE_BISHOP]);
						*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_WHITE_BISHOP]); 
						break;
	case(WHITE_ROOK):	*zobrist = (*zobrist ^ randTable[move.startLocation][HASH_WHITE_ROOK]);	
						*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_WHITE_ROOK]); 
						break;
	case(WHITE_QUEEN):	*zobrist = (*zobrist ^ randTable[move.startLocation][HASH_WHITE_QUEEN]);
						*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_WHITE_QUEEN]); 
						break;
	case(WHITE_KING):	*zobrist = (*zobrist ^ randTable[move.startLocation][HASH_WHITE_KING]);
						*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_WHITE_KING]); 
						break;
	case(BLACK_PAWN):	*zobrist = (*zobrist ^ randTable[move.startLocation][HASH_BLACK_PAWN]);
						*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_PAWN]); 
						break;
	case(BLACK_KNIGHT):	*zobrist = (*zobrist ^ randTable[move.startLocation][HASH_BLACK_KNIGHT]);
						*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_KNIGHT]); 
						break;
	case(BLACK_BISHOP):	*zobrist = (*zobrist ^ randTable[move.startLocation][HASH_BLACK_BISHOP]);
						*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_BISHOP]); 
						break;
	case(BLACK_ROOK):	*zobrist = (*zobrist ^ randTable[move.startLocation][HASH_BLACK_ROOK]);
						*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_ROOK]); 
						break;
	case(BLACK_QUEEN):	*zobrist = (*zobrist ^ randTable[move.startLocation][HASH_BLACK_QUEEN]);	
						*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_QUEEN]); 
						break;
	case(BLACK_KING):	*zobrist = (*zobrist ^ randTable[move.startLocation][HASH_BLACK_KING]);
						*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_KING]);
						break;
	default:			fprintf(stderr, "ERROR in update_zobrist:\n");		break; //should not reach this
	}//end switch

	//normal capture:
	if ((move.capturedPiece != NO_CAPTURE) && (move.capturedPiece >= 0) && (move.capturedPiece <= 13)) {
		switch (move.capturedPiece) {
		case(WHITE_PAWN):	*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_WHITE_PAWN]);	break;
		case(WHITE_KNIGHT):	*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_WHITE_KNIGHT]); break;
		case(WHITE_BISHOP):	*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_WHITE_BISHOP]); break;
		case(WHITE_ROOK):	*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_WHITE_ROOK]);	break;
		case(WHITE_QUEEN):	*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_WHITE_QUEEN]);	break;
		case(WHITE_KING):	*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_WHITE_KING]);	break;
		case(BLACK_PAWN):	*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_PAWN]);	break;
		case(BLACK_KNIGHT):	*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_KNIGHT]); break;
		case(BLACK_BISHOP):	*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_BISHOP]); break;
		case(BLACK_ROOK):	*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_ROOK]);	break;
		case(BLACK_QUEEN):	*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_QUEEN]);	break;
		case(BLACK_KING):	*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_KING]);	break;
		default:			fprintf(stderr, "ERROR in update_zobrist:\n");		break; //should not reach this
		}//end switch
	}// end if 
	//Castling: XOR ROOKS:
	if ((move.capturedPiece != NO_CAPTURE) && (move.capturedPiece >= 82) && (move.capturedPiece <= 85))
		update_zobrist_castling(move, zobrist);	
	// If Promotion:
	if ((move.capturedPiece != NO_CAPTURE) && (move.capturedPiece >= 32 && (move.capturedPiece <= 79))) 
		update_zobrist_promotion(move, zobrist);

	//update_zobrist_turn:
	*zobrist = *zobrist ^ randTurn;
}// updates_zobrist()


void update_zobrist_castling(MoveList move, volatile unsigned long long *zobrist) {
	switch (move.capturedPiece) {
	case(WHITE_CASTLE_KINGSIDE): *zobrist = (*zobrist ^ randTable[7][HASH_WHITE_ROOK]);
								 *zobrist = (*zobrist ^ randTable[5][HASH_WHITE_ROOK]);
							     break;

	case(WHITE_CASTLE_QUEENSIDE): *zobrist = (*zobrist ^ randTable[0][HASH_WHITE_ROOK]);
								  *zobrist = (*zobrist ^ randTable[3][HASH_WHITE_ROOK]);
								  break;

	case(BLACK_CASTLE_KINGSIDE): *zobrist = (*zobrist ^ randTable[63][HASH_BLACK_ROOK]);
								 *zobrist = (*zobrist ^ randTable[61][HASH_BLACK_ROOK]);
								 break;

	case(BLACK_CASTLE_QUEENSIDE): *zobrist = (*zobrist ^ randTable[56][HASH_BLACK_ROOK]);
								  *zobrist = (*zobrist ^ randTable[59][HASH_BLACK_ROOK]);
								  break;
	}//end switch

}//update_zobrist_castling()


void update_zobrist_promotion(MoveList move, volatile unsigned long long *zobrist) {
	
	//HASH IN PROMOTED PIECE
	*zobrist = (*zobrist ^ randTable[move.endLocation][(((move.endLocation / 8) == 7) ? HASH_WHITE_PAWN : HASH_BLACK_PAWN)]);
	if (((move.capturedPiece >= 32) && (move.capturedPiece <= 37)) || ((move.capturedPiece >= 56) && (move.capturedPiece <= 61)))
		*zobrist = (*zobrist ^ randTable[move.endLocation][(((move.endLocation / 8) == 7) ? HASH_WHITE_QUEEN : HASH_BLACK_QUEEN)]);

	else if (((move.capturedPiece >= 38) && (move.capturedPiece <= 43)) || ((move.capturedPiece >= 62) && (move.capturedPiece <= 67)))
		*zobrist = (*zobrist ^ randTable[move.endLocation][(((move.endLocation / 8) == 7) ? HASH_WHITE_ROOK : HASH_BLACK_ROOK)]);

	else if (((move.capturedPiece >= 44) && (move.capturedPiece <= 49)) || ((move.capturedPiece >= 68) && (move.capturedPiece <= 73)))
		*zobrist = (*zobrist ^ randTable[move.endLocation][(((move.endLocation / 8) == 7) ? HASH_WHITE_KNIGHT : HASH_BLACK_KNIGHT)]);

	else if (((move.capturedPiece >= 50) && (move.capturedPiece <= 55)) || ((move.capturedPiece >= 74) && (move.capturedPiece <= 79)))
		*zobrist = (*zobrist ^ randTable[move.endLocation][(((move.endLocation / 8) == 7) ? HASH_WHITE_BISHOP : HASH_BLACK_BISHOP)]);


	//HASH OUT CAPTURED PIECE
	//capture black queen;
	if ((move.capturedPiece == 37) || (move.capturedPiece == 43) || (move.capturedPiece == 49) || (move.capturedPiece == 55))
		*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_QUEEN]);
	//capture black rook
	if ((move.capturedPiece == 34) || (move.capturedPiece == 40) || (move.capturedPiece == 46) || (move.capturedPiece == 52))
		*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_ROOK]);
	//capture black knight
	if ((move.capturedPiece == 35) || (move.capturedPiece == 41) || (move.capturedPiece == 47) || (move.capturedPiece == 53))
		*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_KNIGHT]);
	//capture black bishop
	if ((move.capturedPiece == 36) || (move.capturedPiece == 42) || (move.capturedPiece == 48) || (move.capturedPiece == 54))
		*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_BISHOP]);

	//capture white queen:
	if ((move.capturedPiece == 61) || (move.capturedPiece == 67) || (move.capturedPiece == 73) || (move.capturedPiece == 79))
		*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_QUEEN]);
	//capture white rook:
	if ((move.capturedPiece == 58) || (move.capturedPiece == 64) || (move.capturedPiece == 70) || (move.capturedPiece == 76))
		*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_QUEEN]);
	//capture white knight:
	if ((move.capturedPiece == 59) || (move.capturedPiece == 55) || (move.capturedPiece == 61) || (move.capturedPiece == 67))
		*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_QUEEN]);
	//capture white bishop:
	if ((move.capturedPiece == 60) || (move.capturedPiece == 66) || (move.capturedPiece == 72) || (move.capturedPiece == 78))
		*zobrist = (*zobrist ^ randTable[move.endLocation][HASH_BLACK_QUEEN]);
}
//HASH FUNCTION:
//PsuedoCode:
//function hash(string, a, num_buckets):
// hash = 0;
// string_len = length(string)
// for (i = 0, 1... string length)
//     hash += (a ** ((string_len - (i + 1))) * char_code(string[i])
// hash = hash % num_buckets
// return hash;
