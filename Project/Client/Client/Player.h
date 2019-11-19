#pragma once
#include "Obj.h"

class CPlayer :
	public CObj
{
private:
	PLAYERINFO m_tInfo;

	CObj* m_Bullet[5];
	int m_nBullet_Count; // 작살 최대 개수;

	int m_nInvincible_Time; // 피격 판정 무적시간
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

