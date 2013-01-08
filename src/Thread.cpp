#include "..\hdr\Thread.h"

CThread::CThread()
{
	h_ReceiveThread = 0;
	id_ReceiveThread = 0;
	p_OpponentSocket = 0;
	p_PlayerSocket = 0;
}

CThread::~CThread()
{}

/////////////////////////////////////////////////////////////////
void CThread::CreateReceiveThread(LPVOID p_GE)
{
	h_ReceiveThread = CreateThread(0, 0, &CThread::ReceiveThread, (LPVOID)p_GE, NULL, &id_ReceiveThread);
}

void CThread::DeleteReceiveThread()
{
	TerminateThread(h_ReceiveThread, NULL);
}

//////////////////////////////////////////////////////////////////////
DWORD WINAPI CThread::ReceiveThread(LPVOID lp_CThread)
{
	CGEngine* tGE = ((CGEngine*)lp_CThread);

	int retVal = 0; //uses as variable that gets count of received symbols
	
	CMessage *tMsg = new CMessage();
	bool GameOver = false;
	bool bRetVal = false;
	BYTE xShot = NULL;
	BYTE yShot = NULL;

	CCount vCount;	//count of all ships (4, 3, 2, 1)
	memset(&vCount, 0, sizeof(CCount));
	vCount.vCount |= M_ALLSHIP_COUNT;

	while(!GameOver)
	{
		retVal = 0;
		
		retVal = tGE->GetOppSocket()->Receive(reinterpret_cast<char*>(tMsg), sizeof(CMessage));
		if(retVal > 0)
		{
			
			switch(tMsg->GetKey())
			{
			case MSG_READY: //ready flag to start game
				{
					tGE->OppIsReady();	//opponent is ready to battle							
				}break;

			case MSG_SHOT:	//a shot from opponent
				{
					xShot = tMsg->GetX();
					yShot = tMsg->GetY();
					if(xShot < 1 || xShot > 10 || yShot < 1 || yShot > 10) 
					{
						cout<<"\nMSG_SHOT error! \n";
						system("pause");
						break;
					}
					bRetVal = tGE->GetBoard()->Shot(xShot, yShot);

					if(bRetVal)	//'direct hit' or 'missed' ?
					{
						tMsg->SetKey(MSG_DIRECTHIT);	//direct hit
						tGE->GetOppSocket()->Send(reinterpret_cast<char*>(tMsg), sizeof(CMessage));

						BYTE tWhole = tGE->GetBoard()->IsWholeBroken(tMsg->GetX(), tMsg->GetY());
						if(tWhole)	//is whole ship broken?
						{
							tMsg->SetKey(MSG_WHOLESHIP);
							tGE->GetOppSocket()->Send(reinterpret_cast<char*>(tMsg), sizeof(CMessage));

							switch(tWhole)
							{
								case SINGLE_SHIP:
								{
									vCount.vSi -= 1;
								}break;
								case DOUBLE_SHIP:
								{
									vCount.vDb -= 1;
								}break;
								case TRIPLE_SHIP:
								{
									vCount.vTr -= 1;
								}break;
								case FFOLD_SHIP:
								{
									vCount.vFf -= 1;
								}
								default:
								{}
							}
						}

						if(!vCount.vCount)
						{
							tMsg->SetKey(MSG_LOSE);
							tGE->GetOppSocket()->Send(reinterpret_cast<char*>(tMsg), sizeof(CMessage));
							tGE->WinLose(false);

							tMsg->SetKey(MSG_READY);
							tGE->GetOppSocket()->Send(reinterpret_cast<char*>(tMsg), sizeof(CMessage));
							GameOver = true;	//close receive thread
						}
					}
					else
					{
						tMsg->SetKey(MSG_MISSED);	//bad shot
						tGE->GetOppSocket()->Send(reinterpret_cast<char*>(tMsg), sizeof(CMessage));
						tGE->SetTurn(true);
					}			

					tGE->OppIsReady();
					tMsg->SetKey(MSG_READY);
					tGE->GetOppSocket()->Send(reinterpret_cast<char*>(tMsg), sizeof(CMessage));
				}break;
				
			case MSG_MISSED:	//bullet is missed
				{
					tGE->SetTurn(false);
					xShot = tMsg->GetX();
					yShot = tMsg->GetY();
					tGE->GetStatField()->SetCell(xShot, yShot, M_MISSED);

					tMsg->SetKey(MSG_READY);
					tGE->GetOppSocket()->Send(reinterpret_cast<char*>(tMsg), sizeof(CMessage));
				}break;

			case MSG_DIRECTHIT:	//direct hit
				{
					xShot = tMsg->GetX();
					yShot = tMsg->GetY();
					tGE->GetStatField()->SetCell(xShot, yShot, M_BROKENSHIP);
				}break;

			case MSG_WHOLESHIP:
				{
					xShot = tMsg->GetX();
					yShot = tMsg->GetY();
					tGE->GetStatField()->SetWholeBrokenShip(xShot, yShot);
				}break;

			case MSG_LOSE: 
				{
					tGE->WinLose(true);	//WIN
					GameOver = true;
					tGE->OppIsReady();
				}break;

			default:
				{
					cout<<"\nError! Bad message! \n";
				}break;
			}
		}
	}

	delete tMsg;
	
	return 0;
}

