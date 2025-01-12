#include "Manager.h"

void Manager::play()
{
	IPiece* piece = nullptr;//the IPiece to contain the piece we are now working on
	bool whiteTurn = true;//a bollian to check who are playing now start with white so the defult is true
	int pieceCode = 0,managerCode = 0;//pieceCode - the code from the check in the piece itself || managerCode - the code from the manager
	char gameString[] = "rnbkqbnrpppppppp################################PPPPPPPPRNBKQBNR0",codeMsg[] = "0";//gameString - the string of the game || codeMsg - the msg to send to the server
	std::string move = "";//the string that recive from the fronted about the move
	Pipe fronted = connectFronted();//the pipe to connect with the fronted

	//initializing the board
	InitializingBoard(gameString);

	//starting the game
	fronted.sendMessageToGraphics(gameString);

	//while (isMate)
	while (true)
	{
		//reciving the message of the move
		move = fronted.getMessageFromGraphics();
		piece = type(move);//checking what is the piece in the place we want to move
		if (!piece || whiteTurn == !piece->isWhite)//check error code 2
		{
			codeMsg[0] = '2';
		}
		//check error code 3//need to be 
		else if (board[(RIGHT_WALL - move[DST_NUM])][(move[DST_LETTER] - FLOOR)] != nullptr &&
			( whiteTurn && board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR]->isWhite ||//check move white to white
			!whiteTurn && !board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR]->isWhite ))//check move black to clack
		{
			codeMsg[0] = '3';
		}

		else if (selfCheck(move, piece, whiteTurn))//check error code 4(self check)
		{
			codeMsg[0] = '4';
		}
		else//check the piece error codes(5,6,7)(6 error at pawn case part of it check in here because at the pawn itself 
												//there is no connection to the board)
		{
			//if it a pawn that cant to his move
			if (piece->IsValid("e4e5") == 0 && piece->IsValid("e4f5") == 0 && !(piece->IsValid("e4e3") == 0) && pawnEatCheck(move,piece))
			{
				codeMsg[0] = '6';
			}
			else
			{
				//if (codeMsg[0] == '0' && isBlock(piece,move))
				//{
				//	codeMsg[0] = '6';
				//}
				//else
				//{
					pieceCode = piece->Move(move, board);
					codeMsg[0] = static_cast<char>('0' + pieceCode);
				//}
			}
		}
		

		if (codeMsg[0] == CODE_PASS && isCheck(move,piece,whiteTurn))//check if it pass and accured a chess in the last move
		{
			codeMsg[0] = '1';
		}
		if (codeMsg[0] == CODE_PASS || codeMsg[0] == '1')//check if it pass all the tests or happend a chess in the last move
		{
			whiteTurn = not whiteTurn;//changing the player who is playing 
		}
		fronted.sendMessageToGraphics(codeMsg);//sent the code messege
		codeMsg[0] = CODE_PASS;//setting for the next move to 0
		
	}
	fronted.close();
}

//the function initialize the board to the base game string
//gameBoard - the board of the game in string
void Manager::InitializingBoard(std::string gameBoard)
{
	for (int i = 0;i < 64;i++)
	{
		if (gameBoard[i] != CLEAR_SPACE)
		{
			if (gameBoard[i] == BLACK_KNIGHT || gameBoard[i] == WHITE_KNIGHT)
			{
				board[i / 8][i % 8] = new Knight();
				if (gameBoard[i] == BLACK_KNIGHT)//if it a black piece it should be known as one
				{
					board[i / 8][i % 8]->isWhite = false;
				}
			}
			else if (gameBoard[i] == BLACK_KING || gameBoard[i] == WHITE_KING)
			{
				board[i / 8][i % 8] = new King();
				if (gameBoard[i] == BLACK_KING)//if it a black piece it should be known as one
				{
					board[i / 8][i % 8]->isWhite = false;
				}
			}
			else if (gameBoard[i] == BLACK_PAWN || gameBoard[i] == WHITE_PAWN)
			{
				board[i / 8][i % 8] = new Pawn();
				if (gameBoard[i] == BLACK_PAWN)//if it a black piece it should be known as one
				{
					board[i / 8][i % 8]->isWhite = false;
				}
			}
			else if (gameBoard[i] == BLACK_QUEEN || gameBoard[i] == WHITE_QUEEN)
			{
				board[i/8][i%8] = new Queen();
				if (gameBoard[i] == BLACK_QUEEN)//if it a black piece it should be known as one
				{
					board[i / 8][i % 8]->isWhite = false;
				}
			 
			}	
			else if (gameBoard[i] == BLACK_ROOK || gameBoard[i] == WHITE_ROOK)
			{
				board[i/8][i%8] = new Rook();
				if (gameBoard[i] == BLACK_ROOK)//if it a black piece it should be known as one
				{
					board[i / 8][i % 8]->isWhite = false;
				}
			}
			else if (gameBoard[i] == BLACK_BISHOP || gameBoard[i] == WHITE_BISHOP)
			{
				board[i/8][i%8] = new Bishops();
				if (gameBoard[i] == BLACK_BISHOP)//if it a black piece it should be known as one
				{
					board[i / 8][i % 8]->isWhite = false;
				}
			}
		}
	}
}

