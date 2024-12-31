#include "Pawn.h"

//the function return if the player can move the move he wanted to make and if not return why
//move - the move the player wanted to make in this struct <dest><source> for example: "e4e5"
//the function return the code that need to sent to the fronted
int Pawn::Move(std::string move)
{	
    int code = 0;
    if (!(code = IsValid(move)))//check if we pass the check as a knight
    {
        _moves++;
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
    
    //in here(code 6 ckeck) we assume that if the pawn goes in diagonal way it because it eat and it pass the check
    //because in here we does not have a connection with the main board

    //checking if the pawn do one of it regular move to nullify the number of moves he allready done
    if (std::string(1, move[0]) + move[1] == std::string(1, static_cast<char>(move[2] - 1)) + static_cast<char>(move[3] - 1) ||//checking if he move in diagonal
        std::string(1, move[0]) + move[1] == std::string(1, move[2]) + static_cast<char>(move[3] - 1))//checking if he goes up by one square
    {
        _moves = 0;
    }
    //check code 6(if it ilegal move to do)
    if (std::string(0, move[0]) + move[1] != std::string(1, static_cast<char>(move[2] - 1)) + static_cast<char>(move[3] - 1) &&//checking if he eat in diagonal
        std::string(0, move[0]) + move[1] != std::string(1, move[2]) + static_cast<char>(move[3] - 1) &&//checking if he goes up by one square
        std::string(0, move[0]) + move[1] != std::string(1, move[2]) + static_cast<char>(move[3] - 2) && _moves)//check if he goes by two squares up in the first move he does     
    {
        _moves = saveForSpacialCase;
        return 6;
    }
    _moves = saveForSpacialCase;
    return 0;
}