#include "IPiece.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishops.h"
#include "Pipe.h"

#define CODE_PASS '0'
#define BLACK_KNIGHT 'n'
#define WHITE_KNIGHT 'N'
#define BLACK_QUEEN 'q'
#define WHITE_QUEEN 'Q'
#define BLACK_PAWN 'p'
#define WHITE_PAWN 'P'
#define BLACK_KING 'k'
#define WHITE_KING 'K'
#define BLACK_ROOK 'r'
#define WHITE_ROOK 'R'
#define BLACK_BISHOP 'b'
#define WHITE_BISHOP 'B'
#define CLEAR_SPACE '#'


class Manager
{
public:
	IPiece* board[8][8];
public:
	void play();
public:
	void InitializingBoard(std::string gameBoard);
	Pipe connectFronted();
	IPiece* type(std::string move);
	bool pawnEatCheck(std::string move, IPiece* piece);
	bool isBlock(IPiece* piece, std::string move);
	bool selfCheck(std::string move,IPiece* piece,bool whiteTurn);
	bool isCheck(std::string move);
	//bool isMate(std::string move);
};

