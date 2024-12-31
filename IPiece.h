#pragma once
#include <string>
//defining the curners of the board
#define FLOOR 'a'
#define CELLING 'h'
#define LEFT_WALL '0'
#define RIGHT_WALL '8'


class IPiece
{
protected:
	virtual int Move(std::string) = 0;//The function that move the piece if the piece can move
	virtual int IsValid(std::string) = 0;//the function check if the piece can move
};