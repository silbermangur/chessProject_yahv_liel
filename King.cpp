#include "King.h"

//the function return if the player can move the move he wanted to make and if not return why
//move - the move the player wanted to make in this struct <dest><source> for example: "e4e5"
//the function return the code that need to sent to the fronted
int King::Move(std::string move, IPiece* board[8][8])
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
int King::IsValid(std::string move)
{
    bool codesix = true;
    int i = 0, j = 0, k = 0;
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
    //check by this table
    // i | j | k | row   | coll
    // 0 | 0 | 0 | row+0 | coll+1
    // 0 | 0 | 1 | row+1 | coll+1
    // 0 | 1 | 0 | row+0 | coll-1
    // 0 | 1 | 1 | row+1 | coll-1
    // 1 | 0 | 0 | row-1 | coll+1
    // 1 | 0 | 1 | row-1 | coll-1
    for (i = 0; i < 2;i++)
    {
        for (j = 0;j < 2 ;j++)
        {
            for (k = 0;k < 2;k++)
            {                
                if (std::string(1, move[SRC_LETTER]) + move[SRC_NUM] == std::string(1, static_cast<char>(move[DST_LETTER] + k-i)) + static_cast<char>(move[DST_NUM] + 1-(2*j)))
                {
                    codesix = false;  
                }
            }
        }
        if (i == 1 && j == 1)//i tried to connect the two last option but i didnt succed so i write them after
        {
            break;
        }
    }
    if (std::string(1, move[SRC_LETTER]) + move[SRC_NUM] == std::string(1, static_cast<char>(move[DST_LETTER] + 1)) + static_cast<char>(move[DST_NUM]) ||// row+1 | coll
        std::string(1, move[SRC_LETTER]) + move[SRC_NUM] == std::string(1, static_cast<char>(move[DST_LETTER] - 1)) + static_cast<char>(move[DST_NUM]))  // row-1 | coll
    {
        codesix = false;
    }
    if (codesix)
    {
        return 6;
    }
    return 0;
}