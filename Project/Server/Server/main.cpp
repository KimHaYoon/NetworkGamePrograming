#include "main.h"

int		g_iClinetNumber = 0;
bool	g_bGameStart = false;
unordered_map<int, PLAYERINFO>			g_Clients;

DWORD WINAPI ProcessClient( LPVOID arg );
void Init();
void PrintPlayerInfo(const PLAYERINFO & tInfo);

void SendPlayersInfo(const SOCKET& socket);
void SendGameStart(const SOCKET& socket);

CRITICAL_SECTION g_CS;
GAME_STATE g_eState;

int main()
{
	InitializeCriticalSection( &g_CS );
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
	retval = listen( listen_sock, 2 );			// 2명까지만 접속가능
	if ( retval == SOCKET_ERROR )
	{
		cout << "listen 에러" << endl;
	}

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;
	
	Init();

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
		cout << "접속한 클라이언트 수 : " << g_iClinetNumber + 1 << endl;

		// 접속한 클라이언트 정보 출력
		printf( "\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", inet_ntoa( clientaddr.sin_addr ), ntohs( clientaddr.sin_port ) );

		send( client_sock, ( char* )&g_Clients[g_iClinetNumber], sizeof( PLAYERINFO ), 0 );			// 초기 정보 전송
		PrintPlayerInfo( g_Clients[g_iClinetNumber] );
		++g_iClinetNumber;
																								
		hThread = CreateThread( NULL, 0, ProcessClient, ( LPVOID )client_sock, 0, NULL );				// 스레드 생성

		// closesocket() - client socket
		if ( hThread == NULL )
		{
			closesocket( client_sock );
		}

		else
		{
			CloseHandle( hThread );
		}

		closesocket( client_sock );
	}
	// closesocket()
	closesocket( listen_sock );

	// 윈속 종료
	WSACleanup();

	DeleteCriticalSection( &g_CS );

	return 0;
}

DWORD WINAPI ProcessClient( LPVOID arg )
{
	SOCKET client_sock = ( SOCKET )arg;
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof( clientaddr );
	getpeername( client_sock, ( SOCKADDR * )&clientaddr, &addrlen );

	while ( true )
	{
		
		SendGameStart( client_sock );

		SendPlayersInfo(client_sock);
	}

	return 0;
}

void Init()
{
	PLAYERINFO* tInfo = new PLAYERINFO[2];
	tInfo[0].x = 50;
	tInfo[0].y = 360;
	tInfo[1].x = 750;
	tInfo[1].y = 360;

	for ( int i = 0; i < 2; ++i )
	{
		tInfo[i].id = i + 1;
		tInfo[i].dir = i;
		g_Clients[i] = tInfo[i];
	}

	g_eState = GAME_WAIT;
}

void PrintPlayerInfo( const PLAYERINFO & tInfo )
{
	EnterCriticalSection( &g_CS );
	cout << "ID : " << tInfo.id << endl;
	cout << "X : " << tInfo.x << ", Y : " << tInfo.y << endl;
	cout << "Dir : " << tInfo.dir << endl;
	cout << "HP : " << tInfo.hp << endl;
	LeaveCriticalSection( &g_CS );
}

void SendPlayersInfo( const SOCKET & socket )
{	
	EnterCriticalSection( &g_CS );
	for ( int i = 0; i < 2; ++i )
		send( socket, ( char* )&g_Clients[i], sizeof( PLAYERINFO ), 0 );
	LeaveCriticalSection( &g_CS );
}

void SendGameStart( const SOCKET& socket )
{
	if ( g_iClinetNumber == 2 )
	{
		g_eState = GAME_START;
	}
	send( socket, ( char* )&g_eState, sizeof( GAME_STATE), 0 );
	EnterCriticalSection( &g_CS );
	cout << "Send GameStart : " << g_eState << endl;
	LeaveCriticalSection( &g_CS );
}
