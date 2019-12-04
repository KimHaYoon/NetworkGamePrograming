#include "main.h"

static int		g_iClientNumber = 0;
static int		g_iState = GAME_WAIT;
static float	g_fTime;

static ULONGLONG g_ullFrame = 10.0;
static float	g_fTimeDelta[2] = { 0.f, 0.f };

unordered_map<int, SERVERPLAYER*>		g_Clients;

BULLETINFO		g_tBulletInfo[2][5];


unordered_multimap<int, TILEINFO>				g_Tiles;
unordered_multimap<int, SERVERBALLINFO*>		g_Balls;

// 191203 스테이지 제한시간
static float	g_fStageLimitTime;

float g_fPlayerCollisionTime[2];
bool	g_bPlayerCollisition[2];
float g_BulletCollsionTime;

CRITICAL_SECTION g_CS_Player;
CRITICAL_SECTION g_CS_Bullet;
CRITICAL_SECTION g_CS_Ball;
CRITICAL_SECTION g_CS_Block;


DWORD WINAPI ProcessClient( LPVOID arg );
DWORD WINAPI ProcessUpdate(LPVOID arg);
void Update(const float& fTimeDelta);
void Init();
void PrintPlayerInfo(const PLAYERINFO & tInfo);

void RecvKeysInfo(int clientnum);
void SendPlayersInfo(int clientnum);
void SendGameState(int clientnum);
void SendBulletsInfo(int clientnum);

// 191203 추가
void Stage1_Init();
void BallsUpdate( const float& fTimeDelta );

bool CollisionBall( const BALLINFO& tBall, int x, int y, int width, int height );
bool AABBCollisionCheck(int AminX, int AminY, int AmaxX, int AmaxY, int BminX, int BminY, int BmaxX, int BmaxY);
void PlayerCollisionBall( int id );
void BallCollisionBullet();

// 191128 추가
void SendTileInfo( int clientnum, const GAME_STATE& eState = GAME_START );
void SendBallsInfo( int clientnum, const GAME_STATE& eState = GAME_START );


void TileInit();
TILEINFO** GetTilesInfo( const GAME_STATE& eState = GAME_START );
int	GetTilesSize( const GAME_STATE& eState = GAME_START );

void BallsInit();
void AddBallInfo( SERVERBALLINFO* tBall, const GAME_STATE& eState = GAME_START );
SERVERBALLINFO** GetBallsInfo( const GAME_STATE& eState = GAME_START );
int GetBallsSize( const GAME_STATE& eState = GAME_START );
//====================================================================================================

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
	InitializeCriticalSection( &g_CS_Player );
	InitializeCriticalSection( &g_CS_Bullet );
	InitializeCriticalSection( &g_CS_Ball );
	InitializeCriticalSection( &g_CS_Block );
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

	//Update Thread
	HANDLE hUpdateThread;
	cout << "Create Update Thread" << endl;
	hUpdateThread = CreateThread(NULL, 0, ProcessUpdate, NULL, 0, NULL);

	if (hUpdateThread != NULL)
	{
		CloseHandle(hUpdateThread);
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
		
		g_Clients[g_iClientNumber]->socket = client_sock;
		send( g_Clients[g_iClientNumber]->socket, ( char* )&g_Clients[g_iClientNumber]->info, sizeof( PLAYERINFO ), 0 );			// 초기 정보 전송
		PrintPlayerInfo( g_Clients[g_iClientNumber]->info );
		
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

	DeleteCriticalSection( &g_CS_Player );
	DeleteCriticalSection( &g_CS_Bullet );
	DeleteCriticalSection( &g_CS_Ball );
	DeleteCriticalSection( &g_CS_Block );
	return 0;
}

