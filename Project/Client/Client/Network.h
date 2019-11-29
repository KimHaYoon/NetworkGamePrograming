#pragma once
#include "main.h"

class CNetwork
{
	SOCKET m_Sock;
	//char	m_cKey;
public:
	// 플레이어의 스페이스 키
	bool  m_bKey; // 임시
private:

	PLAYERINFO			m_tPlayerInfo;
	PLAYERINFO			m_tOtherPlayerInfo;
	BULLETINFO			m_tBulletInfo[2][5];

	int					m_iGameState;
	bool				m_bServerOn;

	// 191129 서버의 Update Thread로부터 받아오는 델타타임
	float				m_fServerTime;

	unordered_map<int, PLAYERINFO> m_mapPlayers;


	// 191128 추가
	TILEINFO*	m_pTiles;
	int			m_iTilesSize;

public:
	bool Init( const string& strServerIP = "127.0.0.1" );
	void Update( const float& fTimeDelta );
	void Render( HDC hDC );

public:
	void SetKey( char cKey );

	void SetPlayerInfo(PLAYERINFO tInfo);
	void SetBulletInfo(int playerid, int index, BULLETINFO tInfo);

public:
	PLAYERINFO GetPlayerInfo() const;
	PLAYERINFO GetOtherPlayerInfo() const;
	BULLETINFO GetBulletInfo(int playerid, int index) const;
	int	GetGameState() const;
	bool	GetServerOn() const;

	// 191129 서버 업데이트 쓰레드로 인해 추가
	void RecvServerTime();
	float GetServerTime();
	// 191129 플레이어 스페이스키
	void SendKeysInfo();

// 191128 추가
	TILEINFO* GetTilesInfo() const;
	int GetTilesSize() const;

private:
	void RecvPlayersInfo();
	void RecvBulletsInfo();
	void RecvTilesInfo();

	DECLARE_SINGLE( CNetwork )
};

