#pragma once
#include "main.h"

class CNetwork
{
	SOCKET m_Sock;
	//char	m_cKey;

	PLAYERINFO			m_tPlayerInfo;
	PLAYERINFO			m_tOtherPlayerInfo;
	BULLETINFO			m_tBulletInfo[2][5];

	int					m_iGameState;
	bool				m_bServerOn;

	unordered_map<int, PLAYERINFO> m_mapPlayers;

public:
	bool Init( const string& strServerIP = "127.0.0.1" );
	void Update( const float& fTimeDelta );
	void Render( HDC hDC );

	void SetKey( char cKey );

	void SetPlayerInfo(PLAYERINFO tInfo);
	void SetBulletInfo(int playerid, int index, BULLETINFO tInfo);
	PLAYERINFO GetPlayerInfo() const;
	PLAYERINFO GetOtherPlayerInfo() const;
	BULLETINFO GetBulletInfo(int playerid, int index) const;
	int	GetGameState() const;
	bool	GetServerOn() const;

	void RecvPlayersInfo();
	void RecvBulletsInfo();
	DECLARE_SINGLE( CNetwork )
};

