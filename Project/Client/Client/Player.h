#pragma once
#include "Obj.h"

#define DIR_LEFT  true
#define DIR_RIGHT  false

class CPlayer :
	public CObj
{
private:
	PLAYERINFO m_tInfo;

	CObj* m_Bullet[5];
	int m_nBullet_Count; // 작살 최대 개수;

	int m_nInvincible_Time; // 피격 판정 무적시간

	int maxFrame;
	int nowFrame;
	float fFrame;

	bool m_bMoveAnimation;
	bool m_bShoot;
	bool m_bDir;

	bool m_binvincibile;

public:
	CPlayer();
	virtual ~CPlayer();

	virtual bool Init();
	virtual void Input();
	virtual void Update( const float& fTimeDelta );
	virtual void Render( HDC hDC );

public:
	void SetPlayerInfo(const PLAYERINFO& tInfo);

	void CreateBullets(int id);

	void BulletShot();
};

