#pragma once
#include "main.h"

class CNetwork
{
	SOCKET m_Sock;
	//char	m_cKey;
public:
	// �÷��̾��� �����̽� Ű
	bool  m_bKey; // �ӽ�
private:

	PLAYERINFO			m_tPlayerInfo;
	PLAYERINFO			m_tOtherPlayerInfo;
	BULLETINFO			m_tBulletInfo[2][5];

	int					m_iGameState;
	bool				m_bServerOn;

	// 191129 ������ Update Thread�κ��� �޾ƿ��� ��ŸŸ��
	float				m_fServerTime;

	unordered_map<int, PLAYERINFO> m_mapPlayers;


	// 191128 �߰�
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

	// 191129 ���� ������Ʈ ������� ���� �߰�
	void RecvServerTime();
	float GetServerTime();
	// 191129 �÷��̾� �����̽�Ű
	void SendKeysInfo();

// 191128 �߰�
	TILEINFO* GetTilesInfo() const;
	int GetTilesSize() const;

private:
	void RecvPlayersInfo();
	void RecvBulletsInfo();
	void RecvTilesInfo();

	DECLARE_SINGLE( CNetwork )
};

