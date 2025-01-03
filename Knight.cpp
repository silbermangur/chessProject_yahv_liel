#include "Knight.h"
#include <iostream>

//the function return if the player can move the move he wanted to make and if not return why
//move - the move the player wanted to make in this struct <dest><source> for example: "e4e5"
//the function return the code that need to sent to the fronted
int Knight::Move(std::string move, IPiece* board[8][8])
{
    int code = 0;
    if (!(code = IsValid(move)))//check if we pass the check as a knight
    {
        board[RIGHT_WALL - move[3]][move[2] - FLOOR] = board[RIGHT_WALL - move[1]][move[0] - FLOOR];
        board[RIGHT_WALL - move[1]][move[0] - FLOOR] = nullptr;
        return 0;
    }
    return code;
}

//the function check if the move the player wants to do is valid
//move - the move the player wanted to make in this struct <dest><source> for example: "e4e5"
//the function return the code that need to send to the fronted
int Knight::IsValid(std::string move)
{
    bool legal = false;
    int heg = 1, wid = 2,temp = 0;
    if (move[0] > CELLING || move[0] < FLOOR ||//checking if the source height is in the board
        move[2] > CELLING || move[2] < FLOOR ||//checking if the destination height is in the board
        move[1] > RIGHT_WALL || move[1] < LEFT_WALL ||//checking if the source width is in the board
        move[3] > RIGHT_WALL || move[3] < LEFT_WALL)//checking if the destination width is in the board
    {
        return 5;
    }  
    if (move[0] == move[2] && move[1] == move[3])//check if he try to move the piece to the same location
    {
        return 7;
    }
    //check code 7 is before code 6 because 7 is a specific code 6 check
    for (int i = 0; i < 4;i++)//the loop activate four times because there are 8 options and we check to each time so 8/2 = 4
    {
        if (std::string(1, move[0]) + move[1] == std::string(1, static_cast<char>(move[2] - heg)) + static_cast<char>(move[3] - wid))
        {
            legal = true;
        }
        if (std::string(1, move[0]) + move[1] == std::string(1, static_cast<char>(move[2] - heg)) + static_cast<char>(move[3] + wid))
        {
            legal = true;
        }
        //switching to check other locations
        temp = heg;
        heg = wid;
        wid = temp;
        if (i == 2)//after checking helf moving to check the other half
        {
            heg *= -1;
            wid *= -1;
        }
    }
    if (!legal)//check if the move ilegal if it ilegal return 6(the code for illegalness)
    {
        return 6;
    }

    return 0;
}