#include "main.h"

static int		g_iClientNumber = 0;
static int		g_iState = GAME_WAIT;
unordered_map<int, SERVERPLAYER>		g_Clients;

BULLETINFO		g_tBulletInfo[2][5];


unordered_multimap<int, TILEINFO>		g_Tiles;
unordered_multimap<int, SERVERBALLINFO>		g_Balls;

CRITICAL_SECTION g_CS;


DWORD WINAPI ProcessClient( LPVOID arg );
void Init();
void PrintPlayerInfo(const PLAYERINFO & tInfo);

void SendPlayersInfo(int clientnum);
void SendGameState(int clientnum);
void SendBulletsInfo(int clientnum);

// 191128 �߰�
void SendTileInfo( int clientnum, const GAME_STATE& eState = GAME_START );
void SendBallsInfo( int clientnum, const GAME_STATE& eState = GAME_START );


void TileInit();
TILEINFO* GetTilesInfo( const GAME_STATE& eState = GAME_START );
int	GetTilesSize( const GAME_STATE& eState = GAME_START );

void BallsInit();
void AddBallInfo( const SERVERBALLINFO& tBall, const GAME_STATE& eState = GAME_START );
BALLINFO* GetBallsInfo( const GAME_STATE& eState = GAME_START );
int GetBallsSize( const GAME_STATE& eState = GAME_START );
//====================================================================================================

// ���� �Լ� ���� ���
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
	retval = listen( listen_sock, SOMAXCONN );			// 2������� ���Ӱ���
	if ( retval == SOCKET_ERROR )
	{
		cout << "listen ����" << endl;
	}

	// ������ ��ſ� ����� ����
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
			cout << "accept ����" << endl;
			break;
		}
		cout << "������ Ŭ���̾�Ʈ �� : " << g_iClientNumber + 1 << ", ������ Ŭ���̾�Ʈ ���� : " << client_sock << endl;

		// ������ Ŭ���̾�Ʈ ���� ���
		printf( "\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", inet_ntoa( clientaddr.sin_addr ), ntohs( clientaddr.sin_port ) );
		
		g_Clients[g_iClientNumber].socket = client_sock;
		send( g_Clients[g_iClientNumber].socket, ( char* )&g_Clients[g_iClientNumber].info, sizeof( PLAYERINFO ), 0 );			// �ʱ� ���� ����
		PrintPlayerInfo( g_Clients[g_iClientNumber].info );		
		
		hThread = CreateThread( NULL, 0, ProcessClient, NULL, 0, NULL );				// ������ ����

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

	// ���� ����
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
		
		if (g_iState >= GAME_STAGE1) // ��������1 �̻����
		{
			SendPlayersInfo(clientnum);
			SendTileInfo( clientnum, ( GAME_STATE )g_iState );
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
	TileInit();
	BallsInit();
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
		// ���� Ŭ���̾�Ʈ�ѹ��� �÷��̾������� �ٸ� Ŭ���̾�Ʈ�� �÷��̾� ���� ����
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


void TileInit()
{
	// Stage1
	{
		TILEINFO blockInfo;
		blockInfo.id = 0;
		blockInfo.x = 100;
		blockInfo.y = 250;
		blockInfo.maxFrame = 5;
		blockInfo.nowFrame = 0;
		blockInfo.type = 2;
		blockInfo.color = 0;
		blockInfo.cx = 100;
		blockInfo.cy = 40;
		g_Tiles.insert( make_pair( GAME_STAGE1, blockInfo ) );

		blockInfo.id = 1;
		blockInfo.x = 600;
		blockInfo.y = 250;
		g_Tiles.insert( make_pair( GAME_STAGE1, blockInfo ) );

		blockInfo.id = 2;
		blockInfo.x = 350;
		blockInfo.y = 150;
		g_Tiles.insert( make_pair( GAME_STAGE1, blockInfo ) );
	}
}

TILEINFO * GetTilesInfo( const GAME_STATE & eState )
{
	TILEINFO* pTiles = NULL;

	if ( eState == GAME_START )
	{
		pTiles = new TILEINFO[g_Tiles.size()];

		auto iter = g_Tiles.begin();
		for ( int i = 0; i < g_Tiles.size(); ++i )
		{
			pTiles[i] = iter->second;

			++iter;
		}

		return pTiles;
	}

	auto iterRange = g_Tiles.equal_range( eState );
	int iSize = distance( iterRange.first, iterRange.second );

	pTiles = new TILEINFO[g_Tiles.count( eState )];

	auto iter = iterRange.first;

	for ( int i = 0; i < g_Tiles.count( eState ); ++i )
	{
		pTiles[i] = iter->second;
		++iter;
	}

	return pTiles;
}

int GetTilesSize( const GAME_STATE & eState )
{
	return g_Tiles.count( eState );
}

void BallsInit()
{
	// Stage1
	{
		SERVERBALLINFO ballInfo;
		ballInfo.info.x = 200;
		ballInfo.info.y = 250;
		ballInfo.info.radius = 25;
		ballInfo.info.type = 'A';

		AddBallInfo( ballInfo, GAME_STAGE1 );

		ballInfo.info.x = 600;
		ballInfo.info.y = 250;
		AddBallInfo( ballInfo, GAME_STAGE1 );
	}
}

void AddBallInfo( const SERVERBALLINFO & tBall, const GAME_STATE & eState )
{
	g_Balls.insert( make_pair( eState, tBall ) );
}

BALLINFO * GetBallsInfo( const GAME_STATE & eState )
{
	BALLINFO* pBalls = NULL;

	if ( eState == GAME_START )
	{
		pBalls = new BALLINFO[g_Balls.size()];

		auto iter = g_Balls.begin();
		for ( int i = 0; i < g_Balls.size(); ++i )
		{
			pBalls[i] = iter->second.info;

			++iter;
		}

		return pBalls;
	}

	auto iterRange = g_Balls.equal_range( eState );
	int iSize = distance( iterRange.first, iterRange.second );

	pBalls = new BALLINFO[g_Balls.count( eState )];

	auto iter = iterRange.first;

	for ( int i = 0; i < g_Balls.count( eState ); ++i )
	{
		pBalls[i] = iter->second.info;
		++iter;
	}

	return pBalls;
}

int GetBallsSize( const GAME_STATE & eState )
{
	return g_Balls.count( eState );
}

void SendTileInfo( int clientnum, const GAME_STATE& eState )
{
	int iSize = GetTilesSize( eState );
	TILEINFO* pTiles = GetTilesInfo( eState );
	send( g_Clients[clientnum].socket, ( char* )&iSize, sizeof( iSize ), 0 );
	for ( int i = 0; i < iSize; ++i )
		send( g_Clients[clientnum].socket, ( char* )&pTiles[i], sizeof( TILEINFO ), 0 );
}

void SendBallsInfo( int clientnum, const GAME_STATE & eState )
{
	int iSize = GetBallsSize( eState );
	BALLINFO* pBalls = GetBallsInfo( eState );
	send( g_Clients[clientnum].socket, ( char* )&iSize, sizeof( iSize ), 0 );
	for ( int i = 0; i < iSize; ++i )
		send( g_Clients[clientnum].socket, ( char* )&pBalls[i], sizeof( BALLINFO ), 0 );
}
