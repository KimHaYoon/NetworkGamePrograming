#pragma once
#include "Obj.h"

class CBullet :
	public CObj
{
	BULLETINFO m_tInfo;
	RECT m_rcCollisionBox;
	bool m_bShot = false;
public:
	CBullet();
	virtual ~CBullet();

	// CObj을(를) 통해 상속됨
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);

	void Shot();

	void SetShot(bool shot);
	void SetBulletInfo(BULLETINFO tInfo);

	bool GetShot();
};

