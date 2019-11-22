#pragma once
#include "Obj.h"
class CItem :
	public CObj
{
	ITEMINFO m_tInfo;

	RECT m_rcCollisionBox;
public:
	CItem();
	virtual ~CItem();

	// CObj을(를) 통해 상속됨
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);

	void SetRect();

	void SetItemInfo(ITEMINFO tInfo);
	void SetCollisionBox(RECT rc);

	RECT GetCollisionBox();
};

