#include "..\hdr\GameEngine.h"

CGEngine::CGEngine()
{
	pBoard = new CBoard();
	pStatistic = new CStatistic();
	p_OpponentSocket = NULL;

	bTurn = false;
	bReady = false;
	bGameOver = false;
}

CGEngine::~CGEngine()
{
	delete pBoard;
	delete pStatistic;
}

void CGEngine::Wellcome()
{
	system("cls");
	cout<<"\nWELLCOME TO SEABATTLE!!! \n(Console lan game)\n\n";
	system("pause");
}

bool CGEngine::ChoiceGame()
{
	BYTE ch;
	BYTE n = 3;	//count of user's attempts
	do
	{
		system("cls");
		cout<<"\nChoice game:"
				"\n\'1\' to Create Lan game"
				"\n\'2\' to Connect to Lan game"
				"\n\nLet's enter your choice: ";
		
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		cin>>ch;
		if(cin.rdbuf()->in_avail() > 1)
		{
			cout<<"\n Too many symbols\n";
			system("pause");
		}
		else
		{
			if(ch == '1')	//create game
			{
				bTurn = true;
				return true;
			}
			if(ch == '2')	//join to game
			{
				bTurn = false;
				return false;
			}
			else
			{
				cout<<"\n Error! Incorrect symbol!\n";
				system("pause");
			}
		}
	}while(n--);
	system("cls");
	cout<<"\nUps!!! Count of your attempts is zero! Your choice is \'Connect to game\'!\nBe more concentrated at next time!!!\n";
	return true;
}

void CGEngine::SetShips()
{
	pStatistic->Clear();
	pBoard->Clear();
	bool bRetVal = false;

	cout<<"\nDo you want to set random position of ships? \n";
	bRetVal = AnswerYN();
	if(bRetVal)
	{
		pBoard->SetRandomPosOfShip();	//set random position of ship
	}
	else
	{	
		BYTE tSize, tX, tY, tCount;
		bool tDir;
	
		do
		{
			system("cls");
			pBoard->Show();
			tSize = EnterSize();
			tDir = EnterDir();
			cout<<"\nHorizontal: ";
				tX = EnterPos();
			cout<<"\nVertical: ";
				tY = EnterPos();
			bRetVal = pBoard->SetShip((BYTE)tX, (BYTE)tY, tDir, (BYTE)tSize);
			if(!bRetVal)
			{
				cout<<"\nWrong coordinates! \n";
				system("pause");
			}

			tCount = pBoard->GetCountOfShip();
		}while(tCount);
	}
	system("cls");

	this->pBoard->Show();

	cout<<"\n\nGOOD!!!";
	system("pause");
}

bool CGEngine::Battle()
{
	bGameOver = false;

	bool bPrevShot = false;
	CMessage *tMsg = new CMessage();
	tMsg->SetKey(MSG_READY);		//inform the opponent
	p_OpponentSocket->Send(reinterpret_cast<char*>(tMsg), sizeof(CMessage));

	cout<<"\nPlease, wait! Opponent is not ready yet...\n";
	while(!bReady) 
	{
		Sleep(ONE_SEC);
	}

	BYTE tX = 0, tY = 0;
	while(!bGameOver)
	{
		system("cls");
		cout<<"\nYour field: ";
			pBoard->Show();			//show board
		cout<<"\nStatistic: ";
			pStatistic->Show();		//show statistic field
		//************************************************
		bReady = false;
		if(bTurn)
		{
			do{
			cout<<"\nLet's do shot!"
				"Enter X (horizontal)";
				tX = EnterPos();
			cout<<"Enter Y (vertical)";
				tY = EnterPos();

				bPrevShot = GetStatField()->IsPrevShot(tX, tY);
			}while(!bPrevShot);

			//make msg
				tMsg->SetKey(MSG_SHOT);
				tMsg->SetShot(tX, tY);
			//send msg
			p_OpponentSocket->Send(reinterpret_cast<char*>(tMsg), sizeof(CMessage));

			//wait answer from opponent
			cout<<"\nWait...";
			while(!bReady) 
			{
				Sleep(ONE_SEC);
			}
		}
		else
		{
			cout<<"\nPlease wait, opponent is thinking!\n";
			while(!bReady) 
			{
				Sleep(ONE_SEC);
			}
		}
	}

	delete tMsg;
	return true;
}

