#include "stdafx.h"
#include "board.h"
#include "move.h"
#include "movegen.h"



//SUMMARY: Makes Move given the Movelist parameter and stores the MoveList
//into an array of Moves already done:
void makeMove(Board *board, MoveList move, MoveGen *moveHistory, Move *moveSpace)
{
	int StartCoordinates[2], EndCoordinates[2];
	int i, j, k, l;
	Addr_Conversion(move.startLocation, StartCoordinates);
	Addr_Conversion(move.endLocation,   EndCoordinates);

	i = StartCoordinates[0];
	j = StartCoordinates[1];
	k = EndCoordinates[0];
	l = EndCoordinates[1];
	
	board->boardSpaces[i][j].isOccupied = NOT_OCCUPIED; //set Start Unoccupied
	board->boardSpaces[i][j].pieceType  = EMPTY;		//set Start Piece EMPTY

	board->boardSpaces[k][l].isOccupied = IS_OCCUPIED; //Set endLocation OCCUPIED
	board->boardSpaces[k][l].pieceType = move.piece;	  //Set EndLocation's Piecetype
	
	moveHistory->Moves[moveHistory->count] = move; //Assigns the move to the move history
	moveHistory->count++;							//Appends move history
	//TODO: FIX EP:

	//UPDATE EP SQUARE:
	if (move.capturedPiece == EN_PASSANT) {		
		board->PerftEPCapture++;
		board->PerftCaptureCounter++;
		if (board->turn == WHITE_TURN) {
			board->boardSpaces[k - 1][l].isOccupied = NOT_OCCUPIED;
			board->boardSpaces[k - 1][l].pieceType = EMPTY;			
		}//end if WHITE_TURN

		else if (board->turn == BLACK_TURN) {
			board->boardSpaces[k + 1][l].isOccupied = NOT_OCCUPIED;
			board->boardSpaces[k + 1][l].pieceType = EMPTY;
		}//end if BLACK_TURN
	}//end if
	//TODO: update for EP:
	updateEPSquare(board, move);
	updateColorSpaces(board, move, moveSpace, 0);
	board->turn = ((board->turn == WHITE_TURN) ? BLACK_TURN : WHITE_TURN);
	if (move.capturedPiece != NO_CAPTURE)
		board->PerftCaptureCounter++;
	if ((move.piece == WHITE_KING) || (move.piece == BLACK_KING))
		updateKingCoordinates(board, move.piece, k, l);
}


void unMakeMove(Board *board, MoveGen *moveHistory, Move *moveSpace)
{
	int StartCoordinates[2], EndCoordinates[2];
	int i, j, k, l;
	Addr_Conversion(moveHistory->Moves[moveHistory->count - 1].endLocation, StartCoordinates);
	Addr_Conversion(moveHistory->Moves[moveHistory->count - 1].startLocation, EndCoordinates);

	i = StartCoordinates[0];
	j = StartCoordinates[1];
	k = EndCoordinates[0];
	l = EndCoordinates[1];
	
	//IF CAPTURE OCCURS:
	if (moveHistory->Moves[moveHistory->count - 1].capturedPiece != NO_CAPTURE) {
		if (moveHistory->Moves[moveHistory->count - 1].capturedPiece == EN_PASSANT) {
			//If EP Capture, Undo EP:
			if (moveHistory->Moves[moveHistory->count - 1].piece == WHITE_PAWN) {
				//if white pawn restore black pawn:
				board->boardSpaces[4][j].isOccupied = IS_OCCUPIED;
				board->boardSpaces[4][j].pieceType = BLACK_PAWN;
			}//end if white piece moved
			else if (moveHistory->Moves[moveHistory->count - 1].piece == BLACK_PAWN) {
				board->boardSpaces[3][j].isOccupied = IS_OCCUPIED;
				board->boardSpaces[3][j].pieceType = WHITE_PAWN;
			}//end else if black pawn moved:
			board->boardSpaces[i][j].isOccupied = NOT_OCCUPIED;
			board->boardSpaces[i][j].pieceType = EMPTY;
		}// end if 
		else {
			board->boardSpaces[i][j].isOccupied = IS_OCCUPIED;
			board->boardSpaces[i][j].pieceType = moveHistory->Moves[moveHistory->count - 1].capturedPiece;
		}//else normal capture, no EP:
	}//end if Captured piece

	else {
		board->boardSpaces[i][j].isOccupied = NOT_OCCUPIED;
		board->boardSpaces[i][j].pieceType = EMPTY;
	}//end else no captured

	board->boardSpaces[k][l].isOccupied = IS_OCCUPIED;
	board->boardSpaces[k][l].pieceType = moveHistory->Moves[moveHistory->count - 1].piece;
	
	
	board->turn = ((board->turn == WHITE_TURN) ? BLACK_TURN : WHITE_TURN); //change turn:
	//UPDATE EP
	if (moveHistory->count > 1)		
		updatePrevEPSquare(board, moveHistory->Moves[moveHistory->count - 2]);//CHECK TO SEE IF SEG FAULT HERE
	else
		board->epSquare = NO_EN_PASSANT;
	//END UPDATE EP
	updateColorSpaces(board, moveHistory->Moves[moveHistory->count - 1], moveSpace, 1);

	if ((moveHistory->Moves[moveHistory->count - 1 ].piece == WHITE_KING)|| 
		(moveHistory->Moves[moveHistory->count - 1 ].piece == BLACK_KING))
		updateKingCoordinates(board, moveHistory->Moves[moveHistory->count - 1].piece, k, l);

	//TODO: Free memory for history HERE:

	moveHistory->Moves[moveHistory->count].capturedPiece	= -1;
	moveHistory->Moves[moveHistory->count].startLocation	= -1;
	moveHistory->Moves[moveHistory->count].endLocation		= -1;
	moveHistory->Moves[moveHistory->count].piece			= -1;
	moveHistory->count--;
	//TODO, RESTORE CASTLING RIGHTS/ EN PASSANT:		
}//UnMakeMove

