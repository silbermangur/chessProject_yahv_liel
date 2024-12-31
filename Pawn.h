#pragma once
#include "IPiece.h"
#include <iostream>

class Pawn : public IPiece
{
public:
	int _moves = 0;//the counter to the moves of the pawn
	virtual int Move(std::string move);//The function that move the piece if the piece can move
private:
	virtual int IsValid(std::string move);//the function check if the piece can move
};