//the function connect the backed with the fronted
//return the pipe bitween the backed and fronted
Pipe Manager::connectFronted()
{
	//connecting to the game
	Pipe fronted;
	std::string ans;
	while (!fronted.connect())
	{
		std::cout << "cant connect to graphics\n";
		std::cout << "Do you try to connect again or exit? (0-try again, 1-exit)\n";
		std::cin >> ans;

		if (ans == "0")//if he want to keep tring
		{
			std::cout << "trying connect again..\n";
			Sleep(5000);
			if (fronted.connect())
			{
				break;
			}
		}
		else//else quit the procces
		{
			fronted.close();
			exit(1);
		}
	}
	return fronted;
}


//the function check what is the piece that the player want to move
//move - the current move
//the function return the piece that the player want to move 
IPiece* Manager::type(std::string move)
{
	return board[RIGHT_WALL - move[SRC_NUM]][move[SRC_LETTER] - FLOOR];
}

//the function check if a pawn try to eat a piece witout he capeable of
// move - the move the pwan try to do
// piece - the pawn piece 
bool Manager::pawnEatCheck(std::string move,IPiece* piece)
{
	std::string saveOriginalMove = move;
	int addPlace = 1;
	if (!piece->isWhite)//if it a black piece we need the move from black precpective
	{
		addPlace = -1;
	}
	//that move in digonal and he has nothing to kill
	if (((move[SRC_LETTER] == move[DST_LETTER] - 1 || move[SRC_LETTER] == move[DST_LETTER] + 1) && (move[SRC_NUM] + addPlace == move[DST_NUM])) &&
		(board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR] == nullptr))
	{
		return true;
	}
	//if he move up by one square and a piece is there
	else if ((move[SRC_LETTER] == move[DST_LETTER] && move[SRC_NUM] + addPlace == move[DST_NUM]) &&//checking if he goes up by one square 
		board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR] != nullptr)
	{
		return true;
	}
	//if he move up by two square and a piece is there
	else if (move[SRC_LETTER] == move[DST_LETTER] && move[SRC_NUM] + (2 * addPlace) == move[DST_NUM] &&
		board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR] != nullptr)
	{
		return true;
	}
	move = saveOriginalMove;
	return false;
}