DWORD WINAPI ProcessClient( LPVOID arg )
{
	int clientnum = g_iClientNumber++;
	if (g_iClientNumber == 2)
	{
		Stage1_Init();
	}

	SOCKADDR_IN clientaddr;
	int addrlen = sizeof( clientaddr );
	getpeername( g_Clients[clientnum]->socket, ( SOCKADDR * )&clientaddr, &addrlen );

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

		send(g_Clients[clientnum]->socket, (char *)&g_fTimeDelta[clientnum], sizeof(float), 0);


		
		if (g_iState >= GAME_STAGE1) // 스테이지1 이상부터
		{
			send(g_Clients[clientnum]->socket, (char *)&g_fStageLimitTime, sizeof(float), 0);
			
			RecvKeysInfo(clientnum);

			SendPlayersInfo(clientnum);

			SendTileInfo( clientnum, ( GAME_STATE )g_iState );

			SendBallsInfo( clientnum, ( GAME_STATE )g_iState );

			SendBulletsInfo(clientnum);
		}
	}

	return 0;
}


DWORD WINAPI ProcessUpdate(LPVOID arg)
{
	int clientnum = -1;
	ULONGLONG ullOldTime = GetTickCount64();

	int frame = 0;
	ULONGLONG ullOldTime2 = GetTickCount64();
	float fTimeDelta = 0.f;

	DWORD dwTime = GetTickCount64();

	while (true)
	{
		// 프레임을 고정한다 1초에 약 60
		if (GetTickCount64() - ullOldTime >= g_ullFrame)
		{
			fTimeDelta = GetTickCount64() - ullOldTime;
			fTimeDelta = fTimeDelta / 1000.0f;
			g_fTimeDelta[0] = g_fTimeDelta[1] = fTimeDelta;

			Update(fTimeDelta);

			frame++;
			ullOldTime = GetTickCount64();
		}
		// 1초에 한번씩 FPS 값을 산출하여 화면에 출력한다
		if (GetTickCount64() - ullOldTime2 >= 1000)
		{
			cout << "FPS : " << frame << endl;
			ullOldTime2 = GetTickCount64();
			frame = 0;
		}
	}

	return 0;
}


