#include "main.h"

static int		g_iClientNumber = 0;
static int		g_iState = GAME_WAIT;
unordered_map<int, SERVERPLAYER>		g_Clients;

BULLETINFO		g_tBulletInfo[2][5];

CRITICAL_SECTION g_CS;


DWORD WINAPI ProcessClient( LPVOID arg );
void Init();
void PrintPlayerInfo(const PLAYERINFO & tInfo);

void SendPlayersInfo(int clientnum);
void SendGameState(int clientnum);
void SendBulletsInfo(int clientnum);
// 소켓 함수 오류 출력
void err_display( const char *msg )
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		( LPTSTR )&lpMsgBuf, 0, NULL );
	printf( "[%s] %s", msg, ( char * )lpMsgBuf );
	LocalFree( lpMsgBuf );
}

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
	retval = listen( listen_sock, SOMAXCONN );			// 2명까지만 접속가능
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
		cout << "접속한 클라이언트 수 : " << g_iClientNumber + 1 << ", 접속한 클라이언트 소켓 : " << client_sock << endl;

		// 접속한 클라이언트 정보 출력
		printf( "\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", inet_ntoa( clientaddr.sin_addr ), ntohs( clientaddr.sin_port ) );
		
		g_Clients[g_iClientNumber].socket = client_sock;
		send( g_Clients[g_iClientNumber].socket, ( char* )&g_Clients[g_iClientNumber].info, sizeof( PLAYERINFO ), 0 );			// 초기 정보 전송
		PrintPlayerInfo( g_Clients[g_iClientNumber].info );		
		
		hThread = CreateThread( NULL, 0, ProcessClient, NULL, 0, NULL );				// 스레드 생성

		// closesocket() - client socket
		if ( hThread == NULL )
		{
			closesocket( client_sock );
		}

		else
		{
			CloseHandle( hThread );
		}
	}
	closesocket(client_sock);

	// closesocket()
	closesocket( listen_sock );

	// 윈속 종료
	WSACleanup();

	DeleteCriticalSection( &g_CS );

	return 0;
}

DWORD WINAPI ProcessClient( LPVOID arg )
{
	int clientnum = g_iClientNumber++;

	SOCKADDR_IN clientaddr;
	int addrlen = sizeof( clientaddr );
	getpeername( g_Clients[clientnum].socket, ( SOCKADDR * )&clientaddr, &addrlen );

	ULONGLONG ullOldTime = GetTickCount64();

	float iTime = 0.f;

	DWORD dwTime = GetTickCount64();

	while ( true )
	{
		/*if ( GetTickCount64() - ullOldTime < 10.f )
		{
			continue;
		}

		ullOldTime = GetTickCount64();*/

		DWORD dwNow = GetTickCount64();

		if ( dwTime + 16 > dwNow )
		{
			continue;
		}

		dwTime = dwNow;

		SendGameState( clientnum );


		if (g_iState >= GAME_STAGE1) // 스테이지1 이상부터
		{
			SendPlayersInfo(clientnum);

			SendBulletsInfo(clientnum);
		}
	}

	return 0;
}

void Init()
{
	PLAYERINFO* tInfo = new PLAYERINFO[2];
	tInfo[0].x = 50;
	tInfo[0].y = 360;
	tInfo[1].x = 700;
	tInfo[1].y = 360;

	for ( int i = 0; i < 2; ++i )
	{
		tInfo[i].id = i + 1;
		tInfo[i].dir = i;
		g_Clients[i].info = tInfo[i];
	}

	for (int id = 0; id < 2; ++id)
	{
		for (int i = 0; i < 5; ++i)
		{
			g_tBulletInfo[id][i].id = ((id + 1) * 10) + i;
			g_tBulletInfo[id][i].x = 0;
			g_tBulletInfo[id][i].y = 0;
			g_tBulletInfo[id][i].shot = false;
		}
	}

	g_iState = GAME_WAIT;
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

void SendPlayersInfo(int clientnum)
{	
	EnterCriticalSection(&g_CS);
	//////////////////////////////////////////////////////////////
		// 현재 클라이언트넘버의 플레이어정보와 다른 클라이언트의 플레이어 정보 전송
	int ret = recv(g_Clients[clientnum].socket, (char *)&g_Clients[clientnum].info, sizeof(PLAYERINFO), 0);
	if (ret == SOCKET_ERROR)
	{
		err_display("send()");
		cout << g_Clients[clientnum].socket << " send fail!" << endl;
	}

	ret = send(g_Clients[clientnum].socket, (char *)&g_Clients[(clientnum + 1) % 2].info, sizeof(PLAYERINFO), 0);
	if (ret == SOCKET_ERROR)
	{
		err_display("send()");
		cout << g_Clients[clientnum].socket << " send fail!" << endl;
	}
	/////////////////////////////////////////////////////////////
	LeaveCriticalSection(&g_CS);
}

void SendGameState( int clientnum )
{
	EnterCriticalSection(&g_CS);

	if ( g_iClientNumber == 1 )
		g_iState = GAME_START;
	else if( g_iClientNumber == 2 )
		g_iState = GAME_STAGE1;
	LeaveCriticalSection(&g_CS);

	int ret = send( g_Clients[clientnum].socket, ( char* )&g_iState, sizeof( g_iState ), 0 );

	EnterCriticalSection(&g_CS);
	if ( ret == SOCKET_ERROR )
	{
		err_display( "send()" );
		cout << g_Clients[clientnum].socket << " send fail!" << endl;
	}

	else
	{
		cout << g_Clients[clientnum].socket << " send GameState : " << g_iState << endl;
	}
	LeaveCriticalSection(&g_CS);
}

void SendBulletsInfo(int clientnum)
{
	EnterCriticalSection(&g_CS);
	for (int i = 0; i < 5; ++i)
	{
		int ret = recv(g_Clients[clientnum].socket, 
			(char *)&g_tBulletInfo[clientnum][i], sizeof(BULLETINFO), 0);
	}
	LeaveCriticalSection(&g_CS);

	EnterCriticalSection(&g_CS);
	for (int i = 0; i < 5; ++i)
	{
		int ret = send(g_Clients[clientnum].socket,
			(char *)&g_tBulletInfo[(clientnum + 1) % 2][i], sizeof(BULLETINFO), 0);
	}
	LeaveCriticalSection(&g_CS);
}