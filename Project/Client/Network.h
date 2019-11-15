#pragma once
#include "main.h"

class CNetwork
{
	SOCKET m_Sock;
	//char	m_cKey;

	//PLAYERINFO			m_tPlayerInfo;

	//unordered_map<int, PLAYERINFO> m_Players;

public:
	bool Init( const string& strServerIP = "127.0.0.1" );
	void Update();
	void Render( HDC hDC );

	void SetKey( char cKey );

	DECLARE_SINGLE( CNetwork )
};

