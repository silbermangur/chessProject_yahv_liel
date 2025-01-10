#include "Pawn.h"

//the function return if the player can move the move he wanted to make and if not return why
//move - the move the player wanted to make in this struct <dest><source> for example: "e4e5"
//the function return the code that need to sent to the fronted
int Pawn::Move(std::string move, IPiece* board[8][8])
{	
    int code = 0;
    std::string saveOriginalMove = move;
    if (!board[RIGHT_WALL - move[SRC_NUM]][move[SRC_LETTER] - FLOOR]->isWhite)//if it a black piece we need the move from black precpective
    {
        move[SRC_LETTER] = (CELLING - move[SRC_LETTER] + FLOOR);
        move[SRC_NUM] = (RIGHT_WALL - move[SRC_NUM] + LEFT_WALL);
        move[DST_LETTER] = (CELLING - move[DST_LETTER] + FLOOR);
        move[DST_NUM] = (RIGHT_WALL - move[DST_NUM] + LEFT_WALL);
    }
    if (!(code = IsValid(move)))//check if we pass the check as a knight
    {
        move = saveOriginalMove;
        _moves++;
        board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR] = board[RIGHT_WALL - move[SRC_NUM]][move[SRC_LETTER] - FLOOR];
        board[RIGHT_WALL - move[SRC_NUM]][move[SRC_LETTER] - FLOOR] = nullptr;
        return 0;
    }
    return code;
}

//the function check if the move the player wants to do is valid
//move - the move the player wanted to make in this struct <dest><source> for example: "e4e5"
//the function return the code that need to send to the fronted
int Pawn::IsValid(std::string move)
{
    int saveForSpacialCase = _moves;//it saves the bumbers of move allready done to succesfuly check code 6
    std::string c;
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
    
    //in here(code 6 ckeck) we assume that if the pawn goes in diagonal way it because it eat and it pass the check
    //because in here we does not have a connection with the main board

    //checking if the pawn do one of it regular move to nullify the number of moves he allready done
    if (((move[SRC_LETTER] == move[DST_LETTER] - 1 || move[SRC_LETTER] == move[DST_LETTER] + 1) && (move[SRC_NUM] + 1 == move[DST_NUM])) ||//checking if he move in diagonal
        (move[SRC_LETTER] == move[DST_LETTER] && move[SRC_NUM] == move[DST_NUM] - 1))//checking if he goes up by one square
    {
        _moves = 0;
    }
    //check code 6(if it ilegal move to do)
    if (!((move[SRC_LETTER] == move[DST_LETTER] - 1 || move[SRC_LETTER] == move[DST_LETTER] + 1) && (move[SRC_NUM] + 1 == move[DST_NUM])) &&//checking if he eat in diagonal
        !(move[SRC_LETTER] == move[DST_LETTER] && move[SRC_NUM] + 1 == move[DST_NUM]) &&//checking if he goes up by one square
        !(move[SRC_LETTER] == move[DST_LETTER] && move[SRC_NUM] + 2 == move[DST_NUM] && _moves == 0))//check if he goes by two squares up in the first move he does     
    {
        _moves = saveForSpacialCase;
        return 6;
    }
    _moves++;
    _moves = saveForSpacialCase;
    return 0;
}