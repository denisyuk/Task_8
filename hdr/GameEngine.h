#ifndef ENGINE_H
#define ENGINE_H

#include "defines.h"
#include "CBoard.h"
#include "Statistic.h"
#include "Socket.h"
#include "Message.h"

class CGEngine
{
public:
	CGEngine();
	~CGEngine();

	void Wellcome();
	bool ChoiceGame();		//true - create, false - join
	
	void SetShips();
	bool Battle();
	bool WinLose(IN bool aRez);	//rezultat Win or Lose

	CSocket* GetOppSocket();
	void SetSocket(CSocket* s);
	void OppIsReady();	//Opponent is ready
	void SetTurn(IN bool bVal);
	CBoard* GetBoard();
	CStatistic* GetStatField();

	bool AnswerYN();	//true - yes, false - no
private:
	BYTE EnterSize();
	bool EnterDir();
	BYTE EnterPos();
private:
	//CThread* pThread;
	CBoard *pBoard;
	CStatistic *pStatistic;

	//Sockets
	CSocket* p_OpponentSocket;
	CSocket* p_PlayerSocket; //socket for server

	

	//flags
	bool bTurn;
	bool bReady;
	bool bGameOver;
};

#endif /*ENGINE_H*/