#ifndef EVAL_H
#define EVAL_H

// Function Prototype for eval.c
short int eval(Board *board, unsigned char turnCount, Move *move);

// Placement Table Declarations and Initialization
// These tables are from white's perspective, flip the indexing (8-i, 8-j) and the values (-value) for black's perspecitve
// Direct comments on the table should be read bottom to top
char pawnPlaceTable[8][8];
char knightPlaceTable[8][8];
char bishopPlaceTable[8][8];
char rookPlaceTable[8][8];
char queenPlaceTable[8][8];
char kingPlaceTable[8][8];


// Pawns are encourgaed to press forward and control the center. Pawn structure will be implemented later
pawnPlaceTable[8][8] = {
{ 0,  0,  0,  0,  0,  0,  0,  0 },
{ 75, 75, 75, 75, 75, 75, 75, 75 }, // Promoting pawns is good and you should feel good
{ 10, 15, 20, 35, 35, 20, 15, 10 },
{ 5, 10, 15, 30, 30, 15, 10,  5 },
{ 0,  0,  0, 25, 25,  0,  0,  0 },
{ 5, -5,-10,  0,  0,-10, -5,  5 },
{ 5, 10, 10,-30,-30, 10, 10,  5 },
{ 0,  0,  0,  0,  0,  0,  0,  0 }  // Panws don't exist down here
};

// Knights are encouraged to control the center and stay away from corners and edges, but are less harshly penalized for advancing to the other side
knightPlaceTable[8][8] = {
{ -75,-25,-20,-10,-10,-20,-25,-75 },
{ -40,-20,  0,  5,  5,  0,-20,-40 },
{ -30,  0, 10, 15, 15, 10,  0,-30 },
{ -30,  5, 15, 20, 20, 15,  5,-30 },
{ -30,  5, 15, 20, 20, 15,  5,-30 },
{ -30,  0, 10, 15, 15, 10,  0,-30 },
{ -40,-20,  0, 10, 10,  0,-20,-40 },
{ -75,-50,-40,-20,-20,-40,-50,-75 }  // Corner knights are bad and you should feel bad
};

// Bishops are also encoruaged to control the center and stay away from corners and edges
bishopPlaceTable[8][8] = {
{ -20,-10,-10,-10,-10,-10,-10,-20 },
{ -10,  5,  0,  0,  0,  0,  5,-10 },
{ -10,  0,  5, 10, 10,  5,  0,-10 },
{ -10,  5,  5, 10, 10,  5,  5,-10 },
{ -10,  0, 10, 10, 10, 10,  0,-10 },
{ -10, 10, 10, 10, 10, 10, 10,-10 },
{ -5, 15,  0,  0,  0,  0, 15, -5 }, // Defend your king!
{ -20,-10,-40,-10,-10,-40,-10,-20 }
};

// Rooks should stay put if it is still possible to castlize, and should remain on the center files
rookPlaceTable[8][8] = {
{ -5, 10, 15, 25, 25, 15, 10, -5 },
{ -5, 10, 15, 25, 25, 15, 10, -5 }, // Congrats on advancing, +5 (except on the edges. Edges are bad)
{ -5,  5, 10, 20, 20, 10,  5, -5 },
{ -5,  5, 10, 20, 20, 10,  5, -5 },
{ -5,  5, 10, 20, 20, 10,  5, -5 },
{ -5,  5, 10, 20, 20, 10,  5, -5 },
{ -5,  5, 10, 20, 20, 10,  5, -5 },
{ 15,  5, 15, 20, 20, 15,  5, 15 }  // Starting corners are good. Castling moves the rook to an equally favorable spot
};

// Queens act similarly to rooks, with a litle bonus here and there thanks to their extra mobility
queenPlaceTable[8][8] = {
{ -10,  5, 15, 20, 20, 15,  5,-10 }, // Corners aren't as bad for Queens, but are still bad
{ -5, 15, 15, 20, 20, 15, 15, -5 }, // Congrats on advancing, you get nothing
{ 5, 15, 15, 20, 20, 15, 15,  5 }, // Edges aren't as bad either. In fact they may be good in some cases
{ 5, 20, 20, 30, 30, 20, 20,  5 },
{ 5, 20, 20, 30, 30, 20, 20,  5 }, // That sweet sweet center control
{ 5, 15, 15, 20, 20, 15, 15,  5 },
{ -5, 15, 15, 20, 20, 15, 15, -5 },
{ -10,  5, 15, 20, 20, 15,  5,-10 }
};

// Kings should hide in their corners until the endgame, then they should head toward the center
kingPlaceTable[8][8] = {
{ -50,-55,-65,-75,-75,-65,-55,-50 }, // How would you even get this far?
{ -50,-55,-65,-75,-75,-65,-55,-50 }, // Please stop
{ -50,-55,-65,-75,-75,-65,-55,-50 }, // WHY?
{ -50,-55,-65,-75,-75,-65,-55,-50 }, // No
{ -40,-45,-55,-65,-65,-55,-45,-40 }, // No
{ -30,-35,-45,-50,-50,-45,-35,-30 }, // Don't you dare
{ 20, 20,  0,  0,  0,  0, 20, 20 }, // Could be better
{ 20, 40, 10,  0,  0, 10, 40, 20 }  // Safe
};

// For the endgame, kings should move towards the center
kingPlaceTableEnd[8][8] = {
{ -50,-40,-30,-20,-20,-30,-40,-50 }, // Bad
{ -30,-20,-10,  0,  0,-10,-20,-30 }, // Wait
{ -30,-10, 20, 30, 30, 20,-10,-30 }, // Okay
{ -30,-10, 30, 50, 50, 30,-10,-30 }, // Nice
{ -30,-10, 30, 50, 50, 30,-10,-30 }, // Nice
{ -30,-10, 20, 30, 30, 20,-10,-30 }, // Getting warmer
{ -30,-20,-10,  0,  0,-10,-20,-30 }, // No
{ -50,-40,-30,-20,-20,-30,-40,-50 }  // No
};


#endif // !PLACE_TABLES_H