void Update( const float& fTimeDelta )
{
	EnterCriticalSection( &g_CS_Player );
	g_fTime += fTimeDelta;
	if ( g_fTime > 1.f )
	{
		if ( g_Clients[0]->info.gameStart )
		{
			g_Clients[0]->info.score += 10;
			g_Clients[0]->info.score = g_Clients[0]->info.score % 99999;
		}
		if ( g_Clients[1]->info.gameStart )
		{
			g_Clients[1]->info.score += 10;
			g_Clients[1]->info.score = g_Clients[1]->info.score % 99999;
		}
		g_fTime = 0.f;
	}

	if ( g_iState >= GAME_STAGE1 )
	{
		g_fStageLimitTime -= fTimeDelta;
	}

	LeaveCriticalSection( &g_CS_Player );

	for ( int id = 0; id < 2; ++id )
	{
		if ( g_bPlayerCollisition[id] )
			continue;
		
		g_fPlayerCollisionTime[id] += fTimeDelta;

		if ( g_fPlayerCollisionTime[id] >= 2.f )
		{
			g_bPlayerCollisition[id] = true;
			g_fPlayerCollisionTime[id] = 0.f;
		}
	}

	BallCollisionBullet();

	for ( int id = 0; id < 2; ++id )
	{
		if ( g_bPlayerCollisition[id] )
		{
			PlayerCollisionBall( id );
			g_bPlayerCollisition[id] = false;
		}
	}

	BallsUpdate( fTimeDelta );

	for (int id = 0; id < 2; ++id)
	{
		EnterCriticalSection(&g_CS_Player);
		// 플레이어의 이동
		// 왼쪽 방향키
		if (g_Clients[id]->keys.left == true)
		{
			if(g_Clients[id]->info.x > 10)
				g_Clients[id]->info.x -= (300.f * fTimeDelta);
			g_Clients[id]->info.maxFrame = 5;
		}
		// 오른쪽 방향키
		else if (g_Clients[id]->keys.right == true)
		{
			if(g_Clients[id]->info.x < 730)
				g_Clients[id]->info.x += (300.f * fTimeDelta);
			g_Clients[id]->info.maxFrame = 5;
		}

		// 스페이스 키
		if (g_Clients[id]->keys.space == true)
		{
			g_Clients[id]->info.maxFrame = 3;
			for (int i = 0; i < 5; ++i)
			{
				if (g_tBulletInfo[id][i].shot == false)
				{
					g_tBulletInfo[id][i].shot = true;
					g_tBulletInfo[id][i].x = g_Clients[id]->info.x + 15;
					g_tBulletInfo[id][i].y = g_Clients[id]->info.y;
					g_Clients[id]->keys.space = false;
					break;
				}
			}
		}
		LeaveCriticalSection(&g_CS_Player);


		EnterCriticalSection(&g_CS_Bullet);
		// 총알의 움직임
		for (int i = 0; i < 5; ++i)
		{
			if (g_tBulletInfo[id][i].shot == true)
			{
				float height = 200.f * fTimeDelta;
				g_tBulletInfo[id][i].y -= height;
				g_tBulletInfo[id][i].height += (height * 1.25f);

				if (g_tBulletInfo[id][i].y < 10.f)
				{
					g_tBulletInfo[id][i].shot = false;
					g_tBulletInfo[id][i].height = 70.f;
				}

				// 총알과 타일의 충돌
				TILEINFO** pTiles = GetTilesInfo(GAME_STATE(g_iState));
				int iTileSize = GetTilesSize(GAME_STATE(g_iState));

				for (int j = 0; j < iTileSize; ++j)
				{
					if (pTiles[j]->type == 0)
						continue;

					if (AABBCollisionCheck(
						g_tBulletInfo[id][i].x,
						g_tBulletInfo[id][i].y,
						g_tBulletInfo[id][i].x + 30,
						g_tBulletInfo[id][i].y + g_tBulletInfo[id][i].height,
						pTiles[j]->x,
						pTiles[j]->y,
						pTiles[j]->x + pTiles[j]->cx - 10,
						pTiles[j]->y + pTiles[j]->cy))
					{
						g_tBulletInfo[id][i].shot = false;
						g_tBulletInfo[id][i].x = 0;
						g_tBulletInfo[id][i].y = 0;
						g_tBulletInfo[id][i].height = 70;

						if (pTiles[j]->type == 1)
						{
							pTiles[j]->animation = true;
							pTiles[j]->maxFrame = 5;
							pTiles[j]->type = 0;
						}
						break;
					}
				}
			}
		}
		LeaveCriticalSection(&g_CS_Bullet);
	}
}

void Init()
{
	SERVERPLAYER* tInfo = new SERVERPLAYER[2];
	tInfo[0].info.x = 50;
	tInfo[0].info.y = 360;
	tInfo[1].info.x = 700;
	tInfo[1].info.y = 360;

	for ( int i = 0; i < 2; ++i )
	{
		tInfo[i].info.id = i + 1;
		g_Clients[i] = &tInfo[i];
	}

	for (int id = 0; id < 2; ++id)
	{
		for (int i = 0; i < 5; ++i)
		{
			g_tBulletInfo[id][i].id = ((id + 1) * 10) + i;
			g_tBulletInfo[id][i].x = 0;
			g_tBulletInfo[id][i].y = 0;
			g_tBulletInfo[id][i].height = 70.f;
			g_tBulletInfo[id][i].shot = false;
		}


		g_fPlayerCollisionTime[id] = 0.f;
		g_bPlayerCollisition[id] = false;
	}

	g_iState = GAME_WAIT;
	TileInit();
	BallsInit();
}

void PrintPlayerInfo( const PLAYERINFO & tInfo )
{
	cout << "ID : " << tInfo.id << endl;
	cout << "X : " << tInfo.x << ", Y : " << tInfo.y << endl;
	cout << "HP : " << tInfo.hp << endl;
}


