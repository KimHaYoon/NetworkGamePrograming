#pragma once
#include "Obj.h"

class CTile :
	public CObj
{
	TILEINFO m_tInfo;

	float m_fFrame;
public:
	CTile();
	virtual ~CTile();

	// CObj��(��) ���� ��ӵ�
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);

	void SetTileInfo(TILEINFO tInfo);
	
	TILEINFO GetTileInfo();
};