//Summary: updates colorspaces for movegeneration for the next set. Allows for easier calculations of which piece belongs to both sides:
void updateColorSpaces(Board *board, MoveList  move, Move *moveSpace, int undo)
{
	
	for (int i = 0; i < 16; i++) {
		if (board->turn == WHITE_TURN) {
			if (move.startLocation == moveSpace->whiteSpaces[i][BOARD_POSITION])
				moveSpace->whiteSpaces[i][BOARD_POSITION] = move.endLocation;	//updates on makemove()
			else if (move.endLocation == moveSpace->whiteSpaces[i][BOARD_POSITION])
				moveSpace->whiteSpaces[i][BOARD_POSITION] = move.startLocation; //updates on unmakemove()
		}//white turn
		else {
			if (move.startLocation == moveSpace->blackSpaces[i][BOARD_POSITION])
				moveSpace->blackSpaces[i][BOARD_POSITION] = move.endLocation;
			else if (move.endLocation == moveSpace->blackSpaces[i][BOARD_POSITION])
				moveSpace->blackSpaces[i][BOARD_POSITION] = move.startLocation; //used for undoing moves
		}//black turn
		if (move.capturedPiece != NO_CAPTURE) {
			if (board->turn == WHITE_TURN) {
				if (move.endLocation == moveSpace->blackSpaces[i][BOARD_POSITION]) {
					if (undo == 1)
						moveSpace->blackSpaces[i][PIECE_TYPE] = move.capturedPiece;
					else
						moveSpace->blackSpaces[i][PIECE_TYPE] = -1;					
					//moveSpace->blackSpaces[i][BOARD_POSITION] = -1;
				}//endif				
			}//endif 
			else { //Black Turn capturing white piece
				if (move.endLocation == moveSpace->whiteSpaces[i][BOARD_POSITION]) {
					if (undo == 1)
						moveSpace->whiteSpaces[i][PIECE_TYPE] = move.capturedPiece;
					else
						moveSpace->whiteSpaces[i][PIECE_TYPE] = -1;
					//moveSpace->whiteSpaces[i][BOARD_POSITION] = -1;
				}//end if
			}//end else
		}//end if no capture		
	}//end for 			
}//updateColorSpaces
//MakeMove


void updateKingCoordinates(Board *board, char piece, int i, int j) {
	if (piece == WHITE_KING) {
		board->whiteKingCoordinates[0] = i;
		board->whiteKingCoordinates[1] = j;
	}//end if
	else if (piece == BLACK_KING) {
		board->blackKingCoordinates[0] = i;
		board->blackKingCoordinates[1] = j;
	}//end else
}//updateKingCoordiates

//Summary: Checks if current move moved pawn, if it did then check if moved 2 places. then update the EP Square
//If not, then set EP square to default value
void updateEPSquare(Board *board, MoveList move) {
	int start, end, file;
	start = move.startLocation / 8;
	end   = move.endLocation / 8;
	file = move.startLocation % 8;
	if ((move.piece == WHITE_PAWN || move.piece == BLACK_PAWN) && ( (start == 1 && end == 3) || (start == 6 && end == 4) ) ){
		//printf("Printboard for debug Purposes:\n");
		//printBoard(board);
		//printf("Printboard end\n");
		if ((start == 1) && (end == 3))
			board->epSquare = board->boardSpaces[2][file].boardposition;
		else if ((start == 6) && (end == 4))
			board->epSquare = board->boardSpaces[5][file].boardposition;
		else
			printf("ERROR YOU SHOULDN'T HIT THIS \n");
	}
	else
		board->epSquare = NO_EN_PASSANT;
}//updateEPSquare:


