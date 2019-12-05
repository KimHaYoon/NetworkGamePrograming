#include "Network.h"
#include "ObjectManager.h"
#include "Obj.h"
#include "Scene.h"

#define		SERVER_PORT 9000
#define		MAX_BUFFER	1024

DEFINITION_SINGLE( CNetwork )

CNetwork::CNetwork()
{
	m_bServerOn = false;
	//m_cKey = 0;
}


CNetwork::~CNetwork()
{
	SAFE_DELETE_ARRAY( m_pTiles );
	SAFE_DELETE_ARRAY( m_pBalls );
	SAFE_DELETE_ARRAY( m_pItems );
	closesocket( m_Sock );
	WSACleanup();
}

bool CNetwork::Init( const string& strServerIP )
{
	int retval;

	WSADATA wsa;

	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsa ) != 0 )
	{
		return false;
	}


	// socket()
	m_Sock = socket( AF_INET, SOCK_STREAM, 0 );
	if ( m_Sock == INVALID_SOCKET )
	{
		return false;
	}

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof( serveraddr ) );
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr( strServerIP.c_str() );
	serveraddr.sin_port = htons( SERVERPORT );
	retval = connect( m_Sock, ( SOCKADDR * )&serveraddr, sizeof( serveraddr ) );

	if ( retval == SOCKET_ERROR )
	{
		return false;
	}

	m_bServerOn = true;

	retval = recvn( m_Sock, ( char* )&m_tPlayerInfo, sizeof( PLAYERINFO ), 0 );
	if ( retval == SOCKET_ERROR )
		return false;
	_cprintf( "Client ID : %d \n", m_tPlayerInfo.id );

	for (int id = 0; id < 2; ++id)
	{
		for (int i = 0; i < 5; ++i)
		{
			m_tBulletInfo[id][i].id = ((id + 1) * 10) + i;
			m_tBulletInfo[id][i].x = 0;
			m_tBulletInfo[id][i].y = 0;
			m_tBulletInfo[id][i].shot = false;
		}
	}
	// 클라이언트 시간
	m_fTime = 0.f;

	return true;
}

void CNetwork::Update( const float& fTimeDelta )
{
	int ret = recv(m_Sock, (char*)&m_iGameState, sizeof(int), 0);
	if (ret == SOCKET_ERROR)
		return;

	RecvServerTime();

	//_cprintf( "Client ID : %d, GameState : %d \n", m_tPlayerInfo.id, m_iGameState );
	if (m_iGameState >= GAME_STAGE1) 
	{
		RecvStageLimitTime();

		SendKeysInfo();

		RecvPlayersInfo();

		RecvTilesInfo();

		RecvBallsInfo();

		RecvBulletsInfo();

		RecvItemInfo();
	}


}

void CNetwork::Render( HDC hDC )
{
}

void CNetwork::SetKey( char cKey)
{
	//m_cKey = cKey;
}

void CNetwork::SetPlayerInfo(PLAYERINFO tInfo)
{
	m_tPlayerInfo = tInfo;
}

void CNetwork::SetBulletInfo(int playerid, int index, BULLETINFO tInfo)
{
	m_tBulletInfo[playerid - 1][index] = tInfo;
}

void CNetwork::SetKeyInfo(PLAYERKEYINFO tKey)
{
	m_tPlayerKeyInfo = tKey;
}

void CNetwork::LeftKeyOn()
{
	m_tPlayerKeyInfo.left = true;
}

void CNetwork::RightKeyOn()
{
	m_tPlayerKeyInfo.right = true;
}

void CNetwork::SpaceKeyOn()
{
	m_tPlayerKeyInfo.space = true;
}

PLAYERINFO CNetwork::GetPlayerInfo() const
{
	return m_tPlayerInfo;
}

PLAYERINFO CNetwork::GetOtherPlayerInfo() const
{
	return m_tOtherPlayerInfo;
}