void RecvKeysInfo(int clientnum)
{
	int ret = recv(g_Clients[clientnum]->socket, (char *)&g_Clients[clientnum]->keys, sizeof(PLAYERKEYINFO), 0);
	if (ret == SOCKET_ERROR)
	{
		err_display("recv()");
		cout << g_Clients[clientnum]->socket << " recv fail!" << endl;
	}
}

void SendPlayersInfo(int clientnum)
{	
	//////////////////////////////////////////////////////////////
		// 현재 클라이언트넘버의 플레이어정보와 다른 클라이언트의 플레이어 정보 전송
	int ret = send(g_Clients[clientnum]->socket, (char *)&g_Clients[clientnum]->info, sizeof(PLAYERINFO), 0);
	if (ret == SOCKET_ERROR)
	{
		err_display("send()");
		cout << g_Clients[clientnum]->socket << " send fail!" << endl;
	}

	ret = send(g_Clients[clientnum]->socket, (char *)&g_Clients[(clientnum + 1) % 2]->info, sizeof(PLAYERINFO), 0);
	if (ret == SOCKET_ERROR)
	{
		err_display("send()");
		cout << g_Clients[clientnum]->socket << " send fail!" << endl;
	}
	/////////////////////////////////////////////////////////////
}

void SendGameState( int clientnum )
{
	if ( g_iClientNumber == 1 )
		g_iState = GAME_START;
	else if( g_iClientNumber == 2 )
		g_iState = GAME_STAGE1;

	int ret = send( g_Clients[clientnum]->socket, ( char* )&g_iState, sizeof( g_iState ), 0 );

	if ( ret == SOCKET_ERROR )
	{
		err_display( "send()" );
		cout << g_Clients[clientnum]->socket << " send fail!" << endl;
	}

	else
	{
		//cout << g_Clients[clientnum].socket << " send GameState : " << g_iState << endl;
	}
}

void SendBulletsInfo(int clientnum)
{
	for (int i = 0; i < 5; ++i)
	{
		int ret = send(g_Clients[clientnum]->socket,
			(char *)&g_tBulletInfo[clientnum][i], sizeof(BULLETINFO), 0);
	}
	for (int i = 0; i < 5; ++i)
	{
		int ret = send(g_Clients[clientnum]->socket,
			(char *)&g_tBulletInfo[(clientnum + 1) % 2][i], sizeof(BULLETINFO), 0);
	}
}

void Stage1_Init()
{
	static bool Stage1_Init = false;

	if (Stage1_Init == false)
	{
		g_fStageLimitTime = 70;
		Stage1_Init = true;
	}
}

