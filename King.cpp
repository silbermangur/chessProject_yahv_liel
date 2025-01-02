#include "King.h"

int King::Move(std::string move)
{
    int code = 0;
    if (!(code = IsValid(move)))//check if we pass the check as a knight
    {
        return 0;
    }
    return code;
}

int King::IsValid(std::string move)
{
    int i = 0, j = 0, k = 0;
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
    bool codesix = true;
    for (i = 0; i < 2;i++)
    {
        for (j = 0;j < 2 ;j++)
        {
            for (k = 0;k < 2;k++)
            {                
                if (std::string(1, move[0]) + move[1] == std::string(1, static_cast<char>(move[2] + k-i)) + static_cast<char>(move[3] + 1-(2*j)))
                {
                    codesix = false;  
                }
            }
        }
        if (i == 1 && j == 1)
        {
            break;
        }
    }
    if (std::string(1, move[0]) + move[1] == std::string(1, static_cast<char>(move[2] + 1)) + static_cast<char>(move[3]) ||
        std::string(1, move[0]) + move[1] == std::string(1, static_cast<char>(move[2] - 1)) + static_cast<char>(move[3]))
    {
        codesix = false;
    }
    if (codesix)
    {
        return 6;
    }
    return 0;
}