#pragma once
#include "Obj.h"
class CGameOverLogo :
	public CObj
{
public:
	CGameOverLogo();
	virtual ~CGameOverLogo();

	// CObj��(��) ���� ��ӵ�
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);
};