void BallsUpdate( const float & fTimeDelta )
{
	int iSize = GetBallsSize( ( GAME_STATE )g_iState );
	SERVERBALLINFO** pBalls = GetBallsInfo( ( GAME_STATE )g_iState );

	EnterCriticalSection( &g_CS_Ball );

	for ( int i = 0; i < iSize; ++i )
	{

		int iGravity = ( pBalls[i]->info.y + 80 ) * 1.5 * fTimeDelta; // 낙하속도
		if ( iGravity <= 1 )
			pBalls[i]->yDir = DIR_DOWN;


		if ( pBalls[i]->xDir == DIR_LEFT )
		{
			pBalls[i]->info.x -= (70 * fTimeDelta);

			if ( pBalls[i]->info.x - pBalls[i]->info.radius < -10 ) // 왼쪽 벽에 충돌
				pBalls[i]->xDir = DIR_RIGHT;
		}

		else if ( pBalls[i]->xDir == DIR_RIGHT )
		{
			pBalls[i]->info.x += (70 * fTimeDelta);

			if ( pBalls[i]->info.x + pBalls[i]->info.radius > 801 - 35 ) // 오른쪽 벽에 충돌
				pBalls[i]->xDir = DIR_LEFT;
		}

		if ( pBalls[i]->yDir == DIR_DOWN )
		{
			pBalls[i]->info.y += iGravity;

			if ( pBalls[i]->info.y + pBalls[i]->info.radius > 410 ) // 바닥에 충돌
				pBalls[i]->yDir = DIR_UP;
		}

		else if ( pBalls[i]->yDir == DIR_UP )
		{
			pBalls[i]->info.y -= iGravity;

			if ( pBalls[i]->info.y - pBalls[i]->info.radius < -30 ) // 최대 높이 값
				pBalls[i]->yDir = DIR_DOWN;
		}

		TILEINFO** pTiles = GetTilesInfo(GAME_STATE(g_iState));
		int iTileSize = GetTilesSize(GAME_STATE(g_iState));

		for (int j = 0; j < iTileSize; ++j)
		{
			if (pTiles[j]->type == 0)
				continue;

			if (pBalls[i]->yDir == DIR_UP)
			{
				if (CollisionBall(pBalls[i]->info,
					pTiles[j]->x,
					pTiles[j]->y + pTiles[j]->cy - 5,
					pTiles[j]->cx - 20,
					5))
				{
					pBalls[i]->yDir = DIR_DOWN;
				}
			}

			else if (pBalls[i]->yDir == DIR_DOWN)
			{
				if (CollisionBall(pBalls[i]->info,
					pTiles[j]->x,
					pTiles[j]->y - 5,
					pTiles[j]->cx - 20,
					5))
				{
					pBalls[i]->yDir = DIR_UP;
				}
			}

			if (pBalls[i]->xDir == DIR_LEFT)
			{
				if (CollisionBall(pBalls[i]->info,
					pTiles[j]->x + pTiles[j]->cx - 15,
					pTiles[j]->y + (pTiles[j]->cy / 2) - 5,
					10,
					10))
				{
					pBalls[i]->xDir = DIR_RIGHT;
				}
			}

			else if (pBalls[i]->xDir == DIR_RIGHT)
			{
				if (CollisionBall(pBalls[i]->info,
					pTiles[j]->x - 10,
					pTiles[j]->y + (pTiles[j]->cy / 2) - 5,
					10,
					10))
				{
					pBalls[i]->xDir = DIR_LEFT;
				}
			}
		}
	}

	LeaveCriticalSection( &g_CS_Ball );
}

void PlayerCollisionBall(int id)
{
	for ( auto iterB = g_Balls.begin(); iterB != g_Balls.end(); ++iterB )
	{
		if ( CollisionBall( iterB->second->info, g_Clients[id]->info.x, g_Clients[id]->info.y, 70, 70 ) )
		{
			EnterCriticalSection( &g_CS_Player );
			g_Clients[id]->info.hp -= 1;
			cout << "id : " << id << "공과 충돌" << endl;
			LeaveCriticalSection( &g_CS_Player );
		}
	}
}

void BallCollisionBullet()
{
	for ( auto iterB = g_Balls.begin(); iterB != g_Balls.end(); ++iterB )
	{
		for ( int id = 0; id < 2; ++id )
		{
			for ( int i = 0; i < 5; ++i )
			{
				if ( !g_tBulletInfo[id][i].shot )
					continue;

				if ( CollisionBall( iterB->second->info, g_tBulletInfo[id][i].x, g_tBulletInfo[id][i].y, 30, g_tBulletInfo[id][i].height ) )
				{
					cout << "공이랑 총알 충돌됨 " << endl;
				}
			}
		}
	}
}

bool CollisionBall( const BALLINFO & tBall, int x, int y, int width, int height )
{
	if ( tBall.x < x + width && tBall.x + tBall.radius > x &&
		tBall.y < y + height && tBall.y + tBall.radius > y )
		return true;

	return false;
}

