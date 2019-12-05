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

	// �÷��̾��� ����Ű ��������
	PLAYERKEYINFO		m_tPlayerKeyInfo;

	int					m_iGameState;
	bool				m_bServerOn;

	// 191129 ������ Update Thread�κ��� �޾ƿ��� ��ŸŸ��
	float				m_fServerTime;
	// 191202 Ŭ���̾�Ʈ�� �� ���ӽð�
	float				m_fTime;
	// 191203 �������� ���� �ð�
	float				m_fStageLimitTime;

	unordered_map<int, PLAYERINFO>		m_mapPlayers;
	
	BALLINFO*	m_pBalls;
	int			m_iBallSize;

	// 191128 �߰�
	TILEINFO*	m_pTiles;
	int			m_iTilesSize;

	// 191205
	ITEMINFO* m_pItems;
	int			m_iItemsSize;

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

	// 191129 ���� ������Ʈ ������� ���� �߰�
	void RecvServerTime();
	float GetServerTime();
	// 191202 Ŭ���̾�Ʈ �� �ð� �߰�
	float GetTotalTime();
	float GetStageLimitTime();
	// 191129 �÷��̾� �����̽�Ű
	void SendKeysInfo();

// 191128 �߰�
	TILEINFO* GetTilesInfo() const;
	int GetTilesSize() const;


	// 191203 �߰�
	BALLINFO* GetBallsInfo() const;
	int	GetBallsSize() const;

	//191205 �߰�
	ITEMINFO* GetItemsInfo() const;
	int GetItemsSize() const;

private:
	// 191203 �߰�
	void RecvStageLimitTime();

	// 191205 �߰�
	void RecvItemInfo();


	void RecvPlayersInfo();
	void RecvBulletsInfo();
	void RecvTilesInfo();

	void RecvBallsInfo();

	DECLARE_SINGLE( CNetwork )
};