void updatePrevEPSquare(Board *board, MoveList prevmove) {
	
	int start, end, file;
	start = prevmove.startLocation / 8;
	end = prevmove.endLocation / 8;
	file = prevmove.startLocation % 8;
	//is the first if redundant?
	if (prevmove.capturedPiece == EN_PASSANT) {		
		if (prevmove.piece == WHITE_PAWN)
			board->epSquare = board->boardSpaces[5][file].boardposition;
		else if (prevmove.piece == BLACK_PAWN)
			board->epSquare = board->boardSpaces[2][file].boardposition;
	}//end if previous move was en passant move

	else if ((prevmove.piece == WHITE_PAWN || prevmove.piece == BLACK_PAWN) &&
		(((start == 1) && (end == 3)) || ((start == 6) && (start == 4)))    ){
		if (prevmove.piece == WHITE_PAWN)
			board->epSquare = board->boardSpaces[2][file].boardposition;
		else if (prevmove.piece == BLACK_PAWN)
			board->epSquare = board->boardSpaces[5][file].boardposition;
	}//end else if 


}//updatePrevEPSquare




//Summary: Calculates each possible move a bishop can move to when given a starting space
//Assumes Board is empty so this result must check to see if spaces in the path are occupied
//to determine captures or movement blocking.
void setBishopMoves(Board * board, int i, int j, Move * move)
{
	int x, y, k;

	for (x = 0; x < 13; x++) 
		move->bishopMoves[i][j][x] = -1;
	//NOTE: We can break this function up into smaller easier to read subroutines:
	k = 0;
	y = i;
	x = j;
	//SetTopRightDirection:
	while((x + 1 < 8) && (y + 1 < 8)) {
		x++; y++;		
		move->bishopMoves[i][j][k] = board->boardSpaces[y][x].boardposition;		
		k++;
	}//endWhile	
	y = i;
	x = j;
	//SetBotRightDirection:
	while ((x + 1 < 8) && (y - 1 >= 0)) {
		x++; y--;
		move->bishopMoves[i][j][k] = board->boardSpaces[y][x].boardposition;
		k++;
	}//endWhileBotRight		
	y = i;
	x = j;
	//SetBotleftDirection:
	while ((x - 1 >= 0) && (y - 1 >= 0)) {
		x--; y--;
		move->bishopMoves[i][j][k] = board->boardSpaces[y][x].boardposition;
		k++;
	}//endWhileBotRight
	
	y = i;
	x = j;
	//SetTopLeftDirection:
	while ((x - 1 >= 0) && (y + 1 < 8)) {
		x--; y++;
		move->bishopMoves[i][j][k] = board->boardSpaces[y][x].boardposition;
		k++;
	}//endWhileBotRight
}//setBishopMoves


