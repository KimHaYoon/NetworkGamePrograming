#pragma once
#include "main.h"

class CNetwork
{
	SOCKET m_Sock;
	//char	m_cKey;
private:

	PLAYERINFO			m_tPlayerInfo;
	PLAYERINFO			m_tOtherPlayerInfo;
	BULLETINFO			m_tBulletInfo[2][5];

	// 플레이어의 조작키 프로토콜
	PLAYERKEYINFO		m_tPlayerKeyInfo;

	int					m_iGameState;
	bool				m_bServerOn;

	// 191129 서버의 Update Thread로부터 받아오는 델타타임
	float				m_fServerTime;
	// 191202 클라이언트의 총 접속시간
	float				m_fTime;
	// 191203 스테이지 제한 시간
	float				m_fStageLimitTime;

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
	void SetKeyInfo(PLAYERKEYINFO tKey);
	void LeftKeyOn();
	void RightKeyOn();
	void SpaceKeyOn();
public:
	PLAYERINFO GetPlayerInfo() const;
	PLAYERINFO GetOtherPlayerInfo() const;
	BULLETINFO GetBulletInfo(int playerid, int index) const;
	int	GetGameState() const;
	bool	GetServerOn() const;

	// 191129 서버 업데이트 쓰레드로 인해 추가
	void RecvServerTime();
	float GetServerTime();
	// 191202 클라이언트 총 시간 추가
	float GetTotalTime();
	float GetStageLimitTime();
	// 191129 플레이어 스페이스키
	void SendKeysInfo();

// 191128 추가
	TILEINFO* GetTilesInfo() const;
	int GetTilesSize() const;

private:
	// 191203 추가
	void RecvStageLimitTime();


	void RecvPlayersInfo();
	void RecvBulletsInfo();
	void RecvTilesInfo();

	DECLARE_SINGLE( CNetwork )
};

