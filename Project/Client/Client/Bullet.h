#pragma once
#include "Obj.h"

class CBullet :
	public CObj
{
	BULLETINFO m_tInfo;
	RECT m_rcCollisionBox;
public:
	CBullet();
	virtual ~CBullet();

	// CObj을(를) 통해 상속됨
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);

	void Shot(int x, int y);

	void SetShot(bool shot);
	void SetBulletInfo(BULLETINFO tInfo);

	bool GetShot();
};

