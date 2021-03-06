#include "stdafx.h"
#include "board.h"
#include "move.h"
#include "movegen.h"

extern volatile unsigned long long *zobrist;

//SUMMARY: Makes Move given the Movelist parameter and stores the MoveList
//into an array of Moves already done:
void makeMove(Board *board, MoveList move, MoveGen *moveHistory, Move *moveSpace)
{
	int StartCoordinates[2], EndCoordinates[2];
	int i, j, k, l;

	Addr_Conversion(move.startLocation, StartCoordinates);
	Addr_Conversion(move.endLocation, EndCoordinates);

	i = StartCoordinates[0];
	j = StartCoordinates[1];
	k = EndCoordinates[0];
	l = EndCoordinates[1];

	board->boardSpaces[i][j].isOccupied = NOT_OCCUPIED; //set Start Unoccupied
	board->boardSpaces[i][j].pieceType = EMPTY;		//set Start Piece EMPTY

	board->boardSpaces[k][l].isOccupied = IS_OCCUPIED; //Set endLocation OCCUPIED
	board->boardSpaces[k][l].pieceType = move.piece;	  //Set EndLocation's Piecetype

	moveHistory->Moves[moveHistory->count] = move; //Assigns the move to the move history
	moveHistory->count++;							//Appends move history

													//TODO: CASTLING:
	if ((move.capturedPiece >= 82) && (move.capturedPiece <= 85)) {
		switch (move.capturedPiece) {
		case (WHITE_CASTLE_KINGSIDE):	whiteCastleKingSide(board);		break;
		case (WHITE_CASTLE_QUEENSIDE):	whiteCastleQueenSide(board);	break;
		case (BLACK_CASTLE_KINGSIDE):	blackCastleKingSide(board);		break;
		case(BLACK_CASTLE_QUEENSIDE):	blackCastleQueenSide(board);	break;
		default: printf("CASTLE ERROR"); break;
		}//end switch
		board->PerftCastleCounter++;
	}//if castling

	if ((move.capturedPiece >= 32) && (move.capturedPiece <= 79)) {
		promotePawn(board, move);
		board->PerftPromotionCounter++;
	}//promote

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

	update_zobrist(move, zobrist);
	updateEPSquare(board, move);
	updateColorSpaces(board, move, moveSpace, 0);
	board->turn = ((board->turn == WHITE_TURN) ? BLACK_TURN : WHITE_TURN);
	if ((move.capturedPiece != NO_CAPTURE) && (move.capturedPiece != 32) && (move.capturedPiece != 38) && (move.capturedPiece != 44) &&
		(move.capturedPiece != 50) && (move.capturedPiece != 56) && (move.capturedPiece != 62) && (move.capturedPiece != 68) &&
		(move.capturedPiece != 74) && (move.capturedPiece != 82) && (move.capturedPiece != 83) && (move.capturedPiece != 84) && (move.capturedPiece != 85))//update to make sure castling and capture doesn't increment capture counter
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

	//IF CAPTURE, CASTLING, PROMOTION, or EP OCCURS:
	if (moveHistory->Moves[moveHistory->count - 1].capturedPiece != NO_CAPTURE) {
		if (moveHistory->Moves[moveHistory->count - 1].capturedPiece == EN_PASSANT) {
			//If EP Capture, Undo EP:
			if (moveHistory->Moves[moveHistory->count - 1].piece == WHITE_PAWN) {
				//if white pawn restore black pawn:
				board->boardSpaces[4][j].isOccupied = IS_OCCUPIED;
				board->boardSpaces[4][j].pieceType = BLACK_PAWN;
				board->boardSpaces[5][j].isOccupied = NOT_OCCUPIED;
				board->boardSpaces[5][j].pieceType = EMPTY;
			}//end if white piece moved
			else if (moveHistory->Moves[moveHistory->count - 1].piece == BLACK_PAWN) {
				board->boardSpaces[3][j].isOccupied = IS_OCCUPIED;
				board->boardSpaces[3][j].pieceType = WHITE_PAWN;
				board->boardSpaces[2][j].isOccupied = NOT_OCCUPIED;
				board->boardSpaces[2][j].pieceType = EMPTY;
			}//end else if black pawn moved:	

			board->boardSpaces[i][j].isOccupied = NOT_OCCUPIED;
			board->boardSpaces[i][j].pieceType = EMPTY;
		}// end if 
		 //IF CASTLING, UNDO CASTLING:
		else if ((moveHistory->Moves[moveHistory->count - 1].capturedPiece >= 82) && (moveHistory->Moves[moveHistory->count - 1].capturedPiece <= 85)) {
			switch (moveHistory->Moves[moveHistory->count - 1].capturedPiece) {
			case (WHITE_CASTLE_KINGSIDE):	unmakeWhiteCastleKingSide(board, moveHistory);	 break;
			case (WHITE_CASTLE_QUEENSIDE):	unmakeWhiteCastleQueenSide(board, moveHistory);	 break;
			case (BLACK_CASTLE_KINGSIDE):	unmakeBlackCastleKingSide(board, moveHistory);	 break;
			case(BLACK_CASTLE_QUEENSIDE):	unmakeBlackCastleQueenSide(board, moveHistory);	 break;
			default: printf("CASTLE ERROR"); break;
			}//end switch
		}//if castling, undo castling
		else {
			board->boardSpaces[i][j].isOccupied = IS_OCCUPIED;
			board->boardSpaces[i][j].pieceType = moveHistory->Moves[moveHistory->count - 1].capturedPiece;
		}//else normal capture, no EP:
		 //END EP UNDO

		 //If promotion occurred, undo promotion:
		if ((moveHistory->Moves[moveHistory->count - 1].capturedPiece >= 32) && (moveHistory->Moves[moveHistory->count - 1].capturedPiece <= 79)) {
			if ((moveHistory->Moves[moveHistory->count - 1].capturedPiece >= 32) && (moveHistory->Moves[moveHistory->count - 1].capturedPiece <= 55)) // WHITE PROMOTION
				demoteWhitePiece(board, moveHistory->Moves[moveHistory->count - 1]);

			else if ((moveHistory->Moves[moveHistory->count - 1].capturedPiece >= 56) && (moveHistory->Moves[moveHistory->count - 1].capturedPiece <= 79)) //BLACK PROMOTION
				demoteBlackPiece(board, moveHistory->Moves[moveHistory->count - 1]);
		}//end undo promotion

		 //TODO: UNDO CASTLING

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

	update_zobrist(moveHistory->Moves[moveHistory->count - 1], zobrist);
	//END UPDATE EP	
	updateColorSpaces(board, moveHistory->Moves[moveHistory->count - 1], moveSpace, 1);

	if ((moveHistory->Moves[moveHistory->count - 1].piece == WHITE_KING) ||
		(moveHistory->Moves[moveHistory->count - 1].piece == BLACK_KING))
		updateKingCoordinates(board, moveHistory->Moves[moveHistory->count - 1].piece, k, l);
	if (moveHistory->count > 1) {
		if ((moveHistory->Moves[moveHistory->count - 1].piece == WHITE_KING) ||
			moveHistory->Moves[moveHistory->count - 1].piece == WHITE_ROOK)
			restoreWhiteCastlingRights(board, moveHistory);

		if ((moveHistory->Moves[moveHistory->count - 1].piece == WHITE_KING) ||
			moveHistory->Moves[moveHistory->count - 1].piece == BLACK_ROOK)
			restoreBlackCastlingRights(board, moveHistory);
	}//if more than 1 more. If this is the 0 depth we must restore to full
	else
		board->castlingRights = 0xF;

	moveHistory->Moves[moveHistory->count].capturedPiece = -1;
	moveHistory->Moves[moveHistory->count].startLocation = -1;
	moveHistory->Moves[moveHistory->count].endLocation = -1;
	moveHistory->Moves[moveHistory->count].piece = -1;
	moveHistory->count--;
	//TODO, RESTORE CASTLING RIGHTS/ EN PASSANT:		
}//UnMakeMove

 //Summary: updates colorspaces for movegeneration for the next set. Allows for easier calculations of which piece belongs to both sides:
void updateColorSpaces(Board *board, MoveList  move, Move *moveSpace, int undo)
{
	//TODO: UPDATE THE CASTLING SPACE WHENEVER A CASTLE GOES OFF:
	for (int i = 0; i < 16; i++) {
		if (board->turn == WHITE_TURN) {
			if (move.startLocation == moveSpace->whiteSpaces[i][BOARD_POSITION]) {
				moveSpace->whiteSpaces[i][BOARD_POSITION] = move.endLocation;	//updates on makemove()
				if ((move.capturedPiece >= 32) && (move.capturedPiece <= 55))
					promoteWhiteSpace(moveSpace, move.capturedPiece, i);		//White Pawn Promotion
				if (move.piece == WHITE_KING) {
					if ((move.capturedPiece == WHITE_CASTLE_KINGSIDE) || (move.capturedPiece == WHITE_CASTLE_QUEENSIDE))
						updateWhiteCastleSpace(board, moveSpace, move.capturedPiece);					
				}//if castling occurs, undo castling
			}//updates on MakeMove()
			else if (move.endLocation == moveSpace->whiteSpaces[i][BOARD_POSITION]) {
				moveSpace->whiteSpaces[i][BOARD_POSITION] = move.startLocation; //updates on unmakemove()
				if ((move.capturedPiece >= 32) && (move.capturedPiece <= 55))
					moveSpace->whiteSpaces[i][PIECE_TYPE] = WHITE_PAWN;
				if (move.piece == WHITE_KING) {
					if ((move.capturedPiece == WHITE_CASTLE_KINGSIDE) || (move.capturedPiece == WHITE_CASTLE_QUEENSIDE))
						undoWhiteCastleSpace(board, moveSpace, move.capturedPiece);
				}//undo castling

			}// if end location matches:
		}//white turn
		else if (board->turn == BLACK_TURN) {
			if (move.startLocation == moveSpace->blackSpaces[i][BOARD_POSITION]) {
				moveSpace->blackSpaces[i][BOARD_POSITION] = move.endLocation;
				if ((move.capturedPiece >= 56) && (move.capturedPiece <= 79))
					promoteBlackSpace(moveSpace, move.capturedPiece, i);	//Promotion of black piece	

				if (move.piece == BLACK_KING) {
					if ((move.capturedPiece == BLACK_CASTLE_KINGSIDE) || (move.capturedPiece == BLACK_CASTLE_QUEENSIDE))
						updateBlackCastleSpace(board, moveSpace, move.capturedPiece);
				}//undo castling if possible
			}//updates on MakeMove()
			else if (move.endLocation == moveSpace->blackSpaces[i][BOARD_POSITION]) {
				moveSpace->blackSpaces[i][BOARD_POSITION] = move.startLocation; //used for undoing moves
				if ((move.capturedPiece >= 56) && (move.capturedPiece <= 79))
					moveSpace->blackSpaces[i][PIECE_TYPE] = BLACK_PAWN;

				if (move.piece == BLACK_KING) {
					if ((move.capturedPiece == BLACK_CASTLE_KINGSIDE) || (move.capturedPiece == BLACK_CASTLE_QUEENSIDE))
						undoBlackCastleSpace(board, moveSpace, move.capturedPiece);
				}//undo castling if possible
			}
		}//black turn
		if (move.capturedPiece != NO_CAPTURE) {
			if (board->turn == WHITE_TURN) {
				if (move.endLocation == moveSpace->blackSpaces[i][BOARD_POSITION]) {
					if (undo == 1) {
						if ((move.capturedPiece >= 32) && (move.capturedPiece <= 56))
							demoteWhiteSpace(moveSpace, move, i);
						else
							moveSpace->blackSpaces[i][PIECE_TYPE] = move.capturedPiece;
					}// if undo == 1
					else
						moveSpace->blackSpaces[i][PIECE_TYPE] = -1;
					//moveSpace->blackSpaces[i][BOARD_POSITION] = -1;
				}//endif						

			}//endif white turn:
			else { //Black Turn capturing white piece
				if (move.endLocation == moveSpace->whiteSpaces[i][BOARD_POSITION]) {
					if (undo == 1) {
						if ((move.capturedPiece >= 56) && (move.capturedPiece <= 79))
							demoteBlackSpace(moveSpace, move, i);
						else
							moveSpace->whiteSpaces[i][PIECE_TYPE] = move.capturedPiece;
					}// if undo == 1
					else
						moveSpace->whiteSpaces[i][PIECE_TYPE] = -1;
					//moveSpace->whiteSpaces[i][BOARD_POSITION] = -1;
				}//end if
			}//end else
			if (move.capturedPiece >= 82 && move.capturedPiece <= 85)
				updateCastleColor(board, move, moveSpace, undo);

		}//end if no capture		
	}//end for 			
}//updateColorSpaces

void updateCastleColor(Board * board, MoveList move, Move * moveSpace, int undo)
{
	int kingflag, rookflag;
	kingflag = 0;
	rookflag = 0;
	switch (move.capturedPiece) {
	case(WHITE_CASTLE_KINGSIDE): 
			 
			for (int i = 0; i < 16; i++) {
				if (kingflag && rookflag)
					break;
				if (undo == 1) {
					if ((moveSpace->whiteSpaces[i][BOARD_POSITION] == 5) && moveSpace->whiteSpaces[i][PIECE_TYPE] == WHITE_ROOK) {
						moveSpace->whiteSpaces[i][BOARD_POSITION] = 7;
						rookflag = 1;
					} // end if 
					else if ((moveSpace->whiteSpaces[i][BOARD_POSITION] == 6) && (moveSpace->whiteSpaces[i][PIECE_TYPE] == WHITE_KING)) {
						moveSpace->whiteSpaces[i][BOARD_POSITION] = 4;
						kingflag = 1;
					}// end else if 
				}// end if undo
				
				else {
					if ((moveSpace->whiteSpaces[i][BOARD_POSITION] == 7) && moveSpace->whiteSpaces[i][PIECE_TYPE] == WHITE_ROOK) {
						moveSpace->whiteSpaces[i][BOARD_POSITION] = 5;
						rookflag = 1;
					}// end if 
					else if ((moveSpace->whiteSpaces[i][BOARD_POSITION] == 4) && (moveSpace->whiteSpaces[i][PIECE_TYPE] == WHITE_KING)) {
						moveSpace->whiteSpaces[i][BOARD_POSITION] = 6;
						kingflag = 1;
					}// end else
				} // end else not undo
			}// end for loop
			
			break;
	case(WHITE_CASTLE_QUEENSIDE):

			for (int i = 0; i < 16; i++) {
				if (kingflag && rookflag)
					break;
				if (undo == 1) {
					if ((moveSpace->whiteSpaces[i][BOARD_POSITION] == 3) && moveSpace->whiteSpaces[i][PIECE_TYPE] == WHITE_ROOK) {
						moveSpace->whiteSpaces[i][BOARD_POSITION] = 0;
						rookflag = 1;
					} // end if 
					else if ((moveSpace->whiteSpaces[i][BOARD_POSITION] == 2) && (moveSpace->whiteSpaces[i][PIECE_TYPE] == WHITE_KING)) {
						moveSpace->whiteSpaces[i][BOARD_POSITION] = 4;
						kingflag = 1;
					}// end else if 
				}// end if undo

				else {
					if ((moveSpace->whiteSpaces[i][BOARD_POSITION] == 0) && moveSpace->whiteSpaces[i][PIECE_TYPE] == WHITE_ROOK) {
						moveSpace->whiteSpaces[i][BOARD_POSITION] = 3;
						rookflag = 1;
					}// end if 
					else if ((moveSpace->whiteSpaces[i][BOARD_POSITION] == 4) && (moveSpace->whiteSpaces[i][PIECE_TYPE] == WHITE_KING)) {
						moveSpace->whiteSpaces[i][BOARD_POSITION] = 2;
						kingflag = 1;
					}// end else
				} // end else not undo
			}// end for loop
			break;

	case(BLACK_CASTLE_KINGSIDE):
			for (int i = 0; i < 16; i++) {
				if (kingflag && rookflag)
					break;
				if (undo == 1) {
					if ((moveSpace->blackSpaces[i][BOARD_POSITION] == 61) && moveSpace->blackSpaces[i][PIECE_TYPE] == BLACK_ROOK) {
						moveSpace->blackSpaces[i][BOARD_POSITION] = 63;
						rookflag = 1;
					} // end if 
					else if ((moveSpace->blackSpaces[i][BOARD_POSITION] == 62) && (moveSpace->blackSpaces[i][PIECE_TYPE] == BLACK_KING)) {
						moveSpace->blackSpaces[i][BOARD_POSITION] = 60;
						kingflag = 1;
					}// end else if 
				}// end if undo

				else {
					if ((moveSpace->blackSpaces[i][BOARD_POSITION] == 63) && moveSpace->blackSpaces[i][PIECE_TYPE] == BLACK_ROOK) {
						moveSpace->blackSpaces[i][BOARD_POSITION] = 61;
						rookflag = 1;
					}// end if 
					else if ((moveSpace->blackSpaces[i][BOARD_POSITION] == 60) && (moveSpace->blackSpaces[i][PIECE_TYPE] == BLACK_KING)) {
						moveSpace->blackSpaces[i][BOARD_POSITION] = 62;
						kingflag = 1;
					}// end else
				} // end else not undo
			}// end for loop
			break;

	case(BLACK_CASTLE_QUEENSIDE):
			for (int i = 0; i < 16; i++) {
				if (kingflag && rookflag)
					break;
				if (undo == 1) {
					if ((moveSpace->blackSpaces[i][BOARD_POSITION] == 59) && moveSpace->blackSpaces[i][PIECE_TYPE] == BLACK_ROOK) {
						moveSpace->blackSpaces[i][BOARD_POSITION] = 56;
						rookflag = 1;
					} // end if 
					else if ((moveSpace->blackSpaces[i][BOARD_POSITION] == 58) && (moveSpace->blackSpaces[i][PIECE_TYPE] == BLACK_KING)) {
						moveSpace->blackSpaces[i][BOARD_POSITION] = 60;
						kingflag = 1;
					}// end else if 
				}// end if undo

				else {
					if ((moveSpace->blackSpaces[i][BOARD_POSITION] == 56) && moveSpace->blackSpaces[i][PIECE_TYPE] == BLACK_ROOK) {
						moveSpace->blackSpaces[i][BOARD_POSITION] = 59;
						rookflag = 1;
					}// end if 
					else if ((moveSpace->blackSpaces[i][BOARD_POSITION] == 60) && (moveSpace->blackSpaces[i][PIECE_TYPE] == BLACK_KING)) {
						moveSpace->blackSpaces[i][BOARD_POSITION] = 58;
						kingflag = 1;
					}// end else
				} // end else not undo
			}// end for loop
			break;

	default: break;
	}

} // update Castle Color

 //MakeMove

 //Summary: Updates the whitespace piece type during pawn promotion: Updates the piecetype value to the proper promotion.
void promoteWhiteSpace(Move *moveSpace, char promotedPiece, int i) {
	if ((promotedPiece >= WHITE_PROMOTE_QUEEN_NO_CAPTURE) && (promotedPiece <= WHITE_PROMOTE_QUEEN_CAPTURE_QUEEN))
		moveSpace->whiteSpaces[i][PIECE_TYPE] = WHITE_QUEEN;

	else if ((promotedPiece >= WHITE_PROMOTE_ROOK_NO_CAPTURE) && (promotedPiece <= WHITE_PROMOTE_ROOK_CAPTURE_QUEEN))
		moveSpace->whiteSpaces[i][PIECE_TYPE] = WHITE_ROOK;

	else if ((promotedPiece >= WHITE_PROMOTE_BISHOP_NO_CAPTURE) && (promotedPiece <= WHITE_PROMOTE_BISHOP_CAPTURE_QUEEN))
		moveSpace->whiteSpaces[i][PIECE_TYPE] = WHITE_BISHOP;

	else if ((promotedPiece >= WHITE_PROMOTE_KNIGHT_NO_CAPTURE) && (promotedPiece <= WHITE_PROMOTE_KNIGHT_CAPTURE_QUEEN))
		moveSpace->whiteSpaces[i][PIECE_TYPE] = WHITE_KNIGHT;
}//promoteWhiteSpace

 //Summary: Updates the blackspace piece type during pawn promotion: Updates the piecetype value to the proper promotion.
void promoteBlackSpace(Move *moveSpace, char promotedPiece, int i) {
	if ((promotedPiece >= BLACK_PROMOTE_QUEEN_NO_CAPTURE) && (promotedPiece <= BLACK_PROMOTE_QUEEN_CAPTURE_QUEEN))
		moveSpace->blackSpaces[i][PIECE_TYPE] = BLACK_QUEEN;

	else if ((promotedPiece >= BLACK_PROMOTE_ROOK_NO_CAPTURE) && (promotedPiece <= BLACK_PROMOTE_ROOK_CAPTURE_QUEEN))
		moveSpace->blackSpaces[i][PIECE_TYPE] = BLACK_ROOK;

	else if ((promotedPiece >= BLACK_PROMOTE_BISHOP_NO_CAPTURE) && (promotedPiece <= BLACK_PROMOTE_BISHOP_CAPTURE_QUEEN))
		moveSpace->blackSpaces[i][PIECE_TYPE] = BLACK_BISHOP;

	else if ((promotedPiece >= BLACK_PROMOTE_KNIGHT_NO_CAPTURE) && (promotedPiece <= BLACK_PROMOTE_KNIGHT_CAPTURE_QUEEN))
		moveSpace->blackSpaces[i][PIECE_TYPE] = BLACK_KNIGHT;
}//promoteBlackvSpace

//updating board space after castling
void updateWhiteCastleSpace(Board *board, Move *moveSpace, char side) {
	if (side == WHITE_CASTLE_KINGSIDE) {
		for (int i = 0; i < 16; i++) {
			if ((moveSpace->whiteSpaces[i][PIECE_TYPE] == WHITE_ROOK) && (moveSpace->whiteSpaces[i][BOARD_POSITION] == 7)) {
				moveSpace->whiteSpaces[i][BOARD_POSITION] = 5;
				break;
			}//if found the corresponding rook;
		}//end for 
	}//if white castle kingside

	else if (side == WHITE_CASTLE_QUEENSIDE) {
		for (int i = 0; i < 16; i++) {
			if ((moveSpace->whiteSpaces[i][PIECE_TYPE] == WHITE_ROOK) && (moveSpace->whiteSpaces[i][BOARD_POSITION] == 0)) {
				moveSpace->whiteSpaces[i][BOARD_POSITION] = 3;
			}//if found the corresponding rook
		}//end for
	}//end else queenside castling
}//updateWhiteCastleSpace

void updateBlackCastleSpace(Board *board, Move *moveSpace, char side) {
	if (side == BLACK_CASTLE_KINGSIDE) {
		for (int i = 0; i < 16; i++) {
			if ((moveSpace->blackSpaces[i][PIECE_TYPE] == BLACK_ROOK) && (moveSpace->blackSpaces[i][BOARD_POSITION] == 63)) {
				moveSpace->blackSpaces[i][BOARD_POSITION] = 61;
				break;
			}//if found the corresponding rook;
		}//end for 
	}//if white castle kingside

	else if (side == BLACK_CASTLE_QUEENSIDE) {
		for (int i = 0; i < 16; i++) {
			if ((moveSpace->blackSpaces[i][PIECE_TYPE] == BLACK_ROOK) && (moveSpace->blackSpaces[i][BOARD_POSITION] == 56)) {
				moveSpace->blackSpaces[i][BOARD_POSITION] = 59;
			}//if found the corresponding rook
		}//end for
	}//end else queenside castling
}//updateBlackCastleSpace

//Summary: Reverts whitespace positions for rook after castling is undone.
void undoWhiteCastleSpace(Board *board, Move *moveSpace, char side) {
	if (side == WHITE_CASTLE_KINGSIDE) {
		for (int i = 0; i < 16; i++) {
			if ((moveSpace->whiteSpaces[i][PIECE_TYPE] == WHITE_ROOK) && (moveSpace->whiteSpaces[i][BOARD_POSITION] == 5)) {
				moveSpace->whiteSpaces[i][BOARD_POSITION] = 7;
				break;
			}//end if 
		}//end for
	}//end if white_castle_Kingside

	else if (side == WHITE_CASTLE_QUEENSIDE) {
		for (int i = 0; i < 16; i++) {
			if ((moveSpace->whiteSpaces[i][PIECE_TYPE] == WHITE_ROOK) && (moveSpace->whiteSpaces[i][BOARD_POSITION] == 3)) {
				moveSpace->whiteSpaces[i][BOARD_POSITION] = 0;
				break;
			}//end if 
		}//end for
	}//end whiteCastleQueenside
}//undoWhiteCastleSpace

 //Summary: Reverts blackspace positions for rook after castling is undone.
void undoBlackCastleSpace(Board *board, Move *moveSpace, char side) {
	if (side == BLACK_CASTLE_KINGSIDE) {
		for (int i = 0; i < 16; i++) {
			if ((moveSpace->blackSpaces[i][PIECE_TYPE] == BLACK_ROOK) && (moveSpace->blackSpaces[i][BOARD_POSITION] == 61)) {
				moveSpace->blackSpaces[i][BOARD_POSITION] = 63;
				break;
			}//end if 
		}//end for
	}//end if white_castle_Kingside

	else if (side == BLACK_CASTLE_QUEENSIDE) {
		for (int i = 0; i < 16; i++) {
			if ((moveSpace->blackSpaces[i][PIECE_TYPE] == BLACK_ROOK) && (moveSpace->blackSpaces[i][BOARD_POSITION] == 59)) {
				moveSpace->blackSpaces[i][BOARD_POSITION] = 56;
				break;
			}//end if 
		}//end for
	}//end whiteCastleQueenside
}//undoBlackCastleSpace
//end updating boardspace after castling



 //Summary: Demotes White Piece and updates board accordingly
void demoteWhitePiece(Board *board, MoveList move) {
	int startFile, endFile;
	startFile = move.startLocation % 8;
	endFile = move.endLocation % 8;
	if (move.capturedPiece == WHITE_PROMOTE_QUEEN_NO_CAPTURE || move.capturedPiece == WHITE_PROMOTE_ROOK_NO_CAPTURE ||
		move.capturedPiece == WHITE_PROMOTE_BISHOP_NO_CAPTURE || move.capturedPiece == WHITE_PROMOTE_KNIGHT_NO_CAPTURE) {
		board->boardSpaces[7][endFile].isOccupied = NOT_OCCUPIED;
		board->boardSpaces[7][endFile].pieceType = EMPTY;
		board->boardSpaces[6][startFile].isOccupied = IS_OCCUPIED;
		board->boardSpaces[6][startFile].pieceType = WHITE_PAWN;
	}// if no capture (pawn moved forward)
	else { //if a piece was captured:
		if (move.capturedPiece == WHITE_PROMOTE_QUEEN_CAPTURE_QUEEN || move.capturedPiece == WHITE_PROMOTE_ROOK_CAPTURE_QUEEN ||
			move.capturedPiece == WHITE_PROMOTE_BISHOP_CAPTURE_QUEEN || move.capturedPiece == WHITE_PROMOTE_KNIGHT_CAPTURE_QUEEN) {
			board->boardSpaces[7][endFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[7][endFile].pieceType = BLACK_QUEEN;
			board->boardSpaces[6][startFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[6][startFile].pieceType = WHITE_PAWN;
		}//if captured queen

		else if (move.capturedPiece == WHITE_PROMOTE_QUEEN_CAPTURE_ROOK || move.capturedPiece == WHITE_PROMOTE_ROOK_CAPTURE_ROOK ||
			move.capturedPiece == WHITE_PROMOTE_BISHOP_CAPTURE_ROOK || move.capturedPiece == WHITE_PROMOTE_KNIGHT_CAPTURE_ROOK) {
			board->boardSpaces[7][endFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[7][endFile].pieceType = BLACK_ROOK;
			board->boardSpaces[6][startFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[6][startFile].pieceType = WHITE_PAWN;
		}//if captured rook

		else if (move.capturedPiece == WHITE_PROMOTE_QUEEN_CAPTURE_BISHOP || move.capturedPiece == WHITE_PROMOTE_ROOK_CAPTURE_BISHOP ||
			move.capturedPiece == WHITE_PROMOTE_BISHOP_CAPTURE_BISHOP || move.capturedPiece == WHITE_PROMOTE_KNIGHT_CAPTURE_BISHOP) {
			board->boardSpaces[7][endFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[7][endFile].pieceType = BLACK_BISHOP;
			board->boardSpaces[6][startFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[6][startFile].pieceType = WHITE_PAWN;
		}//if captured bishop

		else if (move.capturedPiece == WHITE_PROMOTE_QUEEN_CAPTURE_KNIGHT || move.capturedPiece == WHITE_PROMOTE_ROOK_CAPTURE_KNIGHT ||
			move.capturedPiece == WHITE_PROMOTE_BISHOP_CAPTURE_KNIGHT || move.capturedPiece == WHITE_PROMOTE_KNIGHT_CAPTURE_KNIGHT) {
			board->boardSpaces[7][endFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[7][endFile].pieceType = BLACK_KNIGHT;
			board->boardSpaces[6][startFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[6][startFile].pieceType = WHITE_PAWN;
		}//if captured rook
		 //if captured knight
	}//end else if piece was captured:
}//demoteWhitePiece

 //Summary: Demotes Black Piece and updates board accordingly
void demoteBlackPiece(Board *board, MoveList move) {
	int startFile, endFile;
	startFile = move.startLocation % 8;
	endFile = move.endLocation % 8;
	if (move.capturedPiece == BLACK_PROMOTE_QUEEN_NO_CAPTURE || move.capturedPiece == BLACK_PROMOTE_ROOK_NO_CAPTURE ||
		move.capturedPiece == BLACK_PROMOTE_BISHOP_NO_CAPTURE || move.capturedPiece == BLACK_PROMOTE_KNIGHT_NO_CAPTURE) {
		board->boardSpaces[0][endFile].isOccupied = NOT_OCCUPIED;
		board->boardSpaces[0][endFile].pieceType = EMPTY;
		board->boardSpaces[1][startFile].isOccupied = IS_OCCUPIED;
		board->boardSpaces[1][startFile].pieceType = BLACK_PAWN;
	}// if no capture (pawn moved forward)
	else { //if a piece was captured:
		if (move.capturedPiece == BLACK_PROMOTE_QUEEN_CAPTURE_QUEEN || move.capturedPiece == BLACK_PROMOTE_ROOK_CAPTURE_QUEEN ||
			move.capturedPiece == BLACK_PROMOTE_BISHOP_CAPTURE_QUEEN || move.capturedPiece == BLACK_PROMOTE_KNIGHT_CAPTURE_QUEEN) {
			board->boardSpaces[0][endFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[0][endFile].pieceType = WHITE_QUEEN;
			board->boardSpaces[1][startFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[1][startFile].pieceType = BLACK_PAWN;
		}//if captured queen

		else if (move.capturedPiece == BLACK_PROMOTE_QUEEN_CAPTURE_ROOK || move.capturedPiece == BLACK_PROMOTE_ROOK_CAPTURE_ROOK ||
			move.capturedPiece == BLACK_PROMOTE_BISHOP_CAPTURE_ROOK || move.capturedPiece == BLACK_PROMOTE_KNIGHT_CAPTURE_ROOK) {
			board->boardSpaces[0][endFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[0][endFile].pieceType = WHITE_ROOK;
			board->boardSpaces[1][startFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[1][startFile].pieceType = BLACK_PAWN;
		}//if captured rook

		else if (move.capturedPiece == BLACK_PROMOTE_QUEEN_CAPTURE_BISHOP || move.capturedPiece == BLACK_PROMOTE_ROOK_CAPTURE_BISHOP ||
			move.capturedPiece == BLACK_PROMOTE_BISHOP_CAPTURE_BISHOP || move.capturedPiece == BLACK_PROMOTE_KNIGHT_CAPTURE_BISHOP) {
			board->boardSpaces[0][endFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[0][endFile].pieceType = WHITE_BISHOP;
			board->boardSpaces[1][startFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[1][startFile].pieceType = BLACK_PAWN;
		}//if captured bishop

		else if (move.capturedPiece == BLACK_PROMOTE_QUEEN_CAPTURE_KNIGHT || move.capturedPiece == BLACK_PROMOTE_ROOK_CAPTURE_KNIGHT ||
			move.capturedPiece == BLACK_PROMOTE_BISHOP_CAPTURE_KNIGHT || move.capturedPiece == BLACK_PROMOTE_KNIGHT_CAPTURE_KNIGHT) {
			board->boardSpaces[0][endFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[0][endFile].pieceType = WHITE_KNIGHT;
			board->boardSpaces[1][startFile].isOccupied = IS_OCCUPIED;
			board->boardSpaces[1][startFile].pieceType = BLACK_PAWN;
		}//if captured rook
		 //if captured knight
	}//end else if piece was captured:
}//demote black PIECE

 //Summary: Recovers piece in the black space that was captured by a white promotion
void demoteWhiteSpace(Move *moveSpace, MoveList move, int i) {
	int startFile, endFile;
	startFile = move.startLocation % 8;
	endFile = move.endLocation % 8;

	if ((move.capturedPiece == 37) || (move.capturedPiece == 43) ||
		(move.capturedPiece == 49) || (move.capturedPiece == 55))
		moveSpace->blackSpaces[i][PIECE_TYPE] = BLACK_QUEEN;
	//If black queen was captured

	else if ((move.capturedPiece == 34) || (move.capturedPiece == 40) ||
		(move.capturedPiece == 46) || (move.capturedPiece == 52))
		moveSpace->blackSpaces[i][PIECE_TYPE] = BLACK_ROOK;
	//if rook captured

	else if ((move.capturedPiece == 36) || (move.capturedPiece == 42) ||
		(move.capturedPiece == 48) || (move.capturedPiece == 54))
		moveSpace->blackSpaces[i][PIECE_TYPE] = BLACK_BISHOP;
	//if bishop 

	else if ((move.capturedPiece == 35) || (move.capturedPiece == 41) ||
		(move.capturedPiece == 47) || (move.capturedPiece == 53))
		moveSpace->blackSpaces[i][PIECE_TYPE] = BLACK_KNIGHT;
	//if knight captured
}//demoteWhiteSpace



 //Summary: Recovers piece in the white spaces that was captured by a white promotion
void demoteBlackSpace(Move *moveSpace, MoveList move, int i) {
	int startFile, endFile;
	startFile = move.startLocation % 8;
	endFile = move.endLocation % 8;

	if ((move.capturedPiece == 61) || (move.capturedPiece == 67) ||
		(move.capturedPiece == 73) || (move.capturedPiece == 79))
		moveSpace->whiteSpaces[i][PIECE_TYPE] = WHITE_QUEEN;
	//If black queen was captured

	else if ((move.capturedPiece == 58) || (move.capturedPiece == 64) ||
		(move.capturedPiece == 70) || (move.capturedPiece == 76))
		moveSpace->whiteSpaces[i][PIECE_TYPE] = WHITE_ROOK;
	//if rook captured

	else if ((move.capturedPiece == 60) || (move.capturedPiece == 66) ||
		(move.capturedPiece == 72) || (move.capturedPiece == 78))
		moveSpace->whiteSpaces[i][PIECE_TYPE] = WHITE_BISHOP;
	//if bishop 

	else if ((move.capturedPiece == 59) || (move.capturedPiece == 65) ||
		(move.capturedPiece == 71) || (move.capturedPiece == 77))
		moveSpace->whiteSpaces[i][PIECE_TYPE] = WHITE_KNIGHT;
	//if knight captured
}//demoteBlackSpace




 //Summary: Updates King Coordinates for Black and white kings:
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


void clearMoveList(MoveList * movelist)
{
	movelist->piece = -1;
	movelist->startLocation = -1;
	movelist->endLocation = -1;
	movelist->capturedPiece = -1;
}//clearMoveList





 //Summary: Checks if current move moved pawn, if it did then check if moved 2 places. then update the EP Square
 //If not, then set EP square to default value
void updateEPSquare(Board *board, MoveList move) {
	int start, end, file;
	start = move.startLocation / 8;
	end = move.endLocation / 8;
	file = move.startLocation % 8;
	if ((move.piece == WHITE_PAWN || move.piece == BLACK_PAWN) && ((start == 1 && end == 3) || (start == 6 && end == 4))) {
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
		(((start == 1) && (end == 3)) || ((start == 6) && (end == 4)))) {
		if (prevmove.piece == WHITE_PAWN)
			board->epSquare = board->boardSpaces[2][file].boardposition;
		else if (prevmove.piece == BLACK_PAWN)
			board->epSquare = board->boardSpaces[5][file].boardposition;
	}//end else if 


}//updatePrevEPSquare

 //Summary: Promotes Pawn to the proper code based on which turn it is.
 //Need to create an unpromote pawn function for unmake move.
 //This function is called when a move is made:
void promotePawn(Board *board, MoveList move) {
	int file = move.endLocation % 8; //horizontal position	
	if (board->turn == WHITE_TURN) {
		board->boardSpaces[7][file].isOccupied = IS_OCCUPIED;
		if ((move.capturedPiece >= WHITE_PROMOTE_QUEEN_NO_CAPTURE) && (move.capturedPiece <= WHITE_PROMOTE_QUEEN_CAPTURE_QUEEN))
			board->boardSpaces[7][file].pieceType = WHITE_QUEEN;

		else if ((move.capturedPiece >= WHITE_PROMOTE_ROOK_NO_CAPTURE) && (move.capturedPiece <= WHITE_PROMOTE_ROOK_CAPTURE_QUEEN))
			board->boardSpaces[7][file].pieceType = WHITE_ROOK;

		else if ((move.capturedPiece >= WHITE_PROMOTE_BISHOP_NO_CAPTURE) && (move.capturedPiece <= WHITE_PROMOTE_BISHOP_CAPTURE_QUEEN))
			board->boardSpaces[7][file].pieceType = WHITE_BISHOP;

		else if ((move.capturedPiece >= WHITE_PROMOTE_KNIGHT_NO_CAPTURE) && (move.capturedPiece <= WHITE_PROMOTE_KNIGHT_CAPTURE_QUEEN))
			board->boardSpaces[7][file].pieceType = WHITE_KNIGHT;
	}// end if White turn:

	else if (board->turn == BLACK_TURN) {
		board->boardSpaces[0][file].isOccupied = IS_OCCUPIED;

		if ((move.capturedPiece >= BLACK_PROMOTE_QUEEN_NO_CAPTURE) && (move.capturedPiece <= BLACK_PROMOTE_QUEEN_CAPTURE_QUEEN))
			board->boardSpaces[0][file].pieceType = BLACK_QUEEN;

		else if ((move.capturedPiece >= BLACK_PROMOTE_ROOK_NO_CAPTURE) && (move.capturedPiece <= BLACK_PROMOTE_ROOK_CAPTURE_QUEEN))
			board->boardSpaces[0][file].pieceType = BLACK_ROOK;

		else if ((move.capturedPiece >= BLACK_PROMOTE_BISHOP_NO_CAPTURE) && (move.capturedPiece <= BLACK_PROMOTE_BISHOP_CAPTURE_QUEEN))
			board->boardSpaces[0][file].pieceType = BLACK_BISHOP;

		else if ((move.capturedPiece >= BLACK_PROMOTE_KNIGHT_NO_CAPTURE) && (move.capturedPiece <= BLACK_PROMOTE_KNIGHT_CAPTURE_QUEEN))
			board->boardSpaces[0][file].pieceType = BLACK_KNIGHT;
	}// end if black turn
}//promotePawn

 //CASTLING
void whiteCastleKingSide(Board *board) {
	//set pieces starting locations empty.
	board->boardSpaces[0][4].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[0][4].pieceType = EMPTY;
	board->boardSpaces[0][7].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[0][7].isOccupied = EMPTY;
	//set new locations
	board->boardSpaces[0][6].isOccupied = IS_OCCUPIED;
	board->boardSpaces[0][6].pieceType = WHITE_KING;
	board->boardSpaces[0][5].isOccupied = IS_OCCUPIED;
	board->boardSpaces[0][5].pieceType = WHITE_ROOK;
	//update King Coordinates
	updateKingCoordinates(board, WHITE_KING, 0, 6);
	//change castling rights
	board->castlingRights -= (board->castlingRights & 0xB); //-(1100) (12)
}//whiteCastleKingSide

void whiteCastleQueenSide(Board *board) {
	//set pieces starting locations empty.
	board->boardSpaces[0][4].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[0][4].pieceType = EMPTY;
	board->boardSpaces[0][0].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[0][0].isOccupied = EMPTY;
	//set new locations
	board->boardSpaces[0][2].isOccupied = IS_OCCUPIED;
	board->boardSpaces[0][2].pieceType = WHITE_KING;
	board->boardSpaces[0][3].isOccupied = IS_OCCUPIED;
	board->boardSpaces[0][3].pieceType = WHITE_ROOK;
	//update King Coordinates
	updateKingCoordinates(board, WHITE_KING, 0, 2);
	//change castling rights
	board->castlingRights -= (board->castlingRights & 0xB); //-(1100)-( (12)| (8) | 4)
}//whiteCastleQueenSide

void blackCastleKingSide(Board *board) {
	//set pieces starting locations empty.
	board->boardSpaces[7][4].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[7][4].pieceType = EMPTY;
	board->boardSpaces[7][7].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[7][7].isOccupied = EMPTY;
	//set new locations
	board->boardSpaces[7][6].isOccupied = IS_OCCUPIED;
	board->boardSpaces[7][6].pieceType = BLACK_KING;
	board->boardSpaces[7][5].isOccupied = IS_OCCUPIED;
	board->boardSpaces[7][5].pieceType = BLACK_ROOK;
	//update King Coordinates
	updateKingCoordinates(board, BLACK_KING, 7, 6);
	//change castling rights
	board->castlingRights -= (board->castlingRights & 0x3); //-(0011) -((3) |(2) |(1)) 
}//blackCastleKingSide

void blackCastleQueenSide(Board *board) {
	//set pieces starting locations empty.
	board->boardSpaces[7][4].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[7][4].pieceType = EMPTY;
	board->boardSpaces[7][7].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[7][7].isOccupied = EMPTY;
	//set new locations
	board->boardSpaces[7][2].isOccupied = IS_OCCUPIED;
	board->boardSpaces[7][2].pieceType = BLACK_KING;
	board->boardSpaces[7][3].isOccupied = IS_OCCUPIED;
	board->boardSpaces[7][3].pieceType = BLACK_ROOK;
	//update King Coordinates
	updateKingCoordinates(board, BLACK_KING, 7, 2);
	//change castling rights
	board->castlingRights -= (board->castlingRights & 0x3); //-(0011) -((3) |(2) |(1)) 
}//blackCastleQueenSide
 //END CASTLING:

 //START UNMAKE CASTLING:
void unmakeWhiteCastleKingSide(Board *board) {
	//set pieces starting locations empty.
	board->boardSpaces[0][6].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[0][6].pieceType = EMPTY;
	board->boardSpaces[0][5].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[0][5].isOccupied = EMPTY;
	//set new locations
	board->boardSpaces[0][4].isOccupied = IS_OCCUPIED;
	board->boardSpaces[0][4].pieceType = WHITE_KING;
	board->boardSpaces[0][7].isOccupied = IS_OCCUPIED;
	board->boardSpaces[0][7].pieceType = WHITE_ROOK;
	//update King Coordinates
	updateKingCoordinates(board, WHITE_KING, 0, 4);
	restoreWhiteCastlingRights(board, moveHistory);
}//unmakeWhiteCastleKingSide

void unmakeWhiteCastleQueenSide(Board *board, MoveGen *moveHistory) {
	//set pieces starting locations empty.
	board->boardSpaces[0][2].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[0][2].pieceType = EMPTY;
	board->boardSpaces[0][3].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[0][3].isOccupied = EMPTY;
	//set new locations
	board->boardSpaces[0][4].isOccupied = IS_OCCUPIED;
	board->boardSpaces[0][4].pieceType = WHITE_KING;
	board->boardSpaces[0][0].isOccupied = IS_OCCUPIED;
	board->boardSpaces[0][0].pieceType = WHITE_ROOK;
	//update King Coordinates
	updateKingCoordinates(board, WHITE_KING, 0, 4);
	restoreWhiteCastlingRights(board, moveHistory);
}//unmakeWhiteCastleQueenside


void unmakeBlackCastleKingSide(Board *board, MoveGen *moveHistory) {
	//set pieces starting locations empty.
	board->boardSpaces[7][6].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[7][6].pieceType = EMPTY;
	board->boardSpaces[7][5].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[7][5].isOccupied = EMPTY;
	//set new locations
	board->boardSpaces[7][4].isOccupied = IS_OCCUPIED;
	board->boardSpaces[7][4].pieceType = BLACK_KING;
	board->boardSpaces[7][7].isOccupied = IS_OCCUPIED;
	board->boardSpaces[7][7].pieceType = BLACK_ROOK;
	//update King Coordinates
	updateKingCoordinates(board, BLACK_KING, 7, 4);
	restoreBlackCastlingRights(board, moveHistory);
}//unmakeBlackCastleKingSide


void unmakeBlackCastleQueenSide(Board *board, MoveGen *moveHistory) {
	//set pieces starting locations empty.
	board->boardSpaces[7][2].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[7][2].pieceType = EMPTY;
	board->boardSpaces[7][3].isOccupied = NOT_OCCUPIED;
	board->boardSpaces[7][3].isOccupied = EMPTY;
	//set new locations
	board->boardSpaces[7][4].isOccupied = IS_OCCUPIED;
	board->boardSpaces[7][4].pieceType = BLACK_KING;
	board->boardSpaces[7][0].isOccupied = IS_OCCUPIED;
	board->boardSpaces[7][0].pieceType = BLACK_ROOK;
	//update King Coordinates
	updateKingCoordinates(board, BLACK_KING, 7, 4);
	//change castling rights	
	restoreBlackCastlingRights(board, moveHistory);
}//unmakeBlackCastleQueenSide

 //END UNMAKE CASTLING

//Summary: Takes away castling rights when a white rook/king moves if necessary
void changeWhiteCastlingRights(Board *board, MoveList move) {
	//undo castling rights if rook/king moves:
	if ((move.piece == WHITE_ROOK) && ((board->castlingRights & 0xC) >= 4)) {
		if ((move.startLocation == 7) && ((board->castlingRights & 0x8) == 0x8))
			board->castlingRights = board->castlingRights - 0x8;
		if ((move.startLocation == 0) && ((board->castlingRights & 0x4) == 0x4))
			board->castlingRights = board->castlingRights - 0x4;
	}//end if white rook moved
	if ((move.piece == WHITE_KING) && ((board->castlingRights & 0xC) >= 4)) {
		board->castlingRights -= (board->castlingRights & 0xC);
	}//end if white King moved
}//changeWhiteCastlingRights

//Summary: Takes away castling rights when a black rook/king moves if necessary
void changeBlackCastlingRights(Board *board, MoveList move) {
	//undo castling rights if rook/king moves:
	if ((move.piece == BLACK_ROOK) && ((board->castlingRights & 0x3) > 0)) {
		if ((move.startLocation == 56) && ((board->castlingRights & 0x2) == 0x2))
			board->castlingRights = board->castlingRights - 0x2;
		if ((move.startLocation == 63) && ((board->castlingRights & 0x1) == 0x1))
			board->castlingRights = board->castlingRights - 0x1;
	}//end if white rook moved
	if ((move.piece == BLACK_KING) && ((board->castlingRights & 0x3) > 0x0)) {
		board->castlingRights -= (board->castlingRights & 0x3);
	}//end if white King moved
}//changeBlackCastlingRights


//Summary Restores White Castling Rights for undoing moves
void restoreWhiteCastlingRights(Board *board, MoveGen *moveHistory) {
	//CHECK MOVEHISTORY TO SEE WHAT THE RIGHTS ARE
	board->castlingRights = (board->castlingRights | 0xB);//RESTORE ALL OF WHITE CASTLING RIGHTS
	for (int i = 0; i < (moveHistory->count - 2); i++) {	//TAKE AWAY RIGHTS BASED ON HISTORY		
		if ((moveHistory->Moves[i].piece == WHITE_ROOK)) 
			board->castlingRights -= (board->castlingRights & ((moveHistory->Moves[i].startLocation == 7) ? 0x8 : 0x4));
		
		else if (moveHistory->Moves[i].piece == WHITE_KING) 
			board->castlingRights -= (board->castlingRights & 0xB);
		
		if ((board->castlingRights & 0xB) == 0) // IF castling rights are gone, end
			break;
	}//end for
}//restoreWhiteCastlingRights

//Restores Black Castling Rights for undoing moves
void restoreBlackCastlingRights(Board *board, MoveGen *moveHistory) {	
	//CHECK MOVEHISTORY TO SEE WHAT THE RIGHTS ARE
	board->castlingRights = (board->castlingRights | 0x3);//RESTORE ALL OF WHITE CASTLING RIGHTS
	for (int i = 0; i < (moveHistory->count - 2); i++) {	//TAKE AWAY RIGHTS BASED ON HISTORY		
		if ((moveHistory->Moves[i].piece == BLACK_ROOK))
			board->castlingRights -= (board->castlingRights & ((moveHistory->Moves[i].startLocation == 63) ? 0x2 : 0x1));

		else if (moveHistory->Moves[i].piece == BLACK_KING)
			board->castlingRights -= (board->castlingRights & 0x3);

		if ((board->castlingRights & 0x3) == 0) // IF castling rights are gone, end
			break;
	}//end for
}//restoreBlackCastlingRights



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
	while ((x + 1 < 8) && (y + 1 < 8)) {
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
	for (x = j + 1; x < 8; x++) {
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

	for (int i = 0; i < 100; i++) {
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

		}//end for j
	}//end for i	
}//setMoves