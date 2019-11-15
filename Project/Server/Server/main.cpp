#include "main.h"

int		g_iClinetNumber = 1;
unordered_map<int, PLAYERINFO>			g_Clients;

int main()
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA	wsa;
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsa ) != 0 )
	{
		cout << "���� �ʱ�ȭ �ȵ�" << endl;
		return 1;
	}

	// socket()
	SOCKET listen_sock = socket( AF_INET, SOCK_STREAM, 0 );
	if ( listen_sock == INVALID_SOCKET )
	{
		cout << "socket ����" << endl;
	}

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof( serveraddr ) );
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );
	serveraddr.sin_port = htons( SERVERPORT );
	retval = bind( listen_sock, ( SOCKADDR * )&serveraddr, sizeof( serveraddr ) );
	if ( retval == SOCKET_ERROR )
	{
		cout << "bind ����" << endl;
	}

	// listen()
	retval = listen( listen_sock, SOMAXCONN );
	if ( retval == SOCKET_ERROR )
	{
		cout << "listen ����" << endl;
	}

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;

	while ( true )
	{
		// accept()
		addrlen = sizeof( clientaddr );
		client_sock = accept( listen_sock, ( SOCKADDR * )&clientaddr, &addrlen );
		if ( client_sock == INVALID_SOCKET )
		{
			cout << "accept ����" << endl;
			break;
		}


		// ������ Ŭ���̾�Ʈ ���� ���
		printf( "\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", inet_ntoa( clientaddr.sin_addr ), ntohs( clientaddr.sin_port ) );

		PLAYERINFO tPlayerInfo{};
		tPlayerInfo.id = g_iClinetNumber;
		g_Clients[g_iClinetNumber] = tPlayerInfo;

		//send( client_sock, ( char* )&tPlayerInfo, sizeof( PLAYERINFO ), 0 );

		//// ������ ����
		//hThread = CreateThread( NULL, 0, ProcessClient, ( LPVOID )client_sock, 0, NULL );

		//// closesocket() - client socket
		//if ( hThread == NULL )
		//{
		//	closesocket( client_sock );
		//}

		//else
		//{
		//	// ������ Ŭ���̾�Ʈ ���
		//	CloseHandle( hThread );
		//}
		closesocket( client_sock );
	}
	// closesocket()
	closesocket( listen_sock );

	// ���� ����
	WSACleanup();

	return 0;
}