#include "IPiece.h"

class Rook : public IPiece
{
public:
	virtual int Move(std::string move, IPiece* board[8][8]);//The function that move the piece if the piece can move
private:
	virtual int IsValid(std::string move);//the function check if the piece can move
};

