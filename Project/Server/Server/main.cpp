#include "main.h"

int		g_iClinetNumber = 1;
unordered_map<int, PLAYERINFO>			g_Clients;

int main()
{
	int retval;

	// 윈속 초기화
	WSADATA	wsa;
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsa ) != 0 )
	{
		cout << "윈속 초기화 안됨" << endl;
		return 1;
	}

	// socket()
	SOCKET listen_sock = socket( AF_INET, SOCK_STREAM, 0 );
	if ( listen_sock == INVALID_SOCKET )
	{
		cout << "socket 에러" << endl;
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
		cout << "bind 에러" << endl;
	}

	// listen()
	retval = listen( listen_sock, SOMAXCONN );
	if ( retval == SOCKET_ERROR )
	{
		cout << "listen 에러" << endl;
	}

	// 데이터 통신에 사용할 변수
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
			cout << "accept 에러" << endl;
			break;
		}


		// 접속한 클라이언트 정보 출력
		printf( "\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", inet_ntoa( clientaddr.sin_addr ), ntohs( clientaddr.sin_port ) );

		PLAYERINFO tPlayerInfo{};
		tPlayerInfo.id = g_iClinetNumber;
		g_Clients[g_iClinetNumber] = tPlayerInfo;

		//send( client_sock, ( char* )&tPlayerInfo, sizeof( PLAYERINFO ), 0 );

		//// 스레드 생성
		//hThread = CreateThread( NULL, 0, ProcessClient, ( LPVOID )client_sock, 0, NULL );

		//// closesocket() - client socket
		//if ( hThread == NULL )
		//{
		//	closesocket( client_sock );
		//}

		//else
		//{
		//	// 종료한 클라이언트 출력
		//	CloseHandle( hThread );
		//}
		closesocket( client_sock );
	}
	// closesocket()
	closesocket( listen_sock );

	// 윈속 종료
	WSACleanup();

	return 0;
}