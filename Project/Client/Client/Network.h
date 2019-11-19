#pragma once
#include "main.h"

class CNetwork
{
	SOCKET m_Sock;
	//char	m_cKey;

	PLAYERINFO			m_tPlayerInfo;
	PLAYERINFO			m_tOtherPlayerInfo;
	bool				m_bGameStart;
	bool				m_bServerOn;
	unordered_map<int, PLAYERINFO> m_mapPlayers;

public:
	bool Init( const string& strServerIP = "127.0.0.1" );
	void Update();
	void Render( HDC hDC );

	void SetKey( char cKey );

	PLAYERINFO GetPlayerInfo() const;
	PLAYERINFO GetOtherPlayerInfo() const;
	bool	GetGameStart() const;
	bool	GetServerOn() const;
	void RecvPlayersInfo();

	DECLARE_SINGLE( CNetwork )
};

