#ifndef SOCKET_H
#define SOCKET_H

#include "defines.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT htons(1111)
#define IP_SERVER_ADDRESS 0

class CSocket
{
//data
private:
	WORD wVersionRequested;
    WSADATA wsaData;

	sockaddr_in ServerAddress;
	sockaddr_in ClientAddress;

	int vClientAddressLen;

	SOCKET vSocket; //socket variable
//methods
public:
	CSocket();
	void SetSocketState(SOCKET s);
	SOCKET GetSocketState();

	void CreateSocket(); //Create socket
	int Connect(char* p_IPBuf);  //Connect to server 
	void Bind();
	void Listen();
	SOCKET Accept();
	int Receive(char*, int);
	void Send(char*, int);
	void Close();
};

#endif /*SOCKET_H*/