#pragma once
#include "Obj.h"

class CTile :
	public CObj
{
	TILEINFO	m_tInfo;

	int			m_iNowFrame;
	float		m_fFrame;

	bool		m_bRender;
public:
	CTile();
	virtual ~CTile();

	// CObj을(를) 통해 상속됨
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);

	void SetTileInfo(TILEINFO tInfo);
	
	TILEINFO GetTileInfo();
};