bool AABBCollisionCheck(int AminX, int AminY, int AmaxX, int AmaxY, int BminX, int BminY, int BmaxX, int BmaxY)
{
	if (AmaxX < BminX)
		return false;
	if (AminX > BmaxX)
		return false;

	if (AmaxY < BminY)
		return false;
	if (AminY > BmaxY)
		return false;

	return true;
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
		blockInfo.type = 1;
		blockInfo.color = 0;
		blockInfo.cx = 100;
		blockInfo.cy = 40;
		blockInfo.animation = false;
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

TILEINFO ** GetTilesInfo( const GAME_STATE & eState )
{
	TILEINFO** pTiles = NULL;

	if ( eState == GAME_START )
	{
		pTiles = new TILEINFO*[g_Tiles.size()];

		auto iter = g_Tiles.begin();
		for ( int i = 0; i < g_Tiles.size(); ++i )
		{
			pTiles[i] = (TILEINFO*)&(iter->second);

			++iter;
		}

		return pTiles;
	}

	auto iterRange = g_Tiles.equal_range( eState );
	int iSize = distance( iterRange.first, iterRange.second );

	pTiles = new TILEINFO*[g_Tiles.count( eState )];

	auto iter = iterRange.first;

	for ( int i = 0; i < g_Tiles.count( eState ); ++i )
	{
		pTiles[i] = (TILEINFO*)&(iter->second);
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
		SERVERBALLINFO* ballInfo = new SERVERBALLINFO;
		ballInfo->info.x = 200;
		ballInfo->info.y = 250;
		ballInfo->info.radius = 25;
		ballInfo->info.type = 'A';
		ballInfo->live = true;
		ballInfo->xDir = DIR_RIGHT;
		ballInfo->yDir = DIR_DOWN;


		AddBallInfo( ballInfo, GAME_STAGE1 );

		ballInfo = new SERVERBALLINFO;
		ballInfo->info.x = 600;
		ballInfo->info.y = 250;
		ballInfo->info.radius = 25;
		ballInfo->info.type = 'A';
		ballInfo->live = true;
		ballInfo->xDir = DIR_LEFT;
		ballInfo->yDir = DIR_DOWN;

		AddBallInfo( ballInfo, GAME_STAGE1 );
	}
}

void AddBallInfo( SERVERBALLINFO * tBall, const GAME_STATE & eState )
{
	g_Balls.insert( make_pair( eState, tBall ) );
}

SERVERBALLINFO ** GetBallsInfo( const GAME_STATE & eState )
{
	SERVERBALLINFO** pBalls = NULL;

	if ( eState == GAME_START )
	{
		pBalls = new SERVERBALLINFO *[g_Balls.size()];

		auto iter = g_Balls.begin();
		for ( int i = 0; i < g_Balls.size(); ++i )
		{
			pBalls[i] = (iter->second);

			++iter;
		}

		return pBalls;
	}

	auto iterRange = g_Balls.equal_range( eState );
	int iSize = distance( iterRange.first, iterRange.second );

	pBalls = new SERVERBALLINFO *[g_Balls.count( eState )];

	auto iter = iterRange.first;

	for ( int i = 0; i < g_Balls.count( eState ); ++i )
	{
		pBalls[i] = iter->second;
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
	TILEINFO** pTiles = GetTilesInfo( eState );
	send( g_Clients[clientnum]->socket, ( char* )&iSize, sizeof( iSize ), 0 );
	for (int i = 0; i < iSize; ++i)
	{
		TILEINFO tInfo = *pTiles[i];
		send(g_Clients[clientnum]->socket, (char*)&tInfo, sizeof(TILEINFO), 0);
	}

}

void SendBallsInfo( int clientnum, const GAME_STATE & eState )
{
	int iSize = GetBallsSize( eState );
	SERVERBALLINFO** pBalls = GetBallsInfo( eState );
	send( g_Clients[clientnum]->socket, ( char* )&iSize, sizeof( iSize ), 0 );

	//cout << "Send Balls Size : " << iSize << endl;

	for ( int i = 0; i < iSize; ++i )
		send( g_Clients[clientnum]->socket, ( char* )&pBalls[i]->info, sizeof( BALLINFO ), 0 );

	//cout << "Send Balls Info" << endl;
}