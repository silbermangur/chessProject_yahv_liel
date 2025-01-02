#include "IPiece.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Pipe.h"

class Manager
{
private:
	IPiece* board[8][8];
public:
	void play();
private:
	void InitializingBoard(std::string gameBoard);
	Pipe connectFronted();
	IPiece* type(std::string move);
	bool isYours(std::string place);
	bool isBlock(IPiece* piece, std::string move);
	bool selfCheck(std::string move);
	bool isCheck(std::string move);
	//bool isMate(std::string move);
};

