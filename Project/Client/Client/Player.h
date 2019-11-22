#pragma once
#include "Obj.h"

#define DIR_LEFT  1
#define DIR_RIGHT  0
class CPlayer :
	public CObj
{
private:
	PLAYERINFO m_tInfo;

	CObj* m_Bullet[5];
	int m_nBullet_Count; // �ۻ� �ִ� ����;

	int m_nInvincible_Time; // �ǰ� ���� �����ð�

	int m_nMaxFrame;
	int m_nNowFrame;

	RECT m_rcCollisionBox;

public:
	CPlayer();
	virtual ~CPlayer();

	virtual bool Init();
	virtual void Input();
	virtual void Update( DWORD dwTime );
	virtual void Render( HDC hDC );

public:
	void SetPlayerInfo(const PLAYERINFO& tInfo);

	void BulletShot();
};

