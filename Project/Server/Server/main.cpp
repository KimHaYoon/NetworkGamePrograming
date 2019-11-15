#include "main.h"

map<int, SOCKETINFO>						g_Clients;
int		g_iClinetNumber = 1;

POS KeyMessage(const char & key);
int FindClientID( const SOCKET& socket );


int main()
{
	WSADATA WSAData;
	WSAStartup( MAKEWORD( 2, 2 ), &WSAData );
	SOCKET listenSocket = socket( AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN serverAddr;
	memset( &serverAddr, 0, sizeof( SOCKADDR_IN ) );
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons( SERVER_PORT );
	serverAddr.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
	::bind( listenSocket, ( struct sockaddr* )&serverAddr, sizeof( SOCKADDR_IN ) );
	listen( listenSocket, 2 );
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof( SOCKADDR_IN );
	memset( &clientAddr, 0, addrLen );
	SOCKET clientSocket;
	DWORD flags;


	while ( true )
	{
		clientSocket = accept( listenSocket, ( struct sockaddr * )&clientAddr, &addrLen );
	}

	closesocket( listenSocket );
	WSACleanup();
}

POS KeyMessage( const char & key )
{
	if ( key == KEY_LEFT )
		return POS{ -100,  0 };

	if ( key == KEY_RIGHT )
		return POS{ 100,  0 };

	if ( key == KEY_UP )
		return POS{ 0,  -100 };

	if ( key == KEY_DOWN )
		return POS{ 0,  100 };

	return POS();
}

int FindClientID( const SOCKET & socket )
{
	for ( auto client : g_Clients )
	{
		if ( client.second.socket == socket )
			return client.first;
	}

	return 0;
}