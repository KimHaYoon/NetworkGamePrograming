#pragma once
#include "main.h"

class CNetwork
{
	SOCKET m_Sock;
	//char	m_cKey;

	PLAYERINFO			m_tPlayerInfo;
	PLAYERINFO			m_tOtherPlayerInfo;
	GAME_STATE			m_eGameState;
	bool				m_bServerOn;

	unordered_map<int, PLAYERINFO> m_mapPlayers;

public:
	bool Init( const string& strServerIP = "127.0.0.1" );
	void Update( const float& fTimeDelta );
	void Render( HDC hDC );

	void SetKey( char cKey );

	PLAYERINFO GetPlayerInfo() const;
	PLAYERINFO GetOtherPlayerInfo() const;
	GAME_STATE	GetGameState() const;
	bool	GetServerOn() const;

	void RecvPlayersInfo();
	DECLARE_SINGLE( CNetwork )
};

