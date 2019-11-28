#pragma once
#include "Obj.h"

class CTile :
	public CObj
{
	TILEINFO m_tInfo;

	float m_fFrame;

	RECT m_rcCollisionBox;
public:
	CTile();
	virtual ~CTile();

	// CObj��(��) ���� ��ӵ�
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);

	void SetTileInfo(TILEINFO tInfo);
	void SetCollisionBox(RECT rc);
	void SetRect();
	
	TILEINFO GetTileInfo();
	RECT GetCollisionBox();
};

