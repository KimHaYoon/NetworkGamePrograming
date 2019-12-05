#pragma once
#include "Obj.h"
class CGameOverLogo :
	public CObj
{
public:
	CGameOverLogo();
	virtual ~CGameOverLogo();

	// CObj을(를) 통해 상속됨
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);
};