//Summary: Calculates each possible movement a knight can move given a starting space
//Assumes Board is empty so this result must check to see if the space is occupied to make captures
//and to see if the occupied piece is friendly or hostile to determine the actual moveset:
//Checks moves counterclockwise:
void setKnightMoves(Board *board, int i, int j, Move *move) {
	int x, k = 0; //initializing variables
	//Initializes all values in the moves to -1 by default (-1 signifies end of moveset)
	for (x = 0; x < 8; x++)
		move->knightMoves[i][j][x] = -1;
		
	for (x = 0; x < 8; x++) {
		switch (x) {
		case 0: if ((i + 2 < 8) && (j + 1 < 8)) {//Check UP -> Right Position:
					move->knightMoves[i][j][k] = board->boardSpaces[i + 2][j + 1].boardposition;
					k++;
				}//endif
				break;

		case 1:	if ((i + 1 < 8) && (j + 2 < 8)) {//Check Right -> UP Position:
					move->knightMoves[i][j][k] = board->boardSpaces[i + 1][j + 2].boardposition;
					k++;
				}//endif			
				break;

		case 2:	if ((i - 1 >= 0) && (j + 2 < 8)) {//Check Right -> DOWN Position:
					move->knightMoves[i][j][k] = board->boardSpaces[i - 1][j + 2].boardposition;
					k++;
				}//endif			
				break;

		case 3:	if ((i - 2 >= 0) && (j + 1 < 8)) {//Check DOWN -> RIGHT Position:
					move->knightMoves[i][j][k] = board->boardSpaces[i - 2][j + 1].boardposition;
					k++;
				}//endif			
				break;

		case 4:	if ((i - 2 >= 0) && (j - 1 >= 0)) {//Check DOWN -> LEFT Position:
					move->knightMoves[i][j][k] = board->boardSpaces[i - 2][j - 1].boardposition;
					k++;
				}//endif			
				break;

		case 5:	if ((i - 1 >= 0) && (j - 2 >= 0)) {//Check LEFT -> DOWN Position:
				move->knightMoves[i][j][k] = board->boardSpaces[i - 1][j - 2].boardposition;
				k++;
				}//endif			
				break;
				
		case 6:	if ((i + 1 < 8) && (j - 2 >= 0)) {//Check LEFT -> UP Position:
				move->knightMoves[i][j][k] = board->boardSpaces[i + 1][j - 2].boardposition;
				k++;
				}//endif			
				break;
				
		case 7:	if ((i + 2 < 8) && (j - 1 >= 0)) {//Check UP -> LEFT Position:
				move->knightMoves[i][j][k] = board->boardSpaces[i + 2][j - 1].boardposition;
				k++;
				}//endif			
				break;

		default:break;
		}//endSwitch
	}//endFor
}//setKnightMoves

//Summary: Calculates each possible movement a rook can move given a starting space.
//Stores the destination board positions as a result. 
//Assumes the Board is empty so this result must be compared to the current boardstate
//before making decisions.
void setRookMoves(Board *board, int i, int j, Move *move)
{
	int x, y, k;			
	for (k = 0; k < 14; k++) {
		move->rookMoves[i][j][k] = 0; //Initialize array to 0 
	}//End FOR initialization
	k = 0;
	//Calculating horizontal +X direction (RIGHT)
	for(x = j + 1; x < 8; x++) {
		move->rookMoves[i][j][k] = board->boardSpaces[i][x].boardposition; //Sets the boardPosition for the valid move:
		k++;
	}//end FOR +X RIGHT	
	 //Calculating horizontal -X direction LEFT	
	for (x = j - 1; x >= 0; x--) {
		move->rookMoves[i][j][k] = board->boardSpaces[i][x].boardposition; //Sets the boardPosition for the valid move:
		k++;
	}//end FOR -X LEFT
	 //Calculating vertical +y direction: UP	
	for (y = i + 1; y < 8; y++) {
		move->rookMoves[i][j][k] = board->boardSpaces[y][j].boardposition;
		k++;
	}//end FOR +Y

	 //Calculating vertical -y direction: Down
	for (y = i - 1; y >= 0; y--) {
		move->rookMoves[i][j][k] = board->boardSpaces[y][j].boardposition;
		k++;
	}//end FOR -Y
}//verticalHorizontal_move


//Summary: Passes the board and arguments to preconfigure the board movement.
//All movement in this board is assumed to be on an empty board and must be compared
//to the present board state to determine obstacle collisions.
void setMoves(Board *board, Move *move, MoveGen *movegen, MoveGen *movehistory) {
	int x = 0;
	int y = 0;

	for (int i = 0; i <= 100; i++) {		
		//Initialize MoveGen List to -1:
		movegen->Moves[i].piece = -1;
		movegen->Moves[i].startLocation = -1;
		movegen->Moves[i].endLocation = -1;
		movegen->Moves[i].capturedPiece = -1;
		//Initialize MoveHistory list to -1:
		movehistory->Moves[i].piece = -1;
		movehistory->Moves[i].startLocation = -1;
		movehistory->Moves[i].endLocation = -1;
		movehistory->Moves[i].capturedPiece = -1;
	}//endfor	
	movegen->count = 0;
	movehistory->count = 0;
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			//printf("boardInt = [%d]", boardInt);			
			setRookMoves(board, i, j, move);
			setKnightMoves(board, i, j, move);
			setBishopMoves(board, i, j, move);
			if (board->boardSpaces[i][j].isOccupied && 
			   ((board->boardSpaces[i][j].pieceType & WHITE) >= 8)) {
				//move->whiteMoves[x] = board->boardSpaces[i][j].boardposition;
				x++;
			}// end if
			else
			{
				//move->blackMoves[y] = board->boardSpaces[i][j].boardposition;
				y++;
			}//end else
			
			//TODO SET PAWN MOVES?
			//SET QUEEN MOVES
		}//end for j
	}//end for i	
}

//setMoves