BULLETINFO CNetwork::GetBulletInfo(int playerid, int index) const
{
	return m_tBulletInfo[playerid - 1][index];
}

int CNetwork::GetGameState() const
{
	return m_iGameState;
}

bool CNetwork::GetServerOn() const
{
	return m_bServerOn;
}

// 191129 추가 서버 업데이트 쓰레드

void CNetwork::RecvServerTime()
{
	recv(m_Sock, (char*)&m_fServerTime, sizeof(float), 0);
	m_fTime += m_fServerTime;
}

float CNetwork::GetServerTime()
{
	return m_fServerTime;
}

float CNetwork::GetTotalTime()
{
	return m_fTime;
}

float CNetwork::GetStageLimitTime()
{
	return m_fStageLimitTime;
}

void CNetwork::SendKeysInfo()
{
	send(m_Sock, (char*)&m_tPlayerKeyInfo, sizeof(PLAYERKEYINFO), 0);
	m_tPlayerKeyInfo.space = false;
	m_tPlayerKeyInfo.right = false;
	m_tPlayerKeyInfo.left = false;
}

TILEINFO * CNetwork::GetTilesInfo() const
{
	return m_pTiles;
}

int CNetwork::GetTilesSize() const
{
	return m_iTilesSize;
}

BALLINFO * CNetwork::GetBallsInfo() const
{
	return m_pBalls;
}

int CNetwork::GetBallsSize() const
{
	return m_iBallSize;
}

ITEMINFO * CNetwork::GetItemsInfo() const
{
	return m_pItems;
}

int CNetwork::GetItemsSize() const
{
	return m_iItemsSize;
}

void CNetwork::RecvStageLimitTime()
{
	recv(m_Sock, (char *)&m_fStageLimitTime, sizeof(float), 0);
}

void CNetwork::RecvItemInfo()
{
	//SAFE_DELETE_ARRAY(m_pItems);
	recvn(m_Sock, (char *)&m_iItemsSize, sizeof(int), 0);

	m_pItems = new ITEMINFO[m_iItemsSize];
	for (int i = 0; i < m_iItemsSize; ++i)
	{
		recvn(m_Sock, (char*)&m_pItems[i], sizeof(ITEMINFO), 0);
	}
}

void CNetwork::RecvPlayersInfo()
{
	recvn( m_Sock, ( char* )&m_tPlayerInfo, sizeof( PLAYERINFO ), 0 );
	recvn( m_Sock, ( char* )&m_tOtherPlayerInfo, sizeof( PLAYERINFO ), 0 );
}

void CNetwork::RecvBulletsInfo()
{
	int id = m_tPlayerInfo.id - 1;

	for (int i = 0; i < 5; ++i)
	{
		recv(m_Sock, (char *)&m_tBulletInfo[id][i], sizeof(BULLETINFO), 0);
	}
	for (int i = 0; i < 5; ++i)
	{
		recv(m_Sock, (char *)&m_tBulletInfo[(id + 1) % 2][i], sizeof(BULLETINFO), 0);
	}
}

void CNetwork::RecvTilesInfo()
{
	recvn( m_Sock, ( char* )&m_iTilesSize, sizeof( m_iTilesSize ), 0 );

	m_pTiles = new TILEINFO[m_iTilesSize];
	for ( int i = 0; i < m_iTilesSize; ++i )
	{
		recvn( m_Sock, ( char* )&m_pTiles[i], sizeof( TILEINFO ), 0 );
	}
}

void CNetwork::RecvBallsInfo()
{
	recvn( m_Sock, ( char* )&m_iBallSize, sizeof( int ), 0 );

	m_pBalls = new BALLINFO[m_iBallSize];
	for ( int i = 0; i < m_iBallSize; ++i )
	{
		recvn( m_Sock, ( char* )&m_pBalls[i], sizeof( BALLINFO ), 0 );
	}
}
