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

	return true;
}

void CNetwork::Update( const float& fTimeDelta )
{
	int ret = recv(m_Sock, (char*)&m_iGameState, sizeof(int), 0);
	if (ret == SOCKET_ERROR)
		return;

	_cprintf( "Client ID : %d, GameState : %d \n", m_tPlayerInfo.id, m_iGameState );
	if (m_iGameState >= GAME_STAGE1) 
	{
		RecvPlayersInfo();

		RecvBulletsInfo();
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
	m_tBulletInfo[playerid][index] = tInfo;
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
	return m_tBulletInfo[playerid][index];
}

int CNetwork::GetGameState() const
{
	return m_iGameState;
}

bool CNetwork::GetServerOn() const
{
	return m_bServerOn;
}

void CNetwork::RecvPlayersInfo()
{
	send( m_Sock, ( char* )&m_tPlayerInfo, sizeof( PLAYERINFO ), 0 );
	cout << "1P 정보 받음" << endl;
	recv( m_Sock, ( char* )&m_tOtherPlayerInfo, sizeof( PLAYERINFO ), 0 );
	cout << "2P 정보 받음" << endl;
}

void CNetwork::RecvBulletsInfo()
{
	int id = m_tPlayerInfo.id;

	for (int i = 0; i < 5; ++i)
	{
		send(m_Sock, (char *)&m_tBulletInfo[id][i], sizeof(BULLETINFO), 0);
	}
	for (int i = 0; i < 5; ++i)
	{
		recv(m_Sock, (char *)&m_tBulletInfo[(id + 1) % 2][i], sizeof(BULLETINFO), 0);
	}
}