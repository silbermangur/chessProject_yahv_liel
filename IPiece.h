#pragma once
#include <string>
//defining the curners of the board
#define FLOOR 'a'
#define CELLING 'h'
#define LEFT_WALL '0'
#define RIGHT_WALL '8'

// defining the indexs in the string from froted
#define SRC_LETTER 0
#define SRC_NUM 1
#define DST_LETTER 2
#define DST_NUM 3


class IPiece
{
public:
	bool isWhite = true;//initialize at true because we asuume it a white piece if else we change it to black(fasle)
	virtual int Move(std::string,IPiece* board[8][8]) = 0;//The function that move the piece if the piece can move
	virtual int IsValid(std::string) = 0;//the function check if the piece can move
};