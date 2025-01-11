#include "Queen.h"
#include <iostream>


int Queen::Move(std::string move, IPiece* board[8][8])
{
    int code = 0;
    if (!(code = IsValid(move)))//check if we pass the check as a knight
    {
        board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR] = board[RIGHT_WALL - move[SRC_NUM]][move[SRC_LETTER] - FLOOR];
        board[RIGHT_WALL - move[SRC_NUM]][move[SRC_LETTER] - FLOOR] = nullptr;
        return 0;
    }
    return code;

}

//the function check if the move the player wants to do is valid
//move - the move the player wanted to make in this struct <dest><source> for example: "e4e5"
//the function return the code that need to send to the fronted
int Queen::IsValid(std::string move)
{
    bool legal = false;
    int heg = 1, wid = 2, temp = 0;
    if (move[SRC_LETTER] > CELLING || move[SRC_LETTER] < FLOOR ||//checking if the source height is in the board
        move[DST_LETTER] > CELLING || move[DST_LETTER] < FLOOR ||//checking if the destination height is in the board
        move[SRC_NUM] > RIGHT_WALL || move[SRC_NUM] < LEFT_WALL ||//checking if the source width is in the board
        move[DST_NUM] > RIGHT_WALL || move[DST_NUM] < LEFT_WALL)//checking if the destination width is in the board
    {
        return 5;
    }
    if (move[SRC_LETTER] == move[DST_LETTER] && move[SRC_NUM] == move[DST_NUM])//check if he try to move the piece to the same location
    {
        return 7;
    }
    //check code 7 is before code 6 because 7 is a specific code 6 check
    if ((std::abs(move[SRC_LETTER] - move[DST_LETTER]) != std::abs(move[SRC_NUM] - move[DST_NUM])) || (!(move[SRC_LETTER] == move[DST_LETTER] || move[SRC_NUM] == move[DST_NUM])))
    {
        return 6;
    }

    return 0;
}