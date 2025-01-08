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
		if (!piece)//check error code 2
		{
			codeMsg[0] = '2';
		}
		//check error code 3
		else if (whiteTurn && isupper(gameString[(RIGHT_WALL - move[3]) * 8 + (move[2] - FLOOR +1)]) ||//white represented by an upercase letter
			!whiteTurn && islower(gameString[(RIGHT_WALL - move[3]) * 8 + (move[2] - FLOOR +1)]))//black represent by lowercase letter
		{
			codeMsg[0] = '3';
		}

		else if (selfCheck(move, piece, whiteTurn))//check error code 4(self check)
		{
			codeMsg[0] = '4';
		}
		else//check the piece error codes(5,6,7)
		{
			pieceCode = piece->Move(move,board);
			codeMsg[0] = static_cast<char>(pieceCode);
			if (codeMsg[0] == '0' && isBlock(piece,move))
			{
				codeMsg[0] = '6';
			}
		}
		if (codeMsg[0] != CODE_PASS && isCheck(move))
		{
			//need to be fullfill
		}
		if (codeMsg[0] == CODE_PASS)//check if it pass all the tests
		{
			//need to be fullfill
			whiteTurn = not whiteTurn;
		}
		codeMsg[0] = CODE_PASS;//setting for the next move to 0
		fronted.sendMessageToGraphics(codeMsg);//sent the code messege
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
			//else if (gameBoard[i] == BLACK_QUEEN || gameBoard[i] == WHITE_QUEEN)
			//{
			//	board[i/8][i%8] = new Queen();
			//	if (gameBoard[i] == BLACK_QUEEN)//if it a black piece it should be known as one
			//	{
			//		board[i / 8][i % 8]->isWhite = false;
			//	}
			// 
			//}	
			//else if (gameBoard[i] == BLACK_ROOK || gameBoard[i] == WHITE_ROOK)
			//{
			//	board[i/8][i%8] = new Rook();
			//	if (gameBoard[i] == BLACK_ROOK)//if it a black piece it should be known as one
			//	{
			//		board[i / 8][i % 8]->isWhite = false;
			//	}
			//}
			//else if (gameBoard[i] == BLACK_BISHOP || gameBoard[i] == WHITE_BISHOP)
			//{
			//	board[i/8][i%8] = new Bishop();
			//	if (gameBoard[i] == BLACK_BISHOP)//if it a black piece it should be known as one
			//	{
			//		board[i / 8][i % 8]->isWhite = false;
			//	}
			//}
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
	return board[move[0] - 'a' + 1][move[1] - '0'];
}

bool Manager::isBlock(IPiece* piece, std::string move)
{
	if (piece->IsValid("e4e5") && piece->IsValid("e4d3") && !piece->IsValid("e4e6"))//check if it a king
	{
		if (board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR] != nullptr)//check if the place is clear
		{
			return false;
		}
		return true;
	}
	else if (piece->IsValid("e4e5") && piece->IsValid("e4d3") && piece->IsValid("e4e6"))//check if it a queen
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
	else if (piece->IsValid("b2d3"))//check if it a Knight
	{
		if (board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR] != nullptr && //first need to be check if there are a piece in the place
			board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR]->isWhite == piece->isWhite)//check if at the dest it the same color
		{
			return false;
		}
		return true;
	}
	else if (piece->IsValid("e4e5") && piece->IsValid("e4f3") && !piece->IsValid("e4d3"))//check if it pawn
	{
		if (board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR] != nullptr || // 
			board[RIGHT_WALL - move[DST_NUM]][move[DST_LETTER] - FLOOR]->isWhite == piece->isWhite)
		{
			return false;
		}
		return true;
	}
	else if (piece->IsValid("e4e7"))//check if it a rook
	{
		int joinToI = 1, joinToJ = 1;
		int iTo = 8, jTo = 8;
		bool foundPiece = false;
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
	IPiece* killed = board[RIGHT_WALL - move[3]][move[2] - FLOOR];//saving the killed piece because it still can be an ilegal move
	//do the move requsted
	board[RIGHT_WALL - move[3]][move[2] - FLOOR] = board[RIGHT_WALL - move[1]][move[0] - FLOOR];
	board[RIGHT_WALL - move[1]][move[0] - FLOOR] = nullptr;
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
			if (board[i][j]->IsValid("e4e5") && board[i][j]->IsValid("e4d3") && !board[i][j]->IsValid("e4e6") && board[i][j]->isWhite != whiteTurn)//check if it a king
			{																								  //(by a combination of moves only the king return answars like this)
				kingPlace = std::string(1, static_cast<char>(i + FLOOR)) + static_cast<char>(8 - j);
				!kingfound;
			}
		}
	}
	
	//checking if a check is heppening
	for (int i = 0; i < 8;i++)
	{
		for (int j = 0;j < 8;j++)
		{
			if (!board[i][j]->isWhite == whiteTurn)//checking if the the other color do the check to the current color playing
			{
				moveToKing = std::string(2, static_cast<char>(i + FLOOR)) + static_cast<char>(8 - j) + kingPlace;
				//checking if the move is avaliable  for the piece to do
				if (board[i][j]->IsValid(moveToKing) == 0 && isBlock(piece,moveToKing))
				{
					//doing the move backwards to does mot destroyd the game
					board[RIGHT_WALL - move[1]][move[0] - FLOOR] = board[RIGHT_WALL - move[3]][move[2] - FLOOR];

					//returning the piece that was killed to her place
					board[RIGHT_WALL - move[3]][move[2] - FLOOR] = killed;

					//if ths piece avaliable of the move returning true
					return true;
				}
			}
		}
	}
	//if there aren't a piece that can do a move to kill the king returning false
	return false;
}

//the function check if there a check happened after the move succeced
// move - the current move
// return true if there are a check and false if there arent a check 
bool Manager::isCheck(std::string move)
{
	return false;
}

int main()
{
	Manager m = Manager();
	m.InitializingBoard("rnbkqbnrpppppppp################################PPPPPPPPRNBKQBNR0");
	m.board[0][1]->Move("b8c6", m.board);

	return 1;
}