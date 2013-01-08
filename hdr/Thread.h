#ifndef THREAD_H
#define THREAD_H

#include "defines.h"
#include "Socket.h"
#include "Message.h"
#include "GameEngine.h"

class CThread
{
//--data--
private:
	//Threads
	HANDLE h_ReceiveThread;
	DWORD id_ReceiveThread;
	CSocket* p_OpponentSocket;
	CSocket* p_PlayerSocket;
//methods
private:
	static DWORD WINAPI ReceiveThread(LPVOID);
	void EnterIP(OUT char *str);

public:
	CThread();
	~CThread();
	void CreateReceiveThread(LPVOID p_GE);
	void DeleteReceiveThread();
	void ClearSockets();

	CSocket* GetOppSocket();

	bool Acception();		//wait acception
	bool Connection();		//wait connection
};

#endif /*THREAD_H*/