bool CGEngine::WinLose(IN bool aRez)
{
	if(aRez)
	{
		cout<<"\nCONGRATULATION!!!\n You are winner!!!\n\n";
		bGameOver = true;
		system("pause");
		return true;
	}
	else
	{
		cout<<"\nGAME OVER\nOops... you lose!!!\n\n";
		system("pause");
		bGameOver = true;
		return false;
	}
}

CSocket* CGEngine::GetOppSocket()
{
	return this->p_OpponentSocket;
}

bool CGEngine::AnswerYN()
{
	short sExit = 0;
	bool bExit = false;
	do
	{
		//system("cls");
		cout<<"\n(1 - to answer \'YES\', \n0 - to answer \'NO\'): ";
		cin>>sExit;

	if(cin.good() && (sExit == 0 || sExit == 1) && cin.rdbuf()->in_avail() == 1)
	{
		bExit = true;
	}
	else
	{
		cout<<"\n\nError! Wrong choice! Try again!\n";
		system("pause");
	}
	}while(!bExit);

	if(sExit == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CGEngine::SetSocket(CSocket* s)
{
	p_OpponentSocket = s;
}

void CGEngine::OppIsReady()
{
	this->bReady = true;
}

void CGEngine::SetTurn(IN bool bVal)
{
	bTurn = bVal;
}

CBoard* CGEngine::GetBoard()
{
	return pBoard;
}

CStatistic* CGEngine::GetStatField()
{
	return pStatistic;
}

///private methods
BYTE CGEngine::EnterSize()
{
	short sSize = 0;
	bool bExit = false;
	do
	{
		//system("cls");
	cout<<	"\nShips:"
			"\n\'1\' - S		(single)"
			"\n\'2\' - S S		(double)"
			"\n\'3\' - S S S	(triple)"
			"\n\'4\' - S S S S	(four fold)"
			"\nEnter size (1 - 4): ";
	cin>>sSize;
	if(cin.good() && sSize > 0 && sSize < 5 && cin.rdbuf()->in_avail() == 1)
	{
		if(pBoard->CheckShip((BYTE)sSize))
		{
			bExit = true;
		}
		else
		{
			cout<<"\nThis ships are already situated!\n";
		}
		
	}
	else
	{
		cout<<"\n\nError! Wrong size! Try again!\n";
		system("pause");
	}
	}while(!bExit);

	return (BYTE)sSize;
}
bool CGEngine::EnterDir()
{
	short sDir = 0;
	bool bExit = false;
	do
	{
		cout<<"\nEnter Direction(0 - horizontal, 1 - vertical): ";
		cin>>sDir;

		if(cin.good() && (sDir == 0 || sDir == 1) && cin.rdbuf()->in_avail() == 1)
		{
			bExit = true;
		}
		else
		{
			cout<<"\n\nError! Wrong direction! Try again!\n";
			system("pause");
		}
	}while(!bExit);

	if(sDir == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

BYTE CGEngine::EnterPos()
{
	short sPos = 0;
	bool bExit = false;
	do
	{
		cout<<"\nEnter position(1 - 10): ";
		cin>>sPos;
		if(cin.good() && sPos > 0 && sPos < 11 && cin.rdbuf()->in_avail() <= 2)
		{
			bExit = true;
		}
		else
		{
			cout<<"\n\nError! Wrong position! Try again!\n";
		}
	}while(!bExit);
	return (BYTE)sPos;
}