bool CThread::Acception()
{
	//Sockets
	p_OpponentSocket = new CSocket();
	p_PlayerSocket = new CSocket();
	p_PlayerSocket->CreateSocket();
	p_OpponentSocket->CreateSocket();

	p_PlayerSocket->Bind();
	p_PlayerSocket->Listen();

	SOCKET sRetVal;
	BYTE n = 3;
	do
	{
		cout<<"\nAccepting...\n";
		sRetVal = p_PlayerSocket->Accept();
		if(sRetVal == INVALID_SOCKET)
		{
			cout<<"\nError! Can't accept! (attempt: "<<(int)n<<")\n";
			system("pause");
		}
		else
		{
			cout<<"\nSuccessful accept!\n";
			p_OpponentSocket->SetSocketState(sRetVal);
//			pThread->CreateReceiveThread((LPVOID)this);
			system("pause");
			return true;
		}
	}while(n--);

	p_OpponentSocket->Close();
	p_PlayerSocket->Close();

	delete p_OpponentSocket;
	delete p_PlayerSocket;
	p_OpponentSocket = NULL;
	p_PlayerSocket = NULL;
	cout<<"\nError! Game is begin again!\n";
	system("pause");
	return false;
}

bool CThread::Connection()
{
	//Sockets
	p_OpponentSocket = new CSocket();
	p_PlayerSocket = new CSocket();
	p_PlayerSocket->CreateSocket();
	p_OpponentSocket->CreateSocket();

	SOCKET sRetVal;
	BYTE n = 5;	
	char *tIP = new char[MAX_IP_SIZE + 1];	//temporary string IP address
	
	do
	{
		EnterIP(tIP);	//enter IP address from keyboard
		cout<<"\nConnecting...\n";
		sRetVal = p_OpponentSocket->Connect(tIP);
		if(sRetVal == INVALID_SOCKET)
		{
			cout<<"\nError! Can't accept! (attempt: "<<(int)n<<")\n";
			system("pause");
		}
		else
		{
			cout<<"\nSuccessful accept!\n";
			system("pause");
			delete[] tIP;
			return true;
		}
	}while(n--);

	p_OpponentSocket->Close();
	p_PlayerSocket->Close();

	delete p_OpponentSocket;
	delete p_PlayerSocket;
	p_OpponentSocket = NULL;
	p_PlayerSocket = NULL;
	cout<<"\nError! Game is begin again!\n";
	system("pause");
	return false;
}

CSocket* CThread::GetOppSocket()
{
	return this->p_OpponentSocket;
}

void CThread::ClearSockets()
{
	p_OpponentSocket->Close();
	p_PlayerSocket->Close();

	delete p_OpponentSocket;
	delete p_PlayerSocket;
	p_OpponentSocket = NULL;
	p_PlayerSocket = NULL;
}

///private
void CThread::EnterIP(OUT char* pIP)
{
	if(pIP == NULL)
	{
		cout<<"\nBad pointer *pIP\n";
		system("pause");
		exit(1);
	}
	
	bool bFlag = false;
	do
	{
		system("cls");
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		cout<<"\nEnter IP address (192.168.xxx.xxx): ";
		cin.getline(pIP, MAX_IP_SIZE, '\n');
		if(cin.good() && cin.rdbuf()->in_avail() < MAX_IP_SIZE)
		{
			bFlag = true;
		}
		else
		{
			cout<<"\nError! Try again!\n";
			system("pause");
		}
	}while(!bFlag);
}