//check if thw way of the piece describe by a move is not block
//piece - the piece that try to move
//move - the move that the piece try to do
//return true if the way blocked and false else
bool Manager::isBlock(IPiece* piece, std::string move)
{
	if (piece->IsValid("e4e5") == 0 && piece->IsValid("e4d3") == 0 && piece->IsValid("e4e6") != 0)//check if it a king
	{
		return false;//the king move by one space at a time and he can eat in all of them
	}
	else if (piece->IsValid("e4e5") == 0 && piece->IsValid("e4d3") == 0 && piece->IsValid("e4e6") == 0)//check if it a queen
	{
		int joinToI = 1,joinToJ = 1;
		int iTo = 8, jTo = 8;
		bool foundPiece = false;
		if (move[SRC_LETTER] - move[DST_LETTER] == move[SRC_NUM] - move[DST_NUM])//the queen move like bishop
		{
			if (move[SRC_LETTER] - move[DST_LETTER] > 0)
			{
				joinToJ = -1;
				iTo = 0;
			}
			if (move[SRC_NUM] - move[DST_NUM] > 0)
			{
				joinToI = 1;
				jTo = 0;
			}
			for (int i = (RIGHT_WALL -  move[SRC_NUM] + 1);i < RIGHT_WALL - move[DST_NUM];i+=joinToI)
			{
				for (int j = (move[SRC_LETTER] - FLOOR + 1);j < move[DST_LETTER] - FLOOR;j += joinToJ)
				{
					if (board[RIGHT_WALL - move[SRC_NUM] + i][move[SRC_LETTER] - FLOOR + j] != nullptr)
					{
						foundPiece = true;
					}
				}
			}

		}
		else//move like a rook
		{
			if (move[SRC_LETTER] == move[DST_LETTER])
			{
				if (move[SRC_NUM] - move[DST_NUM] < 0)
				{
					joinToI = -1;
				}
				for (int i = static_cast<int>(RIGHT_WALL - move[SRC_NUM]);i < RIGHT_WALL - move[DST_NUM];i += joinToI)
				{
					if (board[RIGHT_WALL - move[SRC_LETTER]][i] != nullptr)
					{
						foundPiece = true;
					}
				}
			}
			if (move[SRC_NUM] == move[DST_NUM])
			{
				if (move[SRC_LETTER] - move[DST_LETTER] < 0)
				{
					joinToJ = -1;
				}
				for (int J = static_cast<int>(CELLING - move[SRC_LETTER]);J < CELLING - move[DST_LETTER];J += joinToI)
				{
					if (board[J][move[SRC_NUM] - FLOOR] != nullptr)
					{
						foundPiece = true;
					}
				}
			}
		}
		if (board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR] != nullptr && //first need to be check if there are a piece in the place
			board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR]->isWhite && piece->isWhite)//check if at the dest it the same color
		{
			foundPiece = true;
		}
		if (foundPiece)
		{
			return false;
		}
		return true;
	}
	else if (piece->IsValid("b2d3") == 0)//check if it a Knight
	{
		return false;//the knight is *jumping* so it doesnt need to be check and he can eat in all the places he jump to
	}
	else if (piece->IsValid("e4e5") == 0 && piece->IsValid("e4f5") == 0 && piece->IsValid("e4e3") != 0)//check if it pawn
	{
		int moveKind = 1;
		if (!piece->isWhite)
		{
			moveKind = -1;
		}
		if ((move[SRC_NUM] + 2 == move[DST_NUM] || move[SRC_NUM] - 2 == move[DST_NUM]) &&//if he move up by to squares(WHITE OR BLACK)
			board[RIGHT_WALL - move[DST_NUM] + moveKind][move[DST_LETTER] - FLOOR] != nullptr) // if there is a piece one space before the destenation
		{
			return true;
		}
		return false;
	}
	else if (piece->IsValid("e4e7") == 0)//check if it a rook
	{
		int joinToI = 1, joinToJ = 1;
		int iTo = 8, jTo = 8;
		int conditionAdd = -1;
		bool foundPiece = false;
		if (move[SRC_LETTER] == move[DST_LETTER])
		{
			if (move[SRC_NUM] - move[DST_NUM] < 0)
			{
				joinToI = -1;
				conditionAdd = 1;
			}
			for (int i = static_cast<int>(RIGHT_WALL - move[SRC_NUM]);i < RIGHT_WALL - move[DST_NUM] + conditionAdd;i += joinToI)
			{
				if (board[i + move[SRC_NUM]][move[SRC_LETTER] - FLOOR] != nullptr)
				{
					foundPiece = true;
				}
			}
		}
		else if (move[SRC_NUM] == move[DST_NUM])
		{
			if (move[SRC_LETTER] - move[DST_LETTER] < 0)
			{
				joinToJ = -1;
			}
			for (int J = static_cast<int>(CELLING - move[SRC_LETTER]);J < CELLING - move[DST_LETTER];J += joinToI)
			{
				if (board[J][move[SRC_NUM] - FLOOR] != nullptr)
				{
					foundPiece = true;
				}
			}
		}
		if (board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR] != nullptr && //first need to be check if there are a piece in the place
			board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR]->isWhite && piece->isWhite)//check if at the dest it the same color
		{
			foundPiece = true;
		}
		if (foundPiece)
		{
			return false;
		}
		return true;
	}
	else//it a bishop
	{
		int joinToI = 1, joinToJ = 1;
		int iTo = 8, jTo = 8;
		bool foundPiece = false;
		if (move[SRC_LETTER] - move[DST_LETTER] > 0)
		{
			joinToJ = -1;
			iTo = 0;
		}
		if (move[SRC_NUM] - move[DST_NUM] > 0)
		{
			joinToI = 1;
			jTo = 0;
		}
		for (int i = (RIGHT_WALL - move[SRC_NUM] + 1);i < RIGHT_WALL - move[DST_NUM];i += joinToI)
		{
			for (int j = (move[SRC_LETTER] - FLOOR + 1);j < move[DST_LETTER] - FLOOR;j += joinToJ)
			{
				if (board[RIGHT_WALL - move[SRC_NUM] + i][move[SRC_LETTER] - FLOOR + j] != nullptr)
				{
					foundPiece = true;
				}
			}
		}
		if (board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR] != nullptr && //first need to be check if there are a piece in the place
			board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR]->isWhite && piece->isWhite)//check if at the dest it the same color
		{
			foundPiece = true;
		}
		if (foundPiece)
		{
			return false;
		}
		return true;
	}
	return false;
}

