#include "Network.h"
#include "ObjectManager.h"
#include "Obj.h"
#include "Scene.h"

#define		SERVER_PORT 9000
#define		MAX_BUFFER	1024

DEFINITION_SINGLE( CNetwork )

CNetwork::CNetwork()
{
	m_bGameStart = false;
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

	recvn( m_Sock, ( char* )&m_tPlayerInfo, sizeof( PLAYERINFO ), 0 );

	return true;
}

void CNetwork::Update( const float& fTimeDelta )
{
	recvn(m_Sock, (char*)&m_bGameStart, sizeof(bool), 0);

	RecvPlayersInfo();
}

void CNetwork::Render( HDC hDC )
{
}

void CNetwork::SetKey( char cKey)
{
	//m_cKey = cKey;
}

PLAYERINFO CNetwork::GetPlayerInfo() const
{
	return m_tPlayerInfo;
}

PLAYERINFO CNetwork::GetOtherPlayerInfo() const
{
	return m_tOtherPlayerInfo;
}

bool CNetwork::GetGameStart() const
{
	return m_bGameStart;
}

bool CNetwork::GetServerOn() const
{
	return m_bServerOn;
}

void CNetwork::RecvPlayersInfo()
{
	recv( m_Sock, ( char* )&m_tPlayerInfo, sizeof( PLAYERINFO ), 0 );
	cout << "1P 정보 받음" << endl;
	recv( m_Sock, ( char* )&m_tOtherPlayerInfo, sizeof( PLAYERINFO ), 0 );
	cout << "2P 정보 받음" << endl;
}
