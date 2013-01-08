#include "..\hdr\Socket.h"

CSocket::CSocket()
{
	vSocket = -1;
	ZeroMemory(&ServerAddress, sizeof(ServerAddress));
	ZeroMemory(&ClientAddress, sizeof(ClientAddress));

	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = IP_SERVER_ADDRESS; //INADDR_ANY (Any address)
	ServerAddress.sin_port = SERVER_PORT;

	vClientAddressLen = sizeof(ClientAddress);
}

void CSocket::SetSocketState(SOCKET s)
{
	vSocket = s;
}

SOCKET CSocket::GetSocketState()
{
	return vSocket;
}

void CSocket::CreateSocket()
{
	wVersionRequested = MAKEWORD( 2, 2 );
    WSAStartup( wVersionRequested, &wsaData );

	vSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void CSocket::Bind()
{
	bind(vSocket, (LPSOCKADDR)&ServerAddress, sizeof(ServerAddress));
}

void CSocket::Listen()
{
	listen(vSocket,10);
}

SOCKET CSocket::Accept()
{
	return accept(vSocket,(struct sockaddr*)&ClientAddress, &vClientAddressLen);
}

int CSocket::Receive(char* buf, int len)
{
	return recv(vSocket, buf, len, 0);
}

void CSocket::Send(char* buf, int len)
{
	send(vSocket, buf, len, 0);
}

int CSocket::Connect(char* p_IPBuf)
{
	ServerAddress.sin_addr.s_addr = inet_addr(p_IPBuf);
	return connect(vSocket, (struct sockaddr *)&ServerAddress, sizeof(struct sockaddr));
}

void CSocket::Close()
{
	closesocket(vSocket);
}