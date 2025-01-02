#include "Manager.h"

int main()
{
	Manager m = Manager();
	m.play();
	return 1;
}
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
		else if (selfCheck(move))//check error code 4(self check)
		{
			codeMsg[0] = '4';
		}
		else//check the piece error codes(5,6,7)
		{
			pieceCode = piece->Move(move);
			codeMsg[0] = static_cast<char>(pieceCode);
		}
		if (isCheck(move))
		{
			//need to be fullfill
		}
		if (codeMsg[0] == '0')//check if it pass all the tests
		{
			//need to be fullfill
			whiteTurn = not whiteTurn;
		}
		codeMsg[0] = '0';//setting for the next move to 0
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
		if (gameBoard[i] != '#')
		{
			if (gameBoard[i] == 'n' || gameBoard[i] == 'N')
			{
				board[i / 8][i % 8] = new Knight();
			}
			else if (gameBoard[i] == 'k' || gameBoard[i] == 'K')
			{
				board[i / 8][i % 8] = new King();
			}
			else if (gameBoard[i] == 'p' || gameBoard[i] == 'P')
			{
				board[i / 8][i % 8] = new Pawn();
			}
			//else if (gameBoard[i] == 'q' || gameBoard[i] == 'Q')
			//{
			//	board[i/8][i%8] = new Queen();
			//}	
			//else if (gameBoard[i] == 'r' || gameBoard[i] == 'R')
			//{
			//	board[i/8][i%8] = new Rook();
			//}
			//else if (gameBoard[i] == 'b' || gameBoard[i] == 'B')
			//{
			//	board[i/8][i%8] = new Bishop();
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

//the function check if the current move create a selkf check
//move - the current move done by player
//the function return true if it does create a self check if not return false
bool Manager::selfCheck(std::string move)
{
	//need to be fullfil
	return false;
}