//the function check if the current move create a self check
//move - the current move done by player
//the function return true if it does create a self check if not return false
bool Manager::selfCheck(std::string move, IPiece* piece, bool whiteTurn)
{
	IPiece* killed = board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR];//saving the killed piece because it still can be an ilegal move
	//do the move requsted
	board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR] = board[RIGHT_WALL - move[SRC_NUM]][move[SRC_LETTER] - FLOOR];
	board[RIGHT_WALL - move[SRC_NUM]][move[SRC_LETTER] - FLOOR] = nullptr;
	//variables for the king place
	std::string kingPlace;
	bool kingfound = false;
	//the string to check the move
	std::string moveToKing;

	//finding the king place
	for (int i = 0; i < 8 && !kingfound;i++)
	{
		for (int j = 0;j < 8 && !kingfound;j++)
		{
			if (board[i][j] != nullptr &&
				board[i][j]->IsValid("e4e5") == 0 && board[i][j]->IsValid("e4d3") == 0 && board[i][j]->IsValid("e4e6") != 0 && board[i][j]->isWhite == whiteTurn)//check if it a king of the same player that playing right now
			{
				kingPlace = std::string(1, static_cast<char>(j + FLOOR)) + static_cast<char>(RIGHT_WALL - i);
				kingfound = !kingfound;
			}
		}
	}


	//checking if a check is heppening
	for (int i = 0; i < 8;i++)
	{
		for (int j = 0;j < 8;j++)
		{
			if (board[i][j] != nullptr && !board[i][j]->isWhite == whiteTurn)//checking if the the other color do the check to the current color playing
			{
				moveToKing = std::string(1, static_cast<char>(j + FLOOR)) + static_cast<char>(RIGHT_WALL - i) + kingPlace;
				//checking if the move is avaliable  for the piece to do
				if (board[i][j]->IsValid(moveToKing) == 0 && !isBlock(piece,moveToKing))
				{
					//doing the move backwards to does mot destroyd the game
					board[RIGHT_WALL - move[SRC_NUM]][move[SRC_LETTER] - FLOOR] = board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR];

					//returning the piece that was killed to her place
					board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR] = killed;

					//if ths piece avaliable of the move returning true
					return true;
				}
			}
		}
	}
	//doing the move backwards to does mot destroyd the game
	board[RIGHT_WALL - move[SRC_NUM]][move[SRC_LETTER] - FLOOR] = board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR];

	//returning the piece that was killed to her place
	board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR] = killed;
	//if there aren't a piece that can do a move to kill the king returning false
	return false;
}

//the function check if there a check happened after the move succeced
// move - the current move
// return true if there are a check and false if there arent a check 
bool Manager::isCheck(std::string move, IPiece* piece, bool whiteTurn)
{
	//variables for the king place
	std::string kingPlace;
	bool kingfound = false;
	//the string to check the move
	std::string moveToKing;

	//finding the king place
	for (int i = 0; i < 8 && !kingfound;i++)
	{
		for (int j = 0;j < 8 && !kingfound;j++)
		{
			if (board[i][j] != nullptr &&
				board[i][j]->IsValid("e4e5") == 0 && board[i][j]->IsValid("e4d3") == 0 && board[i][j]->IsValid("e4e6") != 0 && board[i][j]->isWhite != whiteTurn)//check if it a king of the other player
			{
				kingPlace = std::string(1, static_cast<char>(j + FLOOR)) + static_cast<char>(RIGHT_WALL - i);
				kingfound = !kingfound;
			}
		}
	}


	//checking if a check is heppening
	for (int i = 0; i < 8;i++)
	{
		for (int j = 0;j < 8;j++)
		{
			if (board[i][j] != nullptr && board[i][j]->isWhite == whiteTurn)//checking if the the current color do the check to the other color playing
			{
				moveToKing = std::string(1, static_cast<char>(j + FLOOR)) + static_cast<char>(RIGHT_WALL - i) + kingPlace;
				//checking if the move is avaliable  for the piece to do
				if (board[i][j]->IsValid(moveToKing) == 0 && !isBlock(piece, moveToKing))
				{

					//if ths piece avaliable of the move returning true
					return true;
				}
			}
		}
	}
	//if there aren't a piece that can do a move to kill the king returning false
	return false;
}

int main()
{
	Manager m = Manager();
	m.play();

	return 1;